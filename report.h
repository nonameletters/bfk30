/*
 * report.h
 *
 *  Created on: Jun 4, 2015
 *      Author: k.kranke
 */

#ifndef REPORT_H_
#define REPORT_H_

#include <stdint.h>

#define HID_REPORT_TIME		0
#define HID_REPORT_POWER	0
#define HID_REPORT_VTG		1
#define HID_REPORT_STAT		0

#define HID_REPORT_SENSOR_CHANNELS	11u	// 3u
#define HID_REPORT_VREG_CHANNELS 	3u

#define MIN_DEF(a,b)	((a)<(b)?(a):(b))


typedef struct __attribute__((packed)) {
	int32_t actual;
#if HID_REPORT_STAT
	int32_t max;
	int32_t min;
	int32_t avg;
#endif
} report_value_t;

#define RPT_V_SCALE	1000
#define RPT_I_SCALE	1000000
#define RPT_P_SCALE	1000000

typedef struct __attribute__((packed)) {
	report_value_t voltage;
	report_value_t current;
#if HID_REPORT_POWER
	report_value_t power;
#endif
} report_ina_t;

typedef struct __attribute__((packed)) {
	report_ina_t ina[HID_REPORT_SENSOR_CHANNELS];
#if HID_REPORT_VTG
	uint16_t vtg_i[HID_REPORT_VREG_CHANNELS];
#endif
} report_t;

/*
 * Content of the IN report (board->PC)
 */
struct __attribute__((packed)) usb_hid_in_report_s {
	uint32_t sequence_number;
	report_t report;
#if HID_REPORT_TIME
	uint32_t systime;
#endif
// t_board, t_rtd
};

#define HID_IN_REPORT_SIZE (sizeof(struct usb_hid_in_report_s))
#define HID_OUT_REPORT_SIZE 4


#endif /* REPORT_H_ */
