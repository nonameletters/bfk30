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

#include <ch.h>
#include <hal.h>
#include "dfu.h"
#include "hal_usb_hid.h"
#include "usbcfg.h"
#include "report.h"

#if CH_CFG_USE_EVENTS && !defined(__DOXYGEN__) 
event_source_t ev_usb_activated;
#endif


//#define USB_DEVICE_VID                  0xabf0
//#define USB_DEVICE_PID                  0x1234

#define USB_DEVICE_VID                  0x0483
#define USB_DEVICE_PID                  0xdf11

#define HID_DESCRIPTOR_OFFSET 9
#define HID_DESCRIPTOR_SIZE		USB_DESC_HID_SIZE


/*
 * Endpoints used
 */
#define USBD1_INTERRUPT_REQUEST_EP      1
#define USBD1_DATA_REQUEST_EP           2
#define USBD1_DATA_AVAILABLE_EP         2
#define USBD1_HID_IN_EP                 3
#define USBD1_HID_OUT_EP                3

#define USB_INTERRUPT_REQUEST_SIZE      0x0010
#define USB_CDC_DATA_SIZE               0x0040
#define USB_DFU_DATA_SIZE               0x0100  /* 256B, page size for most SPI flash devices */
#define USB_HID_DATA_SIZE               0x0100	/* sizeof(hid_report) 11*3*4+3*4 */

#define USB_MAX_DATA_SIZE				0x0100

/*
 * Interfaces
 */
#define USB_NUM_INTERFACES              4
#define USB_DFU_NUM0                    0
#define USB_CDC_CIF_NUM0                1
#define USB_CDC_DIF_NUM0                2
#define USB_HID_NUM0                    3


#define USB_DESC_HID_REPORT	\
  USB_DESC_BYTE (0x06),		/* Usage Page (vendor-defined) */ \
  USB_DESC_WORD (0xFF00),	\
  USB_DESC_BYTE (0x09),		/* Usage (vendor-defined) */ \
  USB_DESC_BYTE (0x01),	\
  USB_DESC_BYTE (0xA1),		/* Collection (application) */ \
  USB_DESC_BYTE (0x01), \
 \
  USB_DESC_BYTE (0x09),		/* Usage (vendor-defined) */ \
  USB_DESC_BYTE (0x01), \
  USB_DESC_BYTE (0x15),		/* Logical minimum (0) */ \
  USB_DESC_BYTE (0x00), \
  USB_DESC_BYTE (0x26),		/* Logical maximum (255) */ \
  USB_DESC_WORD (0x00FF), \
  USB_DESC_BYTE (0x75),		/* Report size 8 bits) */ \
  USB_DESC_BYTE (0x08), \
  USB_DESC_BYTE (0x95),		/* Report count (USB_HID_IN_REPORT_SIZE) */ \
  USB_DESC_BYTE (HID_IN_REPORT_SIZE), \
  USB_DESC_BYTE (0x81),		/* Input (Data, Variable, Absolute) */ \
  USB_DESC_BYTE (0x02), \
 \
  USB_DESC_BYTE (0x09),		/* Usage (vendor-defined) */ \
  USB_DESC_BYTE (0x01), \
  USB_DESC_BYTE (0x15),		/* Logical minimum (0) */ \
  USB_DESC_BYTE (0x00), \
  USB_DESC_BYTE (0x26),		/* Logical maximum (255) */ \
  USB_DESC_WORD (0x00FF), \
  USB_DESC_BYTE (0x75),		/* Report size (8 bits) */ \
  USB_DESC_BYTE (0x08), \
  USB_DESC_BYTE (0x95),		/* Report count (USB_HID_OUT_REPORT_SIZE) */ \
  USB_DESC_BYTE (HID_OUT_REPORT_SIZE), \
  USB_DESC_BYTE (0x91),		/* Output (Data, Variable, Absolute) */ \
  USB_DESC_BYTE (0x02), \
 \
  USB_DESC_BYTE (0xC0)		/* End collection */

#define USB_DESC_HID_REPORT_SIZE		34
#define HID_DESCRIPTOR_TOTAL_SIZE       0x20


