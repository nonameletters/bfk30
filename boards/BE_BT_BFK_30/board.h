/*
 * Board connections
 *
 * Copyright (C) 2016, 2017 Baikal Electronics.
 *
 * Author: Alexander Osipenko <Alexander.Osipenko@baikalelectronics.ru>
 */

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for Baikal Electronics BFK-16 board.
 */

/*
 * Board identifier.
 */
#if !defined(BOARD_BE_BT_BFK30)
	#define BOARD_BE_BT_BFK30
#endif

#if defined(BOARD_BE_BT_BFK30)
	#define BOARD_NAME              "BE-BT-BFK-30"
	#define INA_SENSOR_COUNT		3
	#define TPS_CHANNEL_COUNT		3
#elif defined(BOARD_BE_BT_BFK16)
	#define BOARD_NAME              "BE-BT-BFK16"
	#define INA_SENSOR_COUNT		3
	#define TPS_CHANNEL_COUNT		1
#endif

/*
 * Board frequencies.
 * NOTE: The HSE crystal is not fitted by default on the board.
 */
#define STM32_HSECLK            24000000

/*
 * MCU type as defined in the ST header file stm32f2xx.h.
 */

#define STM32F2XX
#define STM32F2
#define STM32F205xx		// for Chibios 14.0.0 compatibility

#define STM32_VDD               330U

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0 << ((n) * 2))
#define PIN_MODE_OUTPUT(n)          (1 << ((n) * 2))
#define PIN_MODE_ALTERNATE(n)       (2 << ((n) * 2))
#define PIN_MODE_ANALOG(n)          (3 << ((n) * 2))
#define PIN_OTYPE_PUSHPULL(n)       (0 << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1 << (n))
#define PIN_OSPEED_2M(n)            (0 << ((n) * 2))
#define PIN_OSPEED_25M(n)           (1 << ((n) * 2))
#define PIN_OSPEED_50M(n)           (2 << ((n) * 2))
#define PIN_OSPEED_100M(n)          (3 << ((n) * 2))
#define PIN_PUDR_FLOATING(n)        (0 << ((n) * 2))
#define PIN_PUDR_PULLUP(n)          (1 << ((n) * 2))
#define PIN_PUDR_PULLDOWN(n)        (2 << ((n) * 2))
#define PIN_AFIO_AF(n, v)           ((v##U) << ((n % 8) * 4))

/*
 * Port A setup.
 * All input with pull-up except:
 * PA1  - UCD_RESET# (out, ext pullup)
 * PA2	- SYSTEM_OK (in)
 * PA3	- I2C MUX RST#	(out, = 1)		1.6=NC
 * PA4  - Output (SPI1 CS#).
 * PA5  - SPI1_SCK      (alternate 5).
 * PA6  - SPI1_MISO     (alternate 5).
 * PA7  - SPI1_MOSI     (alternate 5).
 * PA8	- I2C3_GP_SCL	(alternate 4) - ext pullup
 * PA9  - VBUS_USB		(OTG_FS_VBUS)  1.6 - input
 * PA10 - USB_ID        (alternate 10) 1.6=GND-100k
 * PA11 - USB_N         (alternate 10).
 * PA12 - USB_P         (alternate 10).
 * PA13 - JTMS/SWDAT    (alternate 0).
 * PA14 - JTCK/SWCLK    (alternate 0).
 * PA15 - JTDI          (alternate 0).
 */

#define VAL_GPIOA_MODER             (PIN_MODE_OUTPUT(1)       | \
                                     PIN_MODE_OUTPUT(2)       | \
                                     PIN_MODE_OUTPUT(3)       | \
                                     PIN_MODE_OUTPUT(4)       | \
                                     PIN_MODE_ALTERNATE(5)    | \
                                     PIN_MODE_ALTERNATE(6)    | \
                                     PIN_MODE_ALTERNATE(7)    | \
                                     PIN_MODE_ALTERNATE(8)    | \
                                     PIN_MODE_INPUT(9)        | \
                                     PIN_MODE_ALTERNATE(10)   | \
                                     PIN_MODE_ALTERNATE(11)   | \
                                     PIN_MODE_ALTERNATE(12)   | \
                                     PIN_MODE_ALTERNATE(13)   | \
                                     PIN_MODE_ALTERNATE(14)   | \
                                     PIN_MODE_ALTERNATE(15))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_OPENDRAIN(8))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_50M(5)        | \
                                     PIN_OSPEED_50M(6)        | \
                                     PIN_OSPEED_50M(7)        | \
                                     PIN_OSPEED_2M(8)         | \
                                     PIN_OSPEED_100M(11)      | \
                                     PIN_OSPEED_100M(12))
