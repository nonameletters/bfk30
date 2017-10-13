#include <stdint.h>
#include <assert.h>

#include <tps56.h>
#include "smbus.h"

#ifdef BOARD_BE_BT_BFK16

/*
 * Resolution: 0.01V
 */
void tps56_set_cV(const pmbus_dev_t* dev, unsigned cV) {
    chDbgAssert(cV >=60 && cV <=187,"Invalid range");
    cV = (cV - 60) & 0x7F;
    // calc odd parity in bit.7
    unsigned par = (cV ^ (cV << 4));        // using native int - shorter code
    par = (par ^ (par << 2));
    par = ~(par ^ (par << 1)) & 0x80;
    smbWrite(dev->i2cp, dev->addr, TPS56_VOUT, cV|par, 1);
}

uint8_t tps56_get_cV(const pmbus_dev_t* dev) {
    return ((smbRead(dev->i2cp, dev->addr, TPS56_VOUT, 1) & 0x7f) + 60);
}

uint8_t tps56_status(const pmbus_dev_t* dev)
{
    return (uint8_t)smbRead(dev->i2cp, dev->addr, TPS56_ST, 1);
}

uint32_t tps56_dump(const pmbus_dev_t* dev) 
{
    msg_t status;
    uint8_t tx_buffer[2];
    uint8_t rx_buffer[2];
    uint32_t val = 0;
    tx_buffer[0] = TPS56_VOUT;
    i2cAcquireBus(dev->i2cp);
    status = i2cMasterTransmitTimeout(dev->i2cp, dev->addr,
                    tx_buffer, 1, rx_buffer, 1, MS2ST(50));
    if( status != MSG_OK ) goto ex;
    val |= rx_buffer[0];
    val <<= 8;

    tx_buffer[0] = TPS56_A;
    status = i2cMasterTransmitTimeout(dev->i2cp, dev->addr,
                    tx_buffer, 1, rx_buffer, 1, MS2ST(50));
    if( status != MSG_OK ) goto ex;
    val |= rx_buffer[0];
    val <<= 8;

    tx_buffer[0] = TPS56_B;
    status = i2cMasterTransmitTimeout(dev->i2cp, dev->addr,
                    tx_buffer, 1, rx_buffer, 1, MS2ST(50));
    if( status != MSG_OK ) goto ex;
    val |= rx_buffer[0];
    val <<= 8;

    tx_buffer[0] = TPS56_ST;
    status = i2cMasterTransmitTimeout(dev->i2cp, dev->addr,
                    tx_buffer, 1, rx_buffer, 1, MS2ST(50));
    if( status != MSG_OK ) goto ex;
    val |= rx_buffer[0];
ex:
    if( status != MSG_OK ) {
        smbWriteLog(dev->i2cp, dev->addr, tx_buffer[0], status);
    }
    i2cReleaseBus(dev->i2cp);
    return val;
}

bool clk25_init(void) 	{ return true; }
bool clk25_check_init(void) { return true; }
bool clk25_enable(void) { return true; }
bool clk25_disable(void) { return true; }

#endif

#ifdef BOARD_BE_BT_BFK20
/*
 * Resolution: 0.01V
 */
void tps65_set_cV(const pmbus_dev_t* dev, unsigned chan, unsigned cV) {
    chDbgAssert(cV >=68 && cV <=195,"Invalid range");
    smbWrite(dev->i2cp, dev->addr, TPS65263_VOUT1_SEL+chan, TPS65263_VOUT_SET(cV-68,1), 1);
}

#define code
#include "register_map.h"


bool clk25_disable(void)
{
    return smbWrite(0, I2C_ADDR_SI5351, SI5351_R_OUTPUT_ENABLE, 0xFF, 1);
}
bool clk25_check_init(void)
{
    return smbRead(0, I2C_ADDR_SI5351, 0x10, 2) == 0x4F4F || clk25_init();
}
bool clk25_enable(void)
{
    return clk25_check_init() && smbWrite(0, I2C_ADDR_SI5351, SI5351_R_OUTPUT_ENABLE, 0xFF&~0x3, 1);
}
bool clk25_init(void)
{
    // Check (wait for) SYS_INIT complete
    int i = 10;
    while( --i ) {
        unsigned s = smbRead(0, I2C_ADDR_SI5351, 0, 1) & SI5351_SYS_INIT;
        if( s & ~0xFFu ) return false;
        if( !(s & SI5351_SYS_INIT ) )
            break;
        chThdSleepMilliseconds(50);
    }
    if( i == 0 )
        return false;
    if ( !clk25_disable() ) return false;	// disable outputs
    if ( !smbWrite(0, I2C_ADDR_SI5351, SI5351_R_INT_STATUS_STICKY, 0x08, 1) ) return false;     // clear sticky flags
    // disable clocks
    for(int i=16; i <= 23; i++) {
        if ( !smbWrite(0, I2C_ADDR_SI5351, i, 0x80, 1) ) return false;
    }
#ifdef CLK25_FAST_LOAD
    if ( !i2cWriteBlock(0, I2C_ADDR_SI5351, clk25_init_15,  sizeof(clk25_init_15 )) ) return false;
    if ( !i2cWriteBlock(0, I2C_ADDR_SI5351, clk25_init_149, sizeof(clk25_init_149)) ) return false;
#else
    for(int i=0; i < NUM_REGS_MAX; i++) {
        if ( !smbWrite(0, I2C_ADDR_SI5351, Reg_Store[i].Reg_Addr, Reg_Store[i].Reg_Val, 1) ) return false;
    }
#endif
    if ( !smbWrite(0, I2C_ADDR_SI5351, SI5351_R_PLL_RESET, 0x0C|SI5351_PLLB_RST|SI5351_PLLA_RST, 1) ) return false;
    i = 4;
    while( --i ) {
        unsigned s = smbRead(0, I2C_ADDR_SI5351, 0, 1) & SI5351_SYS_INIT;
        if( s & ~0xFFu || !i ) return false;
        if( !(s & SI5351_SYS_INIT ) )
            break;
        chThdSleepMilliseconds(50);
    }
    if( i == 0 )
        return false;
    i = 200;
    do {
        chThdSleepMilliseconds(10);
    } while( --i && smbRead(0, I2C_ADDR_SI5351, 0, 1) & (SI5351_LOL_A) );       // wait PLL lock
    if ( !smbWrite(0, I2C_ADDR_SI5351, SI5351_R_INT_MASK, SI5351_LOL_B|SI5351_LOS_S|3, 1) ) return false;
    // leave in disabled state
    return i != 0;
}

#endif
// vim: sw=4:ts=4:et
