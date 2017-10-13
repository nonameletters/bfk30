/*
 * SMB support
 *
 * Copyright (C) 2016, 2017 Baikal Electronics.
 *
 * Author: Alexander Osipenko <Alexander.Osipenko@baikalelectronics.ru>
 */


#include <ch.h>
#include <hal.h>
#include <chprintf.h>

#include "smbus.h"
#include "pmbus_cmd_constants.h"

struct __attribute__((packed)) smb_log {
	uint8_t bus;
	uint8_t address;
	uint8_t command;
	uint8_t flags;
};

static const char* i2c_flags[] = {
"BUS_ERROR",         /**< @brief 01 Bus Error.           */
"ARBITRATION_LOST",  /**< @brief 02 Arbitration Lost.    */
"ACK_FAILURE",       /**< @brief 04 Acknowledge Failure. */
"OVERRUN",           /**< @brief 08 Overrun/Underrun.    */
"PEC_ERROR",         /**< @brief 10 PEC Error in reception.*/
"TIMEOUT",           /**< @brief 20 Hardware timeout.    */
"SMB_ALERT",         /**< @brief 40 SMBus Alert.         */
"80"
};

extern SerialUSBDriver SDU1;
bool smb_silent = false;
void smbWriteLog(I2CDriver *i2cp, i2caddr_t addr, uint8_t cmd, msg_t status)
{
	if ( usbGetDriverStateI(SDU1.config->usbp) == USB_ACTIVE && 
		(!smb_silent || smb_isShellThread()) ) {
		//aquire console ...

		uint8_t bus=0;
		if (i2cp == &I2CD1) bus=1;
		else if(i2cp == &I2CD2) bus=2;
		else if(i2cp == &I2CD3) bus=3;

		i2cflags_t flags = i2cGetErrors(i2cp);
		const char* st = "UNKNOWN";
		if (status == MSG_RESET) st="RESET";
		else if(status == MSG_TIMEOUT) st="TIMEOUT";

		chprintf((BaseSequentialStream*)&SDU1,"\r\nBUS=%d ADDR=%02X CMD=%02X STATUS=%s FLAGS:",bus,addr,cmd,st);
		for (unsigned i = 0; i < 8; ++i) {
			if( flags & (1<<i) ) {
				chprintf((BaseSequentialStream*)&SDU1, " %s", i2c_flags[i]);
			}
		}
		chprintf((BaseSequentialStream*)&SDU1, "\r\n");
	}
	i2cStop(i2cp);
	i2cStart(i2cp, i2cp->config);
}

