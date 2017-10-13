/*
 * pac1720.h
 *
 *  Created on: Jun 20, 2017
 *      Author: root
 */

#ifndef PAC1720_H_
#define PAC1720_H_

#include <ch.h>
#include <hal.h>

#include "i2c_exf.h"

#define DEV_ADR_0V95_3V3_DA1 0x2D // b0010 1101
#define DEV_ADR_1V50_1V8_DA2 0x2C // b0010 1100
#define DEV_ADR_TEMP_DA3     0x1E // b0001 1110

/* ********** ********** ********** ********** ********** ********** */
/* Description of PAC1720 address registers                           */
/* ********** ********** ********** ********** ********** ********** */

#define PAC1720_REG_CONFIG                 0x00
#define PAC1720_REG_CONV_RATE              0x01
#define PAC1720_REG_ONE_SHOT               0x02
#define PAC1720_REG_CHANEL_MSK             0x03

/* Status registers. Check conversion done */
#define PAC1720_REG_STATUS_HL              0x04
#define PAC1720_REG_STATUS_LL              0x05

/* Setup measurement sampling rates */
#define PAC1720_REG_SAMPLING_VSOURSE       0x0A
#define PAC1720_REG_SAMPLING_VSENSE_CH1    0x0B
#define PAC1720_REG_SAMPLING_VSENSE_CH2    0x0C

/* Values registers block */
#define PAC1720_REG_SENCE_VOLTAGE_CH1_HB   0x0D // Current values
#define PAC1720_REG_SENCE_VOLTAGE_CH1_LB   0x0E
#define PAC1720_REG_SENCE_VOLTAGE_CH2_HB   0x0F
#define PAC1720_REG_SENCE_VOLTAGE_CH2_LB   0x10

#define PAC1720_REG_VSOURCE_VOLTAGE_CH1_HB 0x11 // Voltage values
#define PAC1720_REG_VSOURCE_VOLTAGE_CH1_LB 0x12
#define PAC1720_REG_VSOURCE_VOLTAGE_CH2_HB 0x13
#define PAC1720_REG_VSOURCE_VOLTAGE_CH2_LB 0x14

#define PAC1720_REG_POWER_RATIO_CH1_HB     0x15 // Power values
#define PAC1720_REG_POWER_RATIO_CH1_LB     0x16
#define PAC1720_REG_POWER_RATIO_CH2_HB     0x17
#define PAC1720_REG_POWER_RATIO_CH2_LB     0x18

/* Setup Height and Low registers block */
#define PAC1720_REG_SENCE_VOLTAGE_CH1_HL   0x19 // Current limits
#define PAC1720_REG_SENCE_VOLTAGE_CH1_LL   0x1B
#define PAC1720_REG_SENCE_VOLTAGE_CH2_HL   0x1A
#define PAC1720_REG_SENCE_VOLTAGE_CH2_LL   0x1C

#define PAC1720_REG_VSOURCE_VOLTAGE_CH1_HL 0x1D // Voltage limits
#define PAC1720_REG_VSOURCE_VOLTAGE_CH1_LL 0x1F
#define PAC1720_REG_VSOURCE_VOLTAGE_CH2_HL 0x1E
#define PAC1720_REG_VSOURCE_VOLTAGE_CH2_LL 0x20

/* Id registers block */
#define PAC1720_REG_ID_PRODUCT             0xFD
#define PAC1720_REG_ID_SMSC                0xFE
#define PAC1720_REG_ID_REVISION            0xFF

/* Predefined values for registers*/
#define REG_CONFIG_DISABLE                 0x1B // 0x00011011
#define REG_CONFIG_ENABLE_ALL              0x00 // 0 (zero) - means ENABLE

/* Conversion Rate register values */
#define REG_CONV_RATE_1_PER_SEC            0x00
#define REG_CONV_RATE_2_PER_SEC            0x01
#define REG_CONV_RATE_4_PER_SEC            0x02
#define REG_CONV_RATE_C_PER_SEC            0x03 // Default value C - means continuous

// FSR register values
#define FSR_10MV                           0x00 // -10mV to 10 mV
#define FSR_20MV                           0x01 // -20mV to 20 mV
#define FSR_40MV                           0x02 // -40mV to 40 mV
#define FSR_80MV                           0x03 // -80mV to 80 mV

// Vsens dominator register values
#define VSENS_DOM_002v5MS_06BIT_63         0x00 // 63
#define VSENS_DOM_005v0MS_07BIT_127        0x01 // 127
#define VSENS_DOM_010v0MS_08BIT_255        0x02 // 255
#define VSENS_DOM_020v0MS_09BIT_511        0x03 // 511
#define VSENS_DOM_040v0MS_10BIT_1023       0x04 // 1023
#define VSENS_DOM_080v0MS_11BIT_2047       0x05 // 2047
#define VSENS_DOM_160v0MS_11BIT_2047       0x06 // 2047
#define VSENS_DOM_320v0MS_11BIT_2047       0x07 // 2047

// Vsource dominator register values
#define VSORCE_DOM_002v5MS_08BIT_256_255   0x00
#define VSORCE_DOM_005v0MS_09BIT_512_511   0x01
#define VSORCE_DOM_010v0MS_10BIT_1024_1023 0x02
#define VSORCE_DOM_020v0MS_11BIT_2048_2047 0x03

// This device have just 2 channels for measurement
typedef enum _channel_t
{
	CHANNEL1,
	CHANNEL2
}channel_t;

uint8_t getProductId(I2CDriver* i2cp, i2caddr_t addr);
uint8_t getSmscId(I2CDriver* i2cp, i2caddr_t addr);
uint8_t getRevisionId(I2CDriver* i2cp, i2caddr_t addr);

// Use this function to start and reset device
// This function reset and start device. It is OS dependent
void initDevice(I2CDriver* i2cp, i2caddr_t addr);

// This values is used by default
// Use this 3 functions to get values from sensors. The rest functions is additional do not use them directly
float readCurrentValue(I2CDriver* i2cp, i2caddr_t addr, channel_t ch, uint16_t rSens);
float readVoltageValue(I2CDriver* i2cp, i2caddr_t addr, channel_t ch);
float readPowerValue(I2CDriver* i2cp, i2caddr_t addr, channel_t ch, uint16_t rSens);

// Additional functions for calculation
uint16_t getVSensDominator(I2CDriver* i2cp, i2caddr_t addr, channel_t ch);
uint8_t  getVSensFSR(I2CDriver* i2cp, i2caddr_t addr, channel_t ch);
uint16_t getVSensValue(uint8_t highByte, uint8_t lowByte);

uint16_t getVSourceDominator1(I2CDriver* i2cp, i2caddr_t addr, channel_t ch);
uint16_t getVSourceDominator2(I2CDriver* i2cp, i2caddr_t addr, channel_t ch);
float    getVSourceValue(uint8_t highByte, uint8_t lowByte);

uint32_t getVPowerValue(uint8_t highByte, uint8_t lowByte);

void disableAllChannels(I2CDriver* i2cp, i2caddr_t addr);
void enableAllChannels(I2CDriver* i2cp, i2caddr_t addr);
void setConversionRate(I2CDriver* i2cp, i2caddr_t addr, uint8_t rate);

void setVSenceSamplingRegister(I2CDriver* i2cp, i2caddr_t addr, uint8_t value);

#endif /* PAC1720_H_ */
