//------------------------------------------------------------------------------
//  FILE:           pmbus_cmd_constants.h
//
//  Description:    PMBus command number constants.
//
//  Copyright Texas Instruments © 2008
//------------------------------------------------------------------------------

#ifndef pmbus_cmd_constants_h                   // Header file guard
#define pmbus_cmd_constants_h

// PMBus Version 1.1 command number constants
#define PMBUS_CMD_PAGE                      0x00
#define PMBUS_CMD_OPERATION                 0x01
#define PMBUS_CMD_ON_OFF_CONFIG             0x02
#define PMBUS_CMD_CLEAR_FAULTS              0x03
#define PMBUS_CMD_PHASE                     0x04

//#define PMBUS_CMD_PAGE_PLUS_WRITE           0x05
//#define PMBUS_CMD_PAGE_PLUS_READ            0x06

// 0x05 - 0x0F Reserved
#define PMBUS_CMD_WRITE_PROTECT             0x10
#define PMBUS_CMD_STORE_DEFAULT_ALL         0x11
#define PMBUS_CMD_RESTORE_DEFAULT_ALL       0x12
#define PMBUS_CMD_STORE_DEFAULT_CODE        0x13
#define PMBUS_CMD_RESTORE_DEFAULT_CODE      0x14
#define PMBUS_CMD_STORE_USER_ALL            0x15
#define PMBUS_CMD_RESTORE_USER_ALL          0x16
#define PMBUS_CMD_STORE_USER_CODE           0x17
#define PMBUS_CMD_RESTORE_USER_CODE         0x18
#define PMBUS_CMD_CAPABILITY                0x19
#define PMBUS_CMD_QUERY                     0x1A
// 0x1B - 0x1F Reserved
#define PMBUS_CMD_VOUT_MODE                 0x20
#define PMBUS_CMD_VOUT_COMMAND              0x21
#define PMBUS_CMD_VOUT_TRIM                 0x22
#define PMBUS_CMD_VOUT_CAL_OFFSET           0x23
#define PMBUS_CMD_VOUT_MAX                  0x24
#define PMBUS_CMD_VOUT_MARGIN_HIGH          0x25
#define PMBUS_CMD_VOUT_MARGIN_LOW           0x26
#define PMBUS_CMD_VOUT_TRANSITION_RATE      0x27
#define PMBUS_CMD_VOUT_DROOP                0x28
#define PMBUS_CMD_VOUT_SCALE_LOOP           0x29
#define PMBUS_CMD_VOUT_SCALE_MONITOR        0x2A
// 0x2B - 0x2F Reserved
#define PMBUS_CMD_COEFFICIENTS              0x30
#define PMBUS_CMD_POUT_MAX                  0x31
#define PMBUS_CMD_MAX_DUTY                  0x32
#define PMBUS_CMD_FREQUENCY_SWITCH          0x33
// 0x34 Reserved
#define PMBUS_CMD_VIN_ON                    0x35
#define PMBUS_CMD_VIN_OFF                   0x36
#define PMBUS_CMD_INTERLEAVE                0x37
#define PMBUS_CMD_IOUT_CAL_GAIN             0x38
#define PMBUS_CMD_IOUT_CAL_OFFSET           0x39
#define PMBUS_CMD_FAN_CONFIG_1_2            0x3A
#define PMBUS_CMD_FAN_COMMAND_1             0x3B
#define PMBUS_CMD_FAN_COMMAND_2             0x3C
#define PMBUS_CMD_FAN_CONFIG_3_4            0x3D
#define PMBUS_CMD_FAN_COMMAND_3             0x3E
#define PMBUS_CMD_FAN_COMMAND_4             0x3F
#define PMBUS_CMD_VOUT_OV_FAULT_LIMIT       0x40
#define PMBUS_CMD_VOUT_OV_FAULT_RESPONSE    0x41
#define PMBUS_CMD_VOUT_OV_WARN_LIMIT        0x42
#define PMBUS_CMD_VOUT_UV_WARN_LIMIT        0x43
#define PMBUS_CMD_VOUT_UV_FAULT_LIMIT       0x44
#define PMBUS_CMD_VOUT_UV_FAULT_RESPONSE    0x45
#define PMBUS_CMD_IOUT_OC_FAULT_LIMIT       0x46
#define PMBUS_CMD_IOUT_OC_FAULT_RESPONSE    0x47
#define PMBUS_CMD_IOUT_OC_LV_FAULT_LIMIT    0x48
#define PMBUS_CMD_IOUT_OC_LV_FAULT_RESPONSE 0x49
#define PMBUS_CMD_IOUT_OC_WARN_LIMIT        0x4A
#define PMBUS_CMD_IOUT_UC_FAULT_LIMIT       0x4B
#define PMBUS_CMD_IOUT_UC_FAULT_RESPONSE    0x4C
// 0x4D - 0x4E Reserved
#define PMBUS_CMD_OT_FAULT_LIMIT            0x4F
#define PMBUS_CMD_OT_FAULT_RESPONSE         0x50
#define PMBUS_CMD_OT_WARN_LIMIT             0x51
#define PMBUS_CMD_UT_WARN_LIMIT             0x52
#define PMBUS_CMD_UT_FAULT_LIMIT            0x53
#define PMBUS_CMD_UT_FAULT_RESPONSE         0x54
#define PMBUS_CMD_VIN_OV_FAULT_LIMIT        0x55
#define PMBUS_CMD_VIN_OV_FAULT_RESPONSE     0x56
#define PMBUS_CMD_VIN_OV_WARN_LIMIT         0x57
#define PMBUS_CMD_VIN_UV_WARN_LIMIT         0x58
#define PMBUS_CMD_VIN_UV_FAULT_LIMIT        0x59
#define PMBUS_CMD_VIN_UV_FAULT_RESPONSE     0x5A
#define PMBUS_CMD_IIN_OC_FAULT_LIMIT        0x5B
#define PMBUS_CMD_IIN_OC_FAULT_RESPONSE     0x5C
#define PMBUS_CMD_IIN_OC_WARN_LIMIT         0x5D
#define PMBUS_CMD_POWER_GOOD_ON             0x5E
#define PMBUS_CMD_POWER_GOOD_OFF            0x5F
#define PMBUS_CMD_TON_DELAY                 0x60
#define PMBUS_CMD_TON_RISE                  0x61
#define PMBUS_CMD_TON_MAX_FAULT_LIMIT       0x62
#define PMBUS_CMD_TON_MAX_FAULT_RESPONSE    0x63
#define PMBUS_CMD_TOFF_DELAY                0x64
#define PMBUS_CMD_TOFF_FALL                 0x65
#define PMBUS_CMD_TOFF_MAX_WARN_LIMIT       0x66
#define PMBUS_CMD_POUT_OP_FAULT_LIMIT       0x67
#define PMBUS_CMD_POUT_OP_FAULT_RESPONSE    0x68
#define PMBUS_CMD_POUT_OP_WARN_LIMIT        0x69
#define PMBUS_CMD_PIN_OP_WARN_LIMIT         0x6A
// 0x6B - 0x77 Reserved
#define PMBUS_CMD_STATUS_BYTE               0x78
#define PMBUS_CMD_STATUS_WORD               0x79
#define PMBUS_CMD_STATUS_VOUT               0x7A
#define PMBUS_CMD_STATUS_IOUT               0x7B
#define PMBUS_CMD_STATUS_INPUT              0x7C
#define PMBUS_CMD_STATUS_TEMPERATURE        0x7D
#define PMBUS_CMD_STATUS_CML                0x7E
#define PMBUS_CMD_STATUS_OTHER              0x7F
#define PMBUS_CMD_STATUS_MFR_SPECIFIC       0x80
#define PMBUS_CMD_STATUS_FANS_1_2           0x81
#define PMBUS_CMD_STATUS_FANS_3_4           0x82
// 0x83 - 0x87 Reserved
#define PMBUS_CMD_READ_VIN                  0x88
#define PMBUS_CMD_READ_IIN                  0x89
#define PMBUS_CMD_READ_VCAP                 0x8A
#define PMBUS_CMD_READ_VOUT                 0x8B
#define PMBUS_CMD_READ_IOUT                 0x8C
#define PMBUS_CMD_READ_TEMPERATURE_1        0x8D
#define PMBUS_CMD_READ_TEMPERATURE_2        0x8E
#define PMBUS_CMD_READ_TEMPERATURE_3        0x8F
#define PMBUS_CMD_READ_FAN_SPEED_1          0x90
#define PMBUS_CMD_READ_FAN_SPEED_2          0x91
#define PMBUS_CMD_READ_FAN_SPEED_3          0x92
#define PMBUS_CMD_READ_FAN_SPEED_4          0x93
#define PMBUS_CMD_READ_DUTY_CYCLE           0x94
#define PMBUS_CMD_READ_FREQUENCY            0x95
#define PMBUS_CMD_READ_POUT                 0x96
#define PMBUS_CMD_READ_PIN                  0x97
#define PMBUS_CMD_PMBUS_REVISION            0x98
#define PMBUS_CMD_MFR_ID                    0x99
#define PMBUS_CMD_MFR_MODEL                 0x9A
#define PMBUS_CMD_MFR_REVISION              0x9B
#define PMBUS_CMD_MFR_LOCATION              0x9C
#define PMBUS_CMD_MFR_DATE                  0x9D
#define PMBUS_CMD_MFR_SERIAL                0x9E
// 0x9F Reserved
#define PMBUS_CMD_MFR_VIN_MIN               0xA0
#define PMBUS_CMD_MFR_VIN_MAX               0xA1
#define PMBUS_CMD_MFR_IIN_MAX               0xA2
#define PMBUS_CMD_MFR_PIN_MAX               0xA3
#define PMBUS_CMD_MFR_VOUT_MIN              0xA4
#define PMBUS_CMD_MFR_VOUT_MAX              0xA5
#define PMBUS_CMD_MFR_IOUT_MAX              0xA6
#define PMBUS_CMD_MFR_POUT_MAX              0xA7
#define PMBUS_CMD_MFR_TAMBIENT_MAX          0xA8
#define PMBUS_CMD_MFR_TAMBIENT_MIN          0xA9
#define PMBUS_CMD_MFR_EFFICIENCY_LL         0xAA
#define PMBUS_CMD_MFR_EFFICIENCY_HL         0xAB
#define PMBUS_CMD_IC_DEVICE_ID		    0xAD
#define PMBUS_CMD_IC_DEVICE_REV		    0xAE
// 0xAC - 0xAF Reserved                             // --FLASH USAGE--          --ROM USAGE--
#define PMBUS_CMD_USER_DATA_00              0xB0    //
#define PMBUS_CMD_USER_DATA_01              0xB1    //
#define PMBUS_CMD_USER_DATA_02              0xB2    //
#define PMBUS_CMD_USER_DATA_03              0xB3    //
#define PMBUS_CMD_USER_DATA_04              0xB4
#define PMBUS_CMD_USER_DATA_05              0xB5
#define PMBUS_CMD_USER_DATA_06              0xB6
#define PMBUS_CMD_USER_DATA_07              0xB7
#define PMBUS_CMD_USER_DATA_08              0xB8
#define PMBUS_CMD_USER_DATA_09              0xB9
#define PMBUS_CMD_USER_DATA_10              0xBA
#define PMBUS_CMD_USER_DATA_11              0xBB
#define PMBUS_CMD_USER_DATA_12              0xBC
#define PMBUS_CMD_USER_DATA_13              0xBD
#define PMBUS_CMD_USER_DATA_14              0xBE
#define PMBUS_CMD_USER_DATA_15              0xBF
// 0xC0 - 0xCF Reserved                             // --FLASH USAGE--          --ROM USAGE--
#define PMBUS_CMD_MFR_SPECIFIC_00           0xD0    //
#define PMBUS_CMD_MFR_SPECIFIC_01           0xD1    // VOUT_CAL_MONITOR
#define PMBUS_CMD_MFR_SPECIFIC_02           0xD2    // SYSTEM_RESET_CONFIG
#define PMBUS_CMD_MFR_SPECIFIC_03           0xD3    // SYSTEM_WATCHDOG_CONFIG
#define PMBUS_CMD_MFR_SPECIFIC_04           0xD4    // SYSTEM_WATCHDOG_RESET
#define PMBUS_CMD_MFR_SPECIFIC_05           0xD5    // MONITOR_CONFIG
#define PMBUS_CMD_MFR_SPECIFIC_06           0xD6    // NUM_PAGES
#define PMBUS_CMD_MFR_SPECIFIC_07           0xD7    // RUN_TIME_CLOCK
#define PMBUS_CMD_MFR_SPECIFIC_08           0xD8    // RUN_TIME_CLOCK_TRIM
#define PMBUS_CMD_MFR_SPECIFIC_09           0xD9    // ROM_MODE
#define PMBUS_CMD_MFR_SPECIFIC_10           0xDA    // USER_RAM_00
#define PMBUS_CMD_MFR_SPECIFIC_11           0xDB    // SOFT_RESET
#define PMBUS_CMD_MFR_SPECIFIC_12           0xDC    // RESET_COUNT
#define PMBUS_CMD_MFR_SPECIFIC_13           0xDD    // PIN_SELECTED_RAIL_STATES 
#define PMBUS_CMD_MFR_SPECIFIC_14           0xDE    // RESEQUENCE
#define PMBUS_CMD_MFR_SPECIFIC_15           0xDF    // CONSTANTS
#define PMBUS_CMD_MFR_SPECIFIC_16           0xE0    // PWM_SELECT
#define PMBUS_CMD_MFR_SPECIFIC_17           0xE1    // PWM_CONFIG
#define PMBUS_CMD_MFR_SPECIFIC_18           0xE2    // PARM_INFO
#define PMBUS_CMD_MFR_SPECIFIC_19           0xE3    // PARM_VALUE
#define PMBUS_CMD_MFR_SPECIFIC_20           0xE4    // TEMPERATURE_CAL_GAIN
#define PMBUS_CMD_MFR_SPECIFIC_21           0xE5    // TEMPERATURE_CAL_OFFSET
#define PMBUS_CMD_MFR_SPECIFIC_22           0xE6    // TEMPERATURE_BIAS_PIN
#define PMBUS_CMD_MFR_SPECIFIC_23           0xE7    // FAN_CONFIG_INDEX
#define PMBUS_CMD_MFR_SPECIFIC_24           0xE8    // FAN_CONFIG
#define PMBUS_CMD_MFR_SPECIFIC_25           0xE9    // FAULT_RESPONSES           GENERATE_FLASH_CHECKSUM
#define PMBUS_CMD_MFR_SPECIFIC_26           0xEA    // LOGGED_FAULTS             READ_PSA
#define PMBUS_CMD_MFR_SPECIFIC_27           0xEB    // LOGGED_FAULT_DETAIL_INDEX GENERATE_PSA (V1 only)
#define PMBUS_CMD_MFR_SPECIFIC_28           0xEC    // LOGGED_FAULT_DETAIL       READ_ROM_VERSION
#define PMBUS_CMD_MFR_SPECIFIC_29           0xED    // LOGGED_PAGE_PEAKS         START_PARALLEL_MODE
#define PMBUS_CMD_MFR_SPECIFIC_30           0xEE    // LOGGED_COMMON_PEAKS       READ_CHECKSUM
#define PMBUS_CMD_MFR_SPECIFIC_31           0xEF    // LOG_ENABLES               CALCULATE_CHECKSUM
#define PMBUS_CMD_MFR_SPECIFIC_32           0xF0    // EXECUTE_FLASH             EXECUTE_FLASH
#define PMBUS_CMD_MFR_SPECIFIC_33           0xF1    // SECURITY                  ERASE_FLASH_PAGE
#define PMBUS_CMD_MFR_SPECIFIC_34           0xF2    // SECURITY_BIT_MASK         MASS_ERASE_FLASH
#define PMBUS_CMD_MFR_SPECIFIC_35           0xF3    // MFR_STATUS                WRITE_NEXT_BLOCK
#define PMBUS_CMD_MFR_SPECIFIC_36           0xF4    //                           WRITE_BLOCK
#define PMBUS_CMD_MFR_SPECIFIC_37           0xF5    // MARGIN_CONFIG             WRITE_WORD
#define PMBUS_CMD_MFR_SPECIFIC_38           0xF6    // SEQ_CONFIG                WRITE_HALF_WORD
#define PMBUS_CMD_MFR_SPECIFIC_39           0xF7    // GPO_CONFIG_INDEX          WRITE_BYTE
#define PMBUS_CMD_MFR_SPECIFIC_40           0xF8    // GPO_CONFIG                READ_NEXT_BLOCK
#define PMBUS_CMD_MFR_SPECIFIC_41           0xF9    // GPI_CONFIG                READ_BLOCK
#define PMBUS_CMD_MFR_SPECIFIC_42           0xFA    // GPIO_SELECT               READ_WORD
#define PMBUS_CMD_MFR_SPECIFIC_43           0xFB    // GPIO_CONFIG               READ_HALF_WORD
#define PMBUS_CMD_MFR_SPECIFIC_44           0xFC    // MISC_CONFIG               READ_BYTE
#define PMBUS_CMD_MFR_SPECIFIC_45           0xFD    // DEVICE_ID                 WRITE_ADDRESS
#define PMBUS_CMD_MFR_SPECIFIC_COMMAND_EXT  0xFE
#define PMBUS_CMD_PMBUS_COMMAND_EXT         0xFF


