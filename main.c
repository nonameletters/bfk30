/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <string.h>
#include <stdlib.h>		// for strtol
#include <limits.h>

#include <ch.h>
#include <hal.h>

#include "main.h"

#include "sdu.h"
#include "chprintf.h"

#include "dfu.h"
#include "usbcfg.h"
#include "x.h"
#include "clk156.h"
#include "tps56.h"
#include "chipdrivers.h"

#include "hal_usb_hid.h"
#include "report.h"
#include "cg_5p49v5901.h"
#include "spi25.h"
#include "exti_ex.h"

static enum blinkStates blState = BS_IDLE;  // Define variable


volatile unsigned atx_state;
volatile unsigned sequencer_state;
volatile bool dfu_active;
bool atx_present;

thread_t *buttonPressThreadPointer;

/* Virtual serial port over USB.*/
SerialUSBDriver SDU1;
DFU_USBDriver DFU1;
USBHIDDriver HID1;


#if HID_REPORT_STAT

#define FILTER_SHIFT 7
// History length should be 2^FILTER_SHIFT
#define FILTER_HISTORY_LENGTH 128

// Be warned, that with 128 words filter length 32 bit accumulator will overflow with values > 16.7
typedef struct {
    int history[FILTER_HISTORY_LENGTH];
    int filter;
} int_filter_t;

typedef struct {
    int_filter_t current;
    int_filter_t voltage;
    int_filter_t power;
} ina_filter_t;

static unsigned filter_current;
static ina_filter_t ina_history[HID_REPORT_SENSOR_CHANNELS];
#endif

static struct usb_hid_in_report_s usb_hid_in_report;

// Flash 1 is Baikal boot flash
static const sFLASH_USBConfig sFlash1 = {
    &SPID1,
    /*
     * SPI1 configuration structure.
     * Speed 5.25MHz, CPHA=0, CPOL=0, 8bits frames, MSb transmitted first.
     * The slave select line is the pin GPIOE_CS_SPI on the port GPIOE.
     */
    {
        NULL,
        /* HW dependent part.*/
        SPI1_CS_PORT,
        SPI1_CS_PIN,
        SPI_CR1_BR_0 | SPI_CR1_BR_1
#if CH_HAL_MAJOR >= 5
		, 0
#endif
    },
    BOOT_SPI_EN_PORT,
    BOOT_SPI_EN_PIN
};

//sFLASH_USBDriver SFDU1;

#if DFU_INTERFACES > 1
// Flash 2 is External boot flash
static const sFLASH_USBConfig sFlash2 = {
    &SPID1,
    /*
     * SPI1 configuration structure.
     * Speed 5.25MHz, CPHA=1, CPOL=1, 8bits frames, MSb transmitted first.
     * The slave select line is the pin GPIOE_CS_SPI on the port GPIOE.
     */
    {
        NULL,
        /* HW dependent part.*/
        SPI1_CS_PORT,
        SPI1_CS_PIN,
        SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_CPOL | SPI_CR1_CPHA
    },
    BOOT_SPI_EN_PORT,
    32  // not used
};
sFLASH_USBDriver SFDU2;
#endif

#ifdef VIDEO_SPI_PORT
// Flash 2 is Video BIOS flash
static const sFLASH_USBConfig sFlash3 = {
    &SPID2,
    /*
     * SPI1 configuration structure.
     * Speed 5.25MHz, CPHA=1, CPOL=1, 8bits frames, MSb transmitted first.
     * The slave select line is the pin GPIOE_CS_SPI on the port GPIOE.
     */
    {
        NULL,
        /* HW dependent part.*/
        GPIOB,
        9,
        SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_CPOL | SPI_CR1_CPHA
    },
    GPIOC,
    14
};
sFLASH_USBDriver SFDU3;
#endif

/*
 * DFU USB driver configuration
 */
const DFU_USBConfig dfuusbcfg1 =
{
    &USBD1,
    { // TODO: temporary set flash 3 to flash 2. 3rd actually should be I2C EEPROM.
        &SFDU1
#if DFU_INTERFACES == 2
        ,&SFDU2
#elif DFU_INTERFACES == 3
        ,&SFDU3
#endif
    }
};

char con_str_buf[20]; // min 13

/*
 * I2C1/I2C2 config.
 */
const I2CConfig i2cfg1 = {
        OPMODE_I2C, // OPMODE_I2C OPMODE_SMBUS_HOST
        100000,
        STD_DUTY_CYCLE
};

const I2CConfig i2cfg2 = {
        OPMODE_I2C, // OPMODE_I2C OPMODE_SMBUS_HOST
        400000,
        FAST_DUTY_CYCLE_16_9
};

#ifdef __cplusplus
extern "C"
#endif
const I2CConfig i2cfg3 = {   // SMBA pin used as OTG_FS_VBUS
        OPMODE_I2C, // OPMODE_SMBUS_DEVICE,
        100000,		// 400000
        STD_DUTY_CYCLE //FAST_DUTY_CYCLE_16_9
};

static const zl2102_dev_t vtg[3] = {
    {{NULL, 0x20|I2C_BUS2, "v0_9"}, {1, 200,0}, {0, 901,0} },
    {{NULL, 0x21|I2C_BUS2, "v1_5"}, {1, 900,0}, {1, 500,0} },
    {{NULL, 0x38|I2C_BUS2, "v1_8"}, {2, 200,0}, {1, 800,0} }
};

#define VREG_FLAG_PMBUS		1	// supports
#define VREG_FLAG_STNDBY	2	// can be accessed on standby power
#define VREG_FLAG_PWRIN		4	// can be accessed on input power (12V)
#define VREG_FLAG_PERM		8	// permanent voltage setting

