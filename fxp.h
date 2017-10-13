/*
 * fxp.h
 *
 *  Created on: Jun 10, 2015
 *      Author: k.kranke
 */

#ifndef FXP_H_
#define FXP_H_

typedef struct fixed_val {
    uint16_t val;
    uint16_t mval;
//  uint16_t uval;
    char     sign;
} fixed_val_t;


typedef int32_t millival_t;
typedef int32_t milliuval_t;


#ifdef __cplusplus
extern "C" {
#endif

//#define QN_DOUBLE
#ifdef QN_DOUBLE
typedef float qn_t;
qn_t l11_unpack(int16_t d)  __attribute__((const));
qn_t l16u_unpack(uint16_t d, int exp) __attribute__((const));
uint16_t l16u_pack(qn_t v, int exp) __attribute__((const));
#define QN_VAL(x)		(x)

#else

//#define QN_WORD			32
//#define QN_MANTISSA		22
//typedef int32_t qn_t;			// sign+4.integer+
//#define QN_VAL(x)		((qn_t)((x)*(1<<QN_MANTISSA)))

#define QN_VAL(x)		((int32_t)((x)*1000))

uint16_t to_l11(millival_t val) __attribute__((const,warn_unused_result));
uint16_t to_l16u(milliuval_t val, int n) __attribute__((const,warn_unused_result));

static inline millival_t to_mval(const fixed_val_t *val)	{ millival_t x=val->val*1000+val->mval; return val->sign ? -x:x; }
static inline uint16_t fv_l11(const fixed_val_t *val)		{ return to_l11(to_mval(val)); }
static inline int16_t fv_l16u(const fixed_val_t *val, int8_t exp) { return to_l16u(to_mval(val), exp); }
#endif


void linear_to_str(char* buffer, int32_t code, int exp);
void l11_to_str(char* buffer, int16_t code);

void scan_val(fixed_val_t *val, const char* s);

#ifdef __cplusplus
}
#endif

#endif /* FXP_H_ */
