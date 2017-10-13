/*
 * dfu.c
 *
 *  Created on: May 6, 2015
 *      Author: k.kranke
 */

#ifndef DFU_C_
#define DFU_C_

#include "ch.h"
#include "hal.h"

#include <string.h>

#include "sdu.h"
#include "dfu.h"

// For debug
extern uint8_t dfuBuff[1024];
static uint32_t lCount = 0;
volatile static uint8_t lTr = 0;

// For debug time work with SPI flash directly
//#include "spi_flash.h"
#include "chipdrivers.h"

// We assume only single instance of DFU per MCU.
void dfuObjectInit(DFU_USBDriver *dfup) {
	memset(dfup->status, 0, 6);
	dfup->write_address_start = 0;
	dfup->read_address_start = 0;
}

void dfuStart(DFU_USBDriver *dfup, const DFU_USBConfig *config) {
	USBDriver *usbp = config->usbp;
	usbp->dfu = (void*)dfup;
	dfup->config = config;

	dfup->status[DFU_STATUS_STATE] = DFU_STATE_IDLE;
}

void dfuStop(DFU_USBDriver *dfup) {
	USBDriver *usbp = dfup->config->usbp;
	usbp->dfu = 0;
}

void dfuSetTimeout(uint8_t *st, uint32_t ms) {
	st[DFU_STATUS_TIMEOUT+0] = (uint8_t)(ms);
	st[DFU_STATUS_TIMEOUT+1] = (uint8_t)(ms>>8);
	st[DFU_STATUS_TIMEOUT+2] = (uint8_t)(ms>>16);
}

void dfuSetError(uint8_t *st, uint8_t err) {
	st[DFU_STATUS_STATE] = DFU_STATE_ERROR;
	st[DFU_STATUS_ERROR] = err;
}

void dfuDnloadReceive(USBDriver *usbp) {
	DFU_USBDriver *dfup = (DFU_USBDriver*)(usbp->dfu);
	sFLASH_USBDriver *SFDU = dfup->config->SFDU[usbp->alt_setting];
	unsigned bytes_to_write = DFU_BUFFER_LENGTH;
	unsigned i;
	if(dfup->write_address % SFDU->chip->block_erasers[0].eraseblocks[0].size == 0) {
		SFDU->chip->block_erasers[0].block_erase(SFDU, dfup->write_address, SFDU->chip->block_erasers[0].eraseblocks[0].size);
	}

	SFDU->chip->read(SFDU, dfup->flash_buffer2, dfup->write_address, MAX(SFDU->chip->page_size, bytes_to_write));
	for(i = 0; i < MAX(SFDU->chip->page_size, bytes_to_write); i++) {
		if(dfup->flash_buffer2[i % SFDU->chip->page_size] != 0xff) {
			dfuSetError(dfup->status,DFU_ERROR_ERASE);
			return;
		}
	}

	/**/
	while(bytes_to_write) {
		SFDU->chip->write(SFDU, dfup->flash_buffer, dfup->write_address, SFDU->chip->page_size);
		SFDU->chip->read(SFDU, dfup->flash_buffer2, dfup->write_address, SFDU->chip->page_size);
		if(memcmp(dfup->flash_buffer, dfup->flash_buffer2, SFDU->chip->page_size)) {
			dfuSetError(dfup->status,DFU_ERROR_VERIFY);
			return;
		}
		bytes_to_write -= SFDU->chip->page_size;
		dfup->write_address += SFDU->chip->page_size;
	}
	dfup->writing = 0;
	usbp->ep0endcb = 0;
}

