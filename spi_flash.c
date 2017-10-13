#include "spi_flash.h"
#include "chipdrivers.h"

#include "chprintf.h"
#include "sdu.h"

#include <string.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"

int spi_send_command(sFLASH_USBDriver *flash, unsigned int writecnt,
		unsigned int readcnt, const unsigned char *writearr,
		unsigned char *readarr) {
	/*!< Select the FLASH: Chip Select low */
	sFLASH_CS_LOW(flash);

	while(writecnt) {
		sFLASH_SendByte(flash, *writearr);
		writearr++;
		writecnt--;
	}

	while(readcnt) {
		*readarr = sFLASH_SendByte(flash, sFLASH_DUMMY_BYTE);
		readarr++;
		readcnt--;
	}

	/*!< Deselect the FLASH: Chip Select high */
	sFLASH_CS_HIGH(flash);

	return 0;
}

int spi_send_multicommand(sFLASH_USBDriver *flash, struct spi_command *cmds) {
	int result = 0;
	for (; (cmds->writecnt || cmds->readcnt) && !result; cmds++) {
		result = spi_send_command(flash, cmds->writecnt, cmds->readcnt,
					  cmds->writearr, cmds->readarr);
	}
	return result;
}

/*
 * Get the lowest allowed address for read accesses. This often happens to
 * be the lowest allowed address for all commands which take an address.
 * This is a master limitation.
 */
uint32_t spi_get_valid_read_addr(sFLASH_USBDriver *flash) {
	return 0;
}

int spi_chip_write_256(sFLASH_USBDriver *flash, const uint8_t *buf, unsigned int start, unsigned int len) {
	// TODO: Use DMA to write here
	sFLASH_WritePage(flash, buf, start, len);
	return 0;
}

int spi_chip_read(sFLASH_USBDriver *flash, uint8_t *buf, unsigned int start, unsigned int len)
{
	unsigned int addrbase = 0;

	/* Check if the chip fits between lowest valid and highest possible
	 * address. Highest possible address with the current SPI implementation
	 * means 0xffffff, the highest unsigned 24bit number.
	 */
	addrbase = spi_get_valid_read_addr(flash);
	if (addrbase + flash->chip->total_size * 1024 > (1 << 24)) {
		msg_perr("Flash chip size exceeds the allowed access window. ");
		msg_perr("Read will probably fail.\n");
		/* Try to get the best alignment subject to constraints. */
		addrbase = (1 << 24) - flash->chip->total_size * 1024;
	}
	/* Check if alignment is native (at least the largest power of two which
	 * is a factor of the mapped size of the chip).
	 */
	if (ffs(flash->chip->total_size * 1024) > (ffs(addrbase) ? : 33)) {
		msg_perr("Flash chip is not aligned natively in the allowed "
			 "access window.\n");
		msg_perr("Read will probably return garbage.\n");
	}

	// TODO: Use DMA to read
	sFLASH_ReadBuffer(flash, buf, addrbase+start, len);
	return 0;
}

/**
  * @brief  DeInitializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void sFLASH_DeInit(sFLASH_USBDriver* SFDU) {
	SFDU->chip = 0;
	spiStop(SFDU->config->SPID);
}

/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void sFLASH_Init(sFLASH_USBDriver* SFDU, const sFLASH_USBConfig* cfg) {
	SFDU->chip = 0;
	SFDU->config = cfg;
}

/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void sFLASH_Start(sFLASH_USBDriver* SFDU) {

	/*
	 * Initializes the SPI driver 1 in order to access the MEMS. The signals
	 * are already initialized in the board file.
	 */
	spiStart(SFDU->config->SPID, &SFDU->config->spicfg);

	/*!< Deselect the FLASH: Chip Select high */
	sFLASH_CS_HIGH(SFDU);

//	sFLASH_AQUIRE(SFDU);
//	/* Validate params from spi_flash_params table */
//	probe_spi_rdid(SFDU);
	sFLASH_RELEASE(SFDU);
}

/**
  * @brief  Erases the specified FLASH sector.
  * @param  SectorAddr: address of the sector to erase.
  * @retval None
  */
