/*
 * This file is part of the flashrom project.
 *
 * Copyright (C) 2009 Carl-Daniel Hailfinger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 *
 * Header file for flash chip drivers. Included from flash.h.
 * As a general rule, every function listed here should take a pointer to
 * sFLASH_USBDriver as first parameter.
 */

#ifndef __CHIPDRIVERS_H__
#define __CHIPDRIVERS_H__ 1

#include "spi_flash.h"

#ifdef __cplusplus
extern "C" {
#endif

/* spi.c */
int spi_aai_write(sFLASH_USBDriver *flash, const uint8_t *buf, unsigned int start, unsigned int len);
int spi_chip_write_256(sFLASH_USBDriver *flash, const uint8_t *buf, unsigned int start, unsigned int len);
int spi_chip_read(sFLASH_USBDriver *flash, uint8_t *buf, unsigned int start, int unsigned len);

/* spi25.c */
int probe_spi_rdid(sFLASH_USBDriver *flash);
int probe_spi_rdid4(sFLASH_USBDriver *flash);
int probe_spi_rems(sFLASH_USBDriver *flash);
int probe_spi_res1(sFLASH_USBDriver *flash);
int probe_spi_res2(sFLASH_USBDriver *flash);
int probe_spi_res3(sFLASH_USBDriver *flash);
int probe_spi_at25f(sFLASH_USBDriver *flash);
int spi_write_enable(sFLASH_USBDriver *flash);
int spi_write_disable(sFLASH_USBDriver *flash);
int spi_block_erase_20(sFLASH_USBDriver *flash, unsigned int addr, unsigned int blocklen);
int spi_block_erase_50(sFLASH_USBDriver *flash, unsigned int addr, unsigned int blocklen);
int spi_block_erase_52(sFLASH_USBDriver *flash, unsigned int addr, unsigned int blocklen);
int spi_block_erase_60(sFLASH_USBDriver *flash, unsigned int addr, unsigned int blocklen);
int spi_block_erase_62(sFLASH_USBDriver *flash, unsigned int addr, unsigned int blocklen);
int spi_block_erase_81(sFLASH_USBDriver *flash, unsigned int addr, unsigned int blocklen);
int spi_block_erase_c4(sFLASH_USBDriver *flash, unsigned int addr, unsigned int blocklen);
int spi_block_erase_c7(sFLASH_USBDriver *flash, unsigned int addr, unsigned int blocklen);
int spi_block_erase_d7(sFLASH_USBDriver *flash, unsigned int addr, unsigned int blocklen);
int spi_block_erase_d8(sFLASH_USBDriver *flash, unsigned int addr, unsigned int blocklen);
int spi_block_erase_db(sFLASH_USBDriver *flash, unsigned int addr, unsigned int blocklen);
erasefunc_t *spi_get_erasefn_from_opcode(uint8_t opcode);
int spi_chip_write_1(sFLASH_USBDriver *flash, const uint8_t *buf, unsigned int start, unsigned int len);
int spi_byte_program(sFLASH_USBDriver *flash, unsigned int addr, uint8_t databyte);
int spi_nbyte_program(sFLASH_USBDriver *flash, unsigned int addr, const uint8_t *bytes, unsigned int len);
int spi_nbyte_read(sFLASH_USBDriver *flash, unsigned int addr, uint8_t *bytes, unsigned int len);
int spi_read_chunked(sFLASH_USBDriver *flash, uint8_t *buf, unsigned int start, unsigned int len, unsigned int chunksize);
int spi_write_chunked(sFLASH_USBDriver *flash, const uint8_t *buf, unsigned int start, unsigned int len, unsigned int chunksize);

/* spi25_statusreg.c */
uint8_t spi_read_status_register(sFLASH_USBDriver *flash);
int spi_write_status_register(sFLASH_USBDriver *flash, int status);
void spi_prettyprint_status_register_bit(uint8_t status, int bit);
int spi_prettyprint_status_register_plain(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_default_welwip(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_bp1_srwd(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_bp2_srwd(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_bp3_srwd(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_bp4_srwd(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_bp2_bpl(sFLASH_USBDriver *flash);
int spi_disable_blockprotect(sFLASH_USBDriver *flash);
int spi_disable_blockprotect_bp1_srwd(sFLASH_USBDriver *flash);
int spi_disable_blockprotect_bp2_srwd(sFLASH_USBDriver *flash);
int spi_disable_blockprotect_bp3_srwd(sFLASH_USBDriver *flash);
int spi_disable_blockprotect_bp4_srwd(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_amic_a25l032(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_at25df(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_at25df_sec(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_at25f(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_at25f512a(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_at25f512b(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_at25f4096(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_at25fs010(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_at25fs040(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_at26df081a(sFLASH_USBDriver *flash);
int spi_disable_blockprotect_at2x_global_unprotect(sFLASH_USBDriver *flash);
int spi_disable_blockprotect_at2x_global_unprotect_sec(sFLASH_USBDriver *flash);
int spi_disable_blockprotect_at25f(sFLASH_USBDriver *flash);
int spi_disable_blockprotect_at25f512a(sFLASH_USBDriver *flash);
int spi_disable_blockprotect_at25f512b(sFLASH_USBDriver *flash);
int spi_disable_blockprotect_at25fs010(sFLASH_USBDriver *flash);
int spi_disable_blockprotect_at25fs040(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_en25s_wp(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_n25q(sFLASH_USBDriver *flash);
int spi_disable_blockprotect_n25q(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_bp2_ep_srwd(sFLASH_USBDriver *flash);
int spi_disable_blockprotect_bp2_ep_srwd(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_sst25(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_sst25vf016(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_sst25vf040b(sFLASH_USBDriver *flash);

/* sfdp.c */
int probe_spi_sfdp(sFLASH_USBDriver *flash);

/* opaque.c */
int probe_opaque(sFLASH_USBDriver *flash);
int read_opaque(sFLASH_USBDriver *flash, uint8_t *buf, unsigned int start, unsigned int len);
int write_opaque(sFLASH_USBDriver *flash, const uint8_t *buf, unsigned int start, unsigned int len);
int erase_opaque(sFLASH_USBDriver *flash, unsigned int blockaddr, unsigned int blocklen);

/* at45db.c */
int probe_spi_at45db(sFLASH_USBDriver *flash);
int spi_prettyprint_status_register_at45db(sFLASH_USBDriver *flash);
int spi_disable_blockprotect_at45db(sFLASH_USBDriver *flash);
int spi_read_at45db(sFLASH_USBDriver *flash, uint8_t *buf, unsigned int start, unsigned int len);
int spi_read_at45db_e8(sFLASH_USBDriver *flash, uint8_t *buf, unsigned int start, unsigned int len);
int spi_write_at45db(sFLASH_USBDriver *flash, const uint8_t *buf, unsigned int start, unsigned int len);
int spi_erase_at45db_page(sFLASH_USBDriver *flash, unsigned int addr, unsigned int blocklen);
int spi_erase_at45db_block(sFLASH_USBDriver *flash, unsigned int addr, unsigned int blocklen);
int spi_erase_at45db_sector(sFLASH_USBDriver *flash, unsigned int addr, unsigned int blocklen);
int spi_erase_at45db_chip(sFLASH_USBDriver *flash, unsigned int addr, unsigned int blocklen);
int spi_erase_at45cs_sector(sFLASH_USBDriver *flash, unsigned int addr, unsigned int blocklen);

#ifdef __cplusplus
}
#endif

#endif /* !__CHIPDRIVERS_H__ */
