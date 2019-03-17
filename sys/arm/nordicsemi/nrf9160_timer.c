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

#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/callout.h>

#include <machine/frame.h>

#include "nrf9160.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

#define	NRF_TIMER_DEBUG
#undef	NRF_TIMER_DEBUG

#ifdef	NRF_TIMER_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

void
timer_intr(void *arg, struct trapframe *tf, int irq)
{
	struct timer_softc *sc;

	sc = arg;

	dprintf("%s\n", __func__);

	WR4(sc, TIMER_EVENTS_COMPARE(sc->cc_idx), 0);

	callout_callback(&sc->mt);
}

static uint32_t
timer_count(void *arg)
{
	struct timer_softc *sc;
	uint32_t count;

	sc = arg;

	WR4(sc, TIMER_TASKS_CAPTURE(1), 1);
	count = RD4(sc, TIMER_CC(1));

	dprintf("%s: count %d\n", __func__, count);

	return (count);
}

static void
timer_stop(void *arg)
{
	struct timer_softc *sc;

	dprintf("%s\n", __func__);

	sc = arg;

	WR4(sc, TIMER_TASKS_STOP, 1);
	WR4(sc, TIMER_EVENTS_COMPARE(sc->cc_idx), 0);
}

static void
timer_start(void *arg, uint32_t ticks)
{
	struct timer_softc *sc;
	uint32_t reg;

	dprintf("%s: ticks %d\n", __func__, ticks);

	sc = arg;

	if (ticks == 1)
		ticks++;
	WR4(sc, TIMER_TASKS_CAPTURE(3), 1);
	reg = RD4(sc, TIMER_CC(3));
	WR4(sc, TIMER_CC(sc->cc_idx), reg + ticks);
	WR4(sc, TIMER_TASKS_START, 1);
}

void
timer_init(struct timer_softc *sc, uint32_t base)
{

	sc->base = base;
	sc->cc_idx = 0;

	WR4(sc, TIMER_BITMODE, BITMODE_32);
	WR4(sc, TIMER_INTENSET, INTENSET_COMPARE(sc->cc_idx));

	sc->mt.start = timer_start;
	sc->mt.stop = timer_stop;
	sc->mt.count = timer_count;
	sc->mt.arg = sc;
	callout_register(&sc->mt);
}
