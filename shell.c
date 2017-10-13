/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    shell.c
 * @brief   Simple CLI shell code.
 *
 * @addtogroup SHELL
 * @{
 */

#include <string.h>

#include "ch.h"
#include "hal.h"
#include "shell.h"
#include "chprintf.h"
#include "cli_readline.h"

/**
 * @brief   Shell termination event source.
 */
#if CH_CFG_USE_EVENTS
event_source_t shell_terminated;
#endif

#if 0
#define _strtok strtok_r
#else
// Tokenize double quote strings
static char *_strtok(char *str, const char *delim, char **saveptr) {
  char *token;
  if (str)
    *saveptr = str;
  token = *saveptr;

  if (!token)
    return NULL;

  token += strspn(token, delim);
  if( *token == '"' ) {
    *saveptr = token;
    do {
      *saveptr = strpbrk(*saveptr+1, "\""); // and LEAVE " as first char
    } while( *saveptr && (*saveptr)[-1] == '\\' );
  }
  else
    *saveptr = strpbrk(token, delim);
  if (*saveptr)
    *(*saveptr)++ = '\0';

  return *token ? token : NULL;
}
#endif

static void usage(BaseSequentialStream *chp, char *p) {

  chprintf(chp, "Usage: %s\r\n", p);
}

static void list_commands(BaseSequentialStream *chp, const ShellCommand *scp)
{
  while (scp->sc_name != NULL)
  {
    if( scp->sc_name[0] != '.' )
    	chprintf(chp, " \r\ncommand: %s \t%s", scp->sc_name, scp->sc_desc);
    scp++;
  }
}

static void cmd_info(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    usage(chp, "info");
    return;
  }

  chprintf(chp, "Kernel:       %s\r\n", CH_KERNEL_VERSION);
#ifdef PORT_COMPILER_NAME
  chprintf(chp, "Compiler:     %s\r\n", PORT_COMPILER_NAME);
#endif
  chprintf(chp, "Architecture: %s\r\n", PORT_ARCHITECTURE_NAME);
#ifdef PORT_CORE_VARIANT_NAME
  chprintf(chp, "Core Variant: %s\r\n", PORT_CORE_VARIANT_NAME);
#endif
#ifdef PORT_INFO
  chprintf(chp, "Port Info:    %s\r\n", PORT_INFO);
#endif
#ifdef PLATFORM_NAME
  chprintf(chp, "Platform:     %s\r\n", PLATFORM_NAME);
#endif
#ifdef BOARD_NAME
  chprintf(chp, "Board:        %s\r\n", BOARD_NAME);
#endif
#ifdef __DATE__
#ifdef __TIME__
  chprintf(chp, "Build time:   %s%s%s\r\n", __DATE__, " - ", __TIME__);
#endif
#endif
#ifdef STM32F2XX
  chprintf(chp, "Device UUID:  %08X%08X%08X\r\n", *(uint32_t*)0x1FFF7A18,*(uint32_t*)0x1FFF7A14,*(uint32_t*)0x1FFF7A10);
#endif
#ifdef FIRMWARE_VERSION
  chprintf(chp, "FW Version:   %s\r\n", FIRMWARE_VERSION);
#endif
}

//static void cmd_systime(BaseSequentialStream *chp, int argc, char *argv[]) {
//
//  (void)argv;
//  if (argc > 0) {
//    usage(chp, "systime");
//    return;
//  }
//  chprintf(chp, "%lu\r\n", (unsigned long)chVTGetSystemTime());
//}

/**
 * @brief   Array of the default commands.
 */
static const ShellCommand const local_commands[] = {
  {"info", cmd_info},
//  {"systime", cmd_systime},
  {NULL, NULL}
};

static bool cmdexec(const ShellCommand *scp, BaseSequentialStream *chp,
                      char *name, int argc, char *argv[]) {

  while (scp->sc_name != NULL) {
    if (strcmp(scp->sc_name, name) == 0 || (scp->sc_name[0] == '.' && strcmp(scp->sc_name+1, name) == 0)) {
      scp->sc_function(chp, argc, argv);
      return false;
    }
    scp++;
  }
  return true;
}

/**
 * @brief   Shell thread function.
 *
 * @param[in] p         pointer to a @p BaseSequentialStream object
 */
