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
#include <sys/of.h>
#include <sys/systm.h>
#include <sys/time.h>

#include "nrf_rtc.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

#ifdef MDX_OF
static struct mdx_compat_data nrf_rtc_compat_data[] = {
	{ "nordic,nrf-rtc" },
	{ NULL },
};
#endif

void
nrf_rtc_intr(void *arg, int irq)
{
	struct nrf_rtc_softc *sc;

	sc = arg;

	WR4(sc, RTC_EVENTS_OVRFLW, 0);

	sc->ts.tv_sec += sc->period;
}

static int
nrf_rtc_settime(const struct timespec *tp, void *arg)
{
	struct nrf_rtc_softc *sc;

	sc = arg;

	sc->ts.tv_sec = tp->tv_sec;
	sc->ts.tv_nsec = tp->tv_nsec;

	WR4(sc, RTC_TASKS_STOP, 1);
	WR4(sc, RTC_TASKS_CLEAR, 1);
	WR4(sc, RTC_PRESCALER, sc->prescaler);
	WR4(sc, RTC_INTENSET, INTENSET_OVRFLW);
	WR4(sc, RTC_TASKS_START, 1);

	return (0);
}

static int
nrf_rtc_gettime(struct timespec *tp, void *arg)
{
	struct nrf_rtc_softc *sc;
	int cnt;
	time_t sec;
	long nsec;

	sc = arg;

again:
	critical_enter();

	/* A counter overflow could happen here, but interrupts are disabled */

	cnt = RD4(sc, RTC_COUNTER);

	if (RD4(sc, RTC_EVENTS_OVRFLW)) {
		critical_exit();

		/* An overflow interrupt must arise here. */

		goto again;
	}

	sec = cnt / sc->freq;
	nsec = (1000000000 / sc->freq) * (cnt % sc->freq);

	tp->tv_nsec = sc->ts.tv_nsec + nsec;
	tp->tv_sec = sc->ts.tv_sec + sec;

	critical_exit();

	if (tp->tv_nsec >= 1000000000) {
		tp->tv_nsec -= 1000000000;
		tp->tv_sec += 1;
	}

	return (0);
}

struct rtc_driver nrf_rtc_methods = {
	.settime = nrf_rtc_settime,
	.gettime = nrf_rtc_gettime,
};

void
nrf_rtc_init(mdx_device_t dev, uint32_t base, uint16_t prescaler)
{
	struct nrf_rtc_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(*sc));

	sc->base = base;
	sc->prescaler = prescaler;
	sc->freq = (32768 / (sc->prescaler + 1));
	sc->period = 0x1000000 / sc->freq;
}

#ifdef MDX_OF
static int
nrf_rtc_probe(mdx_device_t dev)
{

	if (!mdx_of_is_compatible(dev, "nordic,nrf-rtc"))
		return (MDX_ERROR);

	return (MDX_OK);
}

static int
nrf_rtc_attach(mdx_device_t dev)
{
	struct nrf_rtc_softc *sc;
	int error;

	sc = mdx_device_get_softc(dev);

	error = mdx_of_get_reg(dev, 0, &sc->base, NULL);
	if (error)
		return (error);

	mdx_of_setup_intr(dev, 0, nrf_rtc_intr, sc);

	/* TODO: grab from FDT */
	sc->prescaler = 1;
	sc->freq = (32768 / (sc->prescaler + 1));
	sc->period = 0x1000000 / sc->freq;

	mdx_clock_register(CLOCK_REALTIME, &nrf_rtc_methods, sc);

	return (0);
}

static struct mdx_device_ops nrf_rtc_ops = {
	.probe = nrf_rtc_probe,
	.attach = nrf_rtc_attach,
};

static mdx_driver_t nrf_rtc_driver = {
	"nrf_rtc",
	&nrf_rtc_ops,
	sizeof(struct nrf_rtc_softc),
	nrf_rtc_compat_data,
};

DRIVER_MODULE(nrf_rtc, nrf_rtc_driver);
#endif