#if 0
// PEC C(x) = x8 + x2 + x1 + 1
static const uint8_t pec_crc_table[] = {
    0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15, 0x38, 0x3f, 0x36, 0x31,
    0x24, 0x23, 0x2a, 0x2d, 0x70, 0x77, 0x7e, 0x79, 0x6c, 0x6b, 0x62, 0x65,
    0x48, 0x4f, 0x46, 0x41, 0x54, 0x53, 0x5a, 0x5d, 0xe0, 0xe7, 0xee, 0xe9,
    0xfc, 0xfb, 0xf2, 0xf5, 0xd8, 0xdf, 0xd6, 0xd1, 0xc4, 0xc3, 0xca, 0xcd,
    0x90, 0x97, 0x9e, 0x99, 0x8c, 0x8b, 0x82, 0x85, 0xa8, 0xaf, 0xa6, 0xa1,
    0xb4, 0xb3, 0xba, 0xbd, 0xc7, 0xc0, 0xc9, 0xce, 0xdb, 0xdc, 0xd5, 0xd2,
    0xff, 0xf8, 0xf1, 0xf6, 0xe3, 0xe4, 0xed, 0xea, 0xb7, 0xb0, 0xb9, 0xbe,
    0xab, 0xac, 0xa5, 0xa2, 0x8f, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9d, 0x9a,
    0x27, 0x20, 0x29, 0x2e, 0x3b, 0x3c, 0x35, 0x32, 0x1f, 0x18, 0x11, 0x16,
    0x03, 0x04, 0x0d, 0x0a, 0x57, 0x50, 0x59, 0x5e, 0x4b, 0x4c, 0x45, 0x42,
    0x6f, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7d, 0x7a, 0x89, 0x8e, 0x87, 0x80,
    0x95, 0x92, 0x9b, 0x9c, 0xb1, 0xb6, 0xbf, 0xb8, 0xad, 0xaa, 0xa3, 0xa4,
    0xf9, 0xfe, 0xf7, 0xf0, 0xe5, 0xe2, 0xeb, 0xec, 0xc1, 0xc6, 0xcf, 0xc8,
    0xdd, 0xda, 0xd3, 0xd4, 0x69, 0x6e, 0x67, 0x60, 0x75, 0x72, 0x7b, 0x7c,
    0x51, 0x56, 0x5f, 0x58, 0x4d, 0x4a, 0x43, 0x44, 0x19, 0x1e, 0x17, 0x10,
    0x05, 0x02, 0x0b, 0x0c, 0x21, 0x26, 0x2f, 0x28, 0x3d, 0x3a, 0x33, 0x34,
    0x4e, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5c, 0x5b, 0x76, 0x71, 0x78, 0x7f,
    0x6a, 0x6d, 0x64, 0x63, 0x3e, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2c, 0x2b,
    0x06, 0x01, 0x08, 0x0f, 0x1a, 0x1d, 0x14, 0x13, 0xae, 0xa9, 0xa0, 0xa7,
    0xb2, 0xb5, 0xbc, 0xbb, 0x96, 0x91, 0x98, 0x9f, 0x8a, 0x8d, 0x84, 0x83,
    0xde, 0xd9, 0xd0, 0xd7, 0xc2, 0xc5, 0xcc, 0xcb, 0xe6, 0xe1, 0xe8, 0xef,
    0xfa, 0xfd, 0xf4, 0xf3
};

uint8_t pec_crc(uint8_t *p, uint8_t len)
{
	uint8_t crc = 0x0;

    while (len--) {
        crc = pec_crc_table[crc ^ *p++];
    }

	return crc & 0xFF;
}
#endif

void smbTimerFunc(void* data) {
	chSysLockFromISR();
	//chVTSetI(&_vt, MS2ST(500), _cb, data);
	chBSemSignalI(&((I2CDriver*)data)->sem);
	chSysUnlockFromISR();
}

#ifdef I2C_ADDR_BUSMUX2
static bool smbWrite1(I2CDriver *i2cp, i2caddr_t addr, uint8_t cmd)
{
	uint8_t iobuf[1];
	iobuf[0] = cmd;
	msg_t status = i2cMasterTransmitTimeout(i2cp, addr,
                          iobuf, 1, 0, 0, MS2ST(4));
	if( status != MSG_OK ) {
		smbWriteLog(i2cp,addr,cmd,status);
		return false;
	}
	return true;
}
#endif

static I2CDriver* const smb_bus0[4] = {
    &I2CD1, &I2CD1, &I2CD2, &I2CD3
};

// Check for I2C lines is in the released state
unsigned smbBadBus(unsigned bus)
{
	bus &= 3;
	unsigned nr=0xF;
	i2cAcquireBus(smb_bus0[bus]);
	switch(bus) {
		case 0:
		case 1:
			nr = (!palReadPad(I2C1_SCL_PORT,I2C1_SCL_PIN)) | ((!palReadPad(I2C1_SDA_PORT,I2C1_SDA_PIN)) >> 1);
			break;
		case 2:
			nr = (!palReadPad(I2C2_SCL_PORT,I2C2_SCL_PIN)) | ((!palReadPad(I2C2_SDA_PORT,I2C2_SDA_PIN)) >> 1);
			break;
		case 3:
			nr = (!palReadPad(I2C3_SCL_PORT,I2C3_SCL_PIN)) | ((!palReadPad(I2C3_SDA_PORT,I2C3_SDA_PIN)) >> 1);
			break;
	}
	i2cReleaseBus(smb_bus0[bus]);
	return nr;
}