// ----- Manufacturing Specific and User Specific Aliases -----

//#define UCD90_CMD_                        PMBUS_CMD_MFR_SPECIFIC_00   // 0xD0
#define UCD90_CMD_VOUT_CAL_MONITOR          PMBUS_CMD_MFR_SPECIFIC_01   // 0xD1 VOUT_CAL_MONITOR
#define UCD90_CMD_SYSTEM_RESET_CONFIG       PMBUS_CMD_MFR_SPECIFIC_02   // 0xD2 SYSTEM_RESET_CONFIG
#define UCD90_CMD_SYSTEM_WATCHDOG_CONFIG    PMBUS_CMD_MFR_SPECIFIC_03   // 0xD3 SYSTEM_WATCHDOG_CONFIG
#define UCD90_CMD_SYSTEM_WATCHDOG_RESET     PMBUS_CMD_MFR_SPECIFIC_04   // 0xD4 SYSTEM_WATCHDOG_RESET
#define UCD90_CMD_MONITOR_CONFIG            PMBUS_CMD_MFR_SPECIFIC_05   // 0xD5 MONITOR_CONFIG
#define UCD90_CMD_NUM_PAGES                 PMBUS_CMD_MFR_SPECIFIC_06   // 0xD6 NUM_PAGES
#define UCD90_CMD_RUN_TIME_CLOCK            PMBUS_CMD_MFR_SPECIFIC_07   // 0xD7 RUN_TIME_CLOCK
#define UCD90_CMD_RUN_TIME_CLOCK_TRIM       PMBUS_CMD_MFR_SPECIFIC_08   // 0xD8 RUN_TIME_CLOCK_TRIM
#define UCD90_CMD_ROM_MODE                  PMBUS_CMD_MFR_SPECIFIC_09   // 0xD9 ROM_MODE
#define UCD90_CMD_USER_RAM_00               PMBUS_CMD_MFR_SPECIFIC_10   // 0xDA USER_RAM_00
#define UCD90_CMD_SOFT_RESET                PMBUS_CMD_MFR_SPECIFIC_11   // 0xDB SOFT_RESET
#define UCD90_CMD_RESET_COUNT               PMBUS_CMD_MFR_SPECIFIC_12   // 0xDC RESET_COUNT
#define UCD90_CMD_PIN_SELECTED_RAIL_STATES  PMBUS_CMD_MFR_SPECIFIC_13   // 0xDD PIN_SELECTED_RAIL_STATES 
#define UCD90_CMD_RESEQUENCE                PMBUS_CMD_MFR_SPECIFIC_14   // 0xDE RESEQUENCE
#define UCD90_CMD_CONSTANTS                 PMBUS_CMD_MFR_SPECIFIC_15   // 0xDF CONSTANTS
#define UCD90_CMD_PWM_SELECT                PMBUS_CMD_MFR_SPECIFIC_16   // 0xE0 PWM_SELECT
#define UCD90_CMD_PWM_CONFIG                PMBUS_CMD_MFR_SPECIFIC_17   // 0xE1 PWM_CONFIG
#define UCD90_CMD_PARM_INFO                 PMBUS_CMD_MFR_SPECIFIC_18   // 0xE2 parm_index, parm_offset and parm_length for parm_value cmd
#define UCD90_CMD_PARM_VALUE                PMBUS_CMD_MFR_SPECIFIC_19   // 0xE3 parm_value for TFA & Trace
#define UCD90_CMD_TEMPERATURE_CAL_GAIN      PMBUS_CMD_MFR_SPECIFIC_20   // 0xE4 Temperature cal gain (slope)  degC/V
#define UCD90_CMD_TEMPERATURE_CAL_OFFSET    PMBUS_CMD_MFR_SPECIFIC_21   // 0xE5 Temperature calibration offset  degC

