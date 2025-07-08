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

#include <dev/gpio/gpio.h>

#include "nrf9160.h"
#include "nrf_spis.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

#define	dprintf(fmt, ...)

#ifdef MDX_OF
static struct mdx_compat_data nrf_spis_compat_data[] = {
	{ "nordic,nrf-spis" },
	{ NULL },
};
#endif

static void
nrf_spis_intr(void *arg, int irq)
{
	struct nrf_spis_softc *sc;
	int events_end;

	sc = arg;

	events_end = RD4(sc, SPIS_EVENTS_END);
	if (events_end)
		WR4(sc, SPIS_EVENTS_END, 0);

	dprintf("%s: events_end %d\n", __func__,
	    RD4(sc, SPIS_EVENTS_END));
	dprintf("%s: events_endrx %d\n", __func__,
	    RD4(sc, SPIS_EVENTS_ENDRX));
	dprintf("%s: events_acquired %d\n", __func__,
	    RD4(sc, SPIS_EVENTS_ACQUIRED));
	dprintf("%s: nrecv %d, ntrans %d\n", __func__,
	    RD4(sc, SPIS_RXD_AMOUNT), RD4(sc, SPIS_TXD_AMOUNT));

	sc->slave_rx_len = RD4(sc, SPIS_RXD_AMOUNT);

	printf("%s: arrived %d bytes\n", __func__, sc->slave_rx_len);

	WR4(sc, SPIS_ENABLE, 0);

	mdx_sem_post(&sc->receive_sem);
}

void
nrf_spis_init(mdx_device_t dev, uint32_t base)
{
	struct nrf_spis_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(*sc));
	sc->base = base;
}

void
nrf_spis_configure(mdx_device_t dev, struct spis_conf *conf)
{
	struct nrf_spis_softc *sc;

	sc = mdx_device_get_softc(dev);

	sc->gpio = conf->gpio;
	sc->pin_rdy = conf->pin_rdy;

	WR4(sc, SPIS_PSELSCK, conf->pin_sck);
	WR4(sc, SPIS_PSELMISO, conf->pin_miso);
	WR4(sc, SPIS_PSELMOSI, conf->pin_mosi);
	WR4(sc, SPIS_PSELCSN, conf->pin_csn);
	WR4(sc, SPIS_INTENSET, SPIS_INTENSET_END);
	WR4(sc, SPIS_EVENTS_END, 0);
	WR4(sc, SPIS_SHORTS, SPIS_SHORTS_END_ACQUIRE);
}

int
nrf_spis_receive(mdx_device_t dev, void *tx_buf, void *rx_buf, int len)
{
	struct nrf_spis_softc *sc;

	sc = mdx_device_get_softc(dev);

	WR4(sc, SPIS_TXD_PTR, (uint32_t)tx_buf);
	WR4(sc, SPIS_RXD_PTR, (uint32_t)rx_buf);
	WR4(sc, SPIS_RXD_AMOUNT, 0);
	WR4(sc, SPIS_TXD_AMOUNT, 0);
	WR4(sc, SPIS_TXD_MAXCNT, len);
	WR4(sc, SPIS_RXD_MAXCNT, len);

	WR4(sc, SPIS_ENABLE, SPIS_ENABLE_EN);
	WR4(sc, SPIS_TASKS_RELEASE, 1);

	/* Tell master we are ready. */
	mdx_gpio_set(sc->gpio, 0, sc->pin_rdy, 1);

	mdx_sem_wait(&sc->receive_sem);

	/* We are not ready for the next message. */
	mdx_gpio_set(sc->gpio, 0, sc->pin_rdy, 0);

	return (sc->slave_rx_len);
}

#ifdef MDX_OF
static int
nrf_spis_probe(mdx_device_t dev)
{

	if (!mdx_of_is_compatible(dev, "nordic,nrf-spis"))
		return (MDX_ERROR);

	return (MDX_OK);
}

static int
nrf_spis_attach(mdx_device_t dev)
{
	struct nrf_spis_softc *sc;
	int error;

	sc = mdx_device_get_softc(dev);

	error = mdx_of_get_reg(dev, 0, &sc->base, NULL);
	if (error)
		return (error);

	mdx_of_setup_intr(dev, 0, nrf_spis_intr, sc);

	mdx_sem_init(&sc->receive_sem, 0);

	return (0);
}

static struct mdx_device_ops nrf_spis_ops = {
	.probe = nrf_spis_probe,
	.attach = nrf_spis_attach,
};

static mdx_driver_t nrf_spis_driver = {
	"nrf_spis",
	&nrf_spis_ops,
	sizeof(struct nrf_spis_softc),
	nrf_spis_compat_data,
};

DRIVER_MODULE(nrf_spis, nrf_spis_driver);
#endif
