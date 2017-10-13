#include <ch.h>
#include <stdlib.h>
#include <string.h>

#include "smbus.h"
#include "pmbus_cmd_constants.h"


static const pmbus_cmd_def pmbus_cmd_table[] = {
{ "PAGE",                      PMBUS_CMD_PAGE,                      PCF_BYTE,  0, 0 },
{ "OPERATION",                 PMBUS_CMD_OPERATION,                 PCF_BITS,  0, 0 },
{ "ON_OFF_CONFIG",             PMBUS_CMD_ON_OFF_CONFIG,             PCF_BITS,  0, 0 },
{ "CLEAR_FAULTS",              PMBUS_CMD_CLEAR_FAULTS,              PCF_ZERO,  PCF_WO, 0 },
{ "PHASE",                     PMBUS_CMD_PHASE,                     PCF_BITS,  0, 0 },
{ "WRITE_PROTECT",             PMBUS_CMD_WRITE_PROTECT,             PCF_BITS,  0, 0 },
{ "STORE_DEFAULT_ALL",         PMBUS_CMD_STORE_DEFAULT_ALL,         PCF_ZERO,  PCF_WO, 0 },
{ "RESTORE_DEFAULT_ALL",       PMBUS_CMD_RESTORE_DEFAULT_ALL,       PCF_ZERO,  PCF_WO, 0 },
{ "STORE_DEFAULT_CODE",        PMBUS_CMD_STORE_DEFAULT_CODE,        PCF_BYTE,  PCF_WO, 0 },
{ "RESTORE_DEFAULT_CODE",      PMBUS_CMD_RESTORE_DEFAULT_CODE,      PCF_BYTE,  PCF_WO, 0 },
{ "STORE_USER_ALL",            PMBUS_CMD_STORE_USER_ALL,            PCF_ZERO,  PCF_WO, 0 },
{ "RESTORE_USER_ALL",          PMBUS_CMD_RESTORE_USER_ALL,          PCF_ZERO,  PCF_WO, 0 },
{ "STORE_USER_CODE",           PMBUS_CMD_STORE_USER_CODE,           PCF_BYTE,  PCF_WO, 0 },
{ "RESTORE_USER_CODE",         PMBUS_CMD_RESTORE_USER_CODE,         PCF_BYTE,  PCF_WO, 0 },
{ "CAPABILITY",                PMBUS_CMD_CAPABILITY,                PCF_BYTE,  PCF_RO, 0 },
{ "QUERY",                     PMBUS_CMD_QUERY,                     PCF_BLOCK, PCF_RO, 1 },
{ "VOUT_MODE",                 PMBUS_CMD_VOUT_MODE,                 PCF_BYTE,  0, 0 },
{ "VOUT_COMMAND",              PMBUS_CMD_VOUT_COMMAND,              PCF_L16u,  0, 'V' },
{ "VOUT_TRIM",                 PMBUS_CMD_VOUT_TRIM,                 PCF_WORD,  0, 0 },	//?
{ "VOUT_CAL_OFFSET",           PMBUS_CMD_VOUT_CAL_OFFSET,           PCF_WORD,  0, 0 },	//?
{ "VOUT_MAX",                  PMBUS_CMD_VOUT_MAX,                  PCF_L16u,  0, 'V' },
{ "VOUT_MARGIN_HIGH",          PMBUS_CMD_VOUT_MARGIN_HIGH,          PCF_L16u,  0, 'V' },
{ "VOUT_MARGIN_LOW",           PMBUS_CMD_VOUT_MARGIN_LOW,           PCF_L16u,  0, 'V' },
{ "VOUT_TRANSITION_RATE",      PMBUS_CMD_VOUT_TRANSITION_RATE,      PCF_L11,   0, 0 },
{ "VOUT_DROOP",                PMBUS_CMD_VOUT_DROOP,                PCF_WORD,  0, 0 },	//?
{ "VOUT_SCALE_LOOP",           PMBUS_CMD_VOUT_SCALE_LOOP,           PCF_WORD,  0, 0 },	//?
{ "VOUT_SCALE_MONITOR",        PMBUS_CMD_VOUT_SCALE_MONITOR,        PCF_L11,   0, 0 },
//{ "COEFFICIENTS",              PMBUS_CMD_COEFFICIENTS,              PCF_BLOCK, 0, 5 }, // block read-block write
{ "POUT_MAX",                  PMBUS_CMD_POUT_MAX,                  PCF_L11,   0, 0 },
{ "MAX_DUTY",                  PMBUS_CMD_MAX_DUTY,                  PCF_L11,   0, 0 },
{ "FREQUENCY_SWITCH",          PMBUS_CMD_FREQUENCY_SWITCH,          PCF_L11,   0, 0 },
{ "VIN_ON",                    PMBUS_CMD_VIN_ON,                    PCF_WORD,  0, 0 },
{ "VIN_OFF",                   PMBUS_CMD_VIN_OFF,                   PCF_WORD,  0, 0 },
{ "INTERLEAVE",                PMBUS_CMD_INTERLEAVE,                PCF_BITS,  0, 0 },
{ "IOUT_CAL_GAIN",             PMBUS_CMD_IOUT_CAL_GAIN,             PCF_L11,   0, 0 },
{ "IOUT_CAL_OFFSET",           PMBUS_CMD_IOUT_CAL_OFFSET,           PCF_L11,   0, 0 },
{ "FAN_CONFIG_1_2",            PMBUS_CMD_FAN_CONFIG_1_2,            PCF_BITS,  0, 0 },
{ "FAN_COMMAND_1",             PMBUS_CMD_FAN_COMMAND_1,             PCF_L11,   0, 0 },
{ "FAN_COMMAND_2",             PMBUS_CMD_FAN_COMMAND_2,             PCF_L11,   0, 0 },
{ "FAN_CONFIG_3_4",            PMBUS_CMD_FAN_CONFIG_3_4,            PCF_BITS,  0, 0 },
{ "FAN_COMMAND_3",             PMBUS_CMD_FAN_COMMAND_3,             PCF_L11,   0, 0 },
{ "FAN_COMMAND_4",             PMBUS_CMD_FAN_COMMAND_4,             PCF_L11,   0, 0 },
{ "VOUT_OV_FAULT_LIMIT",       PMBUS_CMD_VOUT_OV_FAULT_LIMIT,       PCF_L16u,  0, 'V' },
{ "VOUT_OV_FAULT_RESPONSE",    PMBUS_CMD_VOUT_OV_FAULT_RESPONSE,    PCF_FR,    0, 0 },
{ "VOUT_OV_WARN_LIMIT",        PMBUS_CMD_VOUT_OV_WARN_LIMIT,        PCF_L16u,  0, 'V' },
{ "VOUT_UV_WARN_LIMIT",        PMBUS_CMD_VOUT_UV_WARN_LIMIT,        PCF_L16u,  0, 'V' },
{ "VOUT_UV_FAULT_LIMIT",       PMBUS_CMD_VOUT_UV_FAULT_LIMIT,       PCF_L16u,  0, 'V' },
{ "VOUT_UV_FAULT_RESPONSE",    PMBUS_CMD_VOUT_UV_FAULT_RESPONSE,    PCF_FR,    0, 0  },
{ "IOUT_OC_FAULT_LIMIT",       PMBUS_CMD_IOUT_OC_FAULT_LIMIT,       PCF_L11,   0, 'A' },
{ "IOUT_OC_FAULT_RESPONSE",    PMBUS_CMD_IOUT_OC_FAULT_RESPONSE,    PCF_FR,    0, 0  },
{ "IOUT_OC_LV_FAULT_LIMIT",    PMBUS_CMD_IOUT_OC_LV_FAULT_LIMIT,    PCF_L11,   0, 'A' },
{ "IOUT_OC_LV_FAULT_RESPONSE", PMBUS_CMD_IOUT_OC_LV_FAULT_RESPONSE, PCF_FR,    0, 0  },
{ "IOUT_OC_WARN_LIMIT",        PMBUS_CMD_IOUT_OC_WARN_LIMIT,        PCF_L11,   0, 'A' },
{ "IOUT_UC_FAULT_LIMIT",       PMBUS_CMD_IOUT_UC_FAULT_LIMIT,       PCF_L11,   0, 'A' },
{ "IOUT_UC_FAULT_RESPONSE",    PMBUS_CMD_IOUT_UC_FAULT_RESPONSE,    PCF_FR,    0, 0  },
{ "OT_FAULT_LIMIT",            PMBUS_CMD_OT_FAULT_LIMIT,            PCF_L11,   0, 'C' },
{ "OT_FAULT_RESPONSE",         PMBUS_CMD_OT_FAULT_RESPONSE,         PCF_FR,    0, 0  },
{ "OT_WARN_LIMIT",             PMBUS_CMD_OT_WARN_LIMIT,             PCF_L11,   0, 'C' },
{ "UT_WARN_LIMIT",             PMBUS_CMD_UT_WARN_LIMIT,             PCF_L11,   0, 'C' },
{ "UT_FAULT_LIMIT",            PMBUS_CMD_UT_FAULT_LIMIT,            PCF_L11,   0, 'C' },
{ "UT_FAULT_RESPONSE",         PMBUS_CMD_UT_FAULT_RESPONSE,         PCF_FR,    0, 0  },
{ "VIN_OV_FAULT_LIMIT",        PMBUS_CMD_VIN_OV_FAULT_LIMIT,        PCF_L11,   0, 'V' },
{ "VIN_OV_FAULT_RESPONSE",     PMBUS_CMD_VIN_OV_FAULT_RESPONSE,     PCF_FR,    0, 0  },
{ "VIN_OV_WARN_LIMIT",         PMBUS_CMD_VIN_OV_WARN_LIMIT,         PCF_L11,   0, 'V' },
{ "VIN_UV_WARN_LIMIT",         PMBUS_CMD_VIN_UV_WARN_LIMIT,         PCF_L11,   0, 'V' },
{ "VIN_UV_FAULT_LIMIT",        PMBUS_CMD_VIN_UV_FAULT_LIMIT,        PCF_L11,   0, 'V' },
{ "VIN_UV_FAULT_RESPONSE",     PMBUS_CMD_VIN_UV_FAULT_RESPONSE,     PCF_FR,    0, 0  },
{ "IIN_OC_FAULT_LIMIT",        PMBUS_CMD_IIN_OC_FAULT_LIMIT,        PCF_L11,   0, 'A' },
{ "IIN_OC_FAULT_RESPONSE",     PMBUS_CMD_IIN_OC_FAULT_RESPONSE,     PCF_FR,    0, 0  },
{ "IIN_OC_WARN_LIMIT",         PMBUS_CMD_IIN_OC_WARN_LIMIT,         PCF_L11,   0, 'A' },
{ "POWER_GOOD_ON",             PMBUS_CMD_POWER_GOOD_ON,             PCF_L16u,  0, 'V' },
{ "POWER_GOOD_OFF",            PMBUS_CMD_POWER_GOOD_OFF,            PCF_L16u,  0, 'V' },
{ "TON_DELAY",                 PMBUS_CMD_TON_DELAY,                 PCF_L11,   0, 's' },
{ "TON_RISE",                  PMBUS_CMD_TON_RISE,                  PCF_L11,   0, 0 },
{ "TON_MAX_FAULT_LIMIT",       PMBUS_CMD_TON_MAX_FAULT_LIMIT,       PCF_L11,   0, 's' },
{ "TON_MAX_FAULT_RESPONSE",    PMBUS_CMD_TON_MAX_FAULT_RESPONSE,    PCF_FR,    0, 0  },
{ "TOFF_DELAY",                PMBUS_CMD_TOFF_DELAY,                PCF_L11,   0, 's' },
{ "TOFF_FALL",                 PMBUS_CMD_TOFF_FALL,                 PCF_L11,   0, 0 },
{ "TOFF_MAX_WARN_LIMIT",       PMBUS_CMD_TOFF_MAX_WARN_LIMIT,       PCF_L11,   0, 's' },
{ "POUT_OP_FAULT_LIMIT",       PMBUS_CMD_POUT_OP_FAULT_LIMIT,       PCF_L11,   0, 0 },
{ "POUT_OP_FAULT_RESPONSE",    PMBUS_CMD_POUT_OP_FAULT_RESPONSE,    PCF_FR,    0, 0  },
{ "POUT_OP_WARN_LIMIT",        PMBUS_CMD_POUT_OP_WARN_LIMIT,        PCF_L11,   0, 0 },
{ "PIN_OP_WARN_LIMIT",         PMBUS_CMD_PIN_OP_WARN_LIMIT,         PCF_L11,   0, 0 },
{ "STATUS_BYTE",               PMBUS_CMD_STATUS_BYTE,               PCF_BITS,  PCF_RO, 0 },
{ "STATUS_WORD",               PMBUS_CMD_STATUS_WORD,               PCF_BITSW, PCF_RO, 0 },
{ "STATUS_VOUT",               PMBUS_CMD_STATUS_VOUT,               PCF_BITS,  PCF_RO, 0 },
{ "STATUS_IOUT",               PMBUS_CMD_STATUS_IOUT,               PCF_BITS,  PCF_RO, 0 },
{ "STATUS_INPUT",              PMBUS_CMD_STATUS_INPUT,              PCF_BITS,  PCF_RO, 0 },
{ "STATUS_TEMPERATURE",        PMBUS_CMD_STATUS_TEMPERATURE,        PCF_BITS,  PCF_RO, 0 },
{ "STATUS_CML",                PMBUS_CMD_STATUS_CML,                PCF_BITS,  PCF_RO, 0 },
{ "STATUS_OTHER",              PMBUS_CMD_STATUS_OTHER,              PCF_BITS,  PCF_RO, 0 },
{ "STATUS_MFR_SPECIFIC",       PMBUS_CMD_STATUS_MFR_SPECIFIC,       PCF_BITS,  PCF_RO, 0 },
{ "STATUS_FANS_1_2",           PMBUS_CMD_STATUS_FANS_1_2,           PCF_BITS,  PCF_RO, 0 },
{ "STATUS_FANS_3_4",           PMBUS_CMD_STATUS_FANS_3_4,           PCF_BITS,  PCF_RO, 0 },
{ "READ_VIN",                  PMBUS_CMD_READ_VIN,                  PCF_L11,   PCF_RO, 'V' },
{ "READ_IIN",                  PMBUS_CMD_READ_IIN,                  PCF_L11,   PCF_RO, 'A' },
{ "READ_VCAP",                 PMBUS_CMD_READ_VCAP,                 PCF_L11,   PCF_RO, 'V' },
{ "READ_VOUT",                 PMBUS_CMD_READ_VOUT,                 PCF_L16u,  PCF_RO, 'V' },
{ "READ_IOUT",                 PMBUS_CMD_READ_IOUT,                 PCF_L11,   PCF_RO, 'A' },
{ "READ_TEMPERATURE_1",        PMBUS_CMD_READ_TEMPERATURE_1,        PCF_L11,   PCF_RO, 'C' },
{ "READ_TEMPERATURE_2",        PMBUS_CMD_READ_TEMPERATURE_2,        PCF_L11,   PCF_RO, 'C' },
{ "READ_TEMPERATURE_3",        PMBUS_CMD_READ_TEMPERATURE_3,        PCF_L11,   PCF_RO, 'C' },
{ "READ_FAN_SPEED_1",          PMBUS_CMD_READ_FAN_SPEED_1,          PCF_L11,   PCF_RO, 0 },
{ "READ_FAN_SPEED_2",          PMBUS_CMD_READ_FAN_SPEED_2,          PCF_L11,   PCF_RO, 0 },
{ "READ_FAN_SPEED_3",          PMBUS_CMD_READ_FAN_SPEED_3,          PCF_L11,   PCF_RO, 0 },
{ "READ_FAN_SPEED_4",          PMBUS_CMD_READ_FAN_SPEED_4,          PCF_L11,   PCF_RO, 0 },
{ "READ_DUTY_CYCLE",           PMBUS_CMD_READ_DUTY_CYCLE,           PCF_L11,   PCF_RO, '%' },
{ "READ_FREQUENCY",            PMBUS_CMD_READ_FREQUENCY,            PCF_L11,   PCF_RO, 'h' },
{ "READ_POUT",                 PMBUS_CMD_READ_POUT,                 PCF_L11,   PCF_RO, 0 },
{ "READ_PIN",                  PMBUS_CMD_READ_PIN,                  PCF_L11,   PCF_RO, 0 },
{ "PMBUS_REVISION",            PMBUS_CMD_PMBUS_REVISION,            PCF_BYTE,  0, 0 },
{ "MFR_ID",                    PMBUS_CMD_MFR_ID,                    PCF_BLOCK, 0, 0 },
{ "MFR_MODEL",                 PMBUS_CMD_MFR_MODEL,                 PCF_BLOCK, 0, 0 },
{ "MFR_REVISION",              PMBUS_CMD_MFR_REVISION,              PCF_BLOCK, 0, 0 },
{ "MFR_LOCATION",              PMBUS_CMD_MFR_LOCATION,              PCF_BLOCK, 0, 0 },
{ "MFR_DATE",                  PMBUS_CMD_MFR_DATE,                  PCF_BLOCK, 0, 0 },
{ "MFR_SERIAL",                PMBUS_CMD_MFR_SERIAL,                PCF_BLOCK, 0, 0 },
{ "MFR_VIN_MIN",               PMBUS_CMD_MFR_VIN_MIN,               PCF_WORD,  PCF_RO, 0 },
{ "MFR_VIN_MAX",               PMBUS_CMD_MFR_VIN_MAX,               PCF_WORD,  PCF_RO, 0 },
{ "MFR_IIN_MAX",               PMBUS_CMD_MFR_IIN_MAX,               PCF_WORD,  PCF_RO, 0 },
{ "MFR_PIN_MAX",               PMBUS_CMD_MFR_PIN_MAX,               PCF_WORD,  PCF_RO, 0 },
{ "MFR_VOUT_MIN",              PMBUS_CMD_MFR_VOUT_MIN,              PCF_WORD,  PCF_RO, 0 },
{ "MFR_VOUT_MAX",              PMBUS_CMD_MFR_VOUT_MAX,              PCF_WORD,  PCF_RO, 0 },
{ "MFR_IOUT_MAX",              PMBUS_CMD_MFR_IOUT_MAX,              PCF_WORD,  PCF_RO, 0 },
{ "MFR_POUT_MAX",              PMBUS_CMD_MFR_POUT_MAX,              PCF_WORD,  PCF_RO, 0 },
{ "MFR_TAMBIENT_MAX",          PMBUS_CMD_MFR_TAMBIENT_MAX,          PCF_WORD,  PCF_RO, 0 },
{ "MFR_TAMBIENT_MIN",          PMBUS_CMD_MFR_TAMBIENT_MIN,          PCF_WORD,  PCF_RO, 0 },
{ "MFR_EFFICIENCY_LL",         PMBUS_CMD_MFR_EFFICIENCY_LL,         PCF_BLOCK, PCF_RO, 0 },
{ "MFR_EFFICIENCY_HL",         PMBUS_CMD_MFR_EFFICIENCY_HL,         PCF_BLOCK, PCF_RO, 0 },
{ "IC_DEVICE_ID",				PMBUS_CMD_IC_DEVICE_ID,         	PCF_BLOCK, PCF_RO, 0 },
{ "IC_DEVICE_REV",				PMBUS_CMD_IC_DEVICE_REV,         	PCF_BLOCK, PCF_RO, 0 },
{ "USER_DATA_0",				PMBUS_CMD_USER_DATA_00,				PCF_BLOCK, 0, 128 },	// B0
//ZL2102
{ "ZL_AUTO_COMP_CONFIG",			0xBC,							PCF_BYTE,  0, 0 },
{ "ZL_AUTO_COMP_CONTROL",			0xBD,							PCF_ZERO,  0, 0 },
{ "ZL_MFR_CONFIG",					0xD0,							PCF_BYTE,  0, 0 },
{ "ZL_USER_CONFIG",					0xD1,							PCF_BYTE,  0, 0 },
{ "ZL_POWER_GOOD_DELAY",			0xD4,							PCF_L11,   0, 'S' },
{ "ZL_PID_TAPS",					0xD5,							PCF_BLOCK, 0, 9 },
{ "ZL_DEVICE_ID",					0xE4,							PCF_BLOCK, PCF_RO, 0 },
{ "ZL_MFR_IOUT_OC_FAULT_RESPONSE",	0xE5,							PCF_FR,  0, 0 },		// def. BF - continuously, 80 shutdown
{ "ZL_MFR_IOUT_UC_FAULT_RESPONSE",	0xE6,							PCF_FR,  0, 0 },		// def. BF - continuously, 80 shutdown
{ "ZL_IOUT_AVG_OC_FAULT_LIMIT",		0xE7,							PCF_L11,   0, 0 },		// def. CB99=7.2A
{ "ZL_IOUT_AVG_UC_FAULT_LIMIT",		0xE8,							PCF_L11,   0, 0 },		// def. CC67=-7.2A
{ "ZL_MISC_CONFIG",					0xE9,							PCF_WORD,  0, 0 },
{ "ZL_SNAPSHOT",					0xEA,							PCF_BLOCK,  PCF_RO, 32 },
{ "ZL_BLANK_PARAMS",				0xEB,							PCF_BLOCK,  PCF_RO, 16 },
{ "ZL_SNAPSHOT_CONTROL",			0xF3,							PCF_BYTE,  0, 0 },		// def 0
{ "ZL_RESTORE_FACTORY",				0xF4,							PCF_ZERO,  PCF_WO, 0 },		// def 0

{ "UCD_SYSTEM_RESET_CONFIG",        0xD2,                           PCF_BLOCK, 0, 9},
{ "UCD_RESET_COUNT",                0xDC,                           PCF_WORD , 0, 0},
{ "UCD_NUM_PAGES",                  0xD6,                           PCF_BYTE,  PCF_RO, 0},
{ "UCD_RUN_TIME_CLOCK",             0xD7,                           PCF_BLOCK, 0, 8},
{ "UCD_PARM_INFO",                  0xE2,                           PCF_BLOCK, 0, 5},
{ "UCD_PARM_VALUE",                 0xE3,                           PCF_BLOCK, 0, 4},
{ "UCD_FAULT_RESPONSES",            0xE9,                           PCF_BLOCK, 0, 9},
{ "UCD_LOGGED_PAGE_PEAKS",          0xED,                           PCF_BLOCK, 0, 9},
{ "UCD_SEQ_CONFIG",                 0xF6,                           PCF_BLOCK, 0, 19},
{ "UCD_GPO_CONFIG_INDEX",           0xF7,                           PCF_BYTE,  0, 0},
{ "UCD_GPO_CONFIG",                 0xF8,                           PCF_BLOCK, 0, 21},
{ "UCD_GPI_CONFIG",                 0xF9,                           PCF_BLOCK, 0, 13},
{ "UCD_GPIO_SELECT",                0xFA,                           PCF_BYTE,  0, 0},
{ "UCD_GPIO_CONFIG",                0xFB,                           PCF_BYTE,  0, 0},
{ "UCD_MISC_CONFIG",                0xFC,                           PCF_BLOCK, 0, 32},
{ "UCD_DEVICE_ID",                  0xFD,                           PCF_BLOCK, 0, 2},  //[46] 0-Brownout, 1-FIFO Entire Log, 2-Enable Log FIFO, 5:4 Max Reseq, 6 Reseq Abort, 7 Reseq Contin

};

