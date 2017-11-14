//------------------------------------------------------------------------------
//
// Copyright (c) 2011 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
//------------------------------------------------------------------------------
//
// This firmware runs on a TI Stellaris LM3S811 processor, with the supplied
// libraries and support code.  It was compiled with the Keil uVision IDE V4.03.
//
// NOTE: This code serves as a minimal example of how a UCD9xxx device is
//       programmed.  The code is not very robust - it lacks most of the
//       recovery/response code that is needed to handle communication errors
//       if they occur.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// INCLUDE FILES
//------------------------------------------------------------------------------

#include <ch.h>
#include <hal.h>
#include <string.h>
#include "chprintf.h"
#include "smbus.h"
//#include "pec.h"
#include "pmbus_cmd_constants.h"
#include "x.h"


//------------------------------------------------------------------------------
// data flash array
//------------------------------------------------------------------------------
//extern const uint32_t dflash[];
#include "ucd9090_flash.c"

unsigned long sizeof_dflash(void) { return sizeof(dflash)/sizeof(*dflash); }

//------------------------------------------------------------------------------
// DEFINITIONS
//------------------------------------------------------------------------------
#define DISPLAY_RX (0)
#define DISPLAY_TX (0)

#define FAIL (0)
#define PASS (1)

#define MAX_PACKET_SIZE (34)

// Set the address for slave module. This is a 7-bit address sent in the
// following format:
//                      [A6:A5:A4:A3:A2:A1:A0:RS]
//
// A zero in the R/S position of the first byte means that the master
// transmits (sends) data to the selected slave, and a one in this position
// means that the master receives data from the slave.
#define SLAVE_ADDRESS I2C_ADDR_SQNR



//------------------------------------------------------------------------------
// MACROS
//------------------------------------------------------------------------------

// Uncomment one of these lines based on the endianess ofthe PMBus master
#define LITTLE_ENDIAN_PMBUS_MASTER (TRUE)
//#define LITTLE_ENDIAN_PMBUS_MASTER (FALSE)

#if LITTLE_ENDIAN_PMBUS_MASTER
    // This is needed because the PMBus master is little endian and the slave
    // is big endian.
    #define SWAP(_DATA_) (((_DATA_) >> 24)                |                    \
                         (((_DATA_) >>  8) & 0x0000FF00)  |                    \
                         (((_DATA_) <<  8) & 0x00FF0000)  |                    \
                         (((_DATA_) << 24) & 0xFF000000))
#else
    #define SWAP(_DATA_)
#endif


//------------------------------------------------------------------------------
// CONSTANTS
//------------------------------------------------------------------------------

static const char device_id[] = "UCD9090|2.3.5.0000|110701";

//------------------------------------------------------------------------------
// GLOBAL VARIABLES
//------------------------------------------------------------------------------

static const unsigned char write_protect_pkt[]  = { UCD90_CMD_PARM_INFO, 0x05, 0x04, 0x00, 0x00, 0x01, 0x04 };
static const unsigned char dflash_control_pkt[] = { UCD90_CMD_PARM_INFO, 0x05, 0x04, 0x14, 0x00, 0x01, 0x04 };

static const unsigned char write_protect_pkt_value[] = { UCD90_CMD_PARM_VALUE, 0x04, 0x00, 0x00, 0x88, 0x20 };
static const unsigned char erase_dflash_value[] = { UCD90_CMD_PARM_VALUE, 0x04, 0x00, 0x00, 0x01, 0x00 };


//------------------------------------------------------------------------------
// EXTERNAL FUNCTIONS
//------------------------------------------------------------------------------

// The ammount of data that will be programmed
extern unsigned long sizeof_dflash(void);


//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// verify_device_id()
//
// Verifying DEVICE_ID matches expected firmware
//------------------------------------------------------------------------------
static bool verify_device_id(BaseSequentialStream* chp)
{
    uint8_t packet[MAX_PACKET_SIZE];

	if ( !smbReadBlock(0, SLAVE_ADDRESS, UCD90_CMD_DEVICE_ID, packet, sizeof(packet)-2) )
		return false;
	packet[SMB_MIN(MAX_PACKET_SIZE-1, packet[0]+1)] = 0;
	chprintf(chp,"Device: %s\r\n", (const char*)packet+1);
    if( strncmp((const char*)packet+1, device_id, sizeof(device_id)-1) == 0 )
		return true;
	chprintf(chp, "Required: %s\r\n", device_id);
	return false;
}


//------------------------------------------------------------------------------
// wait_on_dflash()
//------------------------------------------------------------------------------
static bool wait_on_dflash(void)
{
	uint8_t data[8];
    unsigned long  dflash_control_reg;
	chThdSleepMilliseconds(20);

    do
    {
		if ( !smbWriteBlock(0,SLAVE_ADDRESS, dflash_control_pkt) )
			return false;
		if ( !smbReadBlock(0,SLAVE_ADDRESS, UCD90_CMD_PARM_VALUE, data, 5) )
			return false;
		dflash_control_reg = smbGetData_le(data+1,4);
    }
    while ((dflash_control_reg & 0x00080000) != 0);
    return true;
}


