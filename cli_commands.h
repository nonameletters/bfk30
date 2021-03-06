/*
 * cli_commands.h
 *
 *  Created on: Oct 11, 2017
 *      Author: root
 */

#ifndef CLI_COMMANDS_H_
#define CLI_COMMANDS_H_

#include <ch.h>
#include <hal.h>

#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include "shell.h"
#include "spi_flash.h"
#include "pac1720.h"
#include "chprintf.h"
#include "cg_5p49v5901.h"
#include "ts_mcp9804.h"
#include "sdu.h"

#define RESET_HOLD_TIME_MS 500

typedef struct asciiTable
{
	uint8_t asciiHex;
	uint8_t decCharHex;
} asciiTable_t;

void createSensePrintThread(void);

void cmd_power(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_rcg(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_cg(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_cgInfo(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_cgId(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_printCgValues(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_baikalReset(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_sense(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_idt(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_spi(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_flash(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_readFlash(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_bdfu(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_pr(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_bootCfg(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_tmpSense(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_mezOnOff(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_baikalSpeed(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_toggleEnPwrRefClk(BaseSequentialStream *chp, int argc, char *argv[]);

void otpProrCfg0(BaseSequentialStream *chp);

extern const ShellCommand commands[];
extern sFLASH_USBDriver SFDU1;

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// Power commands later must be moved to separate file
void powerOn(void);
void powerOff(void);
void powerToggle(void);
void baikalReset(void);

void mezOn(void);
void mezOff(void);
void mezToggle(void);

void printPowerStatus(void);
void printStatusMessage(const char *msg, uint32_t status, const char *good, const char *bad);
#endif /* CLI_COMMANDS_H_ */
