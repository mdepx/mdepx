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
#include <arm/stm/stm32f4_gpio.h>

#define	RD4(_sc, _reg)		*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
pin_set(struct stm32f4_gpio_softc *sc, uint32_t port,
    uint32_t pin, uint32_t enable)
{
	uint32_t reg;

	if (enable)
		reg = (1 << pin);
	else
		reg = (1 << (pin + 16));

	WR4(sc, GPIO_BSSR(port), reg);
}

void
pin_configure(struct stm32f4_gpio_softc *sc, const struct gpio_pin *pins)
{
	const struct gpio_pin *cfg;
	uint32_t afr;
	uint32_t reg;
	int i;

	for (i = 0; pins[i].pin != -1; i++) {
		cfg = &pins[i];

		reg = RD4(sc, GPIO_MODER(cfg->port));
		reg &= ~(0x3 << (cfg->pin * 2));
		reg |= (cfg->mode << (cfg->pin * 2));
		WR4(sc, GPIO_MODER(cfg->port), reg);

		if (cfg->mode == MODE_ALT) {
			if (cfg->pin > 7)
				afr = GPIO_AFRH(cfg->port);
			else
				afr = GPIO_AFRL(cfg->port);

			reg = RD4(sc, afr);
			reg &= ~(0xf << ((cfg->pin % 8) * 4));
			reg |= (cfg->alt << ((cfg->pin % 8) * 4));
			WR4(sc, afr, reg);
		}

		reg = RD4(sc, GPIO_OSPEEDR(cfg->port));
		reg &= ~(3 << (cfg->pin * 2));
		reg |= (3 << (cfg->pin * 2));
		WR4(sc, GPIO_OSPEEDR(cfg->port), reg);

		reg = RD4(sc, GPIO_PUPDR(cfg->port));
		reg &= ~(3 << (cfg->pin * 2));
		reg |= (cfg->pupdr << (cfg->pin * 2));
		WR4(sc, GPIO_PUPDR(cfg->port), reg);
	}
}

int
stm32f4_gpio_init(struct stm32f4_gpio_softc *sc, uint32_t base,
    uint32_t cpu_freq, uint32_t baud_rate)
{

	sc->base = base;

	return (0);
}