/*
 * USB Device Descriptor. (old: EF/2/1
 */
static const uint8_t device_descriptor_data[USB_DESC_DEVICE_SIZE] = {
  USB_DESC_DEVICE(
    0x0110,                                 /* bcdUSB (1.1).                */
    0xEF,                                   /* bDeviceClass (Misc class)    */
    0x02,                                   /* bDeviceSubClass (Common)     */
    0x01,                                   /* bDeviceProtocol (IAD)        */
    USB_CDC_DATA_SIZE,                      /* bMaxPacketSize.              */
    USB_DEVICE_VID,                         /* idVendor.                    */
    USB_DEVICE_PID,                         /* idProduct.                   */
    0x0200,                                 /* bcdDevice.                   */
    1,                                      /* iManufacturer.               */
    2,                                      /* iProduct.                    */
    3,                                      /* iSerialNumber.               */
    1)                                      /* bNumConfigurations.          */
};

/*
 * Device Descriptor wrapper.
 */
static const USBDescriptor device_descriptor = {
  sizeof device_descriptor_data,
  device_descriptor_data
};

#define CDC_IF_DESC_SET_SIZE                                                \
  (USB_DESC_INTERFACE_SIZE + 5 + 5 + 4 + 5 + USB_DESC_ENDPOINT_SIZE +       \
   USB_DESC_INTERFACE_SIZE + (USB_DESC_ENDPOINT_SIZE * 2))

#define CDC_IF_DESC_SET(comIfNum, datIfNum, comInEp, datOutEp, datInEp)     \
  /* Interface Descriptor.*/                                                \
  USB_DESC_INTERFACE(                                                       \
    comIfNum,                               /* bInterfaceNumber.        */  \
    0x00,                                   /* bAlternateSetting.       */  \
    0x01,                                   /* bNumEndpoints.           */  \
	CDC_COMMUNICATION_INTERFACE_CLASS,      /* bInterfaceClass.         */  \
    CDC_ABSTRACT_CONTROL_MODEL,             /* bInterfaceSubClass.      */  \
    0x01,                                   /* bInterfaceProtocol (AT
                                               commands, CDC section
                                               4.4).                    */  \
    0),                                     /* iInterface.              */  \
  /* Header Functional Descriptor (CDC section 5.2.3).*/                    \
  USB_DESC_BYTE     (5),                    /* bLength.                 */  \
  USB_DESC_BYTE     (CDC_CS_INTERFACE),     /* bDescriptorType.         */  \
  USB_DESC_BYTE     (CDC_HEADER),           /* bDescriptorSubtype.      */  \
  USB_DESC_BCD      (0x0110),               /* bcdCDC.                  */  \
  /* Call Management Functional Descriptor.*/                               \
  USB_DESC_BYTE     (5),                    /* bFunctionLength.         */  \
  USB_DESC_BYTE     (CDC_CS_INTERFACE),     /* bDescriptorType.         */  \
  USB_DESC_BYTE     (CDC_CALL_MANAGEMENT),  /* bDescriptorSubtype.      */  \
  USB_DESC_BYTE     (0x03),    /*******/    /* bmCapabilities.          */  \
  USB_DESC_BYTE     (datIfNum),             /* bDataInterface.          */  \
  /* Abstract Control Management Functional Descriptor.*/                   \
  USB_DESC_BYTE     (4),                    /* bFunctionLength.         */  \
  USB_DESC_BYTE     (CDC_CS_INTERFACE),     /* bDescriptorType.         */  \
  USB_DESC_BYTE     (CDC_ABSTRACT_CONTROL_MANAGEMENT),                      \
  USB_DESC_BYTE     (0x02),                 /* bmCapabilities.          */  \
  /* Union Functional Descriptor.*/                                         \
  USB_DESC_BYTE     (5),                    /* bFunctionLength.         */  \
  USB_DESC_BYTE     (CDC_CS_INTERFACE),     /* bDescriptorType.         */  \
  USB_DESC_BYTE     (CDC_UNION),            /* bDescriptorSubtype.      */  \
  USB_DESC_BYTE     (comIfNum),             /* bMasterInterface.        */  \
  USB_DESC_BYTE     (datIfNum),             /* bSlaveInterface.         */  \
  /* Endpoint, Interrupt IN.*/                                              \
  USB_DESC_ENDPOINT (                                                       \
    comInEp,                                                                \
    USB_EP_MODE_TYPE_INTR,                  /* bmAttributes.            */  \
    USB_INTERRUPT_REQUEST_SIZE,             /* wMaxPacketSize.          */  \
    0x01),                                  /* bInterval.               */  \
                                                                            \
  /* CDC Data Interface Descriptor.*/                                       \
  USB_DESC_INTERFACE(                                                       \
    datIfNum,                               /* bInterfaceNumber.        */  \
    0x00,                                   /* bAlternateSetting.       */  \
    0x02,                                   /* bNumEndpoints.           */  \
    CDC_DATA_INTERFACE_CLASS,               /* bInterfaceClass.         */  \
    0x00,                                   /* bInterfaceSubClass (CDC
                                               section 4.6).            */  \
    0x00,                                   /* bInterfaceProtocol (CDC
                                               section 4.7).            */  \
    0x00),                                  /* iInterface.              */  \
  /* Endpoint, Bulk OUT.*/                                                  \
  USB_DESC_ENDPOINT(                                                        \
    datOutEp,                               /* bEndpointAddress.        */  \
    USB_EP_MODE_TYPE_BULK,                  /* bmAttributes.            */  \
    USB_CDC_DATA_SIZE,                      /* wMaxPacketSize.          */  \
    0x00),                                  /* bInterval.               */  \
  /* Endpoint, Bulk IN.*/                                                   \
  USB_DESC_ENDPOINT(                                                        \
    datInEp,                                /* bEndpointAddress.        */  \
    USB_EP_MODE_TYPE_BULK,                  /* bmAttributes.            */  \
    USB_CDC_DATA_SIZE,                      /* wMaxPacketSize.          */  \
    0x00)                                   /* bInterval.               */

