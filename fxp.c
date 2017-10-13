/*
 * fxp.c
 *
 *  Created on: Jun 10, 2015
 *      Author: k.kranke
 */


#include <stdint.h>

#ifdef CHPRINTF_USE_FLOAT
#include <math.h>
#endif

#ifndef __linux__
#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#else
#include <math.h>
#include <stdio.h>
#endif

#include "fxp.h"


#ifdef QN_DOUBLE
#if sizeof(qn_t) == sizeof(double)
qn_t l11_unpack(int16_t d) { return ldexp((int16_t)(d&0x400?d|0xF800:d&0x7FF), ((int16_t)d)>>11); }
qn_t l16u_unpack(uint16_t d, int exp) { return ldexp(d,exp); }
#else
qn_t l11_unpack(int16_t d) { return ldexpf((int16_t)(d&0x400?d|0xF800:d&0x7FF), ((int16_t)d)>>11); }
qn_t l16u_unpack(uint16_t d, int exp) { return ldexpf(d,exp); }
#endif
#endif

uint16_t to_l16u(milliuval_t val, int n) {
	if ( n < 0 ) {
		return ((val << (-n))) / 1000;
	} else {
		return ((val >> n)) / 1000;
	}
}


#if 0
const __fp16 sfval[] = {1.5, 1.25, 1.625, 0.5, 0.25, 0.125, 0.002};
uint16_t fp16_to_l11(const __fp16* fpp) // incomplete
{
	uint16_t x = *(const uint16_t*)fpp;
	if( x == 0 ) return 0;
	uint16_t r = (x&0x3FF)|0x400;
	if( x & 0x8000 ) r = (-r)&0xFFF;
	return (((x-0x6000)&0x7C00)<<1) | (r>>1);
}
#endif

uint16_t to_l11(millival_t val)
{
	if (val==0) return 0;
    int sign = (val < 0);
    if (sign) val = -val;
    int n = -10;
    unsigned z = 1000*2, b = 0x200;
    while ( val/1000 < 0x200 && n != -16 ) {
		val <<= 1;
		b <<= 1;
		z <<= 1;
		--n;
	}
    unsigned frac = (val*4) % (z*2);
    val = ((val/1000) << 10);
	for( ; b; b>>=1, z/=2) {
		if( frac >= z ) {
			val |= b;
			frac -= z;
		}
	}
    while (val > 0x3ff) {
        val >>= 1;
        ++n;
    }
    if (sign)
        val = -val;
    return (val & 0x7ff) | ((n & 0x1f) << 11);
}

void scan_val(fixed_val_t *val, const char* s)
{
    int a = 0;
    int c;
    int sign = 0;
    if( *s == '-' ) { sign = 1; ++s; }
    while ((c = *s++) != 0 && (c >= '0') && c <= '9') {
        a *= 10;
        a += c - '0';
    }
    val->val = a;
    a = 0;
    if (c == '.') {
        int i = 0;
        while ((c = *s++) != 0 && (c >= '0') && c <= '9' && i < 3) {
            a *= 10;
            a += c - '0';
            ++i;
        }
        while (i<3) {
			a *= 10;
			++i;
		}
    }
    val->mval = a;
    val->sign = sign;
}

void l11_to_str(char* buffer, int16_t code)
{
	linear_to_str(buffer, (int16_t)((code & 0x400) ? (code|0xF800) : (code & 0x3FF)), ((int16_t)code>>11));
}

void linear_to_str(char* buffer, int32_t code, int exp)
{
#if defined(CHPRINTF_USE_FLOAT) || defined(__linux__)
	int digs=2;
	if( exp < -13 ) digs = 5;
	else if ( exp < -11 ) digs = 4;
	else if ( exp < -6 ) digs = 3;
#ifdef __linux__
	snprintf(buffer,12,"%.*f", digs, ldexp(code,exp));
#else
	chsnprintf(buffer,12,"%.*f", digs, ldexp(code,exp));
#endif
#else
	if (code<0) { *buffer++ = '-'; code = -code; }
	if (exp>=0) { 
		chsnprintf(buffer,11,  "%d", code<<exp);
		return;
	}
	exp = -exp;
	unsigned frac = code & ((1 << exp) - 1);
	if (exp > 13) {
		chsnprintf(buffer,11, "%d.%05d", code>>exp, (100000*frac) >> exp);
	} else if (exp > 11) {
		chsnprintf(buffer,11, "%d.%04d", code>>exp, (10000*frac) >> exp);
	} else if (exp > 6) {
		chsnprintf(buffer,11, "%d.%03d", code>>exp, (1000*frac) >> exp);
	} else {
		chsnprintf(buffer,11, "%d.%02d", code>>exp, (100*frac) >> exp);
	}
#endif
}
