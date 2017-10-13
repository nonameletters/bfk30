/*
 * main.h
 *
 *  Created on: Sep 28, 2017
 *      Author: root
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "cli_commands.h"

enum blinkStates
{
	BS_IDLE,
	BS_FLASH,
	BS_ERROR
};

void cmd_cg(BaseSequentialStream *chp, int argc, char *argv[]);

//// Buffer for Power and Reset  messages
//static msg_t buttonsPress_msgBuf[8];
//MAILBOX_DECL(buttonPress_mb, buttonsPress_msgBuf, sizeof(buttonsPress_msgBuf)/sizeof(buttonsPress_msgBuf[0]));

void cmd_printCgValues(BaseSequentialStream *chp, int argc, char *argv[]);



int8_t asciiToHex(uint8_t ascii);

#endif /* MAIN_H_ */
