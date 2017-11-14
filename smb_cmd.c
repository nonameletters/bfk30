/*
 * SMB commands
 *
 * Copyright (C) 2016, 2017 Baikal Electronics.
 *
 * Author: Alexander Osipenko <Alexander.Osipenko@baikalelectronics.ru>
 */


#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <stdlib.h>
#include <string.h>
#include "smbus.h"
#include "pmbus_cmd_constants.h"
#include "fxp.h"

extern char con_str_buf[20]; // shell conversion buffer

static inline char achar(unsigned c)
{
	return c >= 0x20 && c < 0x80 ? c : '.';
}

static inline int tohex(unsigned c) { 
	if (c >= '0' && c <= '9') return c - '0';
	c |= 20;	// tolower
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	return -1;
}

static unsigned fillbuf(uint8_t* data, unsigned maxdata, const char* str, bool auto_length)
{
	unsigned stored = 0;
	if( str[0]=='"' ) {
		auto_length = true;
		for(stored=1; stored < maxdata; ++stored ) {
			str++;
			switch ( data[stored] = str[0] ) {
				case 0:
				case '"': goto fin;
				case '\\':
					if( str[1] == '0' ) goto fin;
					if( str[1] == '\\' ) { str++; break; }
					if( str[1] == '"' )  { data[stored]='"'; str++; break; }
					{ int c0 = tohex(str[1]);
						if( c0 > 0 ) {
							++str;
							int c1 = tohex(str[1]);
							if( c1 >= 0 ) {
								++str;
								c0 = (c0<<4)|c1;
							}
						}
					  data[stored] = c0;
					}
			}
		}
	}
	else {
		if( str[0]=='0' && (str[1]|0x20) == 'x' ) str+=2;
		if( !auto_length && (auto_length = (str[0] == '*')) ) str++;
		if( auto_length ) stored=1;
		while(stored < maxdata && *str) {
			int c0 = tohex(str[0]);
			if( c0 < 0 ) return 0;
			int c1 = tohex(str[1]);
			if( c1 < 0 ) return 0;
			data[stored++] = (c0<<4)|c1;
			str += 2;
		}
	}
fin:
	if( auto_length ) data[0] = stored-1;
	return stored;
}


void dumphex(BaseSequentialStream *chp,const void* data, unsigned count) {
	while( count ) {
		for( unsigned i=0; i<16; ++i)
			chprintf(chp,i<count ? "%02X ":"   ",((const uint8_t*)data)[i]);
		chprintf(chp,"   [");
		for( unsigned i=0; i<16; ++i)
			chprintf(chp,i<count?"%c":" ",achar(((const uint8_t*)data)[i]));
		chprintf(chp,"]\r\n");
		if( count < 16 ) return;
		count -= 16;
		data += 16;
    }
}

static uint32_t getnum(const char* str)
{
	int radix = 16;
	if( *str == '\'' ) { radix = 10; str++; };
	char *end;
	uint32_t val = strtol(str, &end, radix);
	return (*end != 0 && val==0ul) ? ~0ul : val;
}

