/*
 * sdu.h
 *
 *  Created on: May 6, 2015
 *      Author: k.kranke
 */

#ifndef SDU_H_
#define SDU_H_

#ifdef __cplusplus
extern "C" {
#endif

extern SerialUSBDriver SDU1;

#define sdu_stdio ((BaseSequentialStream *)&SDU1)
#define sdu_printf(format,...) {if(usbGetDriverStateI(SDU1.config->usbp)==USB_ACTIVE) {chprintf(sdu_stdio,format);}}

#ifdef __cplusplus
}
#endif

#endif /* SDU_H_ */