#define VAL_GPIOA_PUPDR             (PIN_PUDR_FLOATING(0)     | \
                                     PIN_PUDR_PULLDOWN(1)     | \
                                     PIN_PUDR_PULLDOWN(2)     | \
                                     PIN_PUDR_PULLDOWN(3)     | \
                                     PIN_PUDR_FLOATING(4)     | \
                                     PIN_PUDR_FLOATING(5)     | \
                                     PIN_PUDR_FLOATING(6)     | \
                                     PIN_PUDR_FLOATING(7)     | \
                                     PIN_PUDR_FLOATING(8)     | \
                                     PIN_PUDR_FLOATING(9)     | \
                                     PIN_PUDR_FLOATING(10)    | \
                                     PIN_PUDR_FLOATING(11)    | \
                                     PIN_PUDR_FLOATING(12)    | \
                                     PIN_PUDR_FLOATING(13)    | \
                                     PIN_PUDR_FLOATING(14)    | \
                                     PIN_PUDR_FLOATING(15))
#define VAL_GPIOA_ODR                (0x00000100)
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(5, 5)        | \
                                     PIN_AFIO_AF(6, 5)        | \
                                     PIN_AFIO_AF(7, 5))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(8, 4) | \
                                     PIN_AFIO_AF(10, 10)      | \
                                     PIN_AFIO_AF(11, 10)      | \
                                     PIN_AFIO_AF(12, 10))

/*
 * Port B setup.
 * PB0  - BUTTON0 RST   (gpio input).	// ext pullup
 * PB1  - BUTTON1 PWR   (gpio input).	// ext pullup
 * PB2	- BOOT1 (100R GND)
 * PB3  - JTDO          (alternate 0).
 * PB4  - JNTRST        (alternate 0).
 * PB5  - I2C1_SMBA     (alternate 4).	// ext pullup
 * PB6  - I2C1_SCL      (alternate 4).	// ext pullup
 * PB7  - I2C1_SDA      (alternate 4).	// ext pullup
 * PB8  - Output MIPS BOOTCFG1 (inverted)
 * PB9  - Output (SPI2 CS#). 			- 1.6-NC
 * PB10 - I2C2_SCL      (alternate 4).	// ext pullup
 * PB11 - I2C2_SDA      (alternate 4).	// ext pullup
 * PB12 - I2C2_SMBA     (alternate 4).	// ext pullup
 * PB13 - EN_3V3_MEZ
 * PB14 - SPI2_MISO     (alternate 5).	- 1.6-NC  2.0: pin J8
 * PB15 - SPI2_MOSI     (alternate 5).	- 1.6-NC  2.0: EN_12V_SW (active 0, ext pullup)
 */
#define VAL_GPIOB_MODER             (PIN_MODE_INPUT(1)        | \
                                     PIN_MODE_ALTERNATE(3)    | \
                                     PIN_MODE_ALTERNATE(4)    | \
                                     PIN_MODE_ALTERNATE(5)    | \
                                     PIN_MODE_ALTERNATE(6)    | \
                                     PIN_MODE_ALTERNATE(7)    | \
                                     PIN_MODE_OUTPUT(8)       | \
                                     PIN_MODE_OUTPUT(9)       | \
                                     PIN_MODE_ALTERNATE(10)   | \
                                     PIN_MODE_ALTERNATE(11)   | \
                                     PIN_MODE_ALTERNATE(12)   | \
                                     PIN_MODE_OUTPUT(13)      | \
                                     PIN_MODE_OUTPUT(14)      | \
                                     PIN_MODE_OUTPUT(15))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(1)    | \
                                     PIN_OTYPE_OPENDRAIN(5)   | \
                                     PIN_OTYPE_OPENDRAIN(6)   | \
                                     PIN_OTYPE_OPENDRAIN(7)   | \
									 PIN_OTYPE_PUSHPULL(8)    | \
                                     PIN_OTYPE_OPENDRAIN(10)  | \
                                     PIN_OTYPE_OPENDRAIN(11)  | \
                                     PIN_OTYPE_OPENDRAIN(12)  | \
									 PIN_OTYPE_PUSHPULL(13)   | \
                                     PIN_OTYPE_OPENDRAIN(15))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_100M(1)       | \
                                     PIN_OSPEED_2M(5)         | \
                                     PIN_OSPEED_2M(6)         | \
                                     PIN_OSPEED_2M(7)         | \
									 PIN_OSPEED_2M(8)         | \
                                     PIN_OSPEED_2M(10)        | \
                                     PIN_OSPEED_2M(11)        | \
                                     PIN_OSPEED_2M(12)        | \
                                     PIN_OSPEED_100M(13)      | \
                                     PIN_OSPEED_100M(14)      | \
                                     PIN_OSPEED_2M(15))
