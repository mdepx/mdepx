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
#include <sys/of.h>
#include <sys/callout.h>

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

#ifdef MDX_OF
static struct mdx_compat_data nrf_timer_compat_data[] = {
	{ "nordic,nrf-timer" },
	{ NULL },
};
#endif

void
nrf_timer_intr(void *arg, int irq)
{
	struct nrf_timer_softc *sc;

	sc = arg;

	dprintf("%s\n", __func__);

	WR4(sc, TIMER_EVENTS_COMPARE(sc->cc_idx), 0);

	mdx_callout_callback(&sc->mt);
}

static uint32_t
nrf_timer_count(void *arg)
{
	struct nrf_timer_softc *sc;
	uint32_t count;

	sc = arg;

	WR4(sc, TIMER_TASKS_CAPTURE(1), 1);
	count = RD4(sc, TIMER_CC(1));

	dprintf("%s: count %d\n", __func__, count);

	return (count);
}

void
nrf_timer_udelay(struct nrf_timer_softc *sc, uint32_t usec)
{
	uint32_t first, last;
	uint32_t delta;
	uint32_t counts;
	uint32_t v;

	v = 1000000 / sc->mt.frequency;

	counts = 0;
	first = nrf_timer_count(sc);
	while (counts < usec) {
		last = nrf_timer_count(sc);
		if (last > first)
			delta = last - first;
		else
			delta = last + (0xffffffff - first) + 1;

		counts += (delta * v);

		first = last;
	}
}

static void
nrf_timer_stop(void *arg)
{
	struct nrf_timer_softc *sc;

	dprintf("%s\n", __func__);

	sc = arg;

	WR4(sc, TIMER_TASKS_STOP, 1);
	WR4(sc, TIMER_EVENTS_COMPARE(sc->cc_idx), 0);
}

static void
nrf_timer_start(void *arg, uint32_t ticks)
{
	struct nrf_timer_softc *sc;
	uint32_t reg;

	dprintf("%s: ticks %d\n", __func__, ticks);

	sc = arg;

	if (ticks < 3)
		ticks = 3;
	WR4(sc, TIMER_TASKS_CAPTURE(3), 1);
	reg = RD4(sc, TIMER_CC(3));
	WR4(sc, TIMER_CC(sc->cc_idx), reg + ticks);
	WR4(sc, TIMER_TASKS_START, 1);
}

void
nrf_timer_init(mdx_device_t dev, uint32_t base, uint32_t frequency)
{
	struct nrf_timer_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(*sc));
	sc->base = base;
	sc->cc_idx = 0;

	WR4(sc, TIMER_BITMODE, BITMODE_32);
	WR4(sc, TIMER_INTENSET, INTENSET_COMPARE(sc->cc_idx));

	bzero(&sc->mt, sizeof(struct mi_timer));
	sc->mt.start = nrf_timer_start;
	sc->mt.stop = nrf_timer_stop;
	sc->mt.count = nrf_timer_count;
	sc->mt.maxcnt = 0xffffffff;
	sc->mt.frequency = frequency;
	sc->mt.arg = sc;
	mdx_callout_register(&sc->mt);
}

#ifdef MDX_OF
static int
nrf_timer_probe(mdx_device_t dev)
{

	if (!mdx_of_is_compatible(dev, "nordic,nrf-timer"))
		return (MDX_ERROR);

	return (MDX_OK);
}

static int
nrf_timer_attach(mdx_device_t dev)
{
	struct nrf_timer_softc *sc;
	int error;

	sc = mdx_device_get_softc(dev);

	error = mdx_of_get_reg(dev, 0, &sc->base, NULL);
	if (error)
		return (error);

	sc->cc_idx = 0;

	WR4(sc, TIMER_BITMODE, BITMODE_32);
	WR4(sc, TIMER_INTENSET, INTENSET_COMPARE(sc->cc_idx));

	mdx_of_setup_intr(dev, 0, nrf_timer_intr, sc);

	bzero(&sc->mt, sizeof(struct mi_timer));
	sc->mt.start = nrf_timer_start;
	sc->mt.stop = nrf_timer_stop;
	sc->mt.count = nrf_timer_count;
	sc->mt.maxcnt = 0xffffffff;
	sc->mt.frequency = 1000000;
	sc->mt.arg = sc;
	mdx_callout_register(&sc->mt);

	return (0);
}

static struct mdx_device_ops nrf_timer_ops = {
	.probe = nrf_timer_probe,
	.attach = nrf_timer_attach,
};

static mdx_driver_t nrf_timer_driver = {
	"nrf_timer",
	&nrf_timer_ops,
	sizeof(struct nrf_timer_softc),
	nrf_timer_compat_data,
};

DRIVER_MODULE(nrf_timer, nrf_timer_driver);
#endif