i2caddr_t smbSelectBus(I2CDriver **i2cp, i2caddr_t addr) {

	unsigned bus = SMB_ADDR_BUS_GET(addr);
	if (*i2cp == NULL) {
		*i2cp = smb_bus0[bus];
	}

	i2cAcquireBus(*i2cp);
	//TODO: wait only for the same device commands (ZL2102)
	chBSemWait(&(*i2cp)->sem);

#ifdef BOARD_BE_BT_BFK20
	if ( bus<=1 && !palReadPad(I2C1_SCL_PORT,I2C1_SCL_PIN) ) goto err;
#endif

#ifdef I2C_ADDR_BUSMUX2
	// PCA9547 selects channel 0 when nRESET signal is active
	if (bus==2) {
		bus = SMB_ADDR_EXT(addr);
		if (SMB_ADDR_SEG_MASK(bus)) {
			//TODO: try to keep gate open until bus error occurs
			if( !palReadPad(I2C_BUSMUX2_RST_PORT, I2C_BUSMUX2_RST_PIN) ) {
				palSetPad(I2C_BUSMUX2_RST_PORT, I2C_BUSMUX2_RST_PIN);
				chSysPolledDelayX(US2RTC(STM32_HCLK, 5));
			}
			if( !palReadPad(I2C2_SCL_PORT,I2C2_SCL_PIN) ) goto err_rst;
			if ( !smbWrite1(&I2CD2,I2C_ADDR_BUSMUX2,(SMB_ADDR_SEG_GET(bus))|8) ) {
				palClearPad(I2C_BUSMUX2_RST_PORT, I2C_BUSMUX2_RST_PIN);
				goto err;
			}
		} else {
			palClearPad(I2C_BUSMUX2_RST_PORT, I2C_BUSMUX2_RST_PIN);
		}
	}
#endif
	return addr & 0xFF;
err_rst:
	palClearPad(I2C_BUSMUX2_RST_PORT, I2C_BUSMUX2_RST_PIN);
err:
	chVTSet(&((*i2cp)->vtm), MS2ST(2), smbTimerFunc, *i2cp);
	i2cReleaseBus(*i2cp);
	return 0;
}

void smbReleaseBus(I2CDriver *i2cp) {
	chVTSet(&i2cp->vtm, MS2ST(3), smbTimerFunc, i2cp);
	i2cReleaseBus(i2cp);
}

// LE
bool smbWrite(I2CDriver *i2cp, i2caddr_t addr, uint8_t cmd, uint32_t value, size_t count)
{
	chDbgAssert(count <= 4,"parameter");

	uint8_t iobuf[6];
	iobuf[0] = cmd;


	// BE
//	size_t i=count;
//	while (i) {
//		iobuf[i] = value;
//		if ( !--i ) break;
//		value >>= 8;
//	}

	// LE
	if( count ) {
		for(size_t i=0; ;) {
			iobuf[++i] = value;
			if( i==count ) break;
			value >>= 8;
		}
	}

//TODO:	acquire?
//	sdu_printf((BaseSequentialStream*)&SDU1, "A=%02X C=%02X, V=%08X C=%d: %02X %02X %02X\r\n", addr, cmd, value, count, iobuf[0], iobuf[1], iobuf[2]);

	if( !(addr=smbSelectBus(&i2cp,addr)) ) return false;
	msg_t status = i2cMasterTransmitTimeout(i2cp, addr,
                          iobuf, count+1, 0, 0, MS2ST(10+4*count));
	if( status != MSG_OK ) {
		smbWriteLog(i2cp,addr,cmd,status);
	}
	chVTSet(&i2cp->vtm, MS2ST(5), smbTimerFunc, i2cp);
	i2cReleaseBus(i2cp);
	return status == MSG_OK;
}

bool i2cWriteBlock(I2CDriver *i2cp, i2caddr_t addr, const uint8_t* data, unsigned count)
{
	if( !(addr=smbSelectBus(&i2cp,addr)) ) return false;
	msg_t status = i2cMasterTransmitTimeout(i2cp, addr,
                          data, count, NULL, 0, MS2ST(10+4*count));
	if( status != MSG_OK ) {
		smbWriteLog(i2cp,addr,data[0],status);
	}
	smbReleaseBus(i2cp);
	return status == MSG_OK;
}

bool smbWriteBlock(I2CDriver *i2cp, i2caddr_t addr, const uint8_t* data)
{
	return i2cWriteBlock(i2cp, addr, data, data[1]+2);
}


