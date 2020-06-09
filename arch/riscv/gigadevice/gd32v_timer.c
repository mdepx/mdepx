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

#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/callout.h>

#include "gd32v_timer.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
gd32v_timer_intr(void *arg, int irq)
{
	struct gd32v_timer_softc *sc;
	mdx_device_t dev;
	uint32_t reg;

	dev = arg;

	sc = mdx_device_get_softc(dev);

	reg = RD4(sc, TIMER_DMAINTEN);
	reg &= ~DMAINTEN_CH0IE;
	WR4(sc, TIMER_DMAINTEN, reg);

	reg = RD4(sc, TIMER_INTF);
	reg &= ~INTF_CH0IF;
	WR4(sc, TIMER_INTF, reg);

	mdx_callout_callback(&sc->mt);
}

static void
gd32v_timer_start(void *arg, uint32_t ticks)
{
	struct gd32v_timer_softc *sc;
	uint32_t reg;
	uint16_t val;

	sc = arg;

	val = RD4(sc, TIMER_CNT);
	val += ticks;
	WR4(sc, TIMER_CH0CV, val);

	reg = RD4(sc, TIMER_CHCTL2);
	reg |= CHCTL2_CH0EN;
	WR4(sc, TIMER_CHCTL2, reg);

	reg = RD4(sc, TIMER_DMAINTEN);
	reg |= DMAINTEN_CH0IE;
	WR4(sc, TIMER_DMAINTEN, reg);

	reg = RD4(sc, TIMER_CTL0);
	reg |= CTL0_CEN;
	WR4(sc, TIMER_CTL0, reg);
}

static void
gd32v_timer_stop(void *arg)
{
	struct gd32v_timer_softc *sc;
	uint32_t reg;

	sc = arg;

	reg = RD4(sc, TIMER_CTL0);
	reg &= ~CTL0_CEN;
	WR4(sc, TIMER_CTL0, reg);
}

static uint32_t
gd32v_timer_count(void *arg)
{
	struct gd32v_timer_softc *sc;
	uint32_t reg;

	sc = arg;

	reg = RD4(sc, TIMER_CNT);

	return (reg);
}

int
gd32v_timer_init(mdx_device_t dev, uint32_t base, int freq)
{
	struct gd32v_timer_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(struct gd32v_timer_softc));
	sc->base = base;

	WR4(sc, TIMER_CTL0, 0);
	WR4(sc, TIMER_PSC, (freq / 10000) - 1);
	WR4(sc, TIMER_CAR, 0xffff);

	sc->mt.start = gd32v_timer_start;
	sc->mt.stop = gd32v_timer_stop;
	sc->mt.count = gd32v_timer_count;
	sc->mt.maxcnt = 0x0000ffff;
	sc->mt.frequency = 10000;
	sc->mt.usec_to_ticks = mdx_time_usec_to_ticks;
	sc->mt.arg = sc;
	mdx_callout_register(&sc->mt);

	return (0);
}
