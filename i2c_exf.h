/*
 * i2c_exf.h
 *
 *  Created on: Oct 9, 2017
 *      Author: root
 */

#ifndef I2C_EXF_H_
#define I2C_EXF_H_

#include <ch.h>
#include <hal.h>


// This function starts with S condition and finishes with P condition
// use this function if you need to read ONLY one byte.
// you can't use it reading block by one byte
uint8_t readByteBlock(I2CDriver* i2cp, i2caddr_t addr, uint8_t devReg);

// This function starts with S condition and finishes with P condition
// this function sends ONLY one byte to device
void writeByteBlock(I2CDriver* i2cp, i2caddr_t addr, uint8_t devReg, uint8_t byteData);


#endif /* I2C_EXF_H_ */