uint32_t smbGetData_le(const uint8_t* iobuf, size_t count)
{
	uint32_t value=0;

	for(;;) {
		--count;
		value |= iobuf[count];
		if( !count ) break;
		value <<= 8;
	}

	return value;
}

uint32_t smbGetData_be(const uint8_t* iobuf, size_t count)
{
	uint32_t value=0;
	size_t i;
	for(i=0;count;) {
		value |= iobuf[i];
		if( ++i==count ) break;
		value <<= 8;
	}

	return value;
}


uint32_t smbRead(I2CDriver *i2cp, i2caddr_t addr, uint8_t cmd, size_t count)
{
	chDbgAssert(count>0&&count<=4,"parameter");
	
	uint8_t iobuf[6];
//#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
//	*(uint32_t*)(void*)iobuf = 0xBEDAFFFF;
//#else
//	*(uint32_t*)(void*)iobuf = 0xFFFFDABE;
//#endif
	iobuf[5] = cmd;

	if( !(addr=smbSelectBus(&i2cp,addr)) ) return 0xBEDAFFFE;
	msg_t status = i2cMasterTransmitTimeout(i2cp, addr,
                          iobuf+5, 1, iobuf, count, MS2ST(10+4*count));
	if( status != MSG_OK ) {
		smbWriteLog(i2cp,addr,cmd,status);
	}	
	smbReleaseBus(i2cp);
	return status != MSG_OK ? 0xBEDAFFFF : smbGetData_le(iobuf,count);
}

int smbCheck(I2CDriver *i2cp, i2caddr_t addr, uint8_t cmd, uint32_t value, size_t count)
{
	chDbgAssert(count>0&&count<=4,"parameter");
	
	uint8_t iobuf[6];
//#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
//	*(uint32_t*)(void*)iobuf = 0xBEDAFFFF;
//#else
//	*(uint32_t*)(void*)iobuf = 0xFFFFDABE;
//#endif
	iobuf[5] = cmd;

	if( !(addr=smbSelectBus(&i2cp,addr)) ) return 0xBEDAFFFF;
	msg_t status = i2cMasterTransmitTimeout(i2cp, addr,
                          iobuf+5, 1, iobuf, count, MS2ST(10+4*count));
	if( status != MSG_OK ) {
		smbWriteLog(i2cp,addr,cmd,status);
		return -1;
	}	
	smbReleaseBus(i2cp);
#if 0	// check all data
	if ( smbGetData_le(iobuf,count) != value &&
		usbGetDriverStateI(SDU1.config->usbp) == USB_ACTIVE && 
		(!smb_silent || smb_isShellThread()) ) {
		//aquire console ...
		chprintf((BaseSequentialStream*)&SDU1, "Check %02x @%02x %0*x %0*x\r\n",addr,cmd,count*2,smbGetData_le(iobuf,count),count*2,value);
	}
	return false;
#endif
	return smbGetData_le(iobuf,count) != value;
}


bool smbReadBlock(I2CDriver *i2cp, i2caddr_t addr, uint8_t cmd, uint8_t* data, size_t count)
{

	uint8_t iobuf[6];
	iobuf[0] = cmd;

	if( !(addr=smbSelectBus(&i2cp,addr)) ) return false;
	msg_t status = i2cMasterTransmitTimeout(i2cp, addr,
                          iobuf, 1, data, count+1, MS2ST(10+4*count));
	if( status != MSG_OK ) {
		smbWriteLog(i2cp,addr,cmd,status);
	}	
	smbReleaseBus(i2cp);
	return status == MSG_OK;
}


i2cflags_t i2cReadBlock(i2caddr_t addr, uint8_t* data, size_t count)
{
	i2cflags_t flags = 0;
	I2CDriver *i2cp = 0;

	if( !(addr=smbSelectBus(&i2cp,addr)) ) return false;
	msg_t status = i2cMasterReceiveTimeout(i2cp, addr,
                          data, count, MS2ST(10+4*count));
	if( status != MSG_OK ) {
		data[0] = 0;
		smbWriteLog(i2cp,addr,0,status);
	}	
	smbReleaseBus(i2cp);
	return flags;
}
