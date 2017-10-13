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
	i2cMasterTransmitTimeout(i2cp, addr, &devReg, 1, buf, 1, 20);
	return buf[0];
}

// ---------- ---------- ---------- ---------- ---------- ----------
void writeByteBlock(I2CDriver* i2cp, i2caddr_t addr, uint8_t devReg, uint8_t byteData)
{
	/* S | Sl_W_Addr | A | Dev_Reg | A | byteData | A | P */
	uint8_t buf[2] = {0x00, 0x00};
	buf[0] = devReg;
	buf[1] = byteData;
	i2cMasterTransmitTimeout(i2cp, addr, buf, 2, 0, 0, 20);
}
