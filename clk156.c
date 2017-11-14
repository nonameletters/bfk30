
#include <stdint.h>
#include <string.h>

#if !defined(__linux__) && !defined(I2C_WGL)
#include <ch.h>
#include <hal.h>
#include "x.h"
#include "smbus.h"
#include "clk156.h"

#else

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
typedef uint32_t i2caddr_t;

#ifdef I2C_WGL_NOCOMMENT
#define cprintf(...)
#else
#define cprintf printf
#endif

char SCL='Z', SDA='Z';

#ifdef I2C_META
void wgl_op()
{
	printf("%c %c\n", SCL, SDA);
}

void wgl_i2c_start()
{
	printf("S ");
}

void wgl_i2c_ack()
{
	printf(" a ");
}

void wgl_i2c_ack_out(bool ack)
{
	printf(" %c ", ack ? 'A':'N');
}

void wgl_i2c_stop()
{
	printf("P\n");
}

void wgl_i2c_bit(unsigned b)
{
	printf("%d", b ? 1 : 0);
}

void wgl_i2c_bit_check(unsigned b)
{
	printf("%d", b ? 1 : 0);
}
#else
void wgl_op()
{
	printf("%c %c\n", SCL, SDA);
}

void wgl_i2c_start()
{
	cprintf("; S\n");
	assert(SCL=='H'&&SDA=='H');
	SDA = 'L';
	wgl_op();
	SCL = 'L';
	wgl_op();	// bit length
}

void wgl_i2c_ack()
{
	cprintf("; a\n");
	assert(SCL=='L');
	SDA = 'X';
	wgl_op();	// bit length
	SCL = 'H'; SDA = '0'; // Check for ACK
	wgl_op();
	wgl_op();	// bit length

	SCL = 'L'; SDA = 'L';
	wgl_op();
}

void wgl_i2c_ack_out(bool ack)
{
	cprintf("; %c\n", ack ? 'A':'N');
	assert(SCL=='L');
	SDA = ack ? 'L' : 'H';
	wgl_op();	// bit length
	wgl_op();
	SCL = 'H';
	wgl_op();
	wgl_op();	// bit length

	SCL = 'L'; SDA = ack ? 'X' : 'L';
	wgl_op();
}

void wgl_i2c_stop()
{
	cprintf("; P\n");
	assert(SCL=='L'&&SDA=='L');
	SDA = 'L';
	wgl_op();	// bit length

	SCL = 'H';
	wgl_op();
	SDA = 'H';
	wgl_op();	// bit length
}

void wgl_i2c_bit(unsigned b)
{
	cprintf("; %d\n", b ? 1 : 0);
	assert(SCL=='L');
//	wgl_op();	// bit length
	SDA = b ? 'H' : 'L';
	wgl_op();
	SCL = 'H';
	wgl_op();

	wgl_op();	// bit length

	SCL = 'L';
	wgl_op();
}

void wgl_i2c_bit_check(unsigned b)
{
	cprintf("; =%d\n", b ? 1 : 0);
	assert(SCL=='L');
//	wgl_op();	// bit length
	SDA = 'X';
	wgl_op();
	SCL = 'H';
	SDA = b ? '1' : '0';
	wgl_op();
	wgl_op();	// bit length

	SDA = 'X'; SCL = 'L';
	wgl_op();
}

#endif

void chThdSleepMilliseconds(uint32_t ms)
{
	//for(unsigned i=0; i < TESTER_FREQ/ms; ++i) wgl_nop();
	printf("Sleep: %d ms\n", ms);
}

static void wgl_cmd(i2caddr_t addr, uint8_t cmd)
{
	addr <<= 1;  // D.0=0 -> write op
	wgl_i2c_start();
	for(int i=0; i<8; ++i) {
		wgl_i2c_bit(addr & 0x80);
		addr <<= 1;
	}
	wgl_i2c_ack();
	for(int i=0; i<8; ++i) {
		wgl_i2c_bit(cmd & 0x80);
		cmd <<= 1;
	}
	wgl_i2c_ack();
}


bool 	 smbWrite(void *i2cp, i2caddr_t addr, uint8_t cmd, uint32_t value, size_t count)
{
	assert(i2cp==NULL&&count<=4);
	cprintf("; %02x %0*x\n", cmd, count*2, value);
	wgl_cmd(addr, cmd);

	while( count -- ) {
		unsigned val = value;
		value >>= 8;
		for(int i=0; i<8; ++i) {
			wgl_i2c_bit(val & 0x80);
			val <<= 1;
		}
		wgl_i2c_ack();
	}
	wgl_i2c_stop();
	return true;
}

