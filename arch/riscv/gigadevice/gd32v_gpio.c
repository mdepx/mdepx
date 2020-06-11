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

#include <dev/gpio/gpio.h>

#include "gd32v_gpio.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static int
gd32v_gpio_pin_configure(mdx_device_t dev, int bank, int pin, int flags)
{
	struct gd32v_gpio_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	KASSERT(bank == 0, ("Bank 0 supported only."));

	reg = RD4(sc, GPIO_CTL(pin));
	reg &= ~CTL_MD_M(pin);
	reg &= ~CTL_CTL_M(pin);

	if (flags & MDX_GPIO_OUTPUT) {
		if (flags & MDX_GPIO_SPEED_LOW)
			reg |= CTL_MD_OUT_2MHZ(pin);
		else if (flags & MDX_GPIO_SPEED_MEDIUM)
			reg |= CTL_MD_OUT_10MHZ(pin);
		else if (flags & MDX_GPIO_SPEED_HIGH)
			reg |= CTL_MD_OUT_50MHZ(pin);

		if (flags & MDX_GPIO_ALT_FUNC) {
			if (flags & MDX_GPIO_PUSH_PULL)
				reg |= CTL_CTL_AFIO_PUSHPULL(pin);
			else if (flags & MDX_GPIO_OPEN_DRAIN)
				reg |= CTL_CTL_AFIO_OPENDRAIN(pin);
		} else {
			if (flags & MDX_GPIO_PUSH_PULL)
				reg |= CTL_CTL_GPIO_PUSHPULL(pin);
			else if (flags & MDX_GPIO_OPEN_DRAIN)
				reg |= CTL_CTL_GPIO_OPENDRAIN(pin);
		}
	} else {
		reg |= CTL_MD_INP(pin);
		if (flags & MDX_GPIO_FLOATING)
			reg |= CTL_CTL_INP_FLOATING(pin);
		else if (flags & MDX_GPIO_PULL_UP ||
		    flags & MDX_GPIO_PULL_DOWN)
			reg |= CTL_CTL_INP_PULLUPDOWN(pin);

		if (flags & MDX_GPIO_PULL_UP)
			WR4(sc, GPIO_BOP, BOP_BOP(pin));
		else if (flags & MDX_GPIO_PULL_DOWN)
			WR4(sc, GPIO_BOP, BOP_CR(pin));
	}

	WR4(sc, GPIO_CTL(pin), reg);

	return (0);
}

static int
gd32v_gpio_pin_set(mdx_device_t dev, int bank, int pin, int val)
{
	struct gd32v_gpio_softc *sc;

	sc = mdx_device_get_softc(dev);

	if (val)
		WR4(sc, GPIO_BOP, BOP_BOP(pin));
	else
		WR4(sc, GPIO_BOP, BOP_CR(pin));

	return (0);
}

static int
gd32v_gpio_pin_get(mdx_device_t dev, int bank, int pin)
{

	/* TODO */

	return (0);
}

static struct mdx_gpio_ops gd32v_gpio_ops = {
	.pin_set = gd32v_gpio_pin_set,
	.pin_get = gd32v_gpio_pin_get,
	.pin_configure = gd32v_gpio_pin_configure,
};

int
gd32v_gpio_init(mdx_device_t dev, uint32_t base)
{
	struct gd32v_gpio_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(struct gd32v_gpio_softc));
	sc->base = base;

	dev->ops = (void *)&gd32v_gpio_ops;

	return (0);
}
