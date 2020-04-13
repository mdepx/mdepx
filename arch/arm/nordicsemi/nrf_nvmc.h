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
#define	NVMC_READYNEXT		0x408	/* Ready flag */
#define	NVMC_CONFIG		0x504	/* Configuration register */
#define	NVMC_ERASEALL		0x50C	/* Erase all non-volatile user memory*/
#define	NVMC_ERASEPAGEPARTIALCFG 0x51C	/* Partial erase configuration */
#define	NVMC_ICACHECNF		0x540	/* I-code cache configuration */
#define	NVMC_IHIT		0x548	/* I-code cache hit counter */
#define	NVMC_IMISS		0x54C	/* I-code cache miss counter */
#define	NVMC_CONFIGNS		0x584
#define	NVMC_WRITEUICRNS	0x588	/* Non-secure APPROTECT enable */

struct nrf_nvmc_softc {
	size_t base;
};

#endif /* !_ARM_NORDICSEMI_NRF_NVMC_H_ */
