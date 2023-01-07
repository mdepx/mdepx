/*-
 * Copyright (c) 2023 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/thread.h>

#include <dev/gpio/gpio.h>

#include "rp2040_sio.h"
#include "rp2040_gpio.h"

#define	RP2040_GPIO_DEBUG
#undef	RP2040_GPIO_DEBUG

#ifdef	RP2040_GPIO_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static int
rp2040_gpio_set_pin(mdx_device_t dev, int pin, int val)
{
	struct rp2040_gpio_softc *sc;

	sc = mdx_device_get_softc(dev);

	if (val)
		WR4(sc, RP2040_SIO_GPIO_OUT_SET, (1 << pin));
	else
		WR4(sc, RP2040_SIO_GPIO_OUT_CLR, (1 << pin));

        return (0);
}

static int
rp2040_gpio_get_pin(mdx_device_t dev, int pin)
{
	struct rp2040_gpio_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	reg = RD4(sc, RP2040_SIO_GPIO_IN);
	if (reg & (1 << pin))
		return (1);

        return (0);
}

static int
rp2040_gpio_pin_configure(mdx_device_t dev, int pin, int flags)
{
	struct rp2040_gpio_softc *sc;

	sc = mdx_device_get_softc(dev);

	if (flags & MDX_GPIO_INPUT)
		WR4(sc, RP2040_SIO_GPIO_OE_CLR, (1 << pin));
	else
		WR4(sc, RP2040_SIO_GPIO_OE_SET, (1 << pin));

	return (0);
}

static struct mdx_gpio_ops rp2040_gpio_ops = {
	.pin_set = rp2040_gpio_set_pin,
	.pin_get = rp2040_gpio_get_pin,
	.pin_configure = rp2040_gpio_pin_configure,
};

void
rp2040_gpio_init(mdx_device_t dev, uint32_t base)
{
	struct rp2040_gpio_softc *sc;

	sc = mdx_device_get_softc(dev);
	sc->base = base;

	dev->ops = &rp2040_gpio_ops;
}