#define IAD_CDC_IF_DESC_SET_SIZE                                            \
  (USB_DESC_INTERFACE_ASSOCIATION_SIZE + CDC_IF_DESC_SET_SIZE)

#define IAD_CDC_IF_DESC_SET(comIfNum, datIfNum, comInEp, datOutEp, datInEp) \
  /* Interface Association Descriptor.*/                                    \
  USB_DESC_INTERFACE_ASSOCIATION(                                           \
    comIfNum,                               /* bFirstInterface.         */  \
    2,                                      /* bInterfaceCount.         */  \
    CDC_COMMUNICATION_INTERFACE_CLASS,      /* bFunctionClass.          */  \
    CDC_ABSTRACT_CONTROL_MODEL,             /* bFunctionSubClass.       */  \
    1,                                      /* bFunctionProcotol.       */  \
    0                                       /* iInterface.              */  \
  ),                                                                        \
  /* CDC Interface descriptor set */                                        \
  CDC_IF_DESC_SET(comIfNum, datIfNum, comInEp, datOutEp, datInEp)

#if DFU_INTERFACES == 3
// 3 DFU interfaces
#define IAD_DFU_IF_DESC                                                     \
  /************ Descriptor of DFU interface 0 Alternate setting 0 *********/\
  0x09,   /* bLength: Interface Descriptor size */                          \
  0x04,   /* bDescriptorType: */                                            \
  USB_DFU_NUM0,   /* bInterfaceNumber: Number of Interface */               \
  0x00,   /* bAlternateSetting: Alternate setting */                        \
  0x00,   /* bNumEndpoints*/                                                \
  0xFE,   /* bInterfaceClass: DFU */                                        \
  0x01,   /* bInterfaceSubClass */                                          \
                                                                            \
  0x02,   /* nInterfaceProtocol, switched to 0x02 while in dfu_mode */      \
                                                                            \
  0x04,   /* iInterface: */                                                 \
                                                                            \
  /************ Descriptor of DFU interface 0 Alternate setting 1 *********/\
  0x09,   /* bLength: Interface Descriptor size */                          \
  0x04,   /* bDescriptorType: */                                            \
  0x00,   /* bInterfaceNumber: Number of Interface */                       \
  0x01,   /* bAlternateSetting: Alternate setting */                        \
  0x00,   /* bNumEndpoints*/                                                \
  0xFE,   /* bInterfaceClass: DFU */                                        \
  0x01,   /* bInterfaceSubClass */                                          \
                                                                            \
  0x02,   /* nInterfaceProtocol, switched to 0x02 while in dfu_mode */      \
                                                                            \
  0x05,   /* iInterface: */                                                 \
                                                                            \
  /************ Descriptor of DFU interface 0 Alternate setting 2 *********/\
  0x09,   /* bLength: Interface Descriptor size */                          \
  0x04,   /* bDescriptorType: */                                            \
  0x00,   /* bInterfaceNumber: Number of Interface */                       \
  0x02,   /* bAlternateSetting: Alternate setting */                        \
  0x00,   /* bNumEndpoints*/                                                \
  0xFE,   /* bInterfaceClass: DFU */                                        \
  0x01,   /* bInterfaceSubClass */                                          \
                                                                            \
  0x02,   /* nInterfaceProtocol, switched to 0x02 while in dfu_mode */      \
                                                                            \
  0x06,   /* iInterface: */                                                 \
                                                                            \
  /******************** DFU Functional Descriptor********************/      \
  0x09,   /*blength = 7 Bytes*/                                             \
  0x21,   /* DFU Functional Descriptor*/                                    \
  0x0B,   /*bmAttributes, bitCanDnload | bitCanUpload */                    \
  0xFF,   /*DetachTimeOut= 255 ms*/                                         \
  0x00,                                                                     \
  (USB_DFU_DATA_SIZE & 0x00FF),                                             \
  (USB_DFU_DATA_SIZE & 0xFF00) >> 8, /* TransferSize = 1024 Byte*/          \
  0x10,                          /* bcdDFUVersion */                        \
  0x01                                                                      \
  /***********************************************************/             \
  /*36*/