//NOTE: TPS65263 no communication if voltage disabled by external pin; forget  preset value
//NOTE: LTC3447 is write-only
const vreg_t vreg[] = {
    {0x20|I2C_BUS2, 	"v09",  	0, QN_VAL(0.700), QN_VAL(0.900), QN_VAL(1.200), VREG_FLAG_PMBUS|VREG_FLAG_PWRIN|VREG_FLAG_PERM,	zl2102_setmv, zl2102_show, zl2102_store},
    {0x21|I2C_BUS2, 	"v15",  	0, QN_VAL(1.200), QN_VAL(1.500), QN_VAL(1.800), VREG_FLAG_PMBUS|VREG_FLAG_PWRIN|VREG_FLAG_PERM,	zl2102_setmv, zl2102_show, zl2102_store},
    {0x38|I2C_BUS2, 	"v18",  	0, QN_VAL(1.200), QN_VAL(1.800), QN_VAL(2.300), VREG_FLAG_PMBUS|VREG_FLAG_PWRIN|VREG_FLAG_PERM,	zl2102_setmv, zl2102_show, zl2102_store},
#ifdef BOARD_BE_BT_BFK20
    {0,             	"pll",  	0, QN_VAL(0.900), QN_VAL(0.900), QN_VAL(1.150), VREG_FLAG_STNDBY, 					vpll_setmv,   vpll_show,   0},
    {I2C_ADDR_TPS65263, "sata090",	0, QN_VAL(0.700), QN_VAL(0.900), QN_VAL(1.200), 0, 									tps_setmv,    tps_show,    0},
    {I2C_ADDR_TPS65263, "pcie095",	1, QN_VAL(0.700), QN_VAL(0.950), QN_VAL(1.200), 0,									tps_setmv,    tps_show,    0},
    {I2C_ADDR_TPS65263, "xgbe095", 	2, QN_VAL(0.700), QN_VAL(0.950), QN_VAL(1.200), 0,									tps_setmv,    tps_show,    0},
{I2C_ADDR_LTC_1p5_PCIE, "pcie15",	0, QN_VAL(1.200), QN_VAL(1.500), QN_VAL(1.800), 0, ltc_setmv, 0, 0},
{I2C_ADDR_LTC_1p5_XGBE, "xgbe15",	0, QN_VAL(1.200), QN_VAL(1.500), QN_VAL(1.800), 0, ltc_setmv, 0, 0},
{I2C_ADDR_LTC_1p8_SATA, "sata18",	0, QN_VAL(1.200), QN_VAL(1.800), QN_VAL(2.000), 0, ltc_setmv, 0, 0},
{I2C_ADDR_LTC_1p8_DDR,  "ddr18",	0, QN_VAL(1.200), QN_VAL(1.800), QN_VAL(2.000), 0, ltc_setmv, 0, 0},
#endif
};

unsigned vreg_data[sizeof(vreg)/sizeof(vreg[0])];

#ifdef BOARD_BE_BT_BFK20
uint16_t vpll_code = 0;
void set_vpll(void) {
    palWriteGroup(GPIOC,3,1,~vpll_code);
}

const pmbus_dev_t tps = {
    &I2CD2, 0x30, "0.9V_SATA/0.95V_PCIe/0.95V_XGBE"
};
uint16_t tps_val[TPS_CHANNEL_COUNT] = {0,0,0}; // {900,950,950}

#else
// TPS56 settings is not permanent!!
const pmbus_dev_t tps = {
    &I2CD2, 0x37, "0.95V XGBE"
};
uint16_t tps_val[TPS_CHANNEL_COUNT] = {0}; // {950}
#endif

// 250 for 3 Ohm, 25 for 30 Ohm
const ina226_t ina[INA_SENSOR_COUNT] = {
#if defined(BOARD_BE_BT_BFK16)
    {{&I2CD1, 0x41, "0.9V CPU core"},	250,3},		//R003
    {{&I2CD1, 0x40, "1.5V CPU PLL"},	250,3},		//R003
    {{&I2CD1, 0x44, "1.8V CPU IO"},	20,30},		//R030 (655mA)
#elif defined(BOARD_BE_BT_BFK20)
    {{&I2CD1, 0x41, "0.90V CPU core"},	250,3},		//R003 2850mA
    {{&I2CD1, 0x46, "0.90V CPU PLL"},	 5,100},	//R100 50mA
    {{&I2CD1, 0x44, "1.80V CPU IO"},    10,30},		//R030
    {{&I2CD1, 0x40, "1.50V DDR"},		100,10},	//R010 380mA
    {{&I2CD1, 0x49, "1.80V DDR PLL"},	 5,100},	//R100 68mA
    {{&I2CD1, 0x45, "0.90V SATA"},		 5,100},	//R100 121mA
    {{&I2CD1, 0x4A, "1.80V SATA"},		 5,100},	//R100 38mA
    {{&I2CD1, 0x42, "0.95V PCIe"},		200,10},	//R010 585mA
    {{&I2CD1, 0x47, "1.50V PCIe"},		 5,100},	//R100 118mA
    {{&I2CD1, 0x43, "0.95V 10G"},		200,10},	//R010 635mA
    {{&I2CD1, 0x48, "1.50V 10G"},		 5,100},	//R100 115mA
#endif
};

typedef struct {
    int8_t vout_mode;
} ucd9090_pageinfo_t;
// gain: 3.3/2 5.0/2 12/2

ucd9090_pageinfo_t ucd9090_pageinfo[UCD9090_PAGES];

#ifdef BOARD_BE_BT_BFK20
// DDR1.8  GPIO19(11)  - GPI1(8)
// SATA1.8 GPIO20(14)  - GPI2(9)
const char* const ucd9090_rails[] = {
    "PLL", "0.9V", "1.5V",    "1.8V",    "0.9V_SATA", "0.95V_PCIe", "0.95V_10G", "3.3V",    "1.5V_PCIe", "1.5V_10G", "Unused"
};
//	 M10     M4      M5        M6        M7            M8           M9            M3        M1            M2
static const uint8_t ucd9090_rails_en[] = {
UCD_GPIO1, UCD_GPIO2,UCD_GPIO3,UCD_GPIO4,UCD_GPIO13,   UCD_GPIO14,  UCD_GPIO15,  UCD_GPIO16,UCD_GPIO17,   UCD_GPIO18, UCD_GPIO9
};
#else
const char* const ucd9090_rails[] = {
    "12V", "5V", "3.3V", "0.9V", "1.5V", "1.8V", "0.95V", "Unused", "Unused", "Unused", "Unused"
};
#endif

#define UCD90_PIN_MODE_UNUSED		0
#define UCD90_PIN_MODE_INPUT		1
#define UCD90_PIN_MODE_PUSHPULL		2
#define UCD90_PIN_MODE_OPENDRAIN	3


//const pmbus_dev_t ucd9090 = {&I2CD1, 0x75, "Power Sequencer"};

bool ucd9090_init_pageinfo(void) {
    for(int page=0; page < UCD9090_PAGES; ++page) {
        if( !smbWrite(0,I2C_ADDR_SQNR,PMBUS_CMD_PAGE,page,1) ) return false;
        uint32_t val = smbRead(0,I2C_ADDR_SQNR,PMBUS_CMD_VOUT_MODE,1);
        if( val & ~0xFFul ) return false;
        ucd9090_pageinfo[page].vout_mode  = smbRead(0,I2C_ADDR_SQNR,PMBUS_CMD_VOUT_MODE,1);
        if( ucd9090_pageinfo[page].vout_mode & 0x10 ) ucd9090_pageinfo[page].vout_mode |= 0xF0;
    }
    return true;
}

