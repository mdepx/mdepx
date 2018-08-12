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

#include "max32660_gpio.h"

#define	RD4(_sc, _reg)		*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
max32660_gpio_mode(struct max32660_gpio_softc *sc,
    uint8_t port, uint8_t pin, enum pin_mode mode)
{
	uint32_t reg;

	reg = RD4(sc, GPIO_AF1_SEL);
	switch (mode) {
	case MODE_IN:
	case MODE_OUT:
	case MODE_AF1:
		reg &= ~(1 << pin);
		break;
	case MODE_AF2:
	case MODE_AF3:
		reg |= (1 << pin);
		break;
	default:
		break;
	}
	WR4(sc, GPIO_AF1_SEL, reg);

	reg = RD4(sc, GPIO_AF0_SEL);
	switch (mode) {
	case MODE_IN:
	case MODE_OUT:
	case MODE_AF3:
		reg |= (1 << pin);
		break;
	case MODE_AF1:
	case MODE_AF2:
		reg &= ~(1 << pin);
		break;
	default:
		break;
	}
	WR4(sc, GPIO_AF0_SEL, reg);

	reg = RD4(sc, GPIO_OUT_EN);
	switch (mode) {
	case MODE_OUT:
		reg |= (1 << pin);
		break;
	default:
		break;
	}
	WR4(sc, GPIO_OUT_EN, reg);
}

void
max32660_gpio_out(struct max32660_gpio_softc *sc,
    uint8_t port, uint8_t pin, uint8_t val)
{
	uint32_t reg;

	reg = RD4(sc, GPIO_OUT);
	if (val)
		reg |= (1 << pin);
	else
		reg &= ~(1 << pin);
	WR4(sc, GPIO_OUT, reg);
}

void
max32660_gpio_init(struct max32660_gpio_softc *sc,
    uint32_t base)
{

	sc->base = base;
}