int	 smbCheck(void *i2cp, i2caddr_t addr, uint8_t cmd, uint32_t value, size_t count)
{
	assert(i2cp==NULL&&count<=4);
	cprintf("; @%02x = %0*x\n", cmd, count, value);
	wgl_cmd(addr, cmd);
	wgl_i2c_stop();

	addr = (addr << 1) | 1;	// D.0=1 -> read op
	wgl_i2c_start();
	for(int i=0; i<8; ++i) {
		wgl_i2c_bit(addr & 0x80);
		addr <<= 1;
	}
	wgl_i2c_ack();

	while( count-- ) {
		unsigned val = value;
		value >>= 8;
		for(int i=0; i<8; ++i) {
			wgl_i2c_bit_check(val & 0x80);
			val <<= 1;
		}
		wgl_i2c_ack_out(count!=0);
	}
	wgl_i2c_stop();
	return 0;	// match
}


uint32_t smbRead(void *i2cp, i2caddr_t addr, uint8_t cmd, size_t count)
{
	assert(i2cp==NULL&&count==1);
	return cmd == 0xFE ? 0xF : 0;
}
#endif

#define SI5342_REVD_REG_CONFIG_PREAMBLE 6
#define SI5342_REVD_REG_CONFIG_POSTAMBLE 6
#include "Si5342-RevD-156_25-Registers.h"


static bool clk156_wait(i2caddr_t addr, uint32_t ms)
{
#if !defined(__linux__) && !defined(I2C_WGL)
    for( uint32_t i=ms/100; i; --i ) {
		chThdSleepMilliseconds(100);
		switch( smbCheck(0, addr, 0xFE, 0x0F, 1) ) {
			case -1: return false;
			case  0: return true;
		}
    };
    return false;
#else
	return true;
#endif
}

static uint16_t clk156_override_disable = 1;
static uint16_t clk156_override[256];

bool clk156_set_override(unsigned address, unsigned value)
{
	if( (address & ~0xFFu) != 0x0100u ) return false; // only page1
	if( (value   & ~0xFFu) != 0u ) return false; // sanity
	if( clk156_override_disable ) {
		memset(clk156_override,0xFF,sizeof(clk156_override));
		clk156_override_disable = 0;
	}
	clk156_override[address & 0xff] = value & 0xFF;
	return true;
}

static inline uint8_t clk156_get_cfg_data(unsigned index)
{
	if( (si5342_revd_registers[index].address & ~0xFFu) == 0x100u && !clk156_override_disable ) {
		uint16_t ovr = clk156_override[si5342_revd_registers[index].address & 0xFF];
		if( (ovr & 0xFF00) == 0 ) return ovr;
	}
	return si5342_revd_registers[index].value;
}

bool clk156_init(i2caddr_t addr)
{
    if( smbCheck(0, addr, 0xFE, 0x0F, 1) ) return false;
    unsigned page = ~0u;
    for(unsigned i=0; i < SI5342_REVD_REG_CONFIG_NUM_REGS; ) {
        unsigned reg = si5342_revd_registers[i].address;
        if( page != (reg & 0x7F00) ) {
            page = (reg & 0x7F00);
            if ( !smbWrite(0, addr, 1, page>>8, 1) ) return false; // select page
        }
        uint32_t val = si5342_revd_registers[i].value;
        unsigned count = 1;
        for(; i+count < SI5342_REVD_REG_CONFIG_NUM_REGS && si5342_revd_registers[i+count].address == reg+count;) {
			val = val | (clk156_get_cfg_data(i+count) << (8*count));
			if( ++count == 4 ) break;
		}
        if ( !smbWrite(0, addr, reg, val, count) ) return false;
        if ( (count==1&&i==1) || (count==2&&i==0) ) chThdSleepMilliseconds(300); // delay after preamble
        i += count;
    }
    return clk156_wait(addr, 300);
}

bool clk156_verify(i2caddr_t addr, unsigned* failreg)
{
	*failreg = 0xFE;
    if( smbCheck(0, addr, 0xFE, 0x0F, 1) ) return false;
    unsigned page = ~0u;
    for(unsigned i=SI5342_REVD_REG_CONFIG_PREAMBLE; i < SI5342_REVD_REG_CONFIG_NUM_REGS-SI5342_REVD_REG_CONFIG_POSTAMBLE; ) {
        unsigned reg = si5342_revd_registers[i].address;
        if( reg & 0x8000 ) { ++i; continue;	/* do not verify marker */ }
        if( page != (reg & 0xFF00) ) {
            page = (reg & 0x7F00);
            if ( !smbWrite(0, addr, 1, page>>8, 1) ) return false; // select page
        }
        uint32_t val = si5342_revd_registers[i].value;
        unsigned count = 1;
        for(; i+count < SI5342_REVD_REG_CONFIG_NUM_REGS-SI5342_REVD_REG_CONFIG_POSTAMBLE && si5342_revd_registers[i+count].address == reg+count;) {
			val = val | (si5342_revd_registers[i+count].value << (8*count));
			if( ++count == 4 ) break;
		}
		*failreg = reg;
        if ( smbCheck(0, addr, reg, val, count) ) return false;
        i += count;
    }
    return true;
}