#define UCD90_CMD_TEMPERATURE_BIAS_PIN      PMBUS_CMD_MFR_SPECIFIC_22   // 0xE6 TEMPERATURE_BIAS_PIN

#define UCD90_CMD_FAN_CONFIG_INDEX          PMBUS_CMD_MFR_SPECIFIC_23   // 0xE7 FAN_CONFIG_INDEX
#define UCD90_CMD_FAN_CONFIG                PMBUS_CMD_MFR_SPECIFIC_24   // 0xE8 FAN_CONFIG
#define UCD90_CMD_FAULT_RESPONSES           PMBUS_CMD_MFR_SPECIFIC_25   // 0xE9 FAULT_RESPONSES
#define UCD90_CMD_LOGGED_FAULTS             PMBUS_CMD_MFR_SPECIFIC_26   // 0xEA LOGGED_FAULTS
#define UCD90_CMD_LOGGED_FAULT_DETAIL_INDEX PMBUS_CMD_MFR_SPECIFIC_27   // 0xEB LOGGED_FAULT_DETAIL_INDEX
#define UCD90_CMD_LOGGED_FAULT_DETAIL       PMBUS_CMD_MFR_SPECIFIC_28   // 0xEC LOGGED_FAULT_DETAIL
#define UCD90_CMD_LOGGED_PAGE_PEAKS         PMBUS_CMD_MFR_SPECIFIC_29   // 0xED LOGGED_PAGE_PEAKS
#define UCD90_CMD_LOGGED_COMMON_PEAKS       PMBUS_CMD_MFR_SPECIFIC_30   // 0xEE LOGGED_COMMON_PEAKS
#define UCD90_CMD_LOG_ENABLES               PMBUS_CMD_MFR_SPECIFIC_31   // 0xEF LOG_ENABLES
#define UCD90_CMD_EXECUTE_FLASH             PMBUS_CMD_MFR_SPECIFIC_32   // 0xF0 Write-only.  Same as ROM's EXECUTE_FLASH command.
                                                                        // Doesn't RUNly do anything if already running from
                                                                        // Flash, but prevents "Invalid command" errors.

