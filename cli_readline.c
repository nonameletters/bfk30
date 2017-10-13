/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Add to readline cmdline-editing by
 * (C) Copyright 2005
 * JinHua Luo, GuangDong Linux Center, <luo.jinhua@gd-linux.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <string.h>

#include "shell.h"
#include "cli_readline.h"
#include "chprintf.h"

#if CONFIG_CMDLINE_EDITING

/*
 * cmdline-editing related codes from vivi.
 * Author: Janghoon Lyu <nandy@mizi.com>
 */

#define putnstr(chp, str, n)	chprintf(chp, "%.*s", (int)n, str)

#define CTL_CH(c)		((c) - 'a' + 1)
#define CTL_BACKSPACE		('\b')
#define DEL			((char)255)
#define DEL7			((char)127)
#define CREAD_HIST_CHAR		('!')

#define getcmd_putch(chp, ch)	streamPut(chp, (uint8_t)ch)
#define getcmd_getch(chp)		streamGet(chp)
#define getcmd_cbeep(chp)		getcmd_putch(chp, '\a')

#define HIST_MAX		SHELL_MAX_HISTORY_DEEP
#define HIST_SIZE		SHELL_MAX_LINE_LENGTH

static int hist_max;
static int hist_add_idx;
static int hist_cur = -1;
static unsigned hist_num;

static char *hist_list[HIST_MAX];
static char hist_lines[HIST_MAX][HIST_SIZE + 1];	/* Save room for NULL */

#define add_idx_minus_one() ((hist_add_idx == 0) ? hist_max : hist_add_idx-1)

void hist_init(void)
{
	int i;

	hist_max = 0;
	hist_add_idx = 0;
	hist_cur = -1;
	hist_num = 0;

	for (i = 0; i < HIST_MAX; i++) {
		hist_list[i] = hist_lines[i];
		hist_list[i][0] = '\0';
	}
}

void cread_print_hist_list(BaseSequentialStream *chp)
{
	int i;
	unsigned long n;

	n = hist_num - hist_max;

	i = hist_add_idx + 1;
	while (1) {
		if (i > hist_max)
			i = 0;
		if (i == hist_add_idx)
			break;
		chprintf(chp, "%s\r\n", hist_list[i]);
		n++;
		i++;
	}
}

static void cread_add_to_hist(char *line)
{
	strcpy(hist_list[hist_add_idx], line);

	if (++hist_add_idx >= HIST_MAX)
		hist_add_idx = 0;

	if (hist_add_idx > hist_max)
		hist_max = hist_add_idx;

	hist_num++;
}

static char *hist_prev(void)
{
	char *ret;
	int old_cur;

	if (hist_cur < 0)
		return NULL;

	old_cur = hist_cur;
	if (--hist_cur < 0)
		hist_cur = hist_max;

	if (hist_cur == hist_add_idx) {
		hist_cur = old_cur;
		ret = NULL;
	} else {
		ret = hist_list[hist_cur];
	}

	return ret;
}

static char *hist_next(void)
{
	char *ret;

	if (hist_cur < 0)
		return NULL;

	if (hist_cur == hist_add_idx)
		return NULL;

	if (++hist_cur > hist_max)
		hist_cur = 0;

	if (hist_cur == hist_add_idx)
		ret = "";
	else
		ret = hist_list[hist_cur];

	return ret;
}

#define BEGINNING_OF_LINE() {			\
	while (num) {				\
		getcmd_putch(chp, CTL_BACKSPACE);	\
		num--;				\
	}					\
}

#define ERASE_TO_EOL() {				\
	if (num < eol_num) {				\
		chprintf(chp, "%*s", (int)(eol_num - num), ""); \
		do {					\
			getcmd_putch(chp, CTL_BACKSPACE);	\
		} while (--eol_num > num);		\
	}						\
}

#define REFRESH_TO_EOL() {			\
	if (num < eol_num) {			\
		wlen = eol_num - num;		\
		putnstr(chp, buf + num, wlen);	\
		num = eol_num;			\
	}					\
}