#ifdef BMC_WITH_PASSWORD
static const char password[] = "iddqd";
const char* password_pos = password;

static int password_good(void) {
    return *password_pos == '\0';
}

static bool password_check(char c) {
    if(*password_pos != '\0') {
        if(*password_pos == c) {
            password_pos += 1;
        } else {
            password_pos = password;
            if(*password_pos == c) {
                password_pos += 1;
            }
            return false;
        }
    }
    return password_good();
}


static void password_reset(void) {
    password_pos = password;
}
#else
//static void password_check(char c) {(void)c;}
static int password_good(void) { return 1; }
static void password_reset(void) {}
#endif

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

int isTrue(const char* str) {
    if( !strcmp("1",str) || !strcmp("on", str) ) { return 1; }
    if( !strcmp("0",str) || !strcmp("off",str) ) { return 0; }
    return *str ? (*str&0xFF) : -1;
}

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)


//static void sequencer_ctl(int st) {
//    if(st) {
//        palClearPad(SEQ_CNTRL_PORT,SEQ_CNTRL_PIN);
//        sequencer_state = 1;
//    } else {
//        palSetPad(SEQ_CNTRL_PORT,SEQ_CNTRL_PIN);
//        sequencer_state = 0;
//    }
//}

//unsigned sqnr_state(void) {
//#ifdef BOARD_BE_BT_BFK20
//    return ((!palReadPad(SEQ_CNTRL_PORT,SEQ_CNTRL_PIN)) | (palReadPad(SYSTEM_OK_PORT,SYSTEM_OK_PIN)<<1));
//#else
//    return ((!palReadPad(SEQ_CNTRL_PORT,SEQ_CNTRL_PIN)) | ((!palReadPad(MIPS_RESETn_PORT, MIPS_RESETn_PIN))<<1));
//
//#warning "Please, re-program UCD"
//#endif
//}

// zl fixed exp = -13
//bool zl2102_setmv(const vreg_t* vreg, unsigned mv) {
//    int32_t val = to_l16u(mv, -13);
//    //smbWrite(vreg->addr, ZL2102_VOUT_MAX, (int16_t)(val * 110 / 100));
//    smbWrite(0,vreg->addr, ZL2102_VOUT_OV_FAULT_LIMIT, val * 115 / 100,2);
//    smbWrite(0,vreg->addr, ZL2102_VOUT_UV_FAULT_LIMIT, val * 85 / 100,2);
//    smbWrite(0,vreg->addr, ZL2102_VOUT_COMMAND, val,2);
//    return true;
//}
//
//
//bool zl2102_store(const vreg_t* vreg) {
//    return smbWrite(0, vreg->addr, ZL2102_STORE_USER_ALL, 0, 0);
//}
//
//bool zl2102_show(const vreg_t* vreg, BaseSequentialStream* chp)
//{
//	uint32_t xs = smbRead(0,vreg->addr, PMBUS_CMD_STATUS_WORD, 2);
//	chprintf(chp, "%-7s ", vreg->name);
//	if ( xs & 0xFFFF0000 ) {
//		chprintf(chp, "no Vin\r\n");
//	}
//	uint16_t val;
//	val = smbRead(0,vreg->addr, PMBUS_CMD_READ_VIN, 2);
//	l11_to_str(con_str_buf,val);
//	chprintf(chp, "Vin=%sV", con_str_buf);
//	val = smbRead(0,vreg->addr, PMBUS_CMD_VOUT_COMMAND, 2);
//	linear_to_str(con_str_buf, val, ZL2102_VOUT_MODE_VALUE);
//	chprintf(chp, " Vset=%sV", con_str_buf);
//	val = smbRead(0,vreg->addr, PMBUS_CMD_READ_VOUT, 2);
//	linear_to_str(con_str_buf, val, ZL2102_VOUT_MODE_VALUE);
//	chprintf(chp, " Vout=%sV", con_str_buf);
//	val = smbRead(0,vreg->addr, PMBUS_CMD_READ_IOUT, 2);
//	l11_to_str(con_str_buf,(int16_t)val);
//	chprintf(chp, " Iout=%-6.6sA", con_str_buf);
//	//val = smbRead(0,vreg->addr, PMBUS_CMD_VOUT_MAX, 2);
//	//linear_to_str(con_str_buf, val, ZL2102_VOUT_MODE_VALUE);
//	//chprintf(chp, " Vmax=%sV", con_str_buf);
//	val = smbRead(0,vreg->addr, PMBUS_CMD_READ_TEMPERATURE_1,2);
//	l11_to_str(con_str_buf,(int16_t)val);
//	chprintf(chp, " TEMP=%sC", con_str_buf);
//
//	val = xs & 0xFFFF;
//	chprintf(chp, " status %4x\r\n", xs & 0xFFFF);
//	while( val ) {
//		chprintf(chp, "  %s\r\n", pmbus_status_msg(&val));
//	}
//	return true;
//}


//#ifdef BOARD_BE_BT_BFK20
//bool tps_setmv(const vreg_t* vr, unsigned mv) {
//	smbWrite(0, vr->addr, TPS65263_VOUT1_SEL+vr->id, TPS65263_VOUT_SET(mv/10-68,1), 1);
//	return true;
//}
//
//bool tps_show(const vreg_t* vr, BaseSequentialStream* chp) {
//	unsigned s = smbRead(0, vr->addr, TPS65263_VOUT1_SEL+vr->id, 1);
//	if (s & 0xFF00) {
//		chprintf(chp, "%s Error\r\n", vr->name);
//		return false;
//	}
//	if ( !(s & 0x80) ) {
//		chprintf(chp, "%s Vset=EXT\r\n", vr->name);
//	} else {
//		s = (s & 0x7F) + 68;
//		chprintf(chp, "%s Vset=%d.%02d\r\n", vr->name, s/100,s%100);
//	}
//	return true;
//}
//bool vpll_setmv(const vreg_t* vr, unsigned mv) {
//	(void)vr;
//	switch( mv ) {
//		case  900: mv = 0; break;
//		case  950: mv = 1; break;
//		case 1000: mv = 2; break;
//		case 1050: mv = 3; break;
//		default: { return false; }
//	}
//    palWriteGroup(GPIOC,3,1,~mv);
//    return true;
//}
//bool vpll_show(const vreg_t* vr, BaseSequentialStream* chp) {
//    unsigned mv=((~GPIOC->ODR >> 1) & 3)*5u+90;
//	//unsigned mv = vreg_data[vr-vreg]*5+90;
//	chprintf(chp, "%-7s Vset=%d.%02dV\r\n", vr->name, mv/100, mv%100);
//	return true;
//}
//
//bool ltc_setmv(const vreg_t* vr, unsigned mv) {
//	return smbWrite(0, vr->addr, (((mv-690)*10)/216)&0x3F,0,0);
//}
//#endif


