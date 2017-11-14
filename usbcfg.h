/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _USBCFG_H_
#define _USBCFG_H_

#define USB_SERIAL_LENGTH  12

#ifdef __cplusplus
extern "C" {
#endif
#include "hal_usb_hid.h"

extern const USBConfig usbcfg;
extern const SerialUSBConfig serusbcfg1;
extern const DFU_USBConfig dfuusbcfg1;
extern const USBHIDConfig hidusbcfg1;
extern USBHIDDriver HID1;
extern SerialUSBDriver SDU1;

void usbSerialInit(void);

#if CH_CFG_USE_EVENTS && !defined(__DOXYGEN__) 
extern event_source_t ev_usb_activated;
#endif


#ifdef __cplusplus
}
#endif

#endif  /* _USBCFG_H_ */

/** @} */