#define DFU_DESCRIPTOR_SIZE             (9+3*9)
#elif DFU_INTERFACES == 2
// 2 DFU interfaces
#define IAD_DFU_IF_DESC                                                     \
  /************ Descriptor of DFU interface 0 Alternate setting 0 *********/\
  0x09,   /* bLength: Interface Descriptor size */                          \
  0x04,   /* bDescriptorType: */                                            \
  USB_DFU_NUM0,   /* bInterfaceNumber: Number of Interface */               \
  0x00,   /* bAlternateSetting: Alternate setting */                        \
  0x00,   /* bNumEndpoints*/                                                \
  0xFE,   /* bInterfaceClass: DFU */                                        \
  0x01,   /* bInterfaceSubClass */                                          \
                                                                            \
  0x02,   /* nInterfaceProtocol, switched to 0x02 while in dfu_mode */      \
                                                                            \
  0x04,   /* iInterface: */                                                 \
                                                                            \
  /************ Descriptor of DFU interface 0 Alternate setting 1 *********/\
  0x09,   /* bLength: Interface Descriptor size */                          \
  0x04,   /* bDescriptorType: */                                            \
  0x00,   /* bInterfaceNumber: Number of Interface */                       \
  0x01,   /* bAlternateSetting: Alternate setting */                        \
  0x00,   /* bNumEndpoints*/                                                \
  0xFE,   /* bInterfaceClass: DFU */                                        \
  0x01,   /* bInterfaceSubClass */                                          \
                                                                            \
  0x02,   /* nInterfaceProtocol, switched to 0x02 while in dfu_mode */      \
                                                                            \
  0x05,   /* iInterface: */                                                 \
  /******************** DFU Functional Descriptor********************/      \
  0x09,   /*blength = 7 Bytes*/                                             \
  0x21,   /* DFU Functional Descriptor*/                                    \
  0x0B,   /*bmAttributes, bitCanDnload | bitCanUpload */                    \
  0xFF,   /*DetachTimeOut= 255 ms*/                                         \
  0x00,                                                                     \
  (USB_DFU_DATA_SIZE & 0x00FF),                                             \
  (USB_DFU_DATA_SIZE & 0xFF00) >> 8, /* TransferSize = 1024 Byte*/          \
  0x10,                          /* bcdDFUVersion */                        \
  0x01                                                                      \
  /***********************************************************/