#define VAL_GPIOB_PUPDR             (0x00000000)
#define VAL_GPIOB_ODR               (0x00000000)
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(5, 4)        | \
                                     PIN_AFIO_AF(6, 4)        | \
                                     PIN_AFIO_AF(7, 4))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(10, 4)       | \
                                     PIN_AFIO_AF(11, 4)       | \
                                     PIN_AFIO_AF(12, 4))

#ifdef BOARD_BE_BT_BFK30
	#define BOOT_FLASH_SEL_BIT 0x00000000		// active high??
#else
	#define BOOT_FLASH_SEL_BIT 0x80000000
#endif

/*
 * Port C setup.
 * All input with pull-up except:
 * PC0	- BMC_TSTSEL3		 (gpio output:0, PD(ext)) 2.0 (HIGH=1/2freq) 1.6-NC
 * PC1	- pll_plus50		 (gpio output:0) 2.0	1.6-NC
 * PC2	- pll_plus100		 (gpio output:0) 2.0	1.6-NC
 * PC3 NC
 * PC4  - UCD Notify         (gpio output) - 2.0: EN_3.3
 * PC5  - UCD Ctrl           (gpio output).
 * PC6  - ATX Power ON       (gpio output).
 * PC7  - ATX Power Good     (gpio input).
 * PC8	- EN_FUSE_PROG		 (gpio out 0) 2.0 1.6-NC
 * PC9	- I2C3_GP_SDA		 (alt 4 ext pullup)
 * PC10 - PWRGD_VTT
 * PC11 - PWRGD_0V95
 * PC12 - LED GREEN          (gpio output).
 * PC13 - PCIe SW prog       (gpio output). 1.6-NC
 * PC14 - LED RED            (gpio output). 1.6-NC
 * PC15 - Boot flash prog    (gpio output OD).		2.0 temp disable (input)
 */
#define VAL_GPIOC_MODER             (PIN_MODE_OUTPUT(0)       | \
									 PIN_MODE_OUTPUT(1)       | \
									 PIN_MODE_OUTPUT(2)       | \
									 PIN_MODE_OUTPUT(3)       | \
									 PIN_MODE_OUTPUT(4)       | \
									 PIN_MODE_OUTPUT(5)       | \
                                     PIN_MODE_OUTPUT(6)       | \
                                     PIN_MODE_INPUT(7)        | \
                                     PIN_MODE_OUTPUT(8)       | \
                                     PIN_MODE_ALTERNATE(9)    | \
                                     PIN_MODE_INPUT(10)       | \
                                     PIN_MODE_INPUT(11)       | \
                                     PIN_MODE_OUTPUT(12)      | \
                                     PIN_MODE_OUTPUT(13)      | \
                                     PIN_MODE_OUTPUT(14)      | \
                                     PIN_MODE_OUTPUT(15))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_OPENDRAIN(1)   | \
									 PIN_OTYPE_OPENDRAIN(2)   | \
									 PIN_OTYPE_PUSHPULL(3)    | \
									 PIN_OTYPE_PUSHPULL(6)    | \
									 PIN_OTYPE_OPENDRAIN(9)   | \
									 PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOC_OSPEEDR           0x00000000
#define VAL_GPIOC_PUPDR             (PIN_PUDR_PULLDOWN(3)     | \
									 PIN_PUDR_FLOATING(5)     | \
                                     PIN_PUDR_PULLUP(6)       | \
                                     PIN_PUDR_PULLUP(7)       | \
                                     PIN_PUDR_FLOATING(9)     | \
									 PIN_PUDR_PULLUP(10)      | \
									 PIN_PUDR_PULLUP(11)      | \
                                     PIN_PUDR_FLOATING(12)    | \
                                     PIN_PUDR_FLOATING(13)    | \
                                     PIN_PUDR_FLOATING(14)    | \
                                     PIN_PUDR_FLOATING(15))
//#define VAL_GPIOC_ODR               ((0x7FFFFFFF&(~((1<<8)|(1<<0))))|(BOOT_FLASH_SEL_BIT))
//#define VAL_GPIOC_ODR               0x00000C86
#define VAL_GPIOC_ODR               0x00000240
#define VAL_GPIOC_AFRL				0x00000000
#define VAL_GPIOC_AFRH              PIN_AFIO_AF(9, 4) //0x00000000

