/*
 * cli_readline.h
 *
 *  Created on: Nov 12, 2015
 *      Author: k.kranke
 */

#ifndef CLI_READLINE_H_
#define CLI_READLINE_H_

#include <ch.h>
#include <hal.h>

#ifdef __cplusplus
extern "C" {
#endif

int cread_line(BaseSequentialStream *chp, char *buf, unsigned int *len);
void cread_print_hist_list(BaseSequentialStream *chp);
void hist_init(void);

#ifdef __cplusplus
}
#endif

#endif /* CLI_READLINE_H_ */
