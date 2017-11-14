/*
 * spi_flash_table.c
 *
 *  Created on: May 12, 2015
 *      Author: k.kranke
 */

#include "spi_flash.h"
#include "flashchips.h"
#include "chipdrivers.h"

const struct flashchip flashchips[] = {

		{
			.vendor		= "Atmel",
			.name		= "AT25DF021",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT25DF021,
			.total_size	= 256,
			.page_size	= 256,
			/* OTP: 128B total, 64B pre-programmed; read 0x77; write 0x9B */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 64} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 8} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 4} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {256 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {256 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25df,
			.unlock		= spi_disable_blockprotect_at2x_global_unprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			.name		= "AT25DF041A",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT25DF041A,
			.total_size	= 512,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 128} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 16} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 8} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {512 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {512 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25df,
			.unlock		= spi_disable_blockprotect_at2x_global_unprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			.name		= "AT25DF081",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT25DF081,
			.total_size	= 1024,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 256} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 32} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 16} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25df,
			.unlock		= spi_disable_blockprotect_at2x_global_unprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			.name		= "AT25DF081A",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT25DF081A,
			.total_size	= 1024,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 256} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 32} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 16} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25df_sec,
			.unlock		= spi_disable_blockprotect_at2x_global_unprotect_sec,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			.name		= "AT25DF161",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT25DF161,
			.total_size	= 2048,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PROBE,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 512} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 64} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 32} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25df_sec,
			.unlock		= spi_disable_blockprotect_at2x_global_unprotect_sec,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			.name		= "AT25DF321",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT25DF321,
			.total_size	= 4096,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 1024} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 128} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 64} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {4 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {4 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25df,
			.unlock		= spi_disable_blockprotect_at2x_global_unprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			.name		= "AT25DF321A",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT25DF321A,
			.total_size	= 4096,
			.page_size	= 256,
			/* OTP: 128B total, 64B pre-programmed; read 0x77; write 0x9B */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 1024} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 128} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 64} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {4 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {4 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25df_sec,
			.unlock		= spi_disable_blockprotect_at2x_global_unprotect_sec,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			.name		= "AT25DF641(A)",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT25DF641,
			.total_size	= 8192,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 2048} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 256} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 128} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {8 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {8 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25df_sec,
			.unlock		= spi_disable_blockprotect_at2x_global_unprotect_sec,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			.name		= "AT25DL081",
			.bustype	= BUS_SPI,
			.manufacture_id = ATMEL_ID,
			.model_id	= ATMEL_AT25DF081,
			.total_size	= 1024,
			.page_size	= 256,
			/* OTP: 128B total, 64B pre-programmed; read 0x77; write 0x9B */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 256} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 32} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 16} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {1 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {1 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25df_sec,
			.unlock		= spi_disable_blockprotect_at2x_global_unprotect_sec,
			.write		= spi_chip_write_256, /* Dual I/O  (0xA2) supported */
			.read		= spi_chip_read, /* Fast read (0x0B), dual I/O  (0x3B) supported */
		},

		{
			.vendor		= "Atmel",
			.name		= "AT25DL161",
			.bustype	= BUS_SPI,
			.manufacture_id = ATMEL_ID,
			.model_id	= ATMEL_AT25DL161,
			.total_size	= 2048,
			.page_size	= 256,
			/* OTP: 128B total, 64B pre-programmed; read 0x77; write 0x9B */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 512} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 64} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 32} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25df_sec,
			.unlock		= spi_disable_blockprotect_at2x_global_unprotect_sec,
			.write		= spi_chip_write_256, /* Dual I/O  (0xA2) supported */
			.read		= spi_chip_read, /* Fast read (0x0B), dual I/O  (0x3B) supported */
		},

		{
			.vendor		= "Atmel",
			.name		= "AT25DQ161",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT25DQ161,
			.total_size	= 2048,
			.page_size	= 256,
			/* OTP: 128B total, 64B pre-programmed; read 0x77; write 0x9B */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 512} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 64} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 32} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25df_sec,
			.unlock		= spi_disable_blockprotect_at2x_global_unprotect_sec,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			.name		= "AT25F512",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT25F512,
			.total_size	= 64,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_at25f,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {32 * 1024, 2} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 1} },
					.block_erase = spi_block_erase_62,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25f,
			.unlock		= spi_disable_blockprotect_at25f,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			.name		= "AT25F512A",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT25F512A,
			.total_size	= 64,
			.page_size	= 128,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_at25f,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {32 * 1024, 2} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 1} },
					.block_erase = spi_block_erase_62,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25f512a,
			/* FIXME: It is not correct to use this one, because the BP1 bit is N/A. */
			.unlock		= spi_disable_blockprotect_at25f512a,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			.name		= "AT25F512B",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT25F512B,
			.total_size	= 64,
			.page_size	= 256,
			/* OTP: 128B total, 64B pre-programmed; read 0x77; write 0x9B */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 16} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 2} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {32 * 1024, 2} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {64 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {64 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}, {
					.eraseblocks = { {64 * 1024, 1} },
					.block_erase = spi_block_erase_62,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25f512b,
			.unlock		= spi_disable_blockprotect_at25f512b,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			/* The A suffix indicates 33MHz instead of 20MHz clock rate.
			 * All other properties seem to be the same.*/
			.name		= "AT25F1024(A)",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT25F1024,
			.total_size	= 128,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_at25f,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {32 * 1024, 4} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {128 * 1024, 1} },
					.block_erase = spi_block_erase_62,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25f,
			.unlock		= spi_disable_blockprotect_at25f,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			.name		= "AT25F2048",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT25F2048,
			.total_size	= 256,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_at25f,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {64 * 1024, 4} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {256 * 1024, 1} },
					.block_erase = spi_block_erase_62,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25f,
			.unlock		= spi_disable_blockprotect_at25f,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			.name		= "AT25F4096",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT25F4096,
			.total_size	= 512,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_at25f,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {64 * 1024, 8} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {512 * 1024, 1} },
					.block_erase = spi_block_erase_62,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25f4096,
			/* "Bits 5-6 are 0s when device is not in an internal write cycle." Better leave them alone: */
			.unlock		= spi_disable_blockprotect_bp2_srwd,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			.name		= "AT25FS010",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT25FS010,
			.total_size	= 128,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 32} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {4 * 1024, 32} },
					.block_erase = spi_block_erase_d7,
				}, {
					.eraseblocks = { {32 * 1024, 4} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {32 * 1024, 4} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {128 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {128 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25fs010,
			.unlock		= spi_disable_blockprotect_at25fs010,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			.name		= "AT25FS040",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT25FS040,
			.total_size	= 512,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 128} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 8} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 8} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {512 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {512 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25fs040,
			.unlock		= spi_disable_blockprotect_at25fs040,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			.name		= "AT26DF041",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT26DF041,
			.total_size	= 512,
			.page_size	= 256,
			/* does not support EWSR nor WREN and has no writable status register bits whatsoever */
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {256, 2048} },
					.block_erase = spi_block_erase_81,
				}, {
					.eraseblocks = { {2 * 1024, 256} },
					.block_erase = spi_block_erase_50,
				}, {
					.eraseblocks = { {4 * 1024, 128} },
					.block_erase = spi_block_erase_20,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain,
			/* Supports also an incompatible page write (of exactly 256 B) and an auto-erasing write. */
			.write		= spi_chip_write_1,
			.read		= spi_chip_read, /* Fast read (0x0B) supported */
		},

		{
			.vendor		= "Atmel",
			.name		= "AT26DF081A",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT26DF081A,
			.total_size	= 1024,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 256} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 32} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 16} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_at26df081a,
			.unlock		= spi_disable_blockprotect_at2x_global_unprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			.name		= "AT26DF161",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT26DF161,
			.total_size	= 2048,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 512} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 64} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 32} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_at25df,
			.unlock		= spi_disable_blockprotect_at2x_global_unprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Atmel",
			.name		= "AT26DF161A",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT26DF161A,
			.total_size	= 2048,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 512} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 64} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 32} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_at26df081a,
			.unlock		= spi_disable_blockprotect_at2x_global_unprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		/*The AT26DF321 has the same ID as the AT25DF321. */
		/*{
			.vendor		= "Atmel",
			.name		= "AT26DF321",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT26DF321,
			.total_size	= 4096,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.printlock	= spi_prettyprint_status_register_at26df081a,
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		  },*/

		{
			.vendor		= "Atmel",
			.name		= "AT26F004",
			.bustype	= BUS_SPI,
			.manufacture_id	= ATMEL_ID,
			.model_id	= ATMEL_AT26F004,
			.total_size	= 512,
			.page_size	= 256,
			.tested		= {.probe = NT, .read = NT, .erase = NT, .write = BAD },
			.feature_bits	= FEATURE_WRSR_WREN,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 128} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 16} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 8} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {512 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {512 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.write		= NULL /* Incompatible Page write */,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "M25P16",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_M25P16,
			.total_size	= 2048,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {64 * 1024, 32} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_bp3_srwd, /* TODO: check */
			.unlock		= spi_disable_blockprotect_bp3_srwd,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "M25P32",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_M25P32,
			.total_size	= 4096,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {64 * 1024, 64} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {4 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_bp3_srwd, /* TODO: check */
			.unlock		= spi_disable_blockprotect_bp3_srwd,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "M25P64",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_M25P64,
			.total_size	= 8192,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {64 * 1024, 128} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {8 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_bp3_srwd, /* TODO: check */
			.unlock		= spi_disable_blockprotect_bp3_srwd,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "M25P128",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_M25P128,
			.total_size	= 16384,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {256 * 1024, 64} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {16 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_bp3_srwd, /* TODO: check */
			.unlock		= spi_disable_blockprotect_bp3_srwd,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "M25PE10",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_M25PE10,
			.total_size	= 128,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 32} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 2} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {128 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "M25PE20",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_M25PE20,
			.total_size	= 256,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 64} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 4} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {256 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "M25PE40",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_M25PE40,
			.total_size	= 512,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 128} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 8} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {512 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "M25PE80",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_M25PE80,
			.total_size	= 1024,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 256} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 16} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "M25PE16",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_M25PE16,
			.total_size	= 2048,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 512} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 32} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "M25PX80",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_M25PX80,
			.total_size	= 1024,
			.page_size	= 256,
			/* OTP: 64B total; read 0x4B, write 0x42 */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	= {
				{
					.eraseblocks = { { 4 * 1024, 256 } },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 16} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_bp2_srwd, /* bit5: T/B */
			.unlock		= spi_disable_blockprotect_bp2_srwd, /* TODO: per 64kB sector lock registers */
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "M25PX16",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_M25PX16,
			.total_size	= 2048,
			.page_size	= 256,
			/* OTP: 64B total; read 0x4B; write 0x42 */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { { 4 * 1024, 512 } },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 32} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_bp2_srwd, /* bit5: T/B */
			.unlock		= spi_disable_blockprotect_bp2_srwd, /* TODO: per 64kB sector lock registers */
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "M25PX32",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_M25PX32,
			.total_size	= 4096,
			.page_size	= 256,
			/* OTP: 64B total; read 0x4B; write 0x42 */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_OK_PRE,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { { 4 * 1024, 1024 } },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 64} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {4 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_bp2_srwd, /* bit5: T/B */
			.unlock		= spi_disable_blockprotect_bp2_srwd, /* TODO: per 64kB sector lock registers */
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "M25PX64",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_M25PX64,
			.total_size	= 8192,
			.page_size	= 256,
			/* OTP: 64B total; read 0x4B; write 0x42 */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { { 4 * 1024, 2048 } },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 128} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {8 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_bp2_srwd, /* bit5: T/B */
			.unlock		= spi_disable_blockprotect_bp2_srwd, /* TODO: per 64kB sector lock registers */
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "M45PE10",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_M45PE10,
			.total_size	= 128,
			.page_size	= 256,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	= {
				{
					.eraseblocks = { {256, 512} },
					.block_erase = spi_block_erase_db,
				}, {
					.eraseblocks = { {64 * 1024, 2} },
					.block_erase = spi_block_erase_d8,
				}
			},
			.printlock	= spi_prettyprint_status_register_default_welwip,
			.unlock		= NULL, /* #WP pin write-protects lower 64kB. */
			.write		= spi_chip_write_256, /* Page write (similar to PP but allows 0->1 changes) */
			.read		= spi_chip_read, /* Fast read (0x0B) supported */
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "M45PE20",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_M45PE20,
			.total_size	= 256,
			.page_size	= 256,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	= {
				{
					.eraseblocks = { {256, 1024} },
					.block_erase = spi_block_erase_db,
				}, {
					.eraseblocks = { {64 * 1024, 4} },
					.block_erase = spi_block_erase_d8,
				}
			},
			.printlock	= spi_prettyprint_status_register_default_welwip,
			.unlock		= NULL, /* #WP pin write-protects lower 64kB. */
			.write		= spi_chip_write_256, /* Page write (similar to PP but allows 0->1 changes) */
			.read		= spi_chip_read, /* Fast read (0x0B) supported */
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "M45PE40",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_M45PE40,
			.total_size	= 512,
			.page_size	= 256,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	= {
				{
					.eraseblocks = { {256, 2048} },
					.block_erase = spi_block_erase_db,
				}, {
					.eraseblocks = { {64 * 1024, 8} },
					.block_erase = spi_block_erase_d8,
				}
			},
			.printlock	= spi_prettyprint_status_register_default_welwip,
			.unlock		= NULL, /* #WP pin write-protects lower 64kB. */
			.write		= spi_chip_write_256, /* Page write (similar to PP but allows 0->1 changes) */
			.read		= spi_chip_read, /* Fast read (0x0B) supported */
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "M45PE80",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_M45PE80,
			.total_size	= 1024,
			.page_size	= 256,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	= {
				{
					.eraseblocks = { {256, 4096} },
					.block_erase = spi_block_erase_db,
				}, {
					.eraseblocks = { {64 * 1024, 16} },
					.block_erase = spi_block_erase_d8,
				}
			},
			.printlock	= spi_prettyprint_status_register_default_welwip,
			.unlock		= NULL, /* #WP pin write-protects lower 64kB. */
			.write		= spi_chip_write_256, /* Page write (similar to PP but allows 0->1 changes) */
			.read		= spi_chip_read, /* Fast read (0x0B) supported */
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "M45PE16",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_M45PE16,
			.total_size	= 2048,
			.page_size	= 256,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	= {
				{
					.eraseblocks = { {256, 8192} },
					.block_erase = spi_block_erase_db,
				}, {
					.eraseblocks = { {64 * 1024, 32} },
					.block_erase = spi_block_erase_d8,
				}
			},
			.printlock	= spi_prettyprint_status_register_default_welwip,
			.unlock		= NULL, /* #WP pin write-protects lower 64kB. */
			.write		= spi_chip_write_256, /* Page write (similar to PP but allows 0->1 changes) */
			.read		= spi_chip_read, /* Fast read (0x0B) supported */
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "N25Q016",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_N25Q016__1E,
			.total_size	= 2048,
			.page_size	= 256,
			/* supports SFDP */
			/* OTP: 64B total; read 0x4B, write 0x42 */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 512} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 64} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 32} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_n25q, /* TODO: config, lock, flag regs */
			.unlock		= spi_disable_blockprotect_n25q, /* TODO: per 64kB sector lock registers */
			.write		= spi_chip_write_256, /* Multi I/O supported */
			.read		= spi_chip_read, /* Fast read (0x0B) and multi I/O supported */
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "N25Q032..1E",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_N25Q032__1E,
			.total_size	= 4096,
			.page_size	= 256,
			/* supports SFDP */
			/* OTP: 64B total; read 0x4B, write 0x42 */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 1024} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 64} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {4 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_n25q, /* TODO: config, lock, flag regs */
			.unlock		= spi_disable_blockprotect_n25q, /* TODO: per 64kB sector lock registers */
			.write		= spi_chip_write_256, /* Multi I/O supported */
			.read		= spi_chip_read, /* Fast read (0x0B) and multi I/O supported */
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "N25Q032..3E",
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_N25Q032__3E,
			.total_size	= 4096,
			.page_size	= 256,
			/* supports SFDP */
			/* OTP: 64B total; read 0x4B, write 0x42 */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 1024} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 64} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {4 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_n25q, /* TODO: config, lock, flag regs */
			.unlock		= spi_disable_blockprotect_n25q, /* TODO: per 64kB sector lock registers */
			.write		= spi_chip_write_256, /* Multi I/O supported */
			.read		= spi_chip_read, /* Fast read (0x0B) and multi I/O supported */
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "N25Q064..1E", /* ..1E = 1.8V, uniform 64KB/4KB blocks/sectors */
			.bustype	= BUS_SPI,
			.manufacture_id	= ST_ID,
			.model_id	= ST_N25Q064__1E,
			.total_size	= 8192,
			.page_size	= 256,
			/* supports SFDP */
			/* OTP: 64B total; read 0x4B, write 0x42 */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 2048 } },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 128} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {8 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_n25q, /* TODO: config, lock, flag regs */
			.unlock		= spi_disable_blockprotect_n25q, /* TODO: per 64kB sector lock registers */
			.write		= spi_chip_write_256, /* Multi I/O supported */
			.read		= spi_chip_read, /* Fast read (0x0B) and multi I/O supported */
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "N25Q064..3E", /* ..3E = 3V, uniform 64KB/4KB blocks/sectors */
			.bustype	= BUS_SPI,
			.manufacture_id = ST_ID,
			.model_id	= ST_N25Q064__3E,
			.total_size	= 8192,
			.page_size	= 256,
			/* supports SFDP */
			/* OTP: 64B total; read 0x4B, write 0x42 */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 2048 } },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 128} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {8 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_n25q, /* TODO: config, lock, flag regs */
			.unlock		= spi_disable_blockprotect_n25q, /* TODO: per 64kB sector lock registers */
			.write		= spi_chip_write_256, /* Multi I/O supported */
			.read		= spi_chip_read, /* Fast read (0x0B) and multi I/O supported */
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "N25Q128..1E", /* ..1E = 1.8V, uniform 64KB/4KB blocks/sectors */
			.bustype	= BUS_SPI,
			.manufacture_id = ST_ID,
			.model_id	= ST_N25Q128__1E,
			.total_size	= 16384,
			.page_size	= 256,
			/* supports SFDP */
			/* OTP: 64B total; read 0x4B, write 0x42 */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	= {
				{
					.eraseblocks = { {4 * 1024, 4096 } },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 256} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {16384 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_n25q, /* TODO: config, lock, flag regs */
			.unlock		= spi_disable_blockprotect_n25q, /* TODO: per 64kB sector lock registers */
			.write		= spi_chip_write_256, /* Multi I/O supported */
			.read		= spi_chip_read, /* Fast read (0x0B) and multi I/O supported */
		},

		{
			.vendor		= "Micron/Numonyx/ST",
			.name		= "N25Q128..3E", /* ..3E = 3V, uniform 64KB/4KB blocks/sectors */
			.bustype	= BUS_SPI,
			.manufacture_id = ST_ID,
			.model_id	= ST_N25Q128__3E,
			.total_size	= 16384,
			.page_size	= 256,
			/* supports SFDP */
			/* OTP: 64B total; read 0x4B, write 0x42 */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	= {
				{
					.eraseblocks = { {4 * 1024, 4096 } },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 256} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {16384 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_n25q, /* TODO: config, lock, flag regs */
			.unlock		= spi_disable_blockprotect_n25q, /* TODO: per 64kB sector lock registers */
			.write		= spi_chip_write_256, /* Multi I/O supported */
			.read		= spi_chip_read, /* Fast read (0x0B) and multi I/O supported */
		},

		{
			.vendor		= "Winbond",
			.name		= "W25Q40.V",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25Q40_V,
			.total_size	= 512,
			.page_size	= 256,
			/* supports SFDP */
			/* OTP: 756B total; read 0x48; write 0x42, erase 0x44, read ID 0x4B */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 128} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 16} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 8} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {512 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {512 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256, /* Multi I/O supported */
			.read		= spi_chip_read, /* Fast read (0x0B) and multi I/O supported */
		},

		{
			.vendor		= "Winbond",
			.name		= "W25Q80.V",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25Q80_V,
			.total_size	= 1024,
			.page_size	= 256,
			/* supports SFDP */
			/* OTP: 1024B total, 256B reserved; read 0x48; write 0x42, erase 0x44, read ID 0x4B */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 256} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 32} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 16} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Winbond",
			.name		= "W25Q16.V",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25Q16_V,
			.total_size	= 2048,
			.page_size	= 256,
			/* supports SFDP */
			/* OTP: 1024B total, 256B reserved; read 0x48; write 0x42, erase 0x44, read ID 0x4B */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 512} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 64} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 32} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Winbond",
			.name		= "W25Q32.V",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25Q32_V,
			.total_size	= 4096,
			.page_size	= 256,
			/* supports SFDP */
			/* OTP: 1024B total, 256B reserved; read 0x48; write 0x42, erase 0x44, read ID 0x4B */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 1024} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 128} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 64} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {4 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {4 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Winbond",
			.name		= "W25Q64.V",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25Q64_V,
			.total_size	= 8192,
			.page_size	= 256,
			/* supports SFDP */
			/* OTP: 1024B total, 256B reserved; read 0x48; write 0x42, erase 0x44, read ID 0x4B */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 2048} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 256} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 128} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {8 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {8 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Winbond",
			.name		= "W25Q128.V",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25Q128_V,
			.total_size	= 16384,
			.page_size	= 256,
			/* supports SFDP */
			/* OTP: 1024B total, 256B reserved; read 0x48; write 0x42, erase 0x44, read ID 0x4B */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 4096} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 512} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 256} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {16 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {16 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Winbond",
			.name		= "W25Q20.W",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25Q20_W,
			.total_size	= 256,
			.page_size	= 256,
			/* OTP: 256B total; read 0x48; write 0x42, erase 0x44, read ID 0x4B */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 64} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 8} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 4} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {256 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {256 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Winbond",
			.name		= "W25Q40.W",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25Q40_W,
			.total_size	= 512,
			.page_size	= 256,
			/* OTP: 256B total; read 0x48; write 0x42, erase 0x44, read ID 0x4B */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 128} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 16} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 8} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {512 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {512 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Winbond",
			.name		= "W25Q80.W",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25Q80_W,
			.total_size	= 1024,
			.page_size	= 256,
			/* OTP: 256B total; read 0x48; write 0x42, erase 0x44, read ID 0x4B */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 256} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 32} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 16} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {1 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {1 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Winbond",
			.name		= "W25Q16.W",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25Q16_W,
			.total_size	= 2048,
			.page_size	= 256,
			/* OTP: 256B total; read 0x48; write 0x42, erase 0x44, read ID 0x4B */
			/* QPI enable 0x38, disable 0xFF */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP | FEATURE_QPI,
			.tested		= TEST_UNTESTED,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 512} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 64} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 32} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Winbond",
			.name		= "W25Q32.W",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25Q32_W,
			.total_size	= 4096,
			.page_size	= 256,
			/* OTP: 256B total; read 0x48; write 0x42, erase 0x44, read ID 0x4B */
			/* QPI enable 0x38, disable 0xFF */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP | FEATURE_QPI,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 1024} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 128} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 64} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {4 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {4 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Winbond",
			.name		= "W25Q64.W",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25Q64_W,
			.total_size	= 8192,
			.page_size	= 256,
			/* OTP: 256B total; read 0x48; write 0x42, erase 0x44, read ID 0x4B */
			/* QPI enable 0x38, disable 0xFF */
			.feature_bits	= FEATURE_WRSR_WREN | FEATURE_OTP | FEATURE_QPI,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 2048} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 256} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 128} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {8 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {8 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Winbond",
			.name		= "W25X10",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25X10,
			.total_size	= 128,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 32} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 2} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {128 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Winbond",
			.name		= "W25X20",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25X20,
			.total_size	= 256,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 64} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 4} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {256 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Winbond",
			.name		= "W25X40",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25X40,
			.total_size	= 512,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 128} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 8} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {512 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Winbond",
			.name		= "W25X80",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25X80,
			.total_size	= 1024,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 256} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {64 * 1024, 16} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Winbond",
			.name		= "W25X16",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25X16,
			.total_size	= 2048,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 512} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 64} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 32} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {2 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Winbond",
			.name		= "W25X32",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25X32,
			.total_size	= 4096,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PREW,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 1024} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 128} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 64} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {4 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {4 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{
			.vendor		= "Winbond",
			.name		= "W25X64",
			.bustype	= BUS_SPI,
			.manufacture_id	= WINBOND_NEX_ID,
			.model_id	= WINBOND_NEX_W25X64,
			.total_size	= 8192,
			.page_size	= 256,
			.feature_bits	= FEATURE_WRSR_WREN,
			.tested		= TEST_OK_PROBE,
			.probe		= probe_spi_rdid,
			.probe_timing	= TIMING_ZERO,
			.block_erasers	=
			{
				{
					.eraseblocks = { {4 * 1024, 2048} },
					.block_erase = spi_block_erase_20,
				}, {
					.eraseblocks = { {32 * 1024, 256} },
					.block_erase = spi_block_erase_52,
				}, {
					.eraseblocks = { {64 * 1024, 128} },
					.block_erase = spi_block_erase_d8,
				}, {
					.eraseblocks = { {8 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_60,
				}, {
					.eraseblocks = { {8 * 1024 * 1024, 1} },
					.block_erase = spi_block_erase_c7,
				}
			},
			.printlock	= spi_prettyprint_status_register_plain, /* TODO: improve */
			.unlock		= spi_disable_blockprotect,
			.write		= spi_chip_write_256,
			.read		= spi_chip_read,
		},

		{0}
};

const unsigned int flashchips_size = ARRAY_SIZE(flashchips);
