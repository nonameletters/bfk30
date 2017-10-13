/*
 * tps56.h
 *
 *  Created on: Sep 29, 2015
 *      Author: k.kranke
 */
#ifndef __tps56_h
#define __tps56_h

#include <ch.h>
#include <hal.h>
#include "chprintf.h"
#include "fxp.h"
#include "x.h"

// TPS56/65 settings is not permanent!!

#ifdef __cplusplus
extern "C" {
#endif


#define TPS56_VOUT		0	// 7-bit VOUT | parity; mV=(code+60)*10
#define TPS56_A			8	// A control reg
#define TPS56_INTERNAL	0x80	// reg TPS56_A
#define TPS56_A_DFLT 	((0<<7)|(3<<5)|(1<<4)|(0<<2)|(3<<0))
#define TPS56_B			9	// B control reg
#define TPS56_B_EN		0x80
#define TPS56_B_DFLT	((3<<4)|7)
#define TPS56_ST		24
#define TPS56_ST_PGOOD	1



/**
 * set/get voltage
 *  - 'voltage' value in units of 0.01 Volt
 *  - voltage should be in range [0.60; 1.87]
 */
void tps56_set_cV(const pmbus_dev_t* dev, unsigned cV);
uint8_t tps56_get_cV(const pmbus_dev_t* dev);
uint8_t tps56_status(const pmbus_dev_t* dev);

uint32_t tps56_dump(const pmbus_dev_t* dev);

//2.0

void tps65_set_cV(const pmbus_dev_t* dev, unsigned chan, unsigned cV);

bool clk25_init(void);
bool clk25_check_init(void);
bool clk25_enable(void);
bool clk25_disable(void);

#ifdef __cplusplus
}
#endif

#endif
