/*-
 * Copyright (c) 2024 Ruslan Bukin <br@bsdpad.com>
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

#include <dev/gpio/gpio.h>

#include <riscv/artinchip/d21x_gpio.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static int
d21x_gpio_pin_configure(mdx_device_t dev, int bank, int pin, int flags)
{
	struct d21x_gpio_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	reg = RD4(sc, GPIO_PIN_CFG(bank, pin));
	if (flags & MDX_GPIO_INPUT) {
		reg |= PIN_CFG_GEN_IE;
		reg &= ~PIN_CFG_GEN_OE;
	} else {
		reg &= ~PIN_CFG_GEN_IE;
		reg |= PIN_CFG_GEN_OE;
	}
	WR4(sc, GPIO_PIN_CFG(bank, pin), reg);

	reg = RD4(sc, GPIO_GEN_OUT_CFG(bank));
	if (flags & MDX_GPIO_INPUT)
		reg &= ~(1 << pin);
	else
		reg |= (1 << pin);
	WR4(sc, GPIO_GEN_OUT_CFG(bank), reg);

	return (0);
}

static int
d21x_gpio_set_pin(mdx_device_t dev, int bank, int pin, int val)
{
	struct d21x_gpio_softc *sc;

	sc = mdx_device_get_softc(dev);

	if (val)
		WR4(sc, GPIO_GEN_OUT_SET(bank), (1 << pin));
	else
		WR4(sc, GPIO_GEN_OUT_CLR(bank), (1 << pin));

	return (0);
}

static int
d21x_gpio_get_pin(mdx_device_t dev, int bank, int pin)
{
	struct d21x_gpio_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	reg = RD4(sc, GPIO_GEN_IN_STA(bank));
	if (reg & (1 << pin))
		return (1);

	return (0);
}

static int
d21x_gpio_pin_set_function(mdx_device_t dev, int bank, int pin, int function)
{
	struct d21x_gpio_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	reg = RD4(sc, GPIO_PIN_CFG(bank, pin));
	reg &= ~(PIN_CFG_PIN_FUN_M);
	reg |= (function << PIN_CFG_PIN_FUN_S);
	WR4(sc, GPIO_PIN_CFG(bank, pin), reg);

	return (0);
}

static int
d21x_gpio_pin_set_dir(mdx_device_t dev, int bank, int pin, int dir)
{

	return (0);
}

static struct mdx_gpio_ops d21x_gpio_ops = {
	.pin_set = d21x_gpio_set_pin,
	.pin_get = d21x_gpio_get_pin,
	.pin_configure = d21x_gpio_pin_configure,
	.pin_set_function = d21x_gpio_pin_set_function,
	.pin_set_dir = d21x_gpio_pin_set_dir,
};

void
d21x_gpio_init(mdx_device_t dev, uint32_t base)
{
	struct d21x_gpio_softc *sc;

	sc = mdx_device_get_softc(dev);
	sc->base = base;

	dev->ops = (void *)&d21x_gpio_ops;
}