#define UCD90_CMD_SECURITY                  PMBUS_CMD_MFR_SPECIFIC_33   // 0xF1 SECURITY
#define UCD90_CMD_SECURITY_BIT_MASK         PMBUS_CMD_MFR_SPECIFIC_34   // 0xF2 SECURITY_BIT_MASK

#define UCD90_CMD_MFR_STATUS                PMBUS_CMD_MFR_SPECIFIC_35   // 0xF3 MFR_STATUS
//#define UCD90_CMD_                        PMBUS_CMD_MFR_SPECIFIC_36   // 0xF4
#define UCD90_CMD_MARGIN_CONFIG             PMBUS_CMD_MFR_SPECIFIC_37   // 0xF5 MARGIN_CONFIG
#define UCD90_CMD_SEQ_CONFIG                PMBUS_CMD_MFR_SPECIFIC_38   // 0xF6 SEQ_CONFIG
#define UCD90_CMD_GPO_CONFIG_INDEX          PMBUS_CMD_MFR_SPECIFIC_39   // 0xF7 GPO_CONFIG_INDEX
#define UCD90_CMD_GPO_CONFIG                PMBUS_CMD_MFR_SPECIFIC_40   // 0xF8 GPO_CONFIG
#define UCD90_CMD_GPI_CONFIG                PMBUS_CMD_MFR_SPECIFIC_41   // 0xF9 GPI_CONFIG
#define UCD90_CMD_GPIO_SELECT               PMBUS_CMD_MFR_SPECIFIC_42   // 0xFA GPIO_SELECT
#define UCD90_CMD_GPIO_CONFIG               PMBUS_CMD_MFR_SPECIFIC_43   // 0xFB GPIO_CONFIG
#define UCD90_CMD_MISC_CONFIG               PMBUS_CMD_MFR_SPECIFIC_44   // 0xFC MISC_CONFIG
#define UCD90_CMD_DEVICE_ID                 PMBUS_CMD_MFR_SPECIFIC_45   // 0xFD Read only Version ID ("UCD90120")

