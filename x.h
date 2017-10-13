/*
 * x.h
 *
 *  Created on: May 16, 2015
 *      Author: k.kranke
 */

#ifndef X_H_
#define X_H_

#include "fxp.h"
#include "smbus.h"

typedef struct pmbus_dev {
    I2CDriver *i2cp;
    i2caddr_t addr;
    const char* name;
} pmbus_dev_t;

typedef struct zl2102_dev {
    pmbus_dev_t dev;
    fixed_val_t max;
    fixed_val_t nominal;
} zl2102_dev_t;

typedef struct ina226 {
    pmbus_dev_t dev;
    int cur_lsb;		// uA
    int	Rsh;			// mOhm
} ina226_t;

typedef struct ina226_data {
	int32_t  voltage;	// LSB 1 mV
	int32_t  current;	// LSB 1 uA
	uint32_t power;		// LSB 1 uW
} ina226_data;

typedef enum {
	ina_voltage=(1<<0), ina_current=(1<<1), ina_power=(1<<2), ina_all=0x7
} ina226_mask;


struct vreg_s;
typedef bool  (*vreg_set)(const struct vreg_s* vreg, unsigned mv);
typedef bool (*vreg_show)(const struct vreg_s* vreg, BaseSequentialStream* scp);
typedef bool (*vreg_store)(const struct vreg_s* vreg);

typedef struct vreg_s {
    i2caddr_t	addr;
    const char* name;
    uint16_t	id;
    uint16_t	min;
    uint16_t	nom;
    uint16_t	max;
    uint16_t	flags;
    vreg_set	setmv;
    vreg_show	show;
    vreg_store	store;
} vreg_t;

bool zl2102_setmv(const vreg_t* vreg, unsigned mv);
bool zl2102_show(const vreg_t* vreg, BaseSequentialStream* scp);
bool zl2102_store(const vreg_t* vreg);
bool tps_setmv(const vreg_t* vreg, unsigned mv);
bool tps_show(const vreg_t* vreg, BaseSequentialStream* scp);
bool vpll_setmv(const vreg_t* vreg, unsigned mv);
bool vpll_show(const vreg_t* vreg, BaseSequentialStream* scp);
bool ltc_setmv(const vreg_t* vreg, unsigned mv);
bool ltc_show(const vreg_t* vreg, BaseSequentialStream* scp);



#include "pmbus_cmd_constants.h"

#define ZL2102_OPERATION    		PMBUS_CMD_OPERATION
#define ZL2102_ON_OFF_CFG   		PMBUS_CMD_ON_OFF_CONFIG
#define ZL2102_CLEAR_FAULT  		PMBUS_CMD_CLEAR_FAULTS				// none
#define ZL2102_VOUT_MODE    		PMBUS_CMD_VOUT_MODE                // 13h
#define ZL2102_VOUT_COMMAND 		PMBUS_CMD_VOUT_COMMAND
#define ZL2102_VOUT_MAX     		PMBUS_CMD_VOUT_MAX				// L16u
#define ZL2102_VOUT_OV_FAULT_LIMIT	  0x40		// L16u
#define ZL2102_VOUT_OV_FAULT_RESPONSE 0x41		// BIT
#define ZL2102_VOUT_UV_FAULT_LIMIT    0x44		// L16U
#define ZL2102_VOUT_UV_FAULT_RESPONSE 0x45		// BIT
#define ZL2102_IOUT_OC_FAULT_LIMIT	  0x46		// L11
#define ZL2102_IOUT_UC_FAULT_LIMIT	  0x47		// L11
#define ZL2102_OT_FAULT_LIMIT	  	  0x4F		// L11 +125
#define ZL2102_OT_FAULT_RESPONSE 	  0x50		// BIT
#define ZL2102_OT_WARN_LIMIT	  	  0x51		// L11 +110
#define ZL2102_UT_WARN_LIMIT	  	  0x52		// L11 -50
#define ZL2102_UT_FAULT_LIMIT	  	  0x53		// L11 -55
#define ZL2102_UT_FAULT_RESPONSE	  0x54		// L11 BIT
#define ZL2102_POWER_GOOD_ON	PMBUS_CMD_POWER_GOOD_ON		// 0x54	L16