void sFLASH_EraseSector(sFLASH_USBDriver* SFDU, uint32_t SectorAddr)
{
  /*!< Send write enable instruction */
  sFLASH_WriteEnable(SFDU);

  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW(SFDU);
  /*!< Send Sector Erase instruction */
  sFLASH_SendByte(SFDU, sFLASH_CMD_SE);
  /*!< Send SectorAddr high nibble address byte */
  sFLASH_SendByte(SFDU, (SectorAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  sFLASH_SendByte(SFDU, (SectorAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  sFLASH_SendByte(SFDU, SectorAddr & 0xFF);
  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH(SFDU);

  /*!< Wait the end of Flash writing */
  sFLASH_WaitForWriteEnd(SFDU);
}

/**
  * @brief  Erases the entire FLASH.
  * @param  None
  * @retval None
  */
void sFLASH_EraseBulk(sFLASH_USBDriver* SFDU)
{
  /*!< Send write enable instruction */
  sFLASH_WriteEnable(SFDU);

  /*!< Bulk Erase */
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW(SFDU);
  /*!< Send Bulk Erase instruction  */
  sFLASH_SendByte(SFDU, sFLASH_CMD_BE);
  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH(SFDU);

  /*!< Wait the end of Flash writing */
  sFLASH_WaitForWriteEnd(SFDU);
}

/**
  * @brief  Writes more than one byte to the FLASH with a single WRITE cycle
  *         (Page WRITE sequence).
  * @note   The number of byte can't exceed the FLASH page size.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH, must be equal
  *         or less than "sFLASH_PAGESIZE" value.
  * @retval None
  */
void sFLASH_WritePage(sFLASH_USBDriver* SFDU, const uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  /*!< Enable the write access to the FLASH */
  sFLASH_WriteEnable(SFDU);

  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW(SFDU);
  /*!< Send "Write to Memory " instruction */
  sFLASH_SendByte(SFDU, sFLASH_CMD_WRITE);
  /*!< Send WriteAddr high nibble address byte to write to */
  sFLASH_SendByte(SFDU, (WriteAddr & 0xFF0000) >> 16);
  /*!< Send WriteAddr medium nibble address byte to write to */
  sFLASH_SendByte(SFDU, (WriteAddr & 0xFF00) >> 8);
  /*!< Send WriteAddr low nibble address byte to write to */
  sFLASH_SendByte(SFDU, WriteAddr & 0xFF);

  /*!< while there is data to be written on the FLASH */
  while (NumByteToWrite--)
  {
    /*!< Send the current byte */
    sFLASH_SendByte(SFDU, *pBuffer);
    /*!< Point on the next byte to be written */
    pBuffer++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH(SFDU);

  /*!< Wait the end of Flash writing */
  sFLASH_WaitForWriteEnd(SFDU);
}

/**
  * @brief  Writes block of data to the FLASH. In this function, the number of
  *         WRITE cycles are reduced, using Page WRITE sequence.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH.
  * @retval None
  */
void sFLASH_WriteBuffer(sFLASH_USBDriver* SFDU, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % sFLASH_SPI_PAGESIZE;
  count = sFLASH_SPI_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / sFLASH_SPI_PAGESIZE;
  NumOfSingle = NumByteToWrite % sFLASH_SPI_PAGESIZE;

  if (Addr == 0) /*!< WriteAddr is sFLASH_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < sFLASH_PAGESIZE */
    {
      sFLASH_WritePage(SFDU, pBuffer, WriteAddr, NumByteToWrite);
    }
    else /*!< NumByteToWrite > sFLASH_PAGESIZE */
    {
      while (NumOfPage--)
      {
        sFLASH_WritePage(SFDU, pBuffer, WriteAddr, sFLASH_SPI_PAGESIZE);
        WriteAddr +=  sFLASH_SPI_PAGESIZE;
        pBuffer += sFLASH_SPI_PAGESIZE;
      }

      sFLASH_WritePage(SFDU, pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /*!< WriteAddr is not sFLASH_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < sFLASH_PAGESIZE */
    {
      if (NumOfSingle > count) /*!< (NumByteToWrite + WriteAddr) > sFLASH_PAGESIZE */
      {
        temp = NumOfSingle - count;

        sFLASH_WritePage(SFDU, pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        sFLASH_WritePage(SFDU, pBuffer, WriteAddr, temp);
      }
      else
      {
        sFLASH_WritePage(SFDU, pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /*!< NumByteToWrite > sFLASH_PAGESIZE */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / sFLASH_SPI_PAGESIZE;
      NumOfSingle = NumByteToWrite % sFLASH_SPI_PAGESIZE;

      sFLASH_WritePage(SFDU, pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        sFLASH_WritePage(SFDU, pBuffer, WriteAddr, sFLASH_SPI_PAGESIZE);
        WriteAddr +=  sFLASH_SPI_PAGESIZE;
        pBuffer += sFLASH_SPI_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
        sFLASH_WritePage(SFDU, pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

/**
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FLASH.
  * @retval None
  */
void sFLASH_ReadBuffer(sFLASH_USBDriver* SFDU, uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW(SFDU);

  /*!< Send "Read from Memory " instruction */
  sFLASH_SendByte(SFDU, sFLASH_CMD_READ);

  /*!< Send ReadAddr high nibble address byte to read from */
  sFLASH_SendByte(SFDU, (ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte to read from */
  sFLASH_SendByte(SFDU, (ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte to read from */
  sFLASH_SendByte(SFDU, ReadAddr & 0xFF);

  while (NumByteToRead--) /*!< while there is data to be read */
  {
    /*!< Read a byte from the FLASH */
    *pBuffer = sFLASH_SendByte(SFDU, sFLASH_DUMMY_BYTE);
    /*!< Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH(SFDU);
}

/**
  * @brief  Reads FLASH identification.
  * @param  None
  * @retval FLASH identification
  */
void sFLASH_ReadID(sFLASH_USBDriver* SFDU, uint8_t *idbuffer)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW(SFDU);

  /*!< Send "RDID " instruction */
  sFLASH_SendByte(SFDU, 0x9F);

  /*!< Read a byte from the FLASH */
  idbuffer[0] = sFLASH_SendByte(SFDU, sFLASH_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  idbuffer[1] = sFLASH_SendByte(SFDU, sFLASH_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  idbuffer[2] = sFLASH_SendByte(SFDU, sFLASH_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  idbuffer[3] = sFLASH_SendByte(SFDU, sFLASH_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  idbuffer[4] = sFLASH_SendByte(SFDU, sFLASH_DUMMY_BYTE);

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH(SFDU);
}

/**
  * @brief  Initiates a read data byte (READ) sequence from the Flash.
  *   This is done by driving the /CS line low to select the device, then the READ
  *   instruction is transmitted followed by 3 bytes address. This function exit
  *   and keep the /CS line low, so the Flash still being selected. With this
  *   technique the whole content of the Flash is read with a single READ instruction.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @retval None
  */
void sFLASH_StartReadSequence(sFLASH_USBDriver* SFDU, uint32_t ReadAddr)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW(SFDU);

  /*!< Send "Read from Memory " instruction */
  sFLASH_SendByte(SFDU, sFLASH_CMD_READ);

  /*!< Send the 24-bit address of the address to read from -------------------*/
  /*!< Send ReadAddr high nibble address byte */
  sFLASH_SendByte(SFDU, (ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte */
  sFLASH_SendByte(SFDU, (ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte */
  sFLASH_SendByte(SFDU, ReadAddr & 0xFF);
}

/**
  * @brief  Reads a byte from the SPI Flash.
  * @note   This function must be used only if the Start_Read_Sequence function
  *         has been previously called.
  * @param  None
  * @retval Byte Read from the SPI Flash.
  */
uint8_t sFLASH_ReadByte(sFLASH_USBDriver* SFDU)
{
  return (sFLASH_SendByte(SFDU, sFLASH_DUMMY_BYTE));
}

/**
  * @brief  Sends a byte through the SPI interface and return the byte received
  *         from the SPI bus.
  * @param  byte: byte to send.
  * @retval The value of the received byte.
  */
uint8_t sFLASH_SendByte(sFLASH_USBDriver* SFDU, uint8_t byte)
{
  uint8_t receive = spiPolledExchange(SFDU->config->SPID, byte);

  return receive;
}

/**
  * @brief  Sends a Half Word through the SPI interface and return the Half Word
  *         received from the SPI bus.
  * @param  HalfWord: Half Word to send.
  * @retval The value of the received Half Word.
  */
uint16_t sFLASH_SendHalfWord(sFLASH_USBDriver* SFDU, uint16_t HalfWord)
{
  uint16_t receive = spiPolledExchange(SFDU->config->SPID, HalfWord & 0xff);
  receive |= spiPolledExchange(SFDU->config->SPID, (HalfWord >> 8) & 0xff) << 8;

  return receive;
}

/**
  * @brief  Enables the write access to the FLASH.
  * @param  None
  * @retval None
  */
void sFLASH_WriteEnable(sFLASH_USBDriver* SFDU)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW(SFDU);

  /*!< Send "Write Enable" instruction */
  sFLASH_SendByte(SFDU, sFLASH_CMD_WREN);

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH(SFDU);
}

/**
  * @brief  Polls the status of the Write In Progress (WIP) flag in the FLASH's
  *         status register and loop until write opertaion has completed.
  * @param  None
  * @retval None
  */
void sFLASH_WaitForWriteEnd(sFLASH_USBDriver* SFDU)
{
  uint8_t flashstatus = 0;

  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW(SFDU);

  /*!< Send "Read Status Register" instruction */
  sFLASH_SendByte(SFDU, sFLASH_CMD_RDSR);

  /*!< Loop as long as the memory is busy with a write cycle */
  do
  {
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    flashstatus = sFLASH_SendByte(SFDU, sFLASH_DUMMY_BYTE);

  }
  while ((flashstatus & sFLASH_WIP_FLAG) == SET); /* Write in progress */

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH(SFDU);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