/*
 * #define VAL_GPIOC_AFRH				(PIN_AFIO_AF(9, 4)       | \
 *                                       PIN_AFIO_AF(10, 7)      | \
 *                                       PIN_AFIO_AF(11, 7))
 */


/*
 * Port D setup.
 * All input with pull-up.
 * PD0  - OSC_INT     (input floating).
 * PD1  - OSC_OUT     (input floating).
 * PD2	- CPU_RESET#  (opendrain)
 */
#define VAL_GPIOD_MODER             (PIN_MODE_OUTPUT(0)       | \
									 PIN_MODE_OUTPUT(2)       | \
									 PIN_MODE_OUTPUT(3))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(0)    | \
                                     PIN_OTYPE_PUSHPULL(2)    | \
									 PIN_OTYPE_PUSHPULL(3))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_100M(2)       | \
		                             PIN_OSPEED_100M(3))
#define VAL_GPIOD_PUPDR             (PIN_PUDR_FLOATING(0)     | \
                                     PIN_PUDR_FLOATING(1)     | \
                                     PIN_PUDR_PULLDOWN(2)     | \
									 PIN_PUDR_PULLDOWN(3))
#define VAL_GPIOD_ODR               0x00000002
//#define VAL_GPIOD_ODR               0x0000000A
#define VAL_GPIOD_AFRL				0x00000000
#define VAL_GPIOD_AFRH				0x00000000

/*
 * Port E setup.
 * All input with pull-up.
 */
#define VAL_GPIOE_MODER             0x00000000
#define VAL_GPIOE_OTYPER            0x00000000
#define VAL_GPIOE_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOE_PUPDR             0xFFFFFFFF
#define VAL_GPIOE_ODR               0xFFFFFFFF
#define VAL_GPIOE_AFRL				0x00000000
#define VAL_GPIOE_AFRH				0x00000000

/*
 * Port F setup.
 * All input with pull-up.
 */
#define VAL_GPIOF_MODER             0x00000000
#define VAL_GPIOF_OTYPER            0x00000000
#define VAL_GPIOF_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOF_PUPDR             0xFFFFFFFF
#define VAL_GPIOF_ODR               0xFFFFFFFF
#define VAL_GPIOF_AFRL				0x00000000
#define VAL_GPIOF_AFRH				0x00000000

/*
 * Port G setup.
 * All input with pull-up.
 */
#define VAL_GPIOG_MODER             0x00000000
#define VAL_GPIOG_OTYPER            0x00000000
#define VAL_GPIOG_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOG_PUPDR             0xFFFFFFFF
#define VAL_GPIOG_ODR               0xFFFFFFBF
#define VAL_GPIOG_AFRL				0x00000000
#define VAL_GPIOG_AFRH				0x00000000

/*
 * Port H setup.
 * All input with pull-up.
 */
#define VAL_GPIOH_MODER             0x00000000
#define VAL_GPIOH_OTYPER            0x00000000
#define VAL_GPIOH_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOH_PUPDR             0xFFFFFFFF
#define VAL_GPIOH_ODR               0xFFFFFFFF
#define VAL_GPIOH_AFRL				0x00000000
#define VAL_GPIOH_AFRH				0x00000000

/*
 * Port I setup.
 * All input with pull-up.
 */
#define VAL_GPIOI_MODER             0x00000000
#define VAL_GPIOI_OTYPER            0x00000000
#define VAL_GPIOI_OSPEEDR           0xFFFFFFFF
#define VAL_GPIOI_PUPDR             0xFFFFFFFF
#define VAL_GPIOI_ODR               0xFFFFFFFF
#define VAL_GPIOI_AFRL				0x00000000
#define VAL_GPIOI_AFRH				0x00000000


#define MIPS_nBOOTCFG1_PORT GPIOB
#define MIPS_nBOOTCFG1_PIN  8

#define SYSTEM_OK_PORT		GPIOA
#define SYSTEM_OK_PIN		2

#define MIPS_RESETn_PORT	GPIOD
#define MIPS_RESETn_PIN		2

#define I2C1_SMBA_PORT		GPIOB
#define I2C1_SMBA_PIN		5
#define I2C1_SCL_PORT		GPIOB
#define I2C1_SCL_PIN		6
#define I2C1_SDA_PORT		GPIOB
#define I2C1_SDA_PIN		7