#define PMBUS_OOC_CONTROLLED			0x10
#define PMBUS_OOC_COMMAND			0x08
#define PMBUS_OOC_PIN				0x04
#define PMBUS_OOC_PIN_POLARITY			0x02
#define PMBUS_OOC_DISABLE_OFF_DELAY		0x01

#define PMBUS_OPERATION_FAST_OFF		0x00
#define PMBUS_OPERATION_SOFT_OFF		0x40
#define PMBUS_OPERATION_ON			0x80
#define PMBUS_OPERATION_ON_LOW_NF		0x94
#define PMBUS_OPERATION_ON_LOW_AF		0x98
#define PMBUS_OPERATION_ON_HIGH_NF		0xA4
#define PMBUS_OPERATION_ON_HIGH_AF		0xA8

#define PMBUS_ST_NOTA		0x01
#define PMBUS_ST_CML		0x02
#define PMBUS_ST_TEMP_FW	0x04
#define PMBUS_ST_VIN_UV_F	0x08
#define PMBUS_ST_IOUT_OC_F	0x10
#define PMBUS_ST_VOUT_OV_F	0x20
#define PMBUS_ST_UNIT_ISOFF	0x40
#define PMBUS_ST_UNIT_BUSY	0x80
#define PMBUS_ST_UNKNOWN_FW	0x0100
#define PMBUS_ST_OTHER		0x0200
#define PMBUS_ST_FAN		0x0400
#define PMBUS_ST_PWGDn		0x0800
#define PMBUS_ST_MFR		0x1000
#define PMBUS_ST_INPUT_FW	0x2000
#define PMBUS_ST_IPOUT_FW	0x4000
#define PMBUS_ST_VOUT_FW	0x8000

//------------------------------------------------------------------------------
#endif // End of header guard: #ifndef pmbus_prototypes_h
 