//------------------------------------------------------------------------------
// pause()
//
// Pause a specified number of milliseconds.  The actual pause time may be up to
// one millisecond greater than the requested time.
//------------------------------------------------------------------------------
void pause(unsigned long milliseconds)
{
	chThdSleepMilliseconds(milliseconds);
}


//------------------------------------------------------------------------------
// main()
//------------------------------------------------------------------------------
bool  ucd9090_config(BaseSequentialStream* chp)
{
    //unsigned char  DataByte;
    uint8_t  packet[MAX_PACKET_SIZE];
    uint8_t  dpacket[4+2];
    unsigned long  dflash_word;
    unsigned long  index;


    if ( !verify_device_id(chp) ) return false;

    //..........................................................................
    // Clearing data flash write protect bit; any flash logging will be disabled
    // until device reset ...
    //..........................................................................
    if ( !smbWriteBlock(0, SLAVE_ADDRESS, write_protect_pkt) ) return false;
    if ( !smbWriteBlock(0, SLAVE_ADDRESS, write_protect_pkt_value) ) return false;


    //..........................................................................
    // Erasing data flash ...
    //..........................................................................
    if ( !smbWriteBlock(0, SLAVE_ADDRESS, dflash_control_pkt) ) return false;
    if ( !smbWriteBlock(0, SLAVE_ADDRESS, erase_dflash_value) ) return false;


    if ( !wait_on_dflash() ) return false;


    //..........................................................................
    // Writing data flash in 4 byte chunks ...
    //..........................................................................
    packet[0]   = UCD90_CMD_PARM_INFO;
    packet[1]   = 5;
    packet[0+2] = 0x02;        // Index for base address            = Data Flash
    packet[3+2] = 0x01;        // Number of elements                = 1
    packet[4+2] = 0x04;        // The size of each element in bytes = 4

	dpacket[0] = UCD90_CMD_PARM_VALUE;
	dpacket[1] = 4;
	
	chprintf(chp,"Programming...\r\n");

    for (index = 0; index < sizeof_dflash(); index++)
    {
        // Offset from the base address
        packet[1+2] = index & 0xFF;
        packet[2+2] = (index >> 8) & 0xFF;

		if ( !smbWriteBlock(0, SLAVE_ADDRESS, packet) ) return false;

		dpacket[2] = dflash[index] >> 24;
		dpacket[3] = dflash[index] >> 16;
		dpacket[4] = dflash[index] >> 8;
		dpacket[5] = dflash[index];

        if ( !smbWriteBlock(0, SLAVE_ADDRESS, dpacket) ) return false;

        if ( !wait_on_dflash() ) return false;
    }


	chprintf(chp,"Verifying...\r\n");
    //..........................................................................
    // Verifying data flash
    //..........................................................................
    packet[0]   = UCD90_CMD_PARM_INFO;
    packet[1]   = 5;
    packet[0+2] = 0x02;        // Index for base address            = Data Flash
    packet[3+2] = 0x01;        // Number of elements                = 1
    packet[4+2] = 0x04;        // The size of each element in bytes = 4

    for (index = 0; index < sizeof_dflash(); index++)
    {
        // Offset from the base address
        packet[1+2] = index & 0xFF;
        packet[2+2] = (index >> 8) & 0xFF;

		if ( !smbWriteBlock(0, SLAVE_ADDRESS, packet) ) return false;
		if ( !smbReadBlock(0, SLAVE_ADDRESS, UCD90_CMD_PARM_VALUE, dpacket, 5) ) return false;

        dflash_word = smbGetData_be(dpacket+1, 4);

        if (dflash_word != dflash[index])
        {
            chprintf(chp, "\n\n*** Data Flash Read Error ***\r\n");
            chprintf(chp, "   Read     : %08X\r\n", dflash_word);
            chprintf(chp, "   Expected : %08X\r\n", dflash[index]);
            return false;
        }
    }

	chprintf(chp,"Restarting...\r\n");
    //..........................................................................
    // Resetting the device
    if ( !smbWrite(0, SLAVE_ADDRESS, UCD90_CMD_SOFT_RESET, 0, 0) ) return false;

    // Waiting for UCD9090 program to startup
    pause(1500);



    // It is a good idea to clear peak and fault logs at this point in order to
    // remove any bogus logging when the device was unconfigured or partially
    // configured.
    //
    // (see LOGGED_FAULTS, LOGGED_PAGED_PEAKS, and LOGGED_COMMON_PEAKS)

   if( !ucd9090_clear_log(I2C_ADDR_SQNR) ) return false;
   if( !smbWrite(0,I2C_ADDR_SQNR, PMBUS_CMD_CLEAR_FAULTS,0,0) ) return false;

    return verify_device_id(chp);
    //..........................................................................
	//return true;
}
