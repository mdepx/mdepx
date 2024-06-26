/*-
 * Copyright (c) 2023-2024 Ruslan Bukin <br@bsdpad.com>
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
	*(volatile uint32_t *)((_sc)->sio_base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->sio_base + _reg) = _val

#define	PADS_RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->pads_base + _reg)
#define	PADS_WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->pads_base + _reg) = _val

static int
rp2040_gpio_set_pin(mdx_device_t dev, int bank, int pin, int val)
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
rp2040_gpio_get_pin(mdx_device_t dev, int bank, int pin)
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
rp2040_gpio_pin_set_dir(mdx_device_t dev, int bank, int pin, int dir)
{
	struct rp2040_gpio_softc *sc;

	sc = mdx_device_get_softc(dev);

	if (dir)
		WR4(sc, RP2040_SIO_GPIO_OE_SET, (1 << pin));
	else
		WR4(sc, RP2040_SIO_GPIO_OE_CLR, (1 << pin));

	return (0);
}

static int
rp2040_gpio_pin_configure(mdx_device_t dev, int bank, int pin, int flags)
{
	struct rp2040_gpio_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	reg = PADS_RD4(sc, RP2040_PADS_BANK0_GPIO_OFFSET(pin));

	/*
	 * Note that on RP2040 setting both pulls enables a "bus keep"
	 * function, i.e. weak pull to whatever is current high/low state
	 * of GPIO.
	 */

	if (flags & MDX_GPIO_PULL_UP)
		reg |= RP2040_PADS_BANK0_GPIO_PUE;
	else
		reg &= ~RP2040_PADS_BANK0_GPIO_PUE;

	if (flags & MDX_GPIO_PULL_DOWN)
		reg |= RP2040_PADS_BANK0_GPIO_PDE;
	else
		reg &= ~RP2040_PADS_BANK0_GPIO_PDE;

	if (flags & MDX_GPIO_SPEED_HIGH) {
		reg &= ~RP2040_PADS_BANK0_GPIO_DRIVE_MASK;
		reg |= RP2040_PADS_BANK0_GPIO_DRIVE_12MA;
	}

	if (flags & MDX_GPIO_SLEW_FAST)
		reg |= RP2040_PADS_BANK0_GPIO_SLEWFAST;
	else
		reg &= ~RP2040_PADS_BANK0_GPIO_SLEWFAST;

	if (flags & MDX_GPIO_HYSTERESIS_EN)
		reg |= RP2040_PADS_BANK0_GPIO_SCHMITT;
	else
		reg &= ~RP2040_PADS_BANK0_GPIO_SCHMITT;

	PADS_WR4(sc, RP2040_PADS_BANK0_GPIO_OFFSET(pin), reg);

	return (0);
}

static int
rp2040_gpio_pin_set_function(mdx_device_t dev, int bank, int pin, int func)
{
	struct rp2040_gpio_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	reg = PADS_RD4(sc, RP2040_PADS_BANK0_GPIO_OFFSET(pin));
	reg &= ~RP2040_PADS_BANK0_GPIO_OD;
	reg |= RP2040_PADS_BANK0_GPIO_IE;
	PADS_WR4(sc, RP2040_PADS_BANK0_GPIO_OFFSET(pin), reg);

	/* The func itself set in iobank0. */

	return (0);
}

static struct mdx_gpio_ops rp2040_gpio_ops = {
	.pin_set = rp2040_gpio_set_pin,
	.pin_get = rp2040_gpio_get_pin,
	.pin_configure = rp2040_gpio_pin_configure,
	.pin_set_function = rp2040_gpio_pin_set_function,
	.pin_set_dir = rp2040_gpio_pin_set_dir,
};

void
rp2040_gpio_init(mdx_device_t dev, uint32_t sio_base, uint32_t pads_base)
{
	struct rp2040_gpio_softc *sc;

	sc = mdx_device_get_softc(dev);
	sc->sio_base = sio_base;
	sc->pads_base = pads_base;

	dev->ops = &rp2040_gpio_ops;
}
