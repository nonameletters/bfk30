#include <ch.h>
#include <hal.h>

#include <stdint.h>
#include <strings.h>

#include <x.h>
#include "smbus.h"

const char* const ucd9090_mfr_status_msg_str[] = {
	"NEW LOGGED_FAULT_DETAIL",
	"SYSTEM WATCHDOG TIMEOUT",
	"STORE_DEFAULT_ALL ERROR",
	"STORE_DEFAULT_ALL DONE",
	"WATCHDOG TIMEOUT",
	"INVALID LOGS",
	"LOGGED_FAULT_DETAIL FULL",
	"RESEQUENCE ERROR",
	"PKGID MISMATCH",
	"HARDCODED PARMS",
	"SEQ_OFF TIMEOUT",
	"SEQ_ON TIMEOUT",
	"SLAVED_FAULT",
};

const char* ucd9090_mfr_status_msg(uint32_t* mfst)
{
	*mfst &= ~0xFFFFE000;
	int i;
	for( i=12; i >= 0; --i) {
		if( *mfst & (1<<i) ) {
			*mfst &= ~(1<<i);
			return ucd9090_mfr_status_msg_str[12-i];
		}
	}
	return "";
}

uint32_t ucd9090_mfr_status(i2caddr_t addr)
{
	uint8_t data[UCD9090_MFR_STATUS_LENGTH+1];
	smbReadBlock(0, addr, UCD9090_MFR_STATUS, data, UCD9090_MFR_STATUS_LENGTH);
	return smbGetData_be(data+1,UCD9090_MFR_STATUS_LENGTH);
}

bool ucd9090_clear_log(i2caddr_t addr)
{	
	uint8_t data[SMB_MAX(UCD9090_CMD_LOGGED_FAULTS_LENGTH,UCD9090_CMD_LOGGED_FAULT_DETAIL_LENGTH)+2];
	if( !smbReadBlock(0, addr, UCD9090_CMD_LOGGED_FAULT_DETAIL, data, UCD9090_CMD_LOGGED_FAULT_DETAIL_LENGTH) )
	    return false;

	bzero(data+2,UCD9090_CMD_LOGGED_FAULTS_LENGTH);
	data[0] = UCD9090_CMD_LOGGED_FAULTS;
	data[1] = UCD9090_CMD_LOGGED_FAULTS_LENGTH;
	return smbWriteBlock(0, addr, data);
}


static uint8_t ina_read_cmd[3] = {INA226_BUSV, INA226_CURRENT, INA226_POWER};
//return (uint16_t)pmbus_read_be(dev,cmd,2); 
bool ina226_read_data(const ina226_t *dev, ina226_data* data, ina226_mask mask)
{
  systime_t tmo = MS2ST(4);
  bool ret = true;
  uint8_t accel_data[4];

  // read all sensor data without disturbance
  // Reading may anywere be inconsistent due to continuous conversion mode
  I2CDriver *i2cp = dev->dev.i2cp;
  i2caddr_t addr = smbSelectBus(&i2cp,dev->dev.addr);
  if (addr == 0) return false;
  for (int i=0; i < 3; ++i) {
	  if ( !(mask & (1<<i)) ) continue;
	  accel_data[0] = ina_read_cmd[i];
	  msg_t status = i2cMasterTransmitTimeout(i2cp, addr,
							  accel_data, 1, accel_data, 2, tmo);

	  if ( status != MSG_OK ) {
	      smbWriteLog(dev->dev.i2cp, dev->dev.addr, accel_data[0], status);
		  ret = false;
		  break;
	  }
	  int16_t result = (accel_data[0] << 8) | accel_data[1];
	  switch (ina_read_cmd[i]) {
		  case INA226_BUSV:
		    // 15 bit signed (d[15]=0), LSB=1.25 mV
			data->voltage = (result * INA226_VLSB) / 1000; 	// mV
			break;
		  case INA226_CURRENT:
		    // 16 bit signed LSB=CurrentLSB (see calibration)
			data->current = (result * dev->cur_lsb) ;	// uA
			break;
		  case INA226_POWER:
		    // 16 bit unsigned, LSB=25*CurrentLSB
		    data->power = ((uint16_t)result * INA226_PLSB * dev->cur_lsb) ;	// uW
		    break;
	  }
  }
  smbReleaseBus(i2cp);
  return ret;
}


bool ina226_config(const ina226_t *dev) {
  bool ret = true;
  systime_t tmo = MS2ST(4);
  uint8_t accel_tx_data[4];

  i2cAcquireBus(dev->dev.i2cp);
  accel_tx_data[0] = INA226_CONFIG;
  accel_tx_data[1] = INA226_DEF_CONFIG >> 8;
  accel_tx_data[2] = INA226_DEF_CONFIG & 0xFF;

  unsigned cal = INA226_CAL_CONST / (dev->cur_lsb * dev->Rsh);
  chDbgAssert(cal<0x10000,"Invalid range");

  if ( MSG_OK != i2cMasterTransmitTimeout(dev->dev.i2cp, dev->dev.addr,
						  accel_tx_data, 3, accel_tx_data, 0, tmo) ) {
	ret = false;
	goto ex;
  }

  accel_tx_data[0] = INA226_CALIBR;
  accel_tx_data[1] = (cal >> 8) & 0xFF;
  accel_tx_data[2] = cal & 0xFF;

  if ( MSG_OK != i2cMasterTransmitTimeout(dev->dev.i2cp, dev->dev.addr,
						  accel_tx_data, 3, accel_tx_data, 0, tmo) ) {
	  ret = false;
  }
ex:
  if( !ret ) {
    smbWriteLog(dev->dev.i2cp, dev->dev.addr, accel_tx_data[0], MSG_TIMEOUT);
  }
  i2cReleaseBus(dev->dev.i2cp);
  return ret;
}