static THD_FUNCTION(shell_thread, p) {
  int n;
  BaseSequentialStream *chp = ((ShellConfig *)p)->sc_channel;
  const ShellCommand *scp = ((ShellConfig *)p)->sc_commands;
  char *lp, *cmd, *tokp, line[SHELL_MAX_LINE_LENGTH];
  char *args[SHELL_MAX_ARGUMENTS + 1];
  unsigned int len;

  chRegSetThreadName("shell");
  chprintf(chp, "\r\nBaikal Electronics Shell\r\n");
  hist_init();
  while (true) {
    chprintf(chp, "bmc> ");

    len = SHELL_MAX_LINE_LENGTH;
    if (cread_line(chp, line, &len) < 0) {
      chprintf(chp, "\r\nlogout");
      break;
    }
    lp = _strtok(line, " \t", &tokp);
    cmd = lp;
    n = 0;
    while ((lp = _strtok(NULL, " \t", &tokp)) != NULL) {
      if (n >= SHELL_MAX_ARGUMENTS) {
        chprintf(chp, "too many arguments\r\n");
        cmd = NULL;
        break;
      }
      args[n++] = lp;
    }
    args[n] = NULL;
    if (cmd != NULL) {
      if (strcmp(cmd, "exit") == 0) {
        if (n > 0) {
          usage(chp, "exit");
          continue;
        }
        break;
      }
      if (strcmp(cmd, "history") == 0) {
        if (n > 0) {
          usage(chp, "history");
          continue;
        }
        cread_print_hist_list(chp);
      }
      else if (strcmp(cmd, "help") == 0) {
        if (n > 0) {
          usage(chp, "help");
          continue;
        }
        chprintf(chp, "Commands: help exit history ");
        list_commands(chp, local_commands);
        if (scp != NULL)
          list_commands(chp, scp);
        chprintf(chp, "\r\n");
      }
      else if (cmdexec(local_commands, chp, cmd, n, args) &&
          ((scp == NULL) || cmdexec(scp, chp, cmd, n, args))) {
        chprintf(chp, "%s", cmd);
        chprintf(chp, " ?\r\n");
      }
    }
  }
  shellExit(MSG_OK);
}

/**
 * @brief   Shell manager initialization.
 *
 * @api
 */
void shellInit(void) {

#if CH_CFG_USE_EVENTS
  chEvtObjectInit(&shell_terminated);
#endif
}

/**
 * @brief   Terminates the shell.
 * @note    Must be invoked from the command handlers.
 * @note    Does not return.
 *
 * @param[in] msg       shell exit code
 *
 * @api
 */
void shellExit(msg_t msg) {

  /* Atomically broadcasting the event source and terminating the thread,
     there is not a chSysUnlock() because the thread terminates upon return.*/
  chSysLock();
#if CH_CFG_USE_EVENTS
  chEvtBroadcastI(&shell_terminated);
#endif
  chThdExitS(msg);
}

/**
 * @brief   Spawns a new shell.
 * @pre     @p CH_CFG_USE_HEAP and @p CH_CFG_USE_DYNAMIC must be enabled.
 *
 * @param[in] scp       pointer to a @p ShellConfig object
 * @param[in] size      size of the shell working area to be allocated
 * @param[in] prio      priority level for the new shell
 * @return              A pointer to the shell thread.
 * @retval NULL         thread creation failed because memory allocation.
 *
 * @api
 */
#if CH_CFG_USE_HEAP && CH_CFG_USE_DYNAMIC
thread_t *shellCreate(const ShellConfig *scp, size_t size, tprio_t prio) {

  return chThdCreateFromHeap(NULL, size, prio, shell_thread, (void *)scp);
}
#endif

/**
 * @brief   Create statically allocated shell thread.
 *
 * @param[in] scp       pointer to a @p ShellConfig object
 * @param[in] wsp       pointer to a working area dedicated to the shell thread stack
 * @param[in] size      size of the shell working area
 * @param[in] prio      priority level for the new shell
 * @return              A pointer to the shell thread.
 *
 * @api
 */
thread_t *shellCreateStatic(const ShellConfig *scp, void *wsp,
                            size_t size, tprio_t prio) {

  return chThdCreateStatic(wsp, size, prio, shell_thread, (void *)scp);
}

/** @} */
