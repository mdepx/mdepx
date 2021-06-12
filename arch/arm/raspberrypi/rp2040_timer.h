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

#ifndef	_RP2040_TIMER_H_
#define	_RP2040_TIMER_H_

#include <sys/callout.h>

#define	RP2040_TIMER_TIMEHW	0x00
#define	RP2040_TIMER_TIMELW	0x04
#define	RP2040_TIMER_TIMEHR	0x08
#define	RP2040_TIMER_TIMELR	0x0c
#define	RP2040_TIMER_ALARM0	0x10
#define	RP2040_TIMER_ALARM1	0x14
#define	RP2040_TIMER_ALARM2	0x18
#define	RP2040_TIMER_ALARM3	0x1c
#define	RP2040_TIMER_ARMED	0x20
#define	RP2040_TIMER_TIMERAWH	0x24
#define	RP2040_TIMER_TIMERAWL	0x28
#define	RP2040_TIMER_DBGPAUSE	0x2c
#define	RP2040_TIMER_PAUSE	0x30
#define	RP2040_TIMER_INTR	0x34
#define	RP2040_TIMER_INTE	0x38
#define	RP2040_TIMER_INTF	0x3c
#define	RP2040_TIMER_INTS	0x40

struct rp2040_timer_softc {
	uint32_t base;
	uint32_t freq;
	struct mi_timer mt;
};

int rp2040_timer_init(struct rp2040_timer_softc *sc,
    uint32_t base, uint32_t freq);
void rp2040_timer_intr(void *arg, int irq);
void rp2040_timer_udelay(struct rp2040_timer_softc *sc, uint32_t ticks);

#endif /* !_RP2040_TIMER_H_ */