#define PCT_SIZE (sizeof(pmbus_cmd_table)/sizeof(pmbus_cmd_table[0]))

const char* pmbus_cmd_units(const pmbus_cmd_def* pc) {
	switch (pc->size) {
		case 'V': return "V";
		case 'A': return "A";
		case 's': return "mS";
		case 'C': return "C";
		case '%': return "%";
		case 'h': return "kHz";
	}
	return "";
}

const pmbus_cmd_def* pmbus_find_command(char* argv) {
	char* tail;
	uint32_t val = strtol(argv, &tail, 16);
	if( *tail == 0 ) { // hex number given
		if (val >= PMBUS_CMD_MFR_SPECIFIC_COMMAND_EXT)
			return NULL;
		for (size_t i=0; i < PCT_SIZE; ++i) {
			if ( pmbus_cmd_table[i].cmd == val )
				return &pmbus_cmd_table[i];
		}
	} else {
		const pmbus_cmd_def* cm = NULL;
		for (size_t i=0; argv[i]; ++i) if(argv[i]>='a'&&argv[i]<='z') argv[i] &= ~0x20; // toupper
		for (size_t i=0; i < PCT_SIZE; ++i) {
			if ( strstr(pmbus_cmd_table[i].command, argv) ) {
				if ( !strcmp(pmbus_cmd_table[i].command, argv) ) return &pmbus_cmd_table[i];
				if ( cm != NULL ) return NULL;	// ambiguous command
				cm = &pmbus_cmd_table[i];
			}
		}
		return cm;
	}
	return NULL;
}


