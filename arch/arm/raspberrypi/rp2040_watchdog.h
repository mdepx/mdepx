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

#ifndef	_RP2040_WATCHDOG_H_
#define	_RP2040_WATCHDOG_H_

#define	RP2040_WATCHDOG_CTRL		0x00
#define	RP2040_WATCHDOG_LOAD		0x04
#define	RP2040_WATCHDOG_REASON		0x08
#define	RP2040_WATCHDOG_SCRATCH0	0x0c
#define	RP2040_WATCHDOG_SCRATCH1	0x10
#define	RP2040_WATCHDOG_SCRATCH2	0x14
#define	RP2040_WATCHDOG_SCRATCH3	0x18
#define	RP2040_WATCHDOG_SCRATCH4	0x1c
#define	RP2040_WATCHDOG_SCRATCH5	0x20
#define	RP2040_WATCHDOG_SCRATCH6	0x24
#define	RP2040_WATCHDOG_SCRATCH7	0x28
#define	RP2040_WATCHDOG_TICK		0x2c
#define	 WATCHDOG_TICK_ENABLE		(1 << 9)

struct rp2040_watchdog_softc {
	uint32_t base;
};

int rp2040_watchdog_init(struct rp2040_watchdog_softc *sc, uint32_t base);

#endif /* !_RP2040_WATCHDOG_H_ */
