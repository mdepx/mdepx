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

#include "nrf_gpiote.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

#ifdef MDX_OF
static struct mdx_compat_data nrf_gpiote_compat_data[] = {
	{ "nordic,nrf-gpiote" },
	{ NULL },
};
#endif

void
nrf_gpiote_intr(void *arg, int irq)
{
	struct nrf_gpiote_softc *sc;
	uint32_t reg;
	int i;

	sc = arg;

	for (i = 0; i < NRF_GPIOTE_NCONFIGS; i++) {
		reg = RD4(sc, GPIOTE_EVENTS_IN(i));
		if (reg) {
			if (sc->intr[i].handler != NULL)
				sc->intr[i].handler(sc->intr[i].arg, irq);
			WR4(sc, GPIOTE_EVENTS_IN(i), 0);
		};
	};
}

int
nrf_gpiote_setup_intr(mdx_device_t dev, int irq,
    void (*handler) (void *arg, int irq),
    void *arg)
{
	struct nrf_gpiote_softc *sc;

	sc = mdx_device_get_softc(dev);

	if (irq < 0)
		return (-1);

	if (irq >= NRF_GPIOTE_NCONFIGS)
		return (-1);

	sc->intr[irq].handler = handler;
	sc->intr[irq].arg = arg;

	return (0);
}

void
nrf_gpiote_intctl(mdx_device_t dev, uint8_t config_id, bool enable)
{
	struct nrf_gpiote_softc *sc;

	sc = mdx_device_get_softc(dev);

	if (enable)
		WR4(sc, GPIOTE_INTENSET, (1 << config_id));
	else
		WR4(sc, GPIOTE_INTENCLR, (1 << config_id));
}

void
nrf_gpiote_config(mdx_device_t dev, uint8_t config_id,
    struct nrf_gpiote_conf *conf)
{
	struct nrf_gpiote_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	reg = CONFIG_PSEL(conf->pin);

	switch (conf->pol) {
	case GPIOTE_POLARITY_LOTOHI:
		reg |= CONFIG_POLARITY_LOTOHI;
		break;
	case GPIOTE_POLARITY_HITOLO:
		reg |= CONFIG_POLARITY_HITOLO;
		break;
	case GPIOTE_POLARITY_TOGGLE:
		reg |= CONFIG_POLARITY_TOGGLE;
		break;
	}

	switch (conf->mode) {
	case GPIOTE_MODE_TASK:
		reg |= CONFIG_MODE_TASK;
		break;
	case GPIOTE_MODE_EVENT:
		reg |= CONFIG_MODE_EVENT;
		break;
	}

	WR4(sc, GPIOTE_CONFIG(config_id), reg);
}

void
nrf_gpiote_init(mdx_device_t dev, uint32_t base)
{
	struct nrf_gpiote_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(*sc));
	sc->base = base;
}

#ifdef MDX_OF
static int
nrf_gpiote_probe(mdx_device_t dev)
{

	if (!mdx_of_is_compatible(dev, "nordic,nrf-gpiote"))
		return (MDX_ERROR);

	return (MDX_OK);
}

static int
nrf_gpiote_attach(mdx_device_t dev)
{
	struct nrf_gpiote_softc *sc;
	int error;

	sc = mdx_device_get_softc(dev);

	error = mdx_of_get_reg(dev, 0, &sc->base, NULL);
	if (error)
		return (error);

	mdx_of_setup_intr(dev, 0, nrf_gpiote_intr, sc);

	return (0);
}

static struct mdx_device_ops nrf_gpiote_ops = {
	.probe = nrf_gpiote_probe,
	.attach = nrf_gpiote_attach,
};

static mdx_driver_t nrf_gpiote_driver = {
	"nrf_gpiote",
	&nrf_gpiote_ops,
	sizeof(struct nrf_gpiote_softc),
	nrf_gpiote_compat_data,
};

DRIVER_MODULE(nrf_gpiote, nrf_gpiote_driver);
#endif