bool dfuRequestsHook(USBDriver *usbp)
{
	DFU_USBDriver    *dfup = (DFU_USBDriver*)(usbp->dfu);
	sFLASH_USBDriver *SFDU = dfup->config->SFDU[usbp->alt_setting];
	unsigned int wValue, wLength;

	if (lCount < 1024)
	{
		lCount++;
	}

	if((usbp->setup[0] & USB_RTYPE_TYPE_MASK) == USB_RTYPE_TYPE_CLASS)
	{
		switch (usbp->setup[1])
		{
			case DFU_DNLOAD:
				dfuBuff[lCount] = 1;
				if(!SFDU->chip)
				{
					buttonsPowerResetOn();
					dfuSetError(dfup->status,DFU_ERROR_TARGET);
					usbSetupTransfer(usbp, NULL, 0, NULL);
					break;
				}

				buttonsPowerResetOff();

				wLength = ((uint16_t)usbp->setup[6] | ((uint16_t)usbp->setup[7] << 8));
				if(dfup->status[DFU_STATUS_STATE] == DFU_STATE_IDLE)
				{
					dfup->status[DFU_STATUS_STATE] = DFU_STATE_DNLOAD_IDLE;
					dfup->writing = 0;
					dfup->write_address = 0;
					sFLASH_AQUIRE(SFDU);
				}

				if(dfup->status[DFU_STATUS_STATE] == DFU_STATE_DNLOAD_IDLE)
				{
					if(wLength != 0)
					{
						if((SFDU->chip->total_size << 10) < (dfup->write_address))
						{
							buttonsPowerResetOn();
							dfuSetError(dfup->status,DFU_ERROR_ADDRESS);
							return false;
						}
						usbSetupTransfer(usbp, dfup->flash_buffer, wLength, dfuDnloadReceive);
						dfup->writing = 1;
					}
					else
					{
						dfup->status[DFU_STATUS_STATE] = DFU_STATE_MANIFEST_SYNC;
						sFLASH_RELEASE(SFDU);
						buttonsPowerResetOn();
						usbSetupTransfer(usbp, NULL, 0, NULL);
					}
				}
				else
				{
					usbSetupTransfer(usbp, NULL, 0, NULL);
				}
				return true;
			case DFU_UPLOAD:
				dfuBuff[lCount] = 2;
				if(!SFDU->chip)
				{
					buttonsPowerResetOn();
					dfuSetError(dfup->status,DFU_ERROR_TARGET);
					usbSetupTransfer(usbp, NULL, 0, NULL);
					break;
				}

				buttonsPowerResetOff();

				if(dfup->status[DFU_STATUS_STATE] == DFU_STATE_IDLE)
				{
					sFLASH_AQUIRE(SFDU);
					dfup->read_address = 0;
				}
				wLength = ((uint16_t)usbp->setup[6] | ((uint16_t)usbp->setup[7] << 8));
				if((SFDU->chip->total_size << 10) > (dfup->read_address))
				{
					SFDU->chip->read(SFDU, dfup->flash_buffer, dfup->read_address, wLength);
					dfup->read_address += wLength;
					usbSetupTransfer(usbp, dfup->flash_buffer, wLength, NULL);
					dfup->status[DFU_STATUS_STATE] = DFU_STATE_UPLOAD_IDLE;
				}
				else
				{
					buttonsPowerResetOn();
					dfuSetError(dfup->status,DFU_ERROR_ADDRESS);
					return false;
				}
				if((SFDU->chip->total_size << 10) == dfup->read_address)
				{
					dfup->status[DFU_STATUS_STATE] = DFU_STATE_IDLE;
					sFLASH_RELEASE(SFDU);
					buttonsPowerResetOn();
				}
				return true;
			case DFU_GETSTATUS:
				dfuBuff[lCount] = 3;
				buttonsPowerResetOff();
//				if (lCount == 148)
//				{
//					lTr = 1;
//				}

				dfuSetTimeout(dfup->status,0);
				if ( dfup->status[DFU_STATUS_STATE] == DFU_STATE_MANIFEST_SYNC )
				{
					dfup->status[DFU_STATUS_STATE] = DFU_STATE_MANIFEST;
					dfuSetTimeout(dfup->status,100);
					sysDfuStop(usbp->alt_setting);
					buttonsPowerResetOn();
				}
				else if (dfup->status[DFU_STATUS_STATE] == DFU_STATE_MANIFEST )
				{
					dfup->status[DFU_STATUS_STATE] = DFU_STATE_DNLOAD_IDLE;
					buttonsPowerResetOn();
				}
				else
				{
					if(!sysDfuReady(usbp->alt_setting))
					{
						sysDfuStart(usbp->alt_setting);
						dfuSetTimeout(dfup->status,1500);
						dfuSetError(dfup->status, DFU_ERROR_POR);
						buttonsPowerResetOn();
					}
					else if(!SFDU->chip)
					{
						dfuSetTimeout(dfup->status,100);
						dfuSetError(dfup->status, DFU_ERROR_TARGET);
						buttonsPowerResetOn();
					}
					else if((dfup->status[DFU_STATUS_STATE] == DFU_STATE_DNLOAD_SYNC) || (dfup->status[DFU_STATUS_STATE] == DFU_STATE_DNLOAD_BUSY))
					{
						if(dfup->writing)
						{
							dfup->status[DFU_STATUS_STATE] = DFU_STATE_DNLOAD_BUSY;
							dfuSetTimeout(dfup->status,100);
						}
						else
						{
							dfup->status[DFU_STATUS_STATE] = DFU_STATE_DNLOAD_IDLE;
						}
					}
				}
				usbSetupTransfer(usbp, dfup->status, 6, NULL);
				return true;
			case DFU_CLRSTATUS:
				dfuBuff[lCount] = 4;
				buttonsPowerResetOff();
				if(!sysDfuReady(usbp->alt_setting))
				{
					buttonsPowerResetOn();
					dfuSetError(dfup->status, DFU_ERROR_POR);
					sFLASH_RELEASE(SFDU);		//??
					sysDfuStop(usbp->alt_setting);
					return false;
				}
				else if(!SFDU->chip)
				{
					sFLASH_AQUIRE(SFDU);
					probe_spi_rdid(SFDU);
					if ( !SFDU->chip )
					{
						buttonsPowerResetOn();
						dfup->status[DFU_STATUS_ERROR] = DFU_ERROR_VENDOR;
						sFLASH_RELEASE(SFDU);		//??
						sysDfuStop(usbp->alt_setting);
						return false;
					}
				}
				dfup->status[DFU_STATUS_STATE] = DFU_STATE_IDLE;
				dfup->status[DFU_STATUS_ERROR] = DFU_ERROR_NONE;
				dfup->writing = 0;
				return true;
			case DFU_GETSTATE:
				dfuBuff[lCount] = 5;
				buttonsPowerResetOff();
				usbSetupTransfer(usbp, dfup->status+DFU_STATUS_STATE, 1, NULL);
				return true;
			case DFU_ABORT:
				dfuBuff[lCount] = 6;
				buttonsPowerResetOn();
				dfup->status[DFU_STATUS_STATE] = DFU_STATE_IDLE;
				dfup->status[DFU_STATUS_ERROR] = DFU_ERROR_NONE;
				dfup->writing = 0;
				sFLASH_RELEASE(SFDU);		//??
				sysDfuStop(usbp->alt_setting);
				return true;
			case DFU_DETACH:
				dfuBuff[lCount] = 7;
				buttonsPowerResetOn();
				//buttonsPowerResetOff();
				return true;
		}
	}
	else if(((usbp->setup[0] & USB_RTYPE_RECIPIENT_MASK) == USB_RTYPE_RECIPIENT_INTERFACE) && (usbp->setup[1] == USB_REQ_SET_INTERFACE))
	{
		dfuBuff[lCount] = 8;
		wValue = ((uint16_t)usbp->setup[2] | ((uint16_t)usbp->setup[3] << 8));
		if( (wValue < DFU_INTERFACES) )
		{
			dfuBuff[lCount] = 9;
			buttonsPowerResetOff();
			usbp->alt_setting = wValue;
			//usbSetupTransfer(usbp, NULL, 0, NULL);	// zlp
			SFDU = dfup->config->SFDU[usbp->alt_setting];
			SFDU->chip = 0;
			dfup->writing = 0;
			dfup->status[DFU_STATUS_STATE] = DFU_STATE_IDLE;
			dfup->status[DFU_STATUS_ERROR] = DFU_ERROR_NONE;
			return true;
		}
		else
		{
			dfuBuff[lCount] = 10;
			buttonsPowerResetOn();
			// Error!
			return false;
		}
	}
	return false;
}

#endif /* DFU_C_ */
