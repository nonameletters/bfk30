/*
 * ts_mcp9804.c
 *
 *  Created on: Nov 10, 2017
 *      Author: root
 */


#include "ts_mcp9804.h"

// ---------- ---------- ---------- ---------- ---------- ----------
uint16_t getTSensorManufactorId(void)
{
	volatile uint16_t result = 0;
	uint8_t buf[2] = {0, 0};

	readBlock(&I2CD1, ADR_TEMP_SENSE, REG_MANUFACTOR_ID, buf, 2);

	result |= buf[0];
	result <<= 8;
	result |= buf[1];
	return result;
}

// ---------- ---------- ---------- ---------- ---------- ----------
uint8_t  getTSensorDeviceId(void)
{
	uint8_t result = 0;
	uint8_t buf[2] = {0, 0};

	readBlock(&I2CD1, ADR_TEMP_SENSE, REG_DEV_REV_ID, buf, 2);

	result = buf[1];
	return result;
}

// ---------- ---------- ---------- ---------- ---------- ----------
uint8_t  getTSensorRevisionId(void)
{
	uint8_t result = 0;
	uint8_t buf[2] = {0, 0};

	readBlock(&I2CD1, ADR_TEMP_SENSE, REG_DEV_REV_ID, buf, 2);

	result = buf[0];
	return result;
}

// ---------- ---------- ---------- ---------- ---------- ----------
float    getTSensorTemperature(void)
{
	float result = 0;
	uint8_t buf[2] = {0, 0};

	readBlock(&I2CD1, ADR_TEMP_SENSE, REG_TEMPERATURE, buf, 2);

	buf[0] = buf[0] & 0x1F; // Clear 7, 6, 5 bits
	result = ((buf[0]&0x0F) * 16) + (buf[1] * 0.0625); // Conversion from DataSheet
	if ((buf[0] & 0x10) == 0x10)
	{
		result = (256 - result) * (-1);
	}
	return result;
}
