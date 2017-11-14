/*
 * i2c_exf.c
 *
 *  Created on: Oct 9, 2017
 *      Author: root
 */

#include "i2c_exf.h"


// ---------- ---------- ---------- ---------- ---------- ----------
uint8_t readByteBlock(I2CDriver* i2cp, i2caddr_t addr, uint8_t devReg)
{
	/* S | Sl_W_Addr | A | Dev_Reg | A | S | Sl_R_Addr | A | readByte | NA | P */
	uint8_t buf[1] = {0x00};
	msg_t result = MSG_OK;
	result = i2cMasterTransmitTimeout(i2cp, addr, &devReg, 1, buf, 1, 20);
	if (result == MSG_TIMEOUT)
	{

		i2cStop(i2cp);
		if (i2cp->i2c == I2C2)
		{
			i2cStart(i2cp, &i2cfg2);
		}
		else if (i2cp->i2c == I2C1)
		{
			i2cStart(i2cp, &i2cfg1);
		}
	}
	return buf[0];
}

// ---------- ---------- ---------- ---------- ---------- ----------
uint8_t readBlock(I2CDriver* i2cp, i2caddr_t addr, uint8_t devReg, uint8_t *outBuff, uint8_t outBuffSize)
{
	if (outBuff == NULL)
	{
		return 0;
	}

	msg_t result = MSG_OK;
	result = i2cMasterTransmitTimeout(i2cp, addr, &devReg, 1, outBuff, outBuffSize, 20);
	if (result == MSG_TIMEOUT)
	{

		i2cStop(i2cp);
		if (i2cp->i2c == I2C2)
		{
			i2cStart(i2cp, &i2cfg2);
		}
		else if (i2cp->i2c == I2C1)
		{
			i2cStart(i2cp, &i2cfg1);
		}
	}
	return outBuff[0];
}

// ---------- ---------- ---------- ---------- ---------- ----------
void writeByteBlock(I2CDriver* i2cp, i2caddr_t addr, uint8_t devReg, uint8_t byteData)
{
	/* S | Sl_W_Addr | A | Dev_Reg | A | byteData | A | P */
	uint8_t buf[2] = {0x00, 0x00};
	buf[0] = devReg;
	buf[1] = byteData;
	msg_t result = MSG_OK;
	result = i2cMasterTransmitTimeout(i2cp, addr, buf, 2, 0, 0, 20);
	if (result == MSG_TIMEOUT)
	{

		i2cStop(i2cp);
		if (i2cp->i2c == I2C2)
		{
			i2cStart(i2cp, &i2cfg2);
		}
		else if (i2cp->i2c == I2C1)
		{
			i2cStart(i2cp, &i2cfg1);
		}
	}
}