//void restore_vreg_vout(void)
//{
//	for(size_t i = 0; i < (sizeof(vreg)/sizeof(vreg[0])); i++) {
//		if( vreg_data[i] != 0 ) {
//			vreg[i].setmv(&vreg[i],vreg_data[i]);
//		}
//	}
//}



//const char* const ucd9090_common_faults[] = {
//    "LOG_NOT_EMPTY","SYSTEM_WATCHDOG_TIMEOUT","RESEQUENCE_ERROR","WATCHDOG_TIMEOUT","RESERVED4","RESERVED5","RESERVED6","RESERVED7","Common: "
//};
//const char* const ucd9090_page_faults[] = {
//    "VOUT_OV","VOUT_UV","TON_MAX","IOUT_OC","IOUT_UC","TEMPERATURE_OT","SEQ_ON_TIMEOUT","SEQ_OFF_TIMEOUT","Page#%02d"
//};
//const char* const ucd9090_gpi_faults[] = {
//    "GPI1","GPI2","GPI3","GPI4","GPI5","GPI6","GPI7","GPI8","GPI:    "
//};
//
//const char* const ucd9090_fault_types_paged[] = {
//	"VOUT_OV", "VOUT_UV", "TON_MAX", "IOUT_OC", "IOUT_UC", "OT", "SEQ_ON", "SEQ_OFF",
//};



// ---------- ---------- ---------- ---------- ---------- ----------
#if defined(BMC_WITH_SYSTEM_CONSOLE)


static const ShellConfig shell_cfg1 = {
    .sc_channel = sdu_stdio,
    .sc_commands = commands
};

static THD_WORKING_AREA(waShell, SHELL_WA_SIZE);
#endif


// buttons virtual timers
//static virtual_timer_t bvt[2];

// Power controller
#define PWC_MB_SIZE 8
static msg_t pwc_msgbuf[PWC_MB_SIZE];
//mailbox_t pwc_mb;
MAILBOX_DECL(pwc_mb, pwc_msgbuf, sizeof(pwc_msgbuf)/sizeof(pwc_msgbuf[0]));


/*
 * DFU connectors
 * TODO: power switching should be in separate process; mutex buttons and commands
 * TODO: do not rely on the ATX/sequencer
 * TODO: flash aquire/release
 */
bool sysDfuReady(unsigned alt)
{
	(void) alt;
	return true;
//    if( alt == 1 ) return true;
//    return ( dfu_active && pwc_locked && (!atx_present||ATX_POWER_GOOD_STATE()) && !palReadPad(SEQ_CNTRL_PORT,SEQ_CNTRL_PIN) );
}
void sysDfuStart(unsigned alt) {
    if( alt == 1 || (dfu_active && pwc_locked) ) return;
    dfu_active = 1;
#if 1
    //powerctl(powerctl_on_lock);
    chSysLockFromISR();
    powerctlI(powerctl_on_lock);
    chSysUnlockFromISR();
#else
    // ATX power must be ON
    // CPU must be in RESET state
    while( palReadPad(MIPS_RESETn_PORT, MIPS_RESETn_PIN) ) {
        palClearPad(MIPS_RESETn_PORT, MIPS_RESETn_PIN);
    }
    //TODO: should be a ON/CHECK phase
    ATX_POWER_ON();
    sequencer_ctl(1);
    //sFLASH_AQUIRE(SFDU);
#endif
}
void sysDfuStop(unsigned alt)  {
    if( alt == 1 || !pwc_locked ) return;
    //sFLASH_RELEASE(SFDU);
#if 1
    chSysLockFromISR();
    powerctlI(powerctl_on_unlock);
    chSysUnlockFromISR();
    dfu_active = 0;
#else
    int i = 20;
    while( !palReadPad(MIPS_RESETn_PORT, MIPS_RESETn_PIN) && i-- ) {
        palSetPad(MIPS_RESETn_PORT, MIPS_RESETn_PIN);
    }
#endif
}

thread_reference_t powerButtonThread;
thread_reference_t resetButtonThread;

// For debug
static uint8_t powerButtonThreadNullCount = 0;
static uint8_t resetButtonThreadNullCount = 0;

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/
/*
 * Blinker thread, times are in milliseconds.
 */