#define ZL2102_STATUS_WORD	PMBUS_CMD_STATUS_WORD		// 0x79 2b
#define ZL2102_READ_VIN     	PMBUS_CMD_READ_VIN		// 0x88 L11
#define ZL2102_READ_VOUT    	PMBUS_CMD_READ_VOUT		// 0x8B L16u
#define ZL2102_READ_IOUT    	PMBUS_CMD_READ_IOUT		// 0x8C L11
#define ZL2102_READ_TEMP    	PMBUS_CMD_READ_TEMPERATURE_1	// 0x8D L11
#define ZL2102_FREQUENCY    	PMBUS_CMD_READ_FREQUENCY	// 0x95 L11
#define ZL2102_USER_CONFIG  	0xd1
#define ZL2102_DDC_CONFIG   	0xd3
#define ZL2102_STORE_USER_ALL	PMBUS_CMD_STORE_USER_ALL
#define ZL2102_RESTORE_USER_ALL PMBUS_CMD_RESTORE_USER_ALL
#define ZL2102_RESTORE_FACTORY  0xF4
#define ZL2102_BLANK_PARAMS  	0xEB				// 0x10
#define ZL2102_MFR_IOUT_OC_FAULT_RESPONSE 0xE5

#define ZL2102_VOUT_MODE_VALUE	-13
#define ZL2102_COMMAND_SPACING	2	// 2ms between consecutive commands
#define ZL2102_CLOCK_RATE		100000

#define INA226_CONFIG   0x0
#define INA226_SHUNTV   0x1		// LSB=2.5uV, FSR=81.92mV, signed
#define INA226_BUSV     0x2		// LSB=1.25mV, FSR=40.96V, signed
#define INA226_POWER    0x3		// 25*LSB(current)
#define INA226_CURRENT  0x4		// LSB-programmable
#define INA226_CALIBR   0x5		// Calibration constant
#define INA226_MASKEN   0x6
#define INA226_ALIMIT   0x7

#define INA226_AVG1	0x000
#define INA226_AVG4	0x200
#define INA226_AVG16	0x400
#define INA226_AVG64	0x600
#define INA226_AVG128	0x800
#define INA226_AVG256	0xA00
#define INA226_AVG512	0xC00
#define INA226_AVG1024	0xE00

// CAL = 0.00512/ (CurLSB*Rshunt)
// CurLSB = Imax/2^15
#define INA226_CAL_CONST		5120000		// for mOhm*uA
//#define INA226_CAL_R003u250   0x1aab		// Rsh=3mOhm  LSB=250uA ~8A max
//#define INA226_CAL_R010u100   0x1400		// Rsh=10mOhm LSB=100uA ~3A max
#define INA226_VLSB     1250				// uV
#define INA226_PLSB     25 					// mW
#define INA226_DEF_CONFIG (0x4127 | INA226_AVG64)	// default + AVG


// GPO_ID:
#define UCD_GPIO5	   0	//   0: GPIO5		PIN.10	CPU_RESET#
#define UCD_GPIO6	   1	//   1: GPIO6		PIN.11	PE_RST#
#define UCD_GPIO7	   2	//   2: GPIO7		PIN.12	-
#define UCD_GPIO8	   3	//   3: GPIO8		PIN.13	DEV_RESET#
#define UCD_GPIO9	   4	//   4: GPIO9		PIN.14	JTAG_RESET
#define UCD_GPIO10	   5	//   5: GPIO10		PIN.15	EJTAG_RESET
#define UCD_GPIO11	   6	//   6: GPIO11		PIN.16	UCD_NOTIFICATION
#define UCD_GPIO12	   7	//   7: GPIO12		PIN.17	SYSTEM_OK
#define UCD_GPIO14	  10	//   A: GPIO14		PIN.21	EN_PCIE
#define UCD_GPIO19	  11	//   B: TDO_GPIO19	PIN.28	EN_DDR_1V8
#define UCD_GPIO18	  12	//   C: TCK_GPIO18	PIN.27	EN_10G_1V5
#define UCD_GPIO21	  13	//   D: TMS_GPIO21	PIN.30	-
#define UCD_GPIO20	  14	//   E: TDI_GPIO20	PIN.29	EN_SATA_1V8
#define UCD_GPIO1	  15	//   F: GPIO1		PIN.4	EN_PLL
#define UCD_GPIO2	  16	//  10: GPIO2		PIN.5	EN_0V9
#define UCD_GPIO3	  17	//  11: GPIO3		PIN.6	EN_1V5
#define UCD_GPIO4	  18	//  12: GPIO4		PIN.7	EN_1V8
#define UCD_GPIO13	  19	//  13: GPIO13		PIN.18	EN_SATA
#define UCD_GPIO15	  20	//  14: GPIO15		PIN.24	EN_10G
#define UCD_GPIO16	  21	//  15: GPIO16		PIN.25	EN_3V3#
#define UCD_GPIO17	  22	//  16: GPIO17		PIN.26	EN_PCIe_1V5