#define DFU_DESCRIPTOR_SIZE             (9+2*9)
#else
// 1 DFU interfaces
#define IAD_DFU_IF_DESC                                                     \
  /************ Descriptor of DFU interface 0 Alternate setting 0 *********/\
  0x09,   /* bLength: Interface Descriptor size */                          \
  0x04,   /* bDescriptorType: */                                            \
  USB_DFU_NUM0,   /* bInterfaceNumber: Number of Interface */               \
  0x00,   /* bAlternateSetting: Alternate setting */                        \
  0x00,   /* bNumEndpoints*/                                                \
  0xFE,   /* bInterfaceClass: DFU */                                        \
  0x01,   /* bInterfaceSubClass */                                          \
                                                                            \
  0x02,   /* nInterfaceProtocol, 1==APP (must switch), 2==DFU */      		\
                                                                            \
  0x04,   /* iInterface: */                                                 \
                                                                            \
  /******************** DFU Functional Descriptor********************/      \
  0x09,   /*blength = 9 Bytes*/                                             \
  0x21,   /* DFU Functional Descriptor*/                                    \
  0x0B,   /*bmAttributes, bitWillDetach | bitCanDnload | bitCanUpload */    \
  0xFF,   /*DetachTimeOut= 255 ms*/                                         \
  0x00,                                                                     \
  (USB_DFU_DATA_SIZE & 0x00FF),                                             \
  (USB_DFU_DATA_SIZE & 0xFF00) >> 8, /* TransferSize = 1024 Byte*/          \
  0x10,                          /* bcdDFUVersion */                        \
  0x01                                                                      \
  /***********************************************************/             \
  /*18*/
#define DFU_DESCRIPTOR_SIZE             (9+1*9)
#endif

// wTotalLength (9+9+7+7)
#define IAD_HID_IF_DESC                                                     \
  /* Interface Descriptor (9 bytes) */                                      \
  USB_DESC_INTERFACE (USB_HID_NUM0, /* bInterfaceNumber */                  \
    0x00,                         /* bAlternateSetting */                   \
    0x02,                         /* bNumEndpoints */                       \
    0x03,                         /* bInterfaceClass: HID */                \
    0x00,                         /* bInterfaceSubClass: None */            \
    0x00,                         /* bInterfaceProtocol: None */            \
    0),                                                                     \
  /* HID descriptor (9 bytes) */                                            \
  USB_DESC_HID (							    \
    0x0110,         		  	/* bcdHID: HID version 1.1 */             \
    0x00,           		  	/* bCountryCode */                        \
    0x01,           		  	/* bNumDescriptors */                     \
    HID_REPORT,        		  	/* bDescriptorType (report desc) */       \
    USB_DESC_HID_REPORT_SIZE),	/* wDescriptorLength */                   \
  /* Endpoint 1 IN Descriptor (7 bytes) */                                  \
  USB_DESC_ENDPOINT(                                                        \
    USB_ENDPOINT_IN(USBD1_HID_IN_EP), /* bEndpointAddress */                \
    0x03,                         /* bmAttributes (Interrupt) */            \
    USB_HID_DATA_SIZE,            /* wMaxPacketSize */                      \
    0x10),                        /* bInterval (10 ms) */                   \
  /* Endpoint 1 OUT Descriptor (7 bytes) */                                 \
  USB_DESC_ENDPOINT(                                                        \
    USB_ENDPOINT_OUT(USBD1_HID_OUT_EP), /* bEndpointAddress */              \
    0x03,                         /* bmAttributes (Interrupt) */            \
    USB_HID_DATA_SIZE,            /* wMaxPacketSize */                      \
    0x10)                         /* bInterval (10 ms) */

  /* Configuration Descriptor tree for out complex USB device: DFU + CDC + HID.*/