#define I2C2_SCL_PORT		GPIOB
#define I2C2_SCL_PIN		10
#define I2C2_SDA_PORT		GPIOB
#define I2C2_SDA_PIN		11
#define I2C2_SMBA_PORT		GPIOB
#define I2C2_SMBA_PIN		12

#define I2C3_SCL_PORT		GPIOA
#define I2C3_SCL_PIN		8
#define I2C3_SDA_PORT		GPIOC
#define I2C3_SDA_PIN		9

#define SPI1_CS_PORT		GPIOA
#define SPI1_CS_PIN			4

#define BOOT_SPI_EN_PORT	GPIOC
#define BOOT_SPI_EN_PIN		15

#define SEQ_CNTRL_PORT      GPIOC
#define SEQ_CNTRL_PIN       5
#define SEQ_NOTIFY_PORT     GPIOC
#define SEQ_NOTIFY_PIN      4
#define SEQ_nRESET_PORT     GPIOA
#define SEQ_nRESET_PIN      1


#define POWER_BTN_PORT      GPIOB
#define POWER_BTN_PIN       1
#define RESET_BTN_PORT      GPIOB
#define RESET_BTN_PIN       0

#define ATX_POWER_ON_PORT   GPIOC
#define ATX_POWER_ON_PIN    6
#define ATX_POWER_OK_PORT   GPIOC
#define ATX_POWER_OK_PIN    7
//NOTE: ATX_POWER_OK exsercise glitches when activating, throwing spurious interrupts


#define LED_PORT			GPIOC
#define LED_GREEN_PIN   	12
#define LED_RED_PIN   	    14


#define ATX_POWER_ON()			(palClearPad(ATX_POWER_ON_PORT, ATX_POWER_ON_PIN))
#define ATX_POWER_OFF()			(palSetPad(ATX_POWER_ON_PORT, ATX_POWER_ON_PIN))
#define ATX_POWER_TOGGLE()		(palTogglePad(ATX_POWER_ON_PORT, ATX_POWER_ON_PIN))

#define ATX_POWER_GOOD_STATE()	(palReadPad(ATX_POWER_OK_PORT, ATX_POWER_OK_PIN))
#define ATX_POWER_ON_STATE()	(!palReadPad(ATX_POWER_ON_PORT, ATX_POWER_ON_PIN))


#define MIPS_RESET_ASSERT()		(palClearPad(MIPS_RESETn_PORT,MIPS_RESETn_PIN))
#define MIPS_RESET_RELEASE()	(palSetPad(MIPS_RESETn_PORT,MIPS_RESETn_PIN))

#define I2C_BUS1				0x100
#define I2C_BUS2				0x200
#define I2C_BUS3				0x300

#define I2C_ADDR_SQNR			(0x75|I2C_BUS1)

#ifdef	BOARD_BE_BT_BFK30
	#define I2C_ADDR_BUSMUX2		(0x70)	// I2C_BUS2 I2CD2 PCA9547 2.0
	#define I2C_BUSMUX2_RST_PORT	GPIOA
	#define I2C_BUSMUX2_RST_PIN		3

	#define I2C_ADDR_TPS65263		(I2C_BUS2|0x60)
	#define I2C_ADDR_LTC_1p5_PCIE	(0x1000|I2C_BUS2|0x66)
	#define I2C_ADDR_LTC_1p5_XGBE	(0x2000|I2C_BUS2|0x66)
	#define I2C_ADDR_LTC_1p8_SATA	(0x4000|I2C_BUS2|0x66)
	#define I2C_ADDR_LTC_1p8_DDR	(0x3000|I2C_BUS2|0x66)

	#define I2C_ADDR_SI5342			(I2C_BUS3|0x6B)	// 0x68|3 CLK156: J30(I2C3)-J73-J1(I2C1)  SDA 2-1-10   SCL 3-2-9
	#define I2C_ADDR_SI5351			(I2C_BUS1|0x60) // CLK25 Gen
	#define I2C_ADDR_MCP9804		(I2C_BUS1|0x1E)	// BUS1 100kHz Temperature sensor
	#define I2C_ADDR_SC18IS602		(I2C_BUS1|0x28)	// BUS1 400kHz SPI bridge - RTD I/F

	// J30 - J43 bridge
	#define I2C_ADDR_RTC			(I2C_BUS3|0x56)
	#define I2C_ADDR_SN				(I2C_BUS3|0x5C)
	#define I2C_ADDR_EEPROM			(I2C_BUS3|0x54)
	#define I2C_ADDR_DDR_SPD		(I2C_BUS3|0x50)

	#define EN12V_PORT				GPIOB
	#define EN12V_PIN				15
#endif



#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
