/*-
 * Copyright (c) 2019-2024 Ruslan Bukin <br@bsdpad.com>
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

#include <dev/gpio/gpio.h>

#include "nrf_gpio.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

#ifdef MDX_OF
static struct mdx_compat_data nrf_gpio_compat_data[] = {
	{ "nordic,nrf-gpio" },
	{ NULL },
};
#endif

static int
nrf_gpio_set_pin(mdx_device_t dev, int bank, int pin, int val)
{
	struct nrf_gpio_softc *sc;

	sc = mdx_device_get_softc(dev);

	if (val)
		WR4(sc, GPIO_OUTSET, (1 << pin));
	else
		WR4(sc, GPIO_OUTCLR, (1 << pin));

	return (0);
}

static int
nrf_gpio_get_pin(mdx_device_t dev, int bank, int pin)
{
	struct nrf_gpio_softc *sc;

	sc = mdx_device_get_softc(dev);

	if (RD4(sc, GPIO_IN) & (1 << pin))
		return (1);

	return (0);
}

static int
nrf_gpio_pin_configure(mdx_device_t dev, int bank, int pin, int flags)
{
	struct nrf_gpio_softc *sc;

	sc = mdx_device_get_softc(dev);

	if (flags & MDX_GPIO_INPUT)
		WR4(sc, GPIO_DIRCLR, (1 << pin));
	else
		WR4(sc, GPIO_DIRSET, (1 << pin));

	return (0);
}

void
nrf_gpio_pincfg(mdx_device_t dev, int pin, int cfg)
{
	struct nrf_gpio_softc *sc;

	sc = mdx_device_get_softc(dev);

	WR4(sc, GPIO_PIN_CNF(pin), cfg);
}

void
nrf_gpio_toggle(mdx_device_t dev, int pin)
{
	struct nrf_gpio_softc *sc;
	int reg;

	sc = mdx_device_get_softc(dev);

	reg = RD4(sc, GPIO_OUT);
	if (reg & (1 << pin))
		WR4(sc, GPIO_OUTCLR, (1 << pin));
	else
		WR4(sc, GPIO_OUTSET, (1 << pin));
}

static struct mdx_gpio_ops nrf_gpio_gpio_ops = {
	.pin_set = nrf_gpio_set_pin,
	.pin_get = nrf_gpio_get_pin,
	.pin_configure = nrf_gpio_pin_configure,
};

void
nrf_gpio_init(mdx_device_t dev, uint32_t base)
{
	struct nrf_gpio_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(*sc));
	sc->base = base;

	dev->ops = &nrf_gpio_gpio_ops;
}

#ifdef MDX_OF
static int
nrf_gpio_probe(mdx_device_t dev)
{

	if (!mdx_of_is_compatible(dev, "nordic,nrf-gpio"))
		return (MDX_ERROR);

	return (MDX_OK);
}

static int
nrf_gpio_attach(mdx_device_t dev)
{
	struct nrf_gpio_softc *sc;
	int error;

	sc = mdx_device_get_softc(dev);

	error = mdx_of_get_reg(dev, 0, &sc->base, NULL);
	if (error)
		return (error);

	dev->ops = &nrf_gpio_gpio_ops;

	return (0);
}

static struct mdx_device_ops nrf_gpio_ops = {
	.probe = nrf_gpio_probe,
	.attach = nrf_gpio_attach,
};

static mdx_driver_t nrf_gpio_driver = {
	"nrf_gpio",
	&nrf_gpio_ops,
	sizeof(struct nrf_gpio_softc),
	nrf_gpio_compat_data,
};

DRIVER_MODULE(nrf_gpio, nrf_gpio_driver);
#endif
