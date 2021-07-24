/*-
 * Copyright (c) 2018 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _ARM_NORDICSEMI_NRF9160_TIMER_H_
#define _ARM_NORDICSEMI_NRF9160_TIMER_H_

#include <sys/callout.h>

#define	TIMER_TASKS_START	0x000	/* Start Timer */
#define	TIMER_TASKS_STOP	0x004	/* Stop Timer */
#define	TIMER_TASKS_COUNT	0x008	/* Increment Timer (Counter mode only) */
#define	TIMER_TASKS_CLEAR	0x00C	/* Clear time */
#define	TIMER_TASKS_SHUTDOWN	0x010	/* Shut down timer. Deprecated */
#define	TIMER_TASKS_CAPTURE(n)	(0x040 + (n) * 0x4)	/* Capture Timer value to CCn register */
#define	TIMER_EVENTS_COMPARE(n)	(0x140 + (n) * 0x4)	/* Compare event on CCn match */
#define	TIMER_SHORTS		0x200	/* Shortcut register */
#define	TIMER_INTENSET		0x304	/* Enable interrupt */
#define	 INTENSET_COMPARE(n)	(1 << ((n) + 16)) /* Enable interrupt for event COMPARE[n] */
#define	TIMER_INTENCLR		0x308	/* Disable interrupt */
#define	TIMER_STATUS		0x400	/* Timer status */
#define	TIMER_MODE		0x504	/* Timer mode selection */
#define	TIMER_BITMODE		0x508	/* Configure the number of bits used by the TIMER */
#define	 BITMODE_16		0	/* 16 bit timer bit width */
#define	 BITMODE_8		1	/* 8 bit timer bit width */
#define	 BITMODE_24		2	/* 24 bit timer bit width */
#define	 BITMODE_32		3	/* 32 bit timer bit width */
#define	TIMER_PRESCALER		0x510	/* Timer prescaler register */
#define	TIMER_CC(n)		(0x540 + (n) * 0x4)	/* Capture/Compare register n */

struct nrf_timer_softc {
	size_t base;
	uint8_t cc_idx;
	struct mi_timer mt;
};

void nrf_timer_init(mdx_device_t dev, uint32_t base, uint32_t frequency);
void nrf_timer_intr(void *arg, int irq);
void nrf_timer_udelay(struct nrf_timer_softc *sc, uint32_t usec);

#endif /* !_ARM_NORDICSEMI_NRF9160_TIMER_H_ */