static const uint8_t main_configuration_descriptor_data[] = {
  /* Configuration Descriptor.*/
  USB_DESC_CONFIGURATION(
    USB_DESC_CONFIGURATION_SIZE +
    DFU_DESCRIPTOR_SIZE +
    (IAD_CDC_IF_DESC_SET_SIZE * 1) +
    HID_DESCRIPTOR_TOTAL_SIZE,                    /* wTotalLength.                */
    USB_NUM_INTERFACES,                     /* bNumInterfaces.              */
    0x01,                                   /* bConfigurationValue.         */
    0,                                      /* iConfiguration.              */
    0xC0,                                   /* bmAttributes (self powered). */
    50                                      /* bMaxPower (100mA).           */
  ),
  IAD_DFU_IF_DESC,
  IAD_CDC_IF_DESC_SET(
    USB_CDC_CIF_NUM0,
    USB_CDC_DIF_NUM0,
    USB_ENDPOINT_IN(USBD1_INTERRUPT_REQUEST_EP),
    USB_ENDPOINT_OUT(USBD1_DATA_AVAILABLE_EP),
    USB_ENDPOINT_IN(USBD1_DATA_REQUEST_EP)
  ),
  IAD_HID_IF_DESC,
};

/*
 * Configuration Descriptor wrapper.
 */
static const USBDescriptor main_configuration_descriptor = {
  sizeof main_configuration_descriptor_data,
  main_configuration_descriptor_data
}; /* CustomHID_ConfigDescriptor */

static const USBDescriptor hid_descriptor = {
  HID_DESCRIPTOR_SIZE,
  main_configuration_descriptor_data + (sizeof(main_configuration_descriptor_data) - HID_DESCRIPTOR_TOTAL_SIZE + HID_DESCRIPTOR_OFFSET)
};

/*
 * U.S. English language identifier.
 */
static const uint8_t usb_string0[] = {
  USB_DESC_BYTE(4),                     /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  USB_DESC_WORD(0x0409)                 /* wLANGID (U.S. English).          */
};

/*
 * Vendor string.
 */
static const uint8_t usb_string1[] = {
  USB_DESC_BYTE(38),                    /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'B', 0, 'a', 0, 'i', 0, 'k', 0, 'a', 0, 'l', 0, ' ', 0, 'E', 0,
  'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
  'c', 0, 's', 0
};

/*
 * Device Description string.
 */
static const uint8_t usb_string2[] = {
  USB_DESC_BYTE(8),                     /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'B', 0, 'M', 0, 'C', 0
};

/*
 * Serial Number string.
 */
#ifndef USB_SERIAL_LENGTH
static const uint8_t usb_string3[] = {
  USB_DESC_BYTE(8),                     /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  '0' + CH_KERNEL_MAJOR, 0,
  '0' + CH_KERNEL_MINOR, 0,
  '0' + CH_KERNEL_PATCH, 0
};
#else
uint8_t usb_string3[2+USB_SERIAL_LENGTH*2];
#endif

/*
 * Baikal Flash string.
 */
static const uint8_t usb_string4[] = {
  USB_DESC_BYTE(14),                    /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'B', 0, 'a', 0, 'i', 0, 'k', 0, 'a', 0, 'l', 0
};

#if DFU_INTERFACES == 3
/*
 * SM750 Flash string.
 */
static const uint8_t usb_string5[] = {
  USB_DESC_BYTE(12),                    /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'S', 0, 'M', 0, '7', 0, '5', 0, '0', 0
};

/*
 * PCIe Flash string.
 */
static const uint8_t usb_string6[] = {
  USB_DESC_BYTE(10),                    /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'P', 0, 'C', 0, 'I', 0, 'e', 0
};

#elif DFU_INTERFACES == 2
static const uint8_t usb_string5[] = {
  USB_DESC_BYTE(14),                    /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'E', 0, 'x', 0, 't', 0, 'S', 0, 'P', 0, 'I', 0
};
#endif