#define UCD9090_PAGES		10
#define UCD9090_MAX_GPIO_ID	22
#define UCD9090_MFR_STATUS 0xF3
#define UCD9090_MFR_STATUS_LENGTH 4
#define UCD9090_MFR_STATUS_UNUSED_BITS 0xFF00E000
#define UCD9090_CMD_LOGGED_FAULTS	0xEA
#define UCD9090_CMD_LOGGED_FAULTS_LENGTH	12
#define UCD9090_CMD_LOGGED_FAULT_DETAIL	0xEC
#define UCD9090_CMD_LOGGED_FAULT_DETAIL_LENGTH 10
#define UCD9090_CMD_LOGGED_FAULT_DETAIL_INDEX 0xEB	//WORD

//++ V2 board:
// I2C programmable bus switch:
#define PCA9547_I2C_ADDR(A)		(0x70+(A)&7)
#define PCA9547_CMD(chan,en)	(((chan)&7)|((en)<<8))

#define PCA9547_LTC_SEL_A		0
#define ltc_select(n) { smbWrite(&I2C2, PCA9547_I2C_ADDR(PCA9547_LTC_SEL_A), PCA9547_CMD(n,1); }

#define LTC_1p5V_PCIe	0
#define LTC_1p5V_xGBe	1
#define LTC_1p8V_DDR	2
#define LTC_1p8V_SATA	3

// TPS65263 triple: 1=0.9SATA, 2=0.95PCIe 3=0.95xGBe
// Vout=((code&0x7F)+68)/100
#define TPS65263_VOUT_SET(code,go)	(((code)&0x7F)|((go)?0x80:0))
#define TPS65263_VOUT1_SEL	0x00
#define TPS65263_VOUT2_SEL	0x01
#define TPS65263_VOUT3_SEL	0x02
#define TPS65263_VOUT1_COM	0x03
#define TPS65263_VOUT2_COM	0x04
#define TPS65263_VOUT3_COM	0x05
#define TPS65263_SYS_STATUS	0x06
#define TPS65263_VOUT_GO	0x80
#define TPS65263_VOUT_COM(nEN,mode,SR) ((((nEN)&1)<<0)|(((mode)&1)<<1|(((SR)&7)<<4)) // default 0
#define TPS65263_ST_PGOOD1	0x01
#define TPS65263_ST_PGOOD2	0x02
#define TPS65263_ST_PGOOD3	0x04
#define TPS65263_ST_OTW		0x08	// TEMP > 125C
#define TPS65263_ST_OC1		0x10
#define TPS65263_ST_OC2		0x20
#define TPS65263_ST_OC3		0x40
#define TPS65263_ST_OTP		0x80	// TEMP > 160C
#define TPS65263_MIN		68
#define TPS65263_MAX		(TPS65263_MIN+127)
//--

// Status bits (reg0), Interrupts Sticky (reg1), Interrupt Mask (reg2) bits:
#define SI5351_R_DEVICE_STATUS		0
#define SI5351_R_INT_STATUS_STICKY	1
#define SI5351_R_INT_MASK		2
#define SI5351_R_OUTPUT_ENABLE		3
#define SI5351_R_PLL_RESET		0xB1
#define SI5351_PLLB_RST		0x80
#define SI5351_PLLA_RST		0x40
#define SI5351_R_XTAL_CL	0xB7
#define SI5351_XTAL_CL_6pf	0x52
#define SI5351_XTAL_CL_8pf	0x92
#define SI5351_XTAL_CL_10pf	0xD2

#define SI5351_SYS_INIT		0x80
#define SI5351_LOL_B		0x40
#define SI5351_LOL_A		0x20
#define SI5351_LOS_S		0x10


// POWER control requests
typedef enum
{
	powerctl_none,
	powerctl_rb,
	powerctl_pb,
	powerctl_off,
	powerctl_in,
	powerctl_on,
	powerctl_on_lock,
	powerctl_on_unlock
} powerctl_req;

int powerctl(powerctl_req req);
int powerctlI(powerctl_req req);



#ifdef __cplusplus
extern "C" {
#endif

const char* ucd9090_mfr_status_msg(uint32_t* st);
uint32_t ucd9090_mfr_status(i2caddr_t addr);
bool  ucd9090_clear_log(i2caddr_t addr);
bool  ucd9090_config(BaseSequentialStream *chp);

bool ina226_read_data(const ina226_t*, ina226_data* data, ina226_mask mask);
void ina226_write(const pmbus_dev_t *dev, uint8_t cmd, uint16_t value);
bool ina226_config(const ina226_t *dev);
#ifdef __cplusplus
}
#endif

#endif /* X_H_ */
