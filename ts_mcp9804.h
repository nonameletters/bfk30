/*
 * ts_mcp9804.h
 *
 *  Created on: Nov 10, 2017
 *      Author: root
 */

#ifndef TS_MCP9804_H_
#define TS_MCP9804_H_

#include "i2c_exf.h"

#define ADR_TEMP_SENSE    0x1E

#define REG_MANUFACTOR_ID 0x06
#define REG_DEV_REV_ID    0x07
#define REG_TEMPERATURE   0x05

uint16_t getTSensorManufactorId(void);
uint8_t  getTSensorDeviceId(void);
uint8_t  getTSensorRevisionId(void);

float    getTSensorTemperature(void);
#endif /* TS_MCP9804_H_ */
