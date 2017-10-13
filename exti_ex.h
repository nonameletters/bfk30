/*
 * exti_ex.h
 *
 *  Created on: Sep 26, 2017
 *      Author: root
 */

#ifndef EXTI_EX_H_
#define EXTI_EX_H_

#include <ch.h>
#include <hal.h>

extern volatile bool pwc_locked;
extern volatile int  pwc_state;

extern const EXTConfig extcfg;

extern virtual_timer_t bvt[2];

extern mailbox_t buttonPress_mb;

void pwc_init(void);

void buttonPowerOn(void);
void buttonPowerOff(void);
void buttonPowerToggle(void);

void buttonResetOn(void);
void buttonResetOff(void);
void buttonResetToggle(void);

void buttonsPowerResetOn(void);
void buttonsPowerResetOff(void);
void buttonsPowerResetToggle(void);
#endif /* EXTI_EX_H_ */