static const char* const pmbus_status_msg_str[16] = {
	"NONE_OF_THE_ABOVE",
	"COMM_MEMORY_LOGIC_EVENT",
	"TEMPERATURE_FAULT_OR_WARNING",
	"VIN_UV_FAULT",
	"IOUT_OC_FAULT",
	"VOUT_OV_FAULT",
	"UNIT_IS_OFF",
	"UNIT_WAS_BUSY",
	"UNKNOWN_FAULT_OR_WARNING",
	"OTHER",
	"FAN_FAULT_OR_WARNING",
	"POWER_GOOD_Negated",
	"MFR_SPECIFIC",
	"INPUT_FAULT_OR_WARNING",
	"IOUT/POUT_FAULT_OR_WARNING",
	"VOUT_FAULT_OR_WARNING"
};


const char* pmbus_status_msg(uint16_t* st)
{
	int i;
	for( i=15; i >= 0; --i) {
		if( *st & (1<<i) ) {
			*st &= ~(1<<i);
			return pmbus_status_msg_str[i];
		}
	}
	return "";
}

static const char* const pmbus_status_vout_str[8] = {
	"TRACKING_ERROR",
	"TOFF_MAX_WARNING",
	"TON_MAX_FAULT",
	"VOUT_MAX_WARNING",
	"VOUT_UV_FAULT",
	"VOUT_UV_WARNING",
	"VOUT_OV_WARNING",
	"VOUT_OV_FAULT"
};
static const char* const pmbus_status_iout_str[8] = {
	"POUT_OP_WARNING",
	"POUT_OP_FAULT",
	"In_Power_Limiting_Mode",
	"Current_Share_Fault",
	"IOUT_UC_FAULT",
	"IOUT_OC_WARNING",
	"IOUT_OC_LV_FAULT",
	"IOUT_OC_FAULT"
};
static const char* const pmbus_status_temperature_str[8] = {
	"Reserved0",
	"Reserved1",
	"Reserved2",
	"Reserved3",
	"UT_FAULT",
	"UT_WARNING",
	"OT_WARNING",
	"OT_FAULT"
};
static const char* const pmbus_status_cml_str[8] = {
	"Other Memory Or Logic Fault",
	"Other Communication Fault",
	"Reserved2",
	"Processor Fault Detected",
	"Memory Fault Detected",
	"Packet Error Check Failed",
	"Invalid/Unsupported Data",
	"Invalid/Unsupported Command"
};
static const char* const pmbus_status_input_str[8] = {
	"PIN_OP_WARNING",
	"IIN_OC_WARNING",
	"IIN_OC_FAULT",
	"Unit Off For Low Input Voltage",
	"VIN_UV_FAULT",
	"VIN_UV_WARNING",
	"VIN_OV_WARNING",
	"VIN_OV_FAULT"
};

