/*-
 * Copyright (c) 2018-2020 Ruslan Bukin <br@bsdpad.com>
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

#include <mips/microchip/pic32_ccp.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
pic32_ccp_intr(void *arg, struct trapframe *frame,
    int mips_irq, int intc_irq)
{
	struct pic32_ccp_softc *sc;

	sc = arg;

	WR4(sc, CCPPR, 0);

	mdx_callout_callback(&sc->mt);
}

void
pic32_ccp_delay(struct pic32_ccp_softc *sc, uint32_t usec)
{
	uint32_t first, last;
	int32_t counts;

	counts = (usec / 8);

	first = RD4(sc, CCPTMR);
	while (counts > 0) {
		last = RD4(sc, CCPTMR);
#if 0
		if (last < first)
			counts -= (0xffffffff - first) + last;
		else
			counts -= (last - first);
#else
		counts -= (int32_t)(last - first);
#endif

		first = last;
	}
}

static void
timer_start(void *arg, uint32_t ticks)
{
	struct pic32_ccp_softc *sc;
	uint32_t reg;
	int counts;

	sc = arg;

	/* Disable the timer. */
	reg = RD4(sc, CCPCON1);
	reg &= ~CCPCON1_ON;
	WR4(sc, CCPCON1, reg);

	WR4(sc, CCPRA, 0);
	WR4(sc, CCPRB, 0);

	/* Enable the timer. */
	counts = RD4(sc, CCPTMR);
	WR4(sc, CCPPR, (ticks + counts));

	reg |= CCPCON1_ON;
	WR4(sc, CCPCON1, reg);
}

static void
timer_stop(void *arg)
{

	/* Don't stop so pic32_ccp_delay() works. */
}

static uint32_t
timer_count(void *arg)
{
	struct pic32_ccp_softc *sc;
	uint32_t counts;

	sc = arg;

	counts = RD4(sc, CCPTMR);

	return (counts);
}

int
pic32_ccp_init(struct pic32_ccp_softc *sc, uint32_t base, uint32_t freq)
{
	uint32_t reg;
	int error;

	sc->base = base;

	WR4(sc, CCPCON1, 0);
	WR4(sc, CCPTMR, 0);

	WR4(sc, CCPRA, 0);
	WR4(sc, CCPRB, 0);

	reg = CCPCON1_T32 | CCPCON1_TMRPS_64;
	reg |= CCPCON1_ON;
	WR4(sc, CCPCON1, reg);

	sc->frequency = freq;
	sc->mt.start = timer_start;
	sc->mt.stop = timer_stop;
	sc->mt.count = timer_count;
	sc->mt.maxcnt = 0xffffffff;
	sc->mt.frequency = freq;
	sc->mt.arg = sc;

	error = mdx_callout_register(&sc->mt);
	if (error != MDX_OK)
		return (error);

	return (MDX_OK);
}