/*
 * Strings wrappers array.
 */
static const USBDescriptor usb_strings[] = {
  {sizeof usb_string0, usb_string0},
  {sizeof usb_string1, usb_string1},
  {sizeof usb_string2, usb_string2},
  {sizeof usb_string3, usb_string3},
  {sizeof usb_string4, usb_string4}
#if DFU_INTERFACES == 2
  ,{sizeof usb_string5, usb_string5}
#elif DFU_INTERFACES == 3
  ,{sizeof usb_string6, usb_string6}
#endif
};


/*
 * HID Report Descriptor
 *
 * This is the description of the format and the content of the
 * different IN or/and OUT reports that your application can
 * receive/send
 *
 * See "Device Class Definition for Human Interface Devices (HID)"
 * (http://www.usb.org/developers/hidpage/HID1_11.pdf) for the
 * detailed descrition of all the fields
 */
static const uint8_t hid_report_descriptor_data[] = {
  USB_DESC_HID_REPORT
};

// HID report descriptor wrapper
static const USBDescriptor hid_report_descriptor = {
  sizeof hid_report_descriptor_data,
  hid_report_descriptor_data
};


/*
 * Handles the GET_DESCRIPTOR callback. All required descriptors must be
 * handled here.
 */
static const USBDescriptor *get_descriptor(USBDriver *usbp,
                                           uint8_t dtype,
                                           uint8_t dindex,
                                           uint16_t lang) {

  (void)usbp;
  (void)lang;
  switch (dtype) {
  case USB_DESCRIPTOR_DEVICE:
    return &device_descriptor;
  case USB_DESCRIPTOR_CONFIGURATION:
    return &main_configuration_descriptor;
  case USB_DESCRIPTOR_STRING:
    if (dindex < sizeof(usb_strings)/sizeof(usb_strings[0]))
      return &usb_strings[dindex];
    break;

    // HID specific descriptors
  case USB_DESCRIPTOR_HID:	// HID Descriptor
    return &hid_descriptor;
  case HID_REPORT:		// HID Report Descriptor
    return &hid_report_descriptor;
  }
  return NULL;
}

/**
 * @brief   IN EP1 state.
 */
static USBInEndpointState ep1instate;

/**
 * @brief   EP1 initialization structure (IN only).
 */
static const USBEndpointConfig ep1config = {
  USB_EP_MODE_TYPE_INTR,
  NULL,
  sduInterruptTransmitted,
  NULL,
  USB_INTERRUPT_REQUEST_SIZE,
  0x0000,
  &ep1instate,
  NULL,
  1,
  NULL
};

/**
 * @brief   IN EP2 state.
 */
static USBInEndpointState ep2instate;

/**
 * @brief   OUT EP2 state.
 */
static USBOutEndpointState ep2outstate;

/**
 * @brief   EP1 initialization structure (both IN and OUT).
 */
static const USBEndpointConfig ep2config = {
  USB_EP_MODE_TYPE_BULK,
  NULL,
  sduDataTransmitted,
  sduDataReceived,
  USB_CDC_DATA_SIZE,
  USB_CDC_DATA_SIZE,
  &ep2instate,
  &ep2outstate,
  2,
  NULL
};

/**
 * @brief   IN EP3 state.
 */
static USBInEndpointState ep3instate;

/**
 * @brief   OUT EP3 state.
 */
static USBOutEndpointState ep3outstate;

/**
 * @brief   EP1 initialization structure (both IN and OUT).
 */
static const USBEndpointConfig ep3config = {
  USB_EP_MODE_TYPE_INTR,
  NULL,
  hidDataTransmitted,
  hidDataReceived,
  USB_HID_DATA_SIZE,
  USB_HID_DATA_SIZE,
  &ep3instate,
  &ep3outstate,
  2,
  NULL
};

/*
 * Handles the USB driver global events.
 */
