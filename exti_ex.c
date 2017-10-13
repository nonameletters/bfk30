/*
 * exti_ex.c
 *
 *  Created on: Sep 26, 2017
 *      Author: root
 */

#include "exti_ex.h"

volatile bool pwc_locked;
volatile int  pwc_state;

virtual_timer_t bvt[2];

// Buffer for Power and Reset  messages
msg_t buttonsPress_msgBuf[8];
MAILBOX_DECL(buttonPress_mb, buttonsPress_msgBuf, sizeof(buttonsPress_msgBuf)/sizeof(buttonsPress_msgBuf[0]));

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
static void timerHook(void *arg)
{
	(void) arg;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// RESET button pressed PB1 port
static void exti_baikalReset(EXTDriver *extp, expchannel_t channel)
{
  (void)extp;
  chSysLockFromISR();
	  if (!chVTIsArmedI(&bvt[channel])) // first press
	  {
		// signal
		chMBPostI(&buttonPress_mb, (msg_t)(2));
		chVTResetI(&bvt[channel]);
		chVTDoSetI(&bvt[channel], MS2ST(300), timerHook, (void*)channel);
	  }
	  /* debounce 200mS.*/
//	  chVTResetI(&bvt[channel]);
//	  chVTDoSetI(&bvt[channel], MS2ST(300), btn_do, (void*)channel);

  chSysUnlockFromISR();

//  chSysLockFromISR();
//	  if (!chVTIsArmedI(&bvt[channel])) {   // first press
//		// signal
//		chMBPostI(&pwc_mb, (msg_t)(powerctl_rb+channel));
//	  }
//	  /* debounce 200mS.*/
//	  chVTResetI(&bvt[channel]);
//	  chVTDoSetI(&bvt[channel], MS2ST(300), baikalResetBtnPressed, (void*)channel);
//  chSysUnlockFromISR();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
/* Triggered when the button is pressed or released. */
// POWER_ON button pressed
static void extcb_rb(EXTDriver *extp, expchannel_t channel)
{
  (void)extp;
  chSysLockFromISR();
	  if (!chVTIsArmedI(&bvt[channel])) // first press
	  {
		// signal
		chMBPostI(&buttonPress_mb, (msg_t)(1));
		chVTResetI(&bvt[channel]);
		chVTDoSetI(&bvt[channel], MS2ST(300), timerHook, (void*)channel);
	  }
	  /* debounce 200mS.*/
//	  chVTResetI(&bvt[channel]);
//	  chVTDoSetI(&bvt[channel], MS2ST(300), btn_do, (void*)channel);

  chSysUnlockFromISR();
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
const EXTConfig extcfg =
{
  {
    {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOB, exti_baikalReset}, // PB0 RESET
    {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOB, extcb_rb},	       // PB1 POWER ON/OFF
    {EXT_CH_MODE_DISABLED, NULL},  // MIPS R# (D) SYS_OK (A)
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}, //4
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}, // 7 ATX Power GOOD
    {EXT_CH_MODE_DISABLED, NULL}, //8
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}, //12
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}, //16
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}
  }
};

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void pwc_init(void)
{
    pwc_state = 0;
    pwc_locked = 0;

    // Reset virtual timers
    for (unsigned i=0; i < sizeof(bvt)/sizeof(bvt[0]); ++i)
    {
        chVTReset(&bvt[i]);
    }

    extStart(&EXTD1, &extcfg);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void buttonPowerOn(void)
{
	EXTI->IMR |= 0x00000002; // TurnOff PB1 - Power button
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void buttonPowerOff(void)
{
	EXTI->IMR &= ~0x00000002; // TurnOff PB1 - Power button
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void buttonPowerToggle(void)
{
	EXTI->IMR ^= 0x00000002; // TurnOff PB1 - Power button
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void buttonResetOn(void)
{
	EXTI->IMR |= 0x00000001; // TurnOff PB0 - Reset button
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void buttonResetOff(void)
{
	EXTI->IMR &= ~0x00000001; // TurnOff PB0 - Reset button
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void buttonResetToggle(void)
{
	EXTI->IMR ^= 0x00000001; // TurnOff PB0 - Reset button
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void buttonsPowerResetOn(void)
{
	buttonResetOn();
	buttonPowerOn();
//	EXTI->IMR |= 0x00000001; // TurnOff PB0 - Reset button
//	EXTI->IMR |= 0x00000002; // TurnOff PB1 - Power button
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void buttonsPowerResetOff(void)
{
	buttonResetOff();
	buttonPowerOff();
//	EXTI->IMR &= ~0x00000001; // TurnOff PB0 - Reset button
//	EXTI->IMR &= ~0x00000002; // TurnOff PB1 - Power button
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void buttonsPowerResetToggle(void)
{
	buttonResetToggle();
	buttonPowerToggle();
//	EXTI->IMR ^= 0x00000001; // TurnOff PB0 - Reset button
//	EXTI->IMR ^= 0x00000002; // TurnOff PB1 - Power button
}
