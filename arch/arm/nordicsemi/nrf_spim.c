/*-
 * Copyright (c) 2025 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/sem.h>

#include "nrf9160.h"
#include "nrf_spim.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

#define	dprintf(fmt, ...)

#ifdef MDX_OF
static struct mdx_compat_data nrf_spim_compat_data[] = {
	{ "nordic,nrf-spim" },
	{ NULL },
};
#endif

static mdx_sem_t transmit_sem;

static void
nrf_spim_intr(void *arg, int irq)
{
	struct nrf_spim_softc *sc;
	int events_end;

	sc = arg;

	events_end = RD4(sc, SPIM_EVENTS_END);
	if (events_end)
		WR4(sc, SPIM_EVENTS_END, 0);

	dprintf("%s: events_end %d\n", __func__,
	    RD4(sc, SPIM_EVENTS_END));
	dprintf("%s: events_endrx %d\n", __func__,
	    RD4(sc, SPIM_EVENTS_ENDRX));
	dprintf("%s: events_acquired %d\n", __func__,
	    RD4(sc, SPIM_EVENTS_ACQUIRED));
	dprintf("%s: nrecv %d, ntrans %d\n", __func__,
	    RD4(sc, SPIM_RXD_AMOUNT), RD4(sc, SPIM_TXD_AMOUNT));

	mdx_sem_post(&transmit_sem);
}

void
nrf_spim_init(mdx_device_t dev, uint32_t base)
{
	struct nrf_spim_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(*sc));
	sc->base = base;
}

void
nrf_spim_configure(mdx_device_t dev, int pin_sck, int pin_miso, int pin_mosi)
{
	struct nrf_spim_softc *sc;

	sc = mdx_device_get_softc(dev);

	WR4(sc, SPIM_PSELSCK, pin_sck);
	WR4(sc, SPIM_PSELMISO, pin_miso);
	WR4(sc, SPIM_PSELMOSI, pin_mosi);
	WR4(sc, SPIM_INTENSET, SPIM_INTENSET_END);
	WR4(sc, SPIM_EVENTS_END, 0);
}

int
nrf_spim_transmit(mdx_device_t dev, void *tx_buf, void *rx_buf, uint32_t len)
{
	struct nrf_spim_softc *sc;

	sc = mdx_device_get_softc(dev);

	dprintf("%s: len %d\n", __func__, len);

	WR4(sc, SPIM_TXD_PTR, (uint32_t)tx_buf);
	WR4(sc, SPIM_RXD_PTR, (uint32_t)rx_buf);
	WR4(sc, SPIM_TXD_MAXCNT, len);
	WR4(sc, SPIM_RXD_MAXCNT, len);
	WR4(sc, SPIM_ENABLE, SPIM_ENABLE_EN);

	mdx_sem_init(&transmit_sem, 0);

	WR4(sc, SPIM_TASKS_START, 1);

	mdx_sem_wait(&transmit_sem);

	dprintf("%s: len %d transmitted\n", __func__, len);

	return (0);
}

#ifdef MDX_OF
static int
nrf_spim_probe(mdx_device_t dev)
{

	if (!mdx_of_is_compatible(dev, "nordic,nrf-spim"))
		return (MDX_ERROR);

	return (MDX_OK);
}

static int
nrf_spim_attach(mdx_device_t dev)
{
	struct nrf_spim_softc *sc;
	int error;

	sc = mdx_device_get_softc(dev);

	error = mdx_of_get_reg(dev, 0, &sc->base, NULL);
	if (error)
		return (error);

	mdx_of_setup_intr(dev, 0, nrf_spim_intr, sc);

	return (0);
}

static struct mdx_device_ops nrf_spim_ops = {
	.probe = nrf_spim_probe,
	.attach = nrf_spim_attach,
};

static mdx_driver_t nrf_spim_driver = {
	"nrf_spim",
	&nrf_spim_ops,
	sizeof(struct nrf_spim_softc),
	nrf_spim_compat_data,
};

DRIVER_MODULE(nrf_spim, nrf_spim_driver);
#endif
