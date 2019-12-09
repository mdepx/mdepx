/*-
 * Copyright (c) 2019 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _ARM_NORDICSEMI_NRF9160_UICR_H_
#define _ARM_NORDICSEMI_NRF9160_UICR_H_

#define	UICR_APPROTECT			0x000 /* Access port protection */
#define	UICR_UNUSED0			0x004 /* Reserved */
#define	UICR_UNUSED1			0x008 /* Reserved */
#define	UICR_UNUSED2			0x00C /* Reserved */
#define	UICR_UNUSED3			0x010 /* Reserved */
#define	UICR_XOSC32M			0x014 /* Oscillator control */
#define	UICR_HFXOSRC			0x01C /* HFXO clock source selection */
#define	UICR_HFXOCNT			0x020 /* HFXO startup counter */
#define	UICR_SECUREAPPROTECT		0x02C /* Secure access port protection */
#define	UICR_ERASEPROTECT		0x030 /* Erase protection */
#define	UICR_OTP(n)			(0x108 + (n) * 0x4) /* OTP bits [31+n*32:0+n*32]. */
#define	UICR_KEYSLOT_CONFIG_DEST(n)	(0x400 + (n) * 0x8) /* Destination address where content of the key value registers */
#define	UICR_KEYSLOT_CONFIG_PERM(n)	(0x404 + (n) * 0x8) /* Define permissions for the key slot with ID=n+1. Bits 0-15 and 16-31 can only be written once. */
#define	UICR_KEYSLOT_KEY_VALUE(n, o)	(0x800 + (n) * 0x10 + (o) * 0x4) /* Define bits [31+o*32:0+o*32] of value assigned to KMU key slot ID=n+1 */

struct nrf_uicr_softc {
	size_t base;
};

void nrf_uicr_init(struct nrf_uicr_softc *sc, uint32_t base);

#endif /* !_ARM_NORDICSEMI_NRF9160_UICR_H_ */