static void cread_add_char(BaseSequentialStream *chp, char ichar, int insert, unsigned long *num,
	       unsigned long *eol_num, char *buf, unsigned long len)
{
	unsigned long wlen;

	/* room ??? */
	if (insert || *num == *eol_num) {
		if (*eol_num > len - 1) {
			getcmd_cbeep(chp);
			return;
		}
		(*eol_num)++;
	}

	if (insert) {
		wlen = *eol_num - *num;
		if (wlen > 1)
			memmove(&buf[*num+1], &buf[*num], wlen-1);

		buf[*num] = ichar;
		putnstr(chp, buf + *num, wlen);
		(*num)++;
		while (--wlen)
			getcmd_putch(chp, CTL_BACKSPACE);
	} else {
		/* echo the character */
		wlen = 1;
		buf[*num] = ichar;
		putnstr(chp, buf + *num, wlen);
		(*num)++;
	}
}

static void cread_add_str(BaseSequentialStream *chp, char *str, int strsize, int insert,
			  unsigned long *num, unsigned long *eol_num,
			  char *buf, unsigned long len)
{
	while (strsize--) {
		cread_add_char(chp, *str, insert, num, eol_num, buf, len);
		str++;
	}
}

int cread_line(BaseSequentialStream *chp, char *buf, unsigned int *len)
{
	unsigned long num = 0;
	unsigned long eol_num = 0;
	unsigned long wlen;
	char ichar;
	int insert = 1;
	int esc_len = 0;
	char esc_save[8];

	while (1) {
		//ichar = getcmd_getch(chp);
		if (streamRead(chp, (uint8_t *)&ichar, 1) == 0) { return -1; }
		if (ichar == '\r') {
			chprintf(chp, "\r\n");
			break;
		}

		/*
		 * handle standard linux xterm esc sequences for arrow key, etc.
		 */
		if (esc_len != 0) {
			if (esc_len == 1) {
				if (ichar == '[' || ichar == 'O') {
					esc_save[esc_len] = ichar;
					esc_len = 2;
				} else {
					cread_add_str(chp, esc_save, esc_len,
						      insert, &num, &eol_num,
						      buf, *len);
					esc_len = 0;
				}
				continue;
			}

			switch (ichar) {
			case 'D':	/* <- key */
				ichar = CTL_CH('b');
				esc_len = 0;
				break;
			case 'C':	/* -> key */
				ichar = CTL_CH('f');
				esc_len = 0;
				break;	/* pass off to ^F handler */
			case 'H':	/* Home key */
				ichar = CTL_CH('a');
				esc_len = 0;
				break;	/* pass off to ^A handler */
			case 'F':	/* End key */
				ichar = CTL_CH('e');
				esc_len = 0;
				break;	/* pass off to ^E handler */
			case 'A':	/* up arrow */
				ichar = CTL_CH('p');
				esc_len = 0;
				break;	/* pass off to ^P handler */
			case 'B':	/* down arrow */
				ichar = CTL_CH('n');
				esc_len = 0;
				break;	/* pass off to ^N handler */
			default:
				esc_save[esc_len++] = ichar;
				cread_add_str(chp, esc_save, esc_len, insert,
					      &num, &eol_num, buf, *len);
				esc_len = 0;
				continue;
			}
		}

		switch (ichar) {
		case 0x1b:
			if (esc_len == 0) {
				esc_save[esc_len] = ichar;
				esc_len = 1;
			} else {
				chprintf(chp, "impossible condition #876\n");
				esc_len = 0;
			}
			break;

		case CTL_CH('a'):
			BEGINNING_OF_LINE();
			break;
		case CTL_CH('c'):	/* ^C - break */
			*buf = '\0';	/* discard input */
			return -1;
		case CTL_CH('f'):
			if (num < eol_num) {
				getcmd_putch(chp, buf[num]);
				num++;
			}
			break;
		case CTL_CH('b'):
			if (num) {
				getcmd_putch(chp, CTL_BACKSPACE);
				num--;
			}
			break;
		case CTL_CH('d'):
			if (num < eol_num) {
				wlen = eol_num - num - 1;
				if (wlen) {
					memmove(&buf[num], &buf[num+1], wlen);
					putnstr(chp, buf + num, wlen);
				}

				getcmd_putch(chp, ' ');
				do {
					getcmd_putch(chp, CTL_BACKSPACE);
				} while (wlen--);
				eol_num--;
			}
			break;
		case CTL_CH('k'):
			ERASE_TO_EOL();
			break;
		case CTL_CH('e'):
			REFRESH_TO_EOL();
			break;
		case CTL_CH('o'):
			insert = !insert;
			break;
		case CTL_CH('x'):
		case CTL_CH('u'):
			BEGINNING_OF_LINE();
			ERASE_TO_EOL();
			break;
		case DEL:
		case DEL7:
		case 8:
			if (num) {
				wlen = eol_num - num;
				num--;
				memmove(&buf[num], &buf[num+1], wlen);
				getcmd_putch(chp, CTL_BACKSPACE);
				if(wlen) {
					putnstr(chp, buf + num, wlen);
				}
				getcmd_putch(chp, ' ');
				do {
					getcmd_putch(chp, CTL_BACKSPACE);
				} while (wlen--);
				eol_num--;
			}
			break;
		case CTL_CH('p'):
		case CTL_CH('n'):
		{
			char *hline;

			esc_len = 0;

			if (ichar == CTL_CH('p'))
				hline = hist_prev();
			else
				hline = hist_next();

			if (!hline) {
				getcmd_cbeep(chp);
				continue;
			}

			/* nuke the current line */
			/* first, go home */
			BEGINNING_OF_LINE();

			/* erase to end of line */
			ERASE_TO_EOL();

			/* copy new line into place and display */
			strcpy(buf, hline);
			eol_num = strlen(buf);
			REFRESH_TO_EOL();
			continue;
		}
#if defined(CONFIG_AUTO_COMPLETE) && CONFIG_AUTO_COMPLETE
		case '\t': {
			int num2, col;

			/* do not autocomplete when in the middle */
			if (num < eol_num) {
				getcmd_cbeep();
				break;
			}

			buf[num] = '\0';
			col = strlen(prompt) + eol_num;
			num2 = num;
			if (cmd_auto_complete(prompt, buf, &num2, &col)) {
				col = num2 - num;
				num += col;
				eol_num += col;
			}
			break;
		}
#endif
		default:
			cread_add_char(chp, ichar, insert, &num, &eol_num, buf,
				       *len);
			break;
		}
	}
	*len = eol_num;
	buf[eol_num] = '\0';	/* lose the newline */

	if (buf[0] && buf[0] != CREAD_HIST_CHAR)
		cread_add_to_hist(buf);
	hist_cur = hist_add_idx;

	return 0;
}

#else  /* CONFIG_CMDLINE_EDITING */

void hist_init(void) {}
void cread_print_hist_list(BaseSequentialStream *chp) {}


int cread_line(BaseSequentialStream *chp, char *line, unsigned int *len)
{
  unsigned size = *len;
  char *p = line;

  while (true) {
    char c;

    if (streamRead(chp, (uint8_t *)&c, 1) == 0)
      return -1;
    if (c == 4) {
      chprintf(chp, "^D");
      return -1;
    }
    if ((c == 8) || (c == 127)) {
      if (p != line) {
        streamPut(chp, 0x08);
        streamPut(chp, 0x20);
        streamPut(chp, 0x08);
        p--;
      }
      continue;
    }
    if (c == '\r') {
      chprintf(chp, "\r\n");
      *p = 0;
      return 0;
    }
    if (c < 0x20)
      continue;
    if (p < line + size - 1) {
      streamPut(chp, c);
      *p++ = (char)c;
    }
  }
}

#endif /* CONFIG_CMDLINE_EDITING */