void usb_event(USBDriver *usbp, usbevent_t event) {

  switch (event) {
  case USB_EVENT_RESET:
	sysDfuStop(usbp->alt_setting);		// release DFU mode to Application
    return;
  case USB_EVENT_ADDRESS:
    return;
  case USB_EVENT_CONFIGURED:
    chSysLockFromISR();

    if (usbGetDriverStateI(usbp) == USB_ACTIVE) {
      /* Enables the endpoints specified into the configuration.
         Note, this callback is invoked from an ISR so I-Class functions
         must be used.*/
      usbInitEndpointI(usbp, USBD1_INTERRUPT_REQUEST_EP, &ep1config);
      usbInitEndpointI(usbp, USBD1_DATA_REQUEST_EP, &ep2config);
      usbInitEndpointI(usbp, USBD1_HID_IN_EP, &ep3config);

      /* Resetting the state of the CDC subsystem.*/
      sduConfigureHookI(&SDU1);
      hidConfigureHookI(&HID1);
#if CH_CFG_USE_EVENTS
	  chEvtBroadcastI(&ev_usb_activated);
#endif
    }
    else if (usbGetDriverStateI(usbp) == USB_SELECTED) {
      usbDisableEndpointsI(usbp);
    }

    chSysUnlockFromISR();
    return;
  case USB_EVENT_UNCONFIGURED:
    return;
  case USB_EVENT_SUSPEND:
    chSysLockFromISR();
    /* Disconnection event on suspend.*/
#if CH_HAL_MAJOR >= 5
    sduSuspendHookI(&SDU1);
#else
    sduDisconnectI(&SDU1);
#endif
    hidDisconnectI(&HID1);
    chSysUnlockFromISR();
    return;
  case USB_EVENT_WAKEUP:
    return;
  case USB_EVENT_STALLED:
    return;
  }
  return;
}

static bool dfu_sduRequestsHook(USBDriver *usbp) {
  if( usbp->setup[4] == 0 && usbp->setup[5]==0 ) {
    return dfuRequestsHook(usbp);
  } else {
	bool revVal = sduRequestsHook(usbp);
    if(!revVal) {
      revVal = hidRequestsHook(usbp);
    }
	return(revVal);
  }
}

/*
 * Handles the USB driver global events.
 */
static void sof_handler(USBDriver *usbp) {

  (void)usbp;

  chSysLockFromISR();
  sduSOFHookI(&SDU1);
  chSysUnlockFromISR();
}


/*
 * VCOM USB driver configuration.
 */
const USBConfig usbcfg = {
  usb_event,
  get_descriptor,
  dfu_sduRequestsHook,
  sof_handler
};


/*
 * Serial over USB driver configuration.
 */
const SerialUSBConfig serusbcfg1 = {
  &USBD1,
  USBD1_DATA_REQUEST_EP,
  USBD1_DATA_AVAILABLE_EP,
  USBD1_INTERRUPT_REQUEST_EP
};

/*
 * HID USB driver configuration
 */
 const USBHIDConfig hidusbcfg1 = {
  &USBD1,
  USBD1_HID_IN_EP,
  USBD1_HID_OUT_EP
};


void usbSerialInit(void) {
#if CH_CFG_USE_EVENTS
  chEvtObjectInit(&ev_usb_activated);
#endif

#ifdef USB_SERIAL_LENGTH
#if USB_SERIAL_LENGTH > 24 || USB_SERIAL_LENGTH <= 0
#error "USB_SERIAL_LENGTH must be 1..24"
#endif
	usb_string3[0] = USB_DESC_BYTE(2+USB_SERIAL_LENGTH*2);
	usb_string3[1] = USB_DESC_BYTE(USB_DESCRIPTOR_STRING);
	for(int i=0; i < USB_SERIAL_LENGTH; ++i) {
		unsigned sc = (((*(uint32_t*)(0x1FFF7A10+(i/8)*4))) >> ((i%8)*4))& 0xF;
		usb_string3[2+(USB_SERIAL_LENGTH-1-i)*2] = sc + (sc<10?'0':'A'-10);
		usb_string3[3+(USB_SERIAL_LENGTH-1-i)*2] = 0;
	}
#endif
}
