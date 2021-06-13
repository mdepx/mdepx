/*-
 * Copyright (c) 2021 Ruslan Bukin <br@bsdpad.com>
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

#ifndef	_RP2040_XOSC_H_
#define	_RP2040_XOSC_H_

#define	RP2040_XOSC_CTRL		0x00
#define	 XOSC_CTRL_FREQ_RANGE_S		0
#define	 XOSC_CTRL_FREQ_RANGE_M		(0xfff << XOSC_CTRL_FREQ_RANGE_S)
#define	 XOSC_CTRL_FREQ_1_15MHZ		(0xaa0 << XOSC_CTRL_FREQ_RANGE_S)
#define	 XOSC_CTRL_ENABLE_S		12
#define	 XOSC_CTRL_ENABLE_M		(0xfff << XOSC_CTRL_ENABLE_S)
#define	 XOSC_CTRL_ENABLE_ENABLE	(0xfab << XOSC_CTRL_ENABLE_S)
#define	 XOSC_CTRL_ENABLE_DISABLE	(0xd1e << XOSC_CTRL_ENABLE_S)
#define	RP2040_XOSC_STATUS		0x04
#define	 XOSC_STATUS_STABLE		(1 << 31)
#define	RP2040_XOSC_DORMANT		0x08
#define	RP2040_XOSC_STARTUP		0x0c
#define	 XOSC_STARTUP_DELAY_S		0
#define	 XOSC_STARTUP_DELAY_M		(0x3fff << XOSC_STARTUP_DELAY_S)
#define	RP2040_XOSC_COUNT		0x1c

struct rp2040_xosc_softc {
	uint32_t base;
};

int rp2040_xosc_init(struct rp2040_xosc_softc *sc, uint32_t base);

#endif /* !_RP2040_XOSC_H_ */