bool clk156_program_nvm(i2caddr_t addr)
{
    if( smbCheck(0, addr, 0xFE, 0x0F, 1) ) return false;
    if ( !smbWrite(0, addr, 1, 0, 1) ) return false;	// select page0
    if ( !smbWrite(0, addr, 0xE3, 0xC7, 1) ) return false;
    // wait for programming complete
    return clk156_wait(addr, 5000);
}

bool clk156_read_nvm_bank(i2caddr_t addr, int bank)
{
    if( smbCheck(0, addr, 0xFE, 0x0F, 1) ) return false;
    if ( !smbWrite(0, addr, 1, 0, 1) ) return false;	// select page0
    if ( !smbWrite(0, addr, 0xE4, bank, 1) ) return false;       // bank to read
    return clk156_wait(addr, 500);
}

#if !defined(__linux__) && !defined(I2C_WGL)

bool clk156_do(i2caddr_t addr, bool enable)
{
    if( !smbWrite(0, addr, 1, 1, 1) ) return false;
    return smbWrite(0, addr, 2, enable, 1);
}

bool clk156_disable(i2caddr_t addr)
{
    return clk156_do(addr, false);
}

bool clk156_enable(i2caddr_t addr)
{
    return clk156_do(addr, true);
}

static inline uint16_t count_writes(uint16_t data)
{
	return ((data & 0xC) ? 2 : ((data & 0x3) ? 1 : 0));
}

uint32_t clk156_free_banks(i2caddr_t addr)
{
    if( smbRead(0, addr, 0xFE, 1) != 0xF ) return 0;
    if ( !smbWrite(0, addr, 1, 0, 1) ) return false;	// select page0
	uint32_t v = smbRead(0, addr, 0xE2, 1);
	if( v > 0xFF ) return 0;
	return 2-count_writes(v);
}

bool clk156_check(BaseSequentialStream *chp, i2caddr_t addr)
{
    if( smbRead(0, addr, 0xFE, 1) != 0xF ) {
		chprintf(chp, "Not ready\r\n");
		return false;
	}

    if ( !smbWrite(0, addr, 1, 0, 1) ) return false;	// select page0
	uint32_t v = smbRead(0, addr, 0x2, 4);
	chprintf(chp, "Device: Si%04x%c-%c\r\n", v&0xFFFF, 'A'+((v>>16)&0xFF), 'A'+((v>>24)&0xFF));

	v = smbRead(0, addr, 0xE2, 1);
	chprintf(chp, "NVM writes: %d\r\n", count_writes(v));

    if ( !smbWrite(0, addr, 1, 2, 1) ) return false;	// select page2
    // scratch 0x026B to 0x0272
    chprintf(chp, "ID: ");
    int isRev = 0;
    for( unsigned reg=0x6B; reg<=0x72; ++reg) {
		v = smbRead(0, addr, reg, 1);
		if( v & ~0xFFL ) return false;
		if( v == 0 ) {
			if( reg == 0x72 )
				break;
			chprintf(chp, " Rev: ");
			isRev = 1;
			continue;
		}
		chprintf(chp, isRev ? "%02x" : "%c", v);
	}
	chprintf(chp, "\r\n");

	return true;
}

#else
// gcc -std=gnu99 -DI2C_WGL_NOCOMMENT -o clk156 clk156.c
#define I2CADDRESS 0x6B

int main(int argc, char **argv)
{

	if( argc == 1 ) {
		printf("%s 1|2\n1 - init\n2 - program\n3 - check\n4 - verify");
		return 0;
	}
	//TODO: Init I2C
	printf("; H/L - drive 1/0; 1/0 - input check 1/0; X - input ignore\n");
	printf("; DriverMode: OpenDrain");
	printf("; Freq = 200kHz\n");
	printf(";SCL SDA\n");
	SCL = 'X'; 	SDA = 'X';
	wgl_op();
	wgl_op();
	SCL = '1'; 	SDA = '1';
	wgl_op();
	wgl_op();
	SCL = 'H'; 	SDA = 'H';
	wgl_op();
	wgl_op();
	uint32_t freg;
	switch( argv[1][0] ) {
		case '1': return !clk156_init(I2CADDRESS);
		case '2': return !clk156_program_nvm(I2CADDRESS);
		case '3': return !smbCheck(0, I2CADDRESS, 0xFE, 0x0F, 1);
		case '4': return !clk156_verify(I2CADDRESS, &freg);
		default: 
			printf("Invalid parameter\n");
			return -1;
	}
}
#endif
