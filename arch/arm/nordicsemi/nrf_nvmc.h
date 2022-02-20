/*-
 * Copyright (c) 2020 Ruslan Bukin <br@bsdpad.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _ARM_NORDICSEMI_NRF_NVMC_H_
#define _ARM_NORDICSEMI_NRF_NVMC_H_

#define	NVMC_READY		0x400	/* Ready flag */
#define	 NVMC_READY_READY	(1 << 0) /* NVMC is ready */
#define	NVMC_READYNEXT		0x408	/* Ready flag */
#define	NVMC_CONFIG		0x504	/* Configuration register */
#define	 CONFIG_WEN_S		0
#define	 CONFIG_WEN_M		(0x7 << CONFIG_WEN_S)
#define	 CONFIG_REN		(0 << CONFIG_WEN_S) /* Read only access */
#define	 CONFIG_WEN		(1 << CONFIG_WEN_S) /* Write enabled */
#define	 CONFIG_EEN		(2 << CONFIG_WEN_S) /* Erase enabled */
#define	 CONFIG_PEEN		(4 << CONFIG_WEN_S) /* Partial erase enabled*/
#define	NVMC_ERASEALL		0x50C	/* Erase all non-volatile user memory*/
#define	NVMC_ERASEPAGEPARTIALCFG 0x51C	/* Partial erase configuration */
#define	 ERASEPAGE_DURATION_S	0	/* Duration in miliseconds */
#define	 ERASEPAGE_DURATION_M	(0x7f << ERASEPAGE_DURATION_S)
#define	NVMC_ICACHECNF		0x540	/* I-code cache configuration */
#define	 ICACHECNF_CACHEEN	(1 << 0) /* Enable cache */
#define	 ICACHECNF_CACHEPROFEN	(1 << 8) /* Enable cache profiling */
#define	NVMC_IHIT		0x548	/* I-code cache hit counter */
#define	NVMC_IMISS		0x54C	/* I-code cache miss counter */
#define	NVMC_CONFIGNS		0x584
#define	 CONFIGNS_WEN_S		0
#define	 CONFIGNS_WEN_M		(0x3 << CONFIGNS_WEN_S)
#define	 CONFIGNS_REN		(0 << CONFIGNS_WEN_S) /* Read only access */
#define	 CONFIGNS_WEN		(1 << CONFIGNS_WEN_S) /* Write enabled */
#define	 CONFIGNS_EEN		(2 << CONFIGNS_WEN_S) /* Erase enabled */
#define	NVMC_WRITEUICRNS	0x588	/* Non-secure APPROTECT enable */
#define	 WRITEUICRNS_SET	(1 << 0) /* Allow NS code to set APPROTECT */
#define	 WRITEUICRNS_KEY_S	4 /* Key to write in order to validate the write operation */
#define	 WRITEUICRNS_KEY	(0xAFBE5A7 << WRITEUICRNS_KEY_S)

struct nrf_nvmc_softc {
	size_t base;
};

void nrf_nvmc_init(mdx_device_t dev, uint32_t base);
void nrf_nvmc_icache_control(mdx_device_t dev, bool enable);

#endif /* !_ARM_NORDICSEMI_NRF_NVMC_H_ */
