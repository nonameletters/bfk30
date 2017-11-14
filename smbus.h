#ifndef __smbus_h
#define __smbus_h

#include <ch.h>
#include <hal.h>
#include "chprintf.h"

#define SMBUS_ARA	0x0C	// Alert Response Address
#define SMBUS_HOST	0x08	// HOST address


typedef struct __attribute__((packed))  {
	const char* command;
	uint8_t cmd;
	uint8_t format;
	uint8_t access;
	uint8_t size;
} pmbus_cmd_def;

// format field
#define PCF_ZERO	0x00
#define PCF_BYTE	0x01
#define PCF_WORD	0x02
#define PCF_BITS	0x11
#define PCF_BITSW	0x12
#define PCF_L11		0x22
#define PCF_L16u	0x32
#define PCF_FR		0x41
#define PCF_BLOCK   0x80
// access field
#define PCF_WO		1
#define	PCF_RO		2


#define SMB_ADDR_EXT(a)		 ((a)&0xF300)
#define SMB_ADDR_SEG_MASK(a) ((a)&0xF000)
#define SMB_ADDR_SEG_GET(a)	 ((((a)>>12)-1)&7)
//#define SMB_ADDR_BUS_MASK(a) ((a)&0x300)
//#define SMB_ADDR_BUS(a) 	 ((a)<<8)
#define SMB_ADDR_BUS_GET(a)	 (((a)>>8)&3)

#define SMB_MAX(a,b) (a>b?a:b)
#define SMB_MIN(a,b) (a>b?b:a)


#ifdef __cplusplus
extern "C" {
#endif
extern volatile int  pwc_state;		// need to know power state
extern bool smb_silent;
bool smb_isShellThread(void);
void smbReleaseBus(I2CDriver *i2cp);
unsigned  smbBadBus(unsigned bus);
i2caddr_t smbSelectBus(I2CDriver **i2cp, i2caddr_t addr);

void smbWriteLog(I2CDriver *i2cp, i2caddr_t addr, uint8_t cmd, msg_t status);
bool 	 smbWrite(I2CDriver *i2cp, i2caddr_t addr, uint8_t cmd, uint32_t value, size_t count);
int 	 smbCheck(I2CDriver *i2cp, i2caddr_t addr, uint8_t cmd, uint32_t value, size_t count);
bool 	 smbReadBlock(I2CDriver *i2cp, i2caddr_t addr, uint8_t cmd, uint8_t* data, size_t count);
bool 	 smbWriteBlock(I2CDriver *i2cp, i2caddr_t addr, const uint8_t* data);
uint32_t smbGetData_le(const uint8_t* iobuf, size_t count);
uint32_t smbGetData_be(const uint8_t* iobuf, size_t count);
uint32_t smbRead(I2CDriver *i2cp, i2caddr_t addr, uint8_t cmd, size_t count);
i2cflags_t i2cReadBlock(i2caddr_t addr, uint8_t* data, size_t count);
bool 	   i2cWriteBlock(I2CDriver *i2cp, i2caddr_t addr, const uint8_t* data, unsigned count);


const pmbus_cmd_def* pmbus_find_command(char* argv);
const char* pmbus_cmd_units(const pmbus_cmd_def* pc);
const char* pmbus_status_msg(uint16_t* st);		// STATUS_WORD
const char* pmbus_status_sub(unsigned cmd, uint8_t* st); // STATUS_XXX
void pmbus_fault_response(BaseSequentialStream* chp, unsigned code);

bool cmd_smb_int(BaseSequentialStream *chp, unsigned saddr, int argc, char *argv[]);
void dumphex(BaseSequentialStream *chp,const void* data, unsigned count);

#ifdef __cplusplus
}
#endif

#endif
