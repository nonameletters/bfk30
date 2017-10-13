/*
 * dfu.h
 *
 *  Created on: May 6, 2015
 *      Author: k.kranke
 */

#ifndef DFU_H_
#define DFU_H_

#include "spi_flash.h"
#include "exti_ex.h"

/**************************************************/
/* DFU Requests  DFU states                       */
/**************************************************/
#define APP_STATE_IDLE                 0
#define APP_STATE_DETACH               1
#define DFU_STATE_IDLE                 2
#define DFU_STATE_DNLOAD_SYNC          3
#define DFU_STATE_DNLOAD_BUSY          4
#define DFU_STATE_DNLOAD_IDLE          5
#define DFU_STATE_MANIFEST_SYNC        6
#define DFU_STATE_MANIFEST             7
#define DFU_STATE_MANIFEST_WAIT_RESET  8
#define DFU_STATE_UPLOAD_IDLE          9
#define DFU_STATE_ERROR                10

/**************************************************/
/* DFU errors                                     */
/**************************************************/
#define DFU_ERROR_NONE              0x00
#define DFU_ERROR_TARGET            0x01
#define DFU_ERROR_FILE              0x02
#define DFU_ERROR_WRITE             0x03
#define DFU_ERROR_ERASE             0x04
#define DFU_ERROR_CHECK_ERASED      0x05
#define DFU_ERROR_PROG              0x06
#define DFU_ERROR_VERIFY            0x07
#define DFU_ERROR_ADDRESS           0x08
#define DFU_ERROR_NOTDONE           0x09
#define DFU_ERROR_FIRMWARE          0x0A
#define DFU_ERROR_VENDOR            0x0B
#define DFU_ERROR_USB               0x0C
#define DFU_ERROR_POR               0x0D
#define DFU_ERROR_UNKNOWN           0x0E
#define DFU_ERROR_STALLEDPKT        0x0F

#define MAX(a, b) ((a > b) ? a : b)

/**************************************************/
/* DFU Requests                                   */
/**************************************************/
typedef enum _DFU_REQUESTS {
	DFU_DETACH = 0,
	DFU_DNLOAD = 1,
	DFU_UPLOAD,
	DFU_GETSTATUS,
	DFU_CLRSTATUS,
	DFU_GETSTATE,
	DFU_ABORT
} DFU_REQUESTS;

/* The device responds to the DFU_GETSTATUS request with a payload packet containing the following data:

   0            bStatus        1        Number          An indication of the status resulting from the
                                                        execution of the most recent request.

   1            bwPollTimeout  3        Number          Minimum time, in milliseconds, that the host
                                                        should wait before sending a subsequent
                                                        DFU_GETSTATUS request. *

   4            bState         1        Number          An indication of the state that the device is going to
                                                        enter immediately following transmission of this
                                                        response. (By the time the host receives this
                                                        information, this is the current state of the device.)

   5            iString        1        Index           Index of status description in string table. **
 */

#define DFU_STATUS_ERROR     0	// size 1
#define DFU_STATUS_TIMEOUT	 1	// size 3
#define DFU_STATUS_STATE     4	// size 1

#define DFU_BUFFER_LENGTH    0x100

#ifdef VIDEO_SPI_PORT
#define DFU_INTERFACES 3
#else
#ifdef BE_INTERNAL
#define DFU_INTERFACES 1	// 2 for external SPI programmer DFU
#else
#define DFU_INTERFACES 1
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   DFU USB Driver configuration structure.
 * @details An instance of this structure must be passed to @p dfuStart()
 *          in order to configure and start the driver operations.
 */
typedef struct {
	USBDriver                 *usbp;
	sFLASH_USBDriver          *SFDU[DFU_INTERFACES];
} DFU_USBConfig;

typedef struct {
	uint8_t status[6];
	const DFU_USBConfig *config;
	uint8_t flash_buffer[DFU_BUFFER_LENGTH];
	uint8_t flash_buffer2[DFU_BUFFER_LENGTH];
	unsigned write_address_start;
	unsigned write_address;
	unsigned read_address_start;
	unsigned read_address;
	unsigned writing;
} DFU_USBDriver;

void dfuObjectInit(DFU_USBDriver *dfup);
void dfuStart(DFU_USBDriver *dfup, const DFU_USBConfig *config);
void dfuStop(DFU_USBDriver *dfup);
bool dfuRequestsHook(USBDriver *usbp);

bool sysDfuReady(unsigned alt);
void sysDfuStart(unsigned alt);
void sysDfuStop(unsigned alt);


#ifdef __cplusplus
}
#endif

#endif /* DFU_H_ */