//static uint16_t led_idle[]       = {800, 200, 0};
//static uint16_t led_conn[]       = {200, 800, 0};
//static uint16_t led_pon[]        = {800, 200, 0};
//static uint16_t led_fail[]       = {100, 200, 100, 700, 0};
//static uint16_t led_prog[]       = {400, 100, 0};

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void blinkIdle(void)
{
	palSetPad(LED_PORT, LED_GREEN_PIN);
	chThdSleepMilliseconds(200);

	palClearPad(LED_PORT, LED_GREEN_PIN);
	chThdSleepMilliseconds(800);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void blinkFlash(void)
{
	palSetPad(LED_PORT, LED_GREEN_PIN);
	chThdSleepMilliseconds(200);
	palClearPad(LED_PORT, LED_GREEN_PIN);

	palSetPad(LED_PORT, LED_RED_PIN);
	chThdSleepMilliseconds(200);
	palClearPad(LED_PORT, LED_RED_PIN);

	chThdSleepMilliseconds(600);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
uint8_t isFlashState(void)
{
	if ((EXTI->IMR & (~0xFFFFFFFC)) == 0x00000000)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
static THD_WORKING_AREA(waBlinkerThread, 256);
static __attribute__((noreturn)) THD_FUNCTION(BlinkerThread, arg)
{
    (void)arg;

    chRegSetThreadName("blinker");
    while (true)
    {
    	if  (isFlashState())
    	{
    		blState = BS_FLASH;
    	}
    	else
    	{
    		blState = BS_IDLE;
    	}

    	switch (blState)
    	{
    		case BS_IDLE:
    			blinkIdle();
    			//chSchWakeupS(buttonPressThreadPointer, (msg_t) 200);
    			break;
    		case BS_FLASH:
    			blinkFlash();
    			break;
    		default:
    			blinkIdle();
    	}
    }
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
static THD_WORKING_AREA(waButtonPowerThread, 256);
static __attribute__((noreturn)) THD_FUNCTION(ButtonPowerThread, arg)
{
    (void)arg;
    chRegSetThreadName("ButtonPowerThread");

	BaseSequentialStream *outChannel2 = sdu_stdio;

	chSysLock();
	for(;;)
	{
			chThdSuspendTimeoutS(&powerButtonThread, TIME_INFINITE);
		chSysUnlock();
			chprintf(outChannel2, " Power on was pressed \r\n");
			//idtOnOff();
			powerToggle();
		chSysLock();
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
static THD_WORKING_AREA(waButtonResetThread, 256);
static __attribute__((noreturn)) THD_FUNCTION(ButtonResetThread, arg)
{
    (void)arg;
    chRegSetThreadName("ButtonResetThread");

	BaseSequentialStream *outChannel2 = sdu_stdio;

	chSysLock();
	for(;;)
	{
			chThdSuspendTimeoutS(&resetButtonThread, TIME_INFINITE);
		chSysUnlock();
			chprintf(outChannel2, "Reset button was pressed \r\n");
			baikalReset();
		chSysLock();
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
static void btn_do(void *arg)
{
  (void)arg;

  chSysLockFromISR();

//  	  chMBPostI(&buttonPress_mb, (msg_t)(1));
	  if (powerButtonThread != NULL)
	  {
		  buttonResetToggle(); //EXTI->IMR ^= 0x00000001; // Toggle RESET button enable PB0
		  chThdResumeI(&powerButtonThread, MSG_OK);
	  }
	  else
	  {
		  powerButtonThreadNullCount++;
	  }

  chSysUnlockFromISR();

}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
static void baikalResetBtnPressed(void *arg)
{
  (void)arg;
  chSysLockFromISR();

	  if (resetButtonThread != NULL)
	  {
		  chThdResumeI(&resetButtonThread, MSG_OK);
	  }
	  else
	  {
		  resetButtonThreadNullCount++;
	  }
  chSysUnlockFromISR();

}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
// MIPS reset or PowerGOOD
static void extcb_mr(EXTDriver *extp, expchannel_t channel) {
  (void)extp; (void)channel;
  chSysLockFromISR();
  uint32_t s = palReadPad(MIPS_RESETn_PORT, MIPS_RESETn_PIN) ? 0x100 : 0;
  if ( palReadPad(ATX_POWER_OK_PORT, ATX_POWER_OK_PIN) ) s |= 0x200;
  //volatile systime_t t = chVTGetSystemTimeX();
  if ( chMBGetUsedCountI(&pwc_mb) > 0 ) {
	  if ( (chMBPeekI(&pwc_mb) & 0xFF) == (msg_t)(powerctl_none) ) {
		  msg_t dummy;
		  chMBFetchI(&pwc_mb, &dummy);
	  }
  }
  chMBPostAheadI(&pwc_mb, (msg_t)(powerctl_none|s));	// power state change
  chSysUnlockFromISR();
}

// VBUS_FS
void extcb_vusb(EXTDriver *extp, expchannel_t channel) {
  (void)extp; (void)channel;
  //chSysLockFromISR();
  //USBD1.state = USB_READY;
  //chSysUnlockFromISR();
  sysDfuStop(USBD1.alt_setting);
  _usb_reset(&USBD1);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
int powerctl(powerctl_req req)
{
    if ( pwc_locked && req != powerctl_on_unlock ) {
        return 1;
    }
    chMBPost(&pwc_mb, (msg_t)(req), TIME_INFINITE);
    return 0;
}

int powerctlI(powerctl_req req)
{
    if (req >= powerctl_on_lock) {
        chMBPostAheadI(&pwc_mb, (msg_t)(req));
    } else {
    if (pwc_locked) return 1;
        chMBPostI(&pwc_mb, (msg_t)(req));
    }
    return 0;
}


static void double_reset_pulse(void)
{
#ifdef BOARD_BE_BT_BFK20_CPU0
	//for(int i=200; --i && !palReadPad(MIPS_RESETn_PORT, MIPS_RESETn_PIN); ) { chThdSleepMilliseconds(2); }
	chThdSleepMilliseconds(200);
	MIPS_RESET_ASSERT();
	//for(int i=200; --i && palReadPad(MIPS_RESETn_PORT, MIPS_RESETn_PIN); ) { chThdSleepMilliseconds(2); }
	chThdSleepMilliseconds(200);	// 10ms->fault
	MIPS_RESET_RELEASE();
#endif
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
static THD_WORKING_AREA(waButtonsPressThread, 512);
static __attribute__((noreturn)) THD_FUNCTION(buttonsPressThread, arg)
{
	(void)arg;
	  chRegSetThreadName("buttonPressThread");

	  msg_t buffMsg;
	  for(;;)
	  {
//			BaseSequentialStream *outChannel2 = shell_cfg1.sc_channel;
//			chprintf(outChannel2, "Power button was pressed \r\n");
		  // timeout TIME_INFINITE
		  if (MSG_OK != chMBFetch(&buttonPress_mb, &buffMsg, TIME_INFINITE))
		  {
			  continue;
		  }

		  if (buffMsg == 1)
		  {
				//BaseSequentialStream *outChannel2 = shell_cfg1.sc_channel;
				//chprintf(outChannel2, "Power button was pressed \r\n");
				buttonResetToggle();
				powerToggle();
		  }
		  else if (buffMsg == 2)
		  {
				//BaseSequentialStream *outChannel2 = shell_cfg1.sc_channel;
				//chprintf(outChannel2, "Reset button was pressed \r\n");
				baikalReset();
		  }
	  }
}

/*
 * Power control thread
 */
static THD_WORKING_AREA(wapwcThread, 512);
static __attribute__((noreturn)) THD_FUNCTION(pwcThread, arg) {
  (void)arg;
  chRegSetThreadName("pwrctl");
  while(1) {
	systime_t timeout = TIME_INFINITE;
    while(1) {
        msg_t msg=powerctl_none;
        if ( MSG_OK != chMBFetch(&pwc_mb, &msg, timeout) )
            break;
        if ( (msg&0xFF) == powerctl_none ) break;
		timeout = TIME_INFINITE;
        if (pwc_locked) {
            if (msg == powerctl_on_unlock) {
                palSetPad(MIPS_RESETn_PORT, MIPS_RESETn_PIN);	// release reset
                for(int i=5; pwc_state == 2 && --i && !palReadPad(MIPS_RESETn_PORT, MIPS_RESETn_PIN); ) {
                    chThdSleepMilliseconds(2);
                }
                double_reset_pulse();
                pwc_locked = 0;
                break;
            }
            continue;
        }
        if (msg == powerctl_on_lock) {
            pwc_locked = 1;
            palClearPad(MIPS_RESETn_PORT, MIPS_RESETn_PIN);
            for(int i=5; --i && palReadPad(MIPS_RESETn_PORT, MIPS_RESETn_PIN); ) {
                chThdSleepMilliseconds(2);
            }
            msg = powerctl_on;
        }
        if (msg == powerctl_pb) {
			//BFK2.0
            if ((pwc_state == 0 || pwc_state==1)&&palReadPad(SEQ_CNTRL_PORT,SEQ_CNTRL_PIN))
                msg = powerctl_on;
            else
                msg = powerctl_off;
        } else if (msg == powerctl_rb) {
            if (pwc_state != 2) 
				continue;
			palClearPad(MIPS_RESETn_PORT, MIPS_RESETn_PIN);
			clk25_disable();
			sequencer_ctl(0);
			palSetPad(MIPS_RESETn_PORT, MIPS_RESETn_PIN);
			pwc_state = 1;
			timeout=MS2ST(200);
			msg = powerctl_none;
			if ( MSG_OK != chMBFetch(&pwc_mb, &msg, MS2ST(200)) || (msg&0x1FF) == powerctl_none ) {
				chThdSleepMilliseconds(500);
				msg = powerctl_on;
			} else {
				if ( msg >= powerctl_on_lock ) {
					chSysLock();
					chMBResetI(&pwc_mb);
					chMBPostAheadI(&pwc_mb,msg);
					chSysUnlock();
				}
				break;
			}
        }

        if (msg == powerctl_on || (pwc_state!=2 && msg == powerctl_in)) {
            if (pwc_state == 0) {
                ATX_POWER_ON();
                for(int i=50; --i && !ATX_POWER_GOOD_STATE(); )
                    chThdSleepMilliseconds(10);
                if ((atx_state=ATX_POWER_GOOD_STATE())) {
					// remove notification from queue??
                    chThdSleepMilliseconds(15);		// extra delay for regulators power-up
                    pwc_state = (!smbBadBus(1)&&!smbBadBus(2)) ? 1 : -1;
                    if (pwc_state == 1) {
						if( !clk25_check_init() )
							pwc_state = -1;
					}
                } else
                    pwc_state = -1;
            } else  if (pwc_state==2 && msg == powerctl_in) {
				chSysLock();
                sequencer_ctl(0);
                timeout = MS2ST(100);
				chSysUnlock();
                continue;
            }
        }
        if (pwc_state == 1 && msg == powerctl_on) {
			MIPS_RESET_ASSERT();
			chThdSleepMilliseconds(2);		// wait for inv pullup
			clk25_disable();				// should be already disabled,
			chSysLock();
			chSysPolledDelayX(US2RTC(STM32_HCLK,50*4));	// >= 40uS
            sequencer_ctl(1);
            timeout = MS2ST(100);
			chSysUnlock();
			for(int i=50; i--;) {
				chThdSleepMilliseconds(10);	// no events: RESET# is overwritten, and SYS_OK have no interrupt
				if (palReadPad(SYSTEM_OK_PORT,SYSTEM_OK_PIN) ) {
					pwc_state = 2;		// before restore_vreg_vout
					break;
				} else if(!i) {
					pwc_state = -2;
				}
			}
			if( pwc_locked ) {
				continue;
			}
			restore_vreg_vout();
			// wait for voltage transition before enabling clock
			//  - extended from 5 to 100 seems to slightly increase stability on BFK2.0
			chThdSleepMilliseconds(100);
			clk25_enable();
			chThdSleepMilliseconds(100);					// >= 40uS*10periods (CLK), Total >200mS (Power)
			//chSysPolledDelayX(US2RTC(STM32_HCLK,40*10));	// >= 40uS*10periods

			//TODO: remove
			//NOTE: for testing ZL power on 
			//chThdSleepMilliseconds(1000);

			MIPS_RESET_RELEASE();
			chMBFetch(&pwc_mb, &msg, MS2ST(2));
#ifdef BOARD_BE_BT_BFK20
			double_reset_pulse();
			chMBFetch(&pwc_mb, &msg, MS2ST(2));
#endif
			continue;
        }
        if (pwc_state == 2 && msg == powerctl_in) {
			chSysLock();
            sequencer_ctl(0);
            timeout = MS2ST(100);
			chSysUnlock();
			clk25_disable();
			continue;
        }
        if (msg == powerctl_off) {
			chSysLock();
            pwc_state = 1;
            sequencer_ctl(0);
			chSysUnlock();
			clk25_disable();
			if( atx_present ) {
				chThdSleepMilliseconds(100);
				chSysLock();
				atx_state = 0;
				pwc_state = 0;
				ATX_POWER_OFF();
				timeout = MS2ST(100);
				chSysUnlock();
			}
			continue;
        }
    }

	if( pwc_state < 0 ) continue;		// reset errors only by commands
    //int os = pwc_state;
    // recalc power state
    if ( !atx_present || (atx_state=ATX_POWER_GOOD_STATE()) != 0 ) {
        if ( palReadPad(SYSTEM_OK_PORT, SYSTEM_OK_PIN) && palReadPad(MIPS_RESETn_PORT, MIPS_RESETn_PIN) ) {
            pwc_state = 2;
        } else {
            if ( !(palReadLatch(MIPS_RESETn_PORT) & (1<<MIPS_RESETn_PIN)) ) {
                // also CPU_RESET button may affect the state
                uint32_t st = smbRead(0, I2C_ADDR_SQNR, PMBUS_CMD_STATUS_WORD, 2);
                if ( st & 0xFFFF0000 ) {	// read error
					if ( pwc_state > 0 ) pwc_state = -pwc_state;
					else pwc_state = -1;
				} else if ( !(st & PMBUS_ST_PWGDn) ) {
                    pwc_state = 2;
                } else {
                    if ( !(palReadLatch(SEQ_CNTRL_PORT) & (1<<SEQ_CNTRL_PIN)) ) {
                        pwc_state = -2;
                    } else {
                        pwc_state = 1;
                    }
                }
            } else {
                pwc_state = 1;
            }
        }
    } else if (ATX_POWER_ON_STATE()) {
        pwc_state = -1; // ATX FAIL
    } else {
        pwc_state = 0;
    }

    //if (usbGetDriverStateI(SDU1.config->usbp) == USB_ACTIVE && os!=pwc_state) {
    //  chprintf(sdu_stdio,"\r\npower state change: %d -> %d\r\n", os, pwc_state);
    //}

  }
}


/*
 * HID input report processing thread
 */
uint8_t my_in_report[HID_OUT_REPORT_SIZE];
static THD_WORKING_AREA(waHidReportThread, 256);
static __attribute__((noreturn)) THD_FUNCTION(HidReportThread, arg)
{
    (void)arg;
    chRegSetThreadName("hid_input");
    while(true) {
        size_t n = hidReadReport(&HID1, my_in_report, sizeof(my_in_report));
        if (!n) {
            chThdSleepMilliseconds(350);
        }
    }
}

#if HID_REPORT_STAT
static void update_filtered_value(int_filter_t* filter, report_value_t* report_value) {
    // Update average, we expect pre incremented value of filter_current here
    filter->filter -= filter->history[filter_current];
    filter->history[filter_current] = report_value->actual;
    filter->filter += filter->history[filter_current];
    report_value->avg = filter->filter >> FILTER_SHIFT;
    // Update min/max
    if(report_value->min > report_value->actual) {
        report_value->min = report_value->actual;
    }
    if(report_value->max < report_value->actual) {
        report_value->max = report_value->actual;
    }
}
#endif

/*
 * Power monitor thread
 */
static THD_WORKING_AREA(waThread4, 384);
// thread local
static ina226_data ina_d;
static __attribute__((noreturn)) THD_FUNCTION(Thread4, arg) {
    (void)arg;
    unsigned i;
    int skip=0;
    chRegSetThreadName("monitor");
    usb_hid_in_report.sequence_number = 0;
	systime_t t0 = chVTGetSystemTime();
    while(true) {
        usb_hid_in_report.sequence_number++;
#if HID_REPORT_TIME
        usb_hid_in_report.systime = ST2MS(t0);
#endif
        if(!dfu_active && sequencer_state && pwc_state>=1 && !pwc_locked) {
			for(i = 0; i < MIN_DEF(HID_REPORT_SENSOR_CHANNELS,(sizeof(ina)/sizeof(ina[0]))); ++i) {
				if ( ina226_read_data(&ina[i],&ina_d,ina_all) ) {
					chSysLock();
					usb_hid_in_report.report.ina[i].current.actual = ina_d.current;
#if HID_REPORT_POWER
					usb_hid_in_report.report.ina[i].power.actual = ina_d.power;
#endif
					usb_hid_in_report.report.ina[i].voltage.actual = ina_d.voltage;
					chSysUnlock();
				}
				//TODO: read error processing ?
			}
            if(skip) {
                --skip;
            } else {
                // In update_filtered_value function we expect future value of filter_current, so we increment the value in advance
#if HID_REPORT_STAT
                filter_current++;
                if(filter_current == FILTER_HISTORY_LENGTH) {
                    filter_current = 0;
                }
                for(i = 0; i < MIN_DEF(HID_REPORT_SENSOR_CHANNELS,(sizeof(ina)/sizeof(ina[0]))); ++i) {
                    update_filtered_value(&ina_history[i].current, &usb_hid_in_report.report.ina[i].current);
                    update_filtered_value(&ina_history[i].power, &usb_hid_in_report.report.ina[i].power);
                    update_filtered_value(&ina_history[i].voltage, &usb_hid_in_report.report.ina[i].voltage);
                }
#endif
            }
#if HID_REPORT_VTG
            for(i = 0; i < (sizeof(vtg)/sizeof(vtg[0])); ++i) {
                usb_hid_in_report.report.vtg_i[i] = smbRead(0, vtg[i].dev.addr, PMBUS_CMD_READ_IOUT, 2);
            }
#endif
        } else {
            // We skip a few first values from sensors to avoid bogus data in statistics
            skip = 2;
            memset(&usb_hid_in_report.report, 0, sizeof(usb_hid_in_report.report));
#if HID_REPORT_STAT
            filter_current = 0;
            memset(&ina_history, 0, sizeof(ina_history));
            for(i = 0; i < MIN_DEF(HID_REPORT_SENSOR_CHANNELS,(sizeof(ina)/sizeof(ina[0]))); ++i) {
                usb_hid_in_report.report.ina[i].current.min = INT32_MAX;
                usb_hid_in_report.report.ina[i].power.min = INT32_MAX;
                usb_hid_in_report.report.ina[i].voltage.min = INT32_MAX;
            }
#endif
        }

        if( (usbGetDriverStateI(&USBD1) == USB_ACTIVE) && (pwc_state == 2) && !(pwc_locked) ) {
            hidWriteReportt(&HID1, (uint8_t*)&usb_hid_in_report, HID_IN_REPORT_SIZE, TIME_IMMEDIATE);
        }
		t0 = chThdSleepUntilWindowed(t0, t0 + MS2ST(200));
    }
}

#ifdef I2C_ADDR_SC18IS602
unsigned rtd_error;
volatile int rtd_t;


static THD_WORKING_AREA(waThread5, 256);
static __attribute__((noreturn)) THD_FUNCTION(Thread5, arg) {
    (void)arg;
    chRegSetThreadName("rtd");
	while( pwc_state < 1 ) {
		chThdSleepMilliseconds(1000);
		continue;
	}
    smbWrite(0,I2C_ADDR_SC18IS602,0xF6,0x0E,1);  	// GPIO 1,2,3 CS0; GPIO3=RTD_READY#
    smbWrite(0,I2C_ADDR_SC18IS602,0xF7,0x80,1);  	// GPIO3-input-only
    smbWrite(0,I2C_ADDR_SC18IS602,0xF0,0xC|0x01,1); // SPI config: CPOL=1 MSB
    smbWrite(0,I2C_ADDR_SC18IS602,0x01,0x8380,2);   // Vbias,50Hz
    chThdSleepMilliseconds(100);    // Vbias settle
    uint8_t data[4];
    smbWrite(0,I2C_ADDR_SC18IS602,0x01,0x8580,2);    // Start self_check
    do {
		chThdSleepMilliseconds(100);
		smbWrite(0,I2C_ADDR_SC18IS602,0x01,0x00,2); // read config
        i2cReadBlock(I2C_ADDR_SC18IS602,data,2);	// get buffer
    } while (data[1] & 0xC);
    // read status
	smbWrite(0,I2C_ADDR_SC18IS602,0x01,0x07,2);		// read faults
    i2cReadBlock(I2C_ADDR_SC18IS602,data,2);   		// get buffer
    rtd_error = data[1] & ~0xC3;
    if( rtd_error != 0 ) {
		rtd_t = INT_MIN;
		chThdExit(-1);
	}

    uint16_t td;
	systime_t t0 = chVTGetSystemTime();
    while(1) {
		if( pwc_state < 1 ) {
			chThdSleepMilliseconds(1000);
			continue;
		}
        if( !smbWrite(0,I2C_ADDR_SC18IS602,0x01,0xA380,2) ) goto ec;    // Start conv
        do {
			chThdSleepMilliseconds(80); // Conv time
			smbWrite(0,I2C_ADDR_SC18IS602,0xF5,0,1);    // read gpio
			i2cReadBlock(I2C_ADDR_SC18IS602,data,1);	// get buffer
        } while ( !(data[0] & 0x8) );						// until data ready
        if( !smbWrite(0,I2C_ADDR_SC18IS602,0x01,1,3) ) goto ec;	// read temp to buffer
        i2cReadBlock(I2C_ADDR_SC18IS602,data,3);	// get buffer
        td = smbGetData_be(data+1, 2);
        //fault = td & 1;
        rtd_t = (td>>1) - 256*32;
        //if (usbGetDriverStateI(SDU1.config->usbp) == USB_ACTIVE) {
		//	chprintf(sdu_stdio,"\r\nTrtd: %d\r\n", rtd_t);
		//}
ec:
		t0 = chThdSleepUntilWindowed(t0, t0 + MS2ST(200));
    }
}
#endif

#if CH_CFG_NO_IDLE_THREAD
#error "Incorrect config: #CH_CFG_NO_IDLE_THREAD required to allow sleep functions in main thread"
#endif

thread_t *shelltp = NULL;

bool smb_isShellThread(void) { return shelltp == chThdGetSelfX(); }

#if CH_CFG_USE_EVENTS
// Shell exit event.
static void uEvtShellHandler(eventid_t id)
{
  (void)id;
//  sdu_printf("\r\nev_shell: %d\r\n", id);
  if (chThdTerminatedX(shelltp)) {
#if CH_CFG_USE_WAITEXIT
	chThdWait(shelltp);	/* Returning memory to heap.        */
	//or chThdRelease(shelltp);    /* Recovers memory of the previous shell - if not static   */
#endif
	password_reset();
    shelltp = NULL;
  }
}

static void uEvtUsbHandler(eventid_t id)
{
  (void)id;
//  sdu_printf("\r\nev_usb: %d\r\n", id);
}

#endif

/*
 * Application entry point.
 */
int main(void)
{
    halInit();

    /*
     * System initializations.
     * - HAL initialization, this also initializes the configured device drivers
     *   and performs the board-specific initializations.
     * - Kernel initialization, the main() function becomes a thread (???) and the
     *   RTOS is active.
     */
    chSysInit();

    pwc_init();

    buttonResetOff(); //EXTI->IMR &= ~0x00000001; // TurnOff PB0 - Reset button

//    palSetPad(GPIOC, 4); // 1V5
//    palSetPad(GPIOC, 5); // 1V8
//    palSetPad(GPIOC, 13); // 0V95
//    palSetPad(GPIOC, 3); // EN_PLL
//    palSetPad(GPIOD, 0); // EN_CLK_REF
//
//    palClearPad(GPIOC, 4); // 1V5
//    palClearPad(GPIOC, 5); // 1V8

//    palSetPad(GPIOC, 15);


    // Init SPI flash interface on SPI1
    sFLASH_Init(&SFDU1, &sFlash1);
    // flash is not powered on, probe_spi will fail
    sFLASH_Start(&SFDU1);   // start spi driver

    usbSerialInit();

    /*
     * Initializes a serial-over-USB CDC driver.
     */
    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serusbcfg1);

	/*
	 * Initializes a USB DFU driver.
	 */
	dfuObjectInit(&DFU1);
	dfuStart(&DFU1, &dfuusbcfg1);
	//TODO: DFU unable to recover state after incorrect requests
	//TODO: DFU programming can take a long time; should use a thread

	/*
	 * Initializes a USB HID driver.
	 */
	hidObjectInit(&HID1);
	hidStart(&HID1, &hidusbcfg1);

	usbDisconnectBus(&USBD1);
	chThdSleepMilliseconds(1500);

    usbStart(&USBD1, &usbcfg);
    usbConnectBus(&USBD1);
    USBD1.otg->GCCFG = GCCFG_NOVBUSSENS|GCCFG_PWRDWN;   // fixed ChibiOS patch

    /*
     * Activates the USB driver and then the USB bus pull-up on D+.
     * Note, a delay is inserted in order to not have to disconnect the cable
     * after a reset.
     */

  chThdSleepMilliseconds(500);



#if CH_CFG_USE_EVENTS
	static const evhandler_t evhndl[] = {
		uEvtUsbHandler,
	#if defined(BMC_WITH_SYSTEM_CONSOLE)
			uEvtShellHandler
	#endif
		};
		event_listener_t el0;
		chEvtRegister(&ev_usb_activated, &el0, 0);
	#if defined(BMC_WITH_SYSTEM_CONSOLE)
		shellInit();	    								// Shell manager initialization.
		event_listener_t els;
		chEvtRegister(&shell_terminated, &els, 1);
	#endif
#endif

    /*
     * Creates the blinker thread.
     */
    chThdCreateStatic(waBlinkerThread, sizeof(waBlinkerThread), NORMALPRIO, BlinkerThread, NULL);

    //chThdCreateStatic(waButtonPowerThread, sizeof(waButtonPowerThread), NORMALPRIO + 1, ButtonPowerThread, NULL);
    //chThdCreateStatic(waButtonResetThread, sizeof(waButtonResetThread), NORMALPRIO + 1, ButtonResetThread, NULL);

    buttonPressThreadPointer = chThdCreateStatic(waButtonsPressThread, sizeof(waButtonsPressThread), NORMALPRIO + 1, buttonsPressThread, NULL);


	i2cStart(&I2CD1, &i2cfg1);
	i2cStart(&I2CD2, &i2cfg2);

	if (cgIdtStatus == CG_STAT_ERASED)
	{
		BaseSequentialStream *outChannel2 = sdu_stdio;
		cmd_cg(outChannel2, 0, NULL);
	}


    chThdCreateStatic(waHidReportThread, sizeof(waHidReportThread), NORMALPRIO-1, HidReportThread, NULL);
    createSensePrintThread();

	while (true)
	{
		if (password_good() && shelltp == 0 && (usbGetDriverStateI(SDU1.config->usbp) == USB_ACTIVE))
		{
			shelltp = shellCreateStatic(&shell_cfg1, waShell, sizeof(waShell), NORMALPRIO-1);
		}

		chEvtDispatch(evhndl, chEvtWaitAny(ALL_EVENTS));
	}
}
// vim:sw=4:ts=4:et