bool cmd_smb_int(BaseSequentialStream *chp, unsigned saddr, int argc, char *argv[]) {
    uint32_t val;
    uint16_t scmd = ~0;
    if( argc > 1 )
      scmd = strtol(argv[1], NULL, 16);

	extern const I2CConfig i2cfg3;
    size_t bus0 = SMB_ADDR_BUS_GET(saddr);
    if (bus0 == 3 && I2CD3.state == I2C_STOP) i2cStart(&I2CD3, &i2cfg3);

    if ( bus0 == 0 || bus0 > 3 ) {
        chprintf(chp, "Wrong SMBus address\r\n");
    }
    if( !strcmp(argv[0], "r") ) {    			// ### Read byte/block
		if( argc == 2 ) {
			val = smbRead(0, saddr, scmd, 1);
			if( val & ~0xFFul ) goto er;
			chprintf(chp, "%02X %d\r\n", val & 0xFF, val & 0xFF);
		} else {
			bool block_data = (argv[2][0] == '*');
			if( block_data ) argv[2]++;
			uint32_t size = getnum(argv[2]);
			uint8_t iobuf[128+2];
			if(size==0 || size > sizeof(iobuf)-2) {
				chprintf(chp, "invalid size\r\n");
				return true;
			}
			if( smbReadBlock(0, saddr, scmd, iobuf, size-1) ) {
				if( block_data ) {
					size = SMB_MIN(size,iobuf[0]); //-- for BLOCK read
					chprintf(chp,"Length=%d\r\n",size);
					dumphex(chp,iobuf+1,size);
				} else {
					dumphex(chp,iobuf,size);
				}
			} else goto er;
		}
    } else if ( !strcmp(argv[0], "rw") ) {  	// ### Read word
        val = smbRead(0, saddr, scmd, 2);
		if( val & ~0xFFFFul ) goto er;
        chprintf(chp, "%04X %d\r\n", val & 0xFFFF, val & 0xFFFF);
    } else if ( !strcmp(argv[0], "rd") ) {  	// ### read dword
        val = smbRead(0, saddr, scmd, 4);
        chprintf(chp, "%08X %d\r\n", val, val);
    } else if ( !strcmp(argv[0], "rv") ) {      // ### read 2 byte U16.13 field (voltage)
        val = smbRead(0, saddr, scmd, 2);
		if( val & ~0xFFFFul ) goto er;
        // should read VOUT_MODE
        int32_t vout_mode  = smbRead(0, saddr, PMBUS_CMD_VOUT_MODE,1);
		if( vout_mode & ~0xFFul ) goto er;
        if( vout_mode & 0x10 ) vout_mode |= 0xF0;
        //get_l16u(&f_val, val);
        //get_ulval(&f_val, val, vout_mode);
        linear_to_str(con_str_buf, val, vout_mode);
        chprintf(chp, "%04x %s\r\n", val, con_str_buf);
    } else if ( !strcmp(argv[0], "rl") ) {      // ### read 2 byte L11 field
        val = smbRead(0, saddr, scmd, 2);
		if( val & ~0xFFFFul ) goto er;
        l11_to_str(con_str_buf, val);
        chprintf(chp, "%04x %s\r\n", val, con_str_buf);
    } else if ( !strcmp(argv[0],"rn") ) {       // ### read data
        if( scmd == 0 ) return false;
        uint8_t iobuf[128+2];
        unsigned size = scmd;
        if(size > sizeof(iobuf)) {
            chprintf(chp, "too large\r\n");
            return true;
        }
        if( !i2cReadBlock(saddr,iobuf,size) ) goto er;   // get buffer
        dumphex(chp,iobuf,size);
    } else if( !strcmp(argv[0],"w") ) {   // ### write hex 1/2/4 or bytes
        if (argc == 1 ) {
            if ( !smbWrite(0, saddr, scmd, 0, 0) ) goto er;
        } else if(argc == 3) {
            val = strtol(argv[2],NULL,16);
            size_t width = (strlen(argv[2])+1)&~1U;
            if( width <= 8 && argv[2][0] != '*') {
				if( !smbWrite(0, saddr, scmd, val, width/2) ) goto er;
			} else {
				uint8_t iobuf[128+2];
				iobuf[0] = scmd;
				width = fillbuf(iobuf+1,sizeof(iobuf)-1,argv[2],false);
				if( width == 0 ) {
					chprintf(chp,"malformed data\r\n");
					return false;
				}
				//dumphex(chp, iobuf, width+1);
				if ( !i2cWriteBlock(0, saddr, iobuf, width+1) ) goto er;
			}
        } else {
			uint8_t iobuf[SMB_MIN(SHELL_MAX_ARGUMENTS-2,128)+2];
			size_t i;
			size_t auto_size = (argv[2][0]=='*' ? 1:0);
			for( i = 1+auto_size; i < sizeof(iobuf) && i < (unsigned)argc-1; ++i) {
				iobuf[i] = getnum(argv[1+i]);
			}
			iobuf[0] = scmd;
			if( auto_size )
				iobuf[1] = i-2;
			//dumphex(chp, iobuf,i);
			if ( !i2cWriteBlock(0, saddr, iobuf, i) ) goto er;
		}
    } else if ( !strcmp(argv[0], "pr") || argc == 1 ) {  // ### ReadPMBus
        const pmbus_cmd_def* pc = pmbus_find_command(argv[argc==1?0:1]);
        if ( pc==NULL ) {
            chprintf(chp, "Unrecognized PMBus command\r\n");
            return true;
        }
        if ( pc->access == PCF_WO ) {
			if ( argc == 1 && pc->format == PCF_ZERO ) {	// simple command like "clear_faults"
				if ( !smbWrite(0, saddr, pc->cmd, 0, 0) ) goto er;
			} else {
				chprintf(chp, "PMBus %s is write only command\r\n", pc->command);
			}
            return true;
        }
        if ( pc->format & PCF_BLOCK ) {
			scmd = pc->size != 0 ? pc->size + 1 : 33;
			scmd = SMB_MIN(scmd,sizeof(con_str_buf));
			if(!smbReadBlock(0, saddr, pc->cmd, (uint8_t*)(void*)con_str_buf, scmd )) goto er;
			scmd = SMB_MIN(scmd,*(uint8_t*)con_str_buf);
			chprintf(chp,"Length=%d\r\n", scmd);
			dumphex(chp, (void*)(con_str_buf+1),scmd);
            return true;
        } else {
            val = smbRead(0, saddr, pc->cmd, pc->format & 0xF);
            chprintf(chp, "%02X %-25s %0*X", pc->cmd, pc->command, 2*(pc->format & 0xF), val);
            if ( pc->cmd == PMBUS_CMD_STATUS_WORD || pc->cmd == PMBUS_CMD_STATUS_BYTE ) {
                uint16_t n = val;
                while( n ) {
                    const char* sm = pmbus_status_msg(&n);
                    chprintf(chp, " %s%s", sm, n==0?"":",");
                }
            } else if ( pc->cmd > PMBUS_CMD_STATUS_WORD && pc->cmd <= PMBUS_CMD_STATUS_FANS_3_4 ) {
                uint8_t n = val;
                while( n ) {
                    const char* sm = pmbus_status_sub(pc->cmd, &n);
                    chprintf(chp, " %s%s", sm, n==0?"":",");
                }
            } else if ( pc->format == PCF_L11 ) {
                l11_to_str(con_str_buf,val);
                chprintf(chp, " %s%s", con_str_buf, pmbus_cmd_units(pc));
            } else if ( pc->format == PCF_FR ) {
				chprintf(chp, ": ");
				pmbus_fault_response(chp, val);
				chprintf(chp,"\r\n");
            } else if ( pc->format == PCF_L16u ) {
				// need to know page's vout_mode
                int8_t vout_mode  = smbRead(0, saddr, PMBUS_CMD_VOUT_MODE, 1);
                if( vout_mode & 0x10 ) vout_mode |= 0xF0;
                linear_to_str(con_str_buf, val, vout_mode);
                chprintf(chp, " %s%s", con_str_buf, pmbus_cmd_units(pc));
            }
            chprintf(chp, "\r\n");
        }
    } else if ( !(val=strcmp(argv[0], "pw")) || argc >= 2) {  // ### WritePMBus
		int ashift = (val?0:1);
        const pmbus_cmd_def* pc = pmbus_find_command(argv[ashift]);
        if ( pc==NULL ) {
            chprintf(chp, "Unrecognized PMBus command\r\n");
            return true;
        }
        if ( pc->access == PCF_RO ) {
            chprintf(chp, "PMBus %s is read only command\r\n", pc->command);
            return true;
        }
        if ( pc->format & PCF_BLOCK ) {
			uint8_t iobuf[128+2];
			iobuf[0] = pc->cmd;
			unsigned width = fillbuf(iobuf+1,sizeof(iobuf)-1,argv[ashift+1],true);
			if( width == 0 ) {
				chprintf(chp,"malformed data\r\n");
				return false;
			}
			//chprintf(chp,">%d %s\r\n", width, argv[ashift+1]);
			//dumphex(chp, iobuf, width+1);
			if ( !i2cWriteBlock(0, saddr, iobuf, width+1) ) goto er;
            return true;
        } else {
			fixed_val_t f_val;
            if ( pc->format == PCF_L11 ) {
                scan_val(&f_val,argv[1+ashift]);
                val = fv_l11(&f_val);
            } else if ( pc->format == PCF_L16u ) {
                scan_val(&f_val,argv[1+ashift]);
                if (f_val.sign ) {
                    chprintf(chp, "Negative value not allowed for %s\r\n", pc->command);
                }
                int8_t vout_mode  = smbRead(0, saddr, PMBUS_CMD_VOUT_MODE, 1);
                if( vout_mode & 0x10 ) vout_mode |= 0xF0;
                val = to_l16u(f_val.val*1000+f_val.mval, vout_mode);
            } else {
                val = getnum(argv[1+ashift]);
            }
            if ( !smbWrite(0, saddr, pc->cmd, val, pc->format & 0xF) ) goto er;
        }
    } else {
        chprintf(chp,"Unknown SMBus/PMBus command\r\n");
		return false;
    }
    return true;
er:
	chprintf(chp, "device error\r\n");
	return true;
}

