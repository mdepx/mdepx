/*-
 * Copyright (c) 2024 Ruslan Bukin <br@bsdpad.com>
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

#ifndef	_ARM_STM_STM32MP_SDMMC_H_
#define	_ARM_STM_STM32MP_SDMMC_H_

#define	SDMMC_POWER		0x000
#define	SDMMC_CLKCR		0x004
#define	SDMMC_ARGR		0x008
#define	SDMMC_CMDR		0x00C
#define	SDMMC_RESPCMDR		0x010
#define	SDMMC_RESP1R		0x014
#define	SDMMC_RESP2R		0x018
#define	SDMMC_RESP3R		0x01C
#define	SDMMC_RESP4R		0x020
#define	SDMMC_DTIMER		0x024
#define	SDMMC_DLENR		0x028
#define	SDMMC_DCTRLR		0x02C
#define	SDMMC_DCNTR		0x030
#define	SDMMC_STAR		0x034
#define	SDMMC_ICR		0x038
#define	SDMMC_MASKR		0x03C
#define	SDMMC_ACKTIMER		0x040
#define	SDMMC_FIFOTHRR		0x044
#define	SDMMC_IDMACTRLR		0x050
#define	SDMMC_IDMABSIZER	0x054
#define	SDMMC_IDMABASER		0x058
#define	SDMMC_IDMALAR		0x064
#define	SDMMC_IDMABAR		0x068
#define	SDMMC_FIFOR(x)		(0x080 + 0x04 * (x))
#define	SDMMC_HWCFGR1		0x3F0
#define	SDMMC_VERR		0x3F4
#define	SDMMC_IPIDR		0x3F8
#define	SDMMC_SIDR		0x3FC

struct stm32mp_sdmmc_softc {
	uint32_t base;
};

#endif	/* !_ARM_STM_STM32MP_SDMMC_H_ */