const char* pmbus_status_sub(unsigned cmd, uint8_t* st) {
	const char* const* stab;
	switch (cmd) {
		case PMBUS_CMD_STATUS_BYTE: stab = pmbus_status_msg_str; break;
		case PMBUS_CMD_STATUS_VOUT: stab = pmbus_status_vout_str; break;
		case PMBUS_CMD_STATUS_IOUT: stab = pmbus_status_iout_str; break;
		case PMBUS_CMD_STATUS_CML:  stab = pmbus_status_cml_str; break;
		case PMBUS_CMD_STATUS_TEMPERATURE:  stab = pmbus_status_temperature_str; break;
		case PMBUS_CMD_STATUS_INPUT:  stab = pmbus_status_input_str; break;
		default: return "";
	}
	for( int i=15; i >= 0; --i) {
		if( *st & (1<<i) ) {
			*st &= ~(1<<i);
			return stab[i];
		}
	}
	return "";
}

static const char* pmbus_fault_response_str[4] = {
	"I=OC_FL", 	"I=OC_FL,V>UV_FL", "I=OC_FL,t<delay", "ShutDown"
};

void pmbus_fault_response(BaseSequentialStream* chp, unsigned code)
{
	if( ((code>>3)&7) == 0 ) {
		chprintf(chp,"%s",pmbus_fault_response_str[3]);
	} else {
		chprintf(chp,"%s; Retry=%d Delay=%d",
		  pmbus_fault_response_str[(code>>6)&3],
		  (code>>3)&7,code&7);
	}
}
