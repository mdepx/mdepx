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
#include <sys/systm.h>
#include <arm/stm/stm32l4_exti.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
stm32l4_exti_intr(void *arg, int irq)
{
	struct stm32l4_exti_softc *sc;
	uint32_t reg;
	int i;

	sc = arg;

	reg = RD4(sc, EXTI_PR(0));
	for (i = 0; i < 32; i++)
		if (reg & (1 << i))
			if (sc->map[i].handler != NULL)
				sc->map[i].handler(sc->map[i].arg);
	WR4(sc, EXTI_PR(0), reg);

	reg = RD4(sc, EXTI_PR(32));
	for (i = 0; i < 7; i++)
		if (reg & (1 << i))
			if (sc->map[32 + i].handler != NULL)
				sc->map[32 + i].handler(sc->map[32 + i].arg);
	WR4(sc, EXTI_PR(32), reg);
}

void
stm32l4_exti_install_intr_map(struct stm32l4_exti_softc *sc,
    const struct exti_intr_entry *map)
{

	sc->map = map;
}

void
stm32l4_exti_setup(struct stm32l4_exti_softc *sc, uint32_t n)
{
	uint32_t reg;

	reg = RD4(sc, EXTI_IMR(n));
	reg |= (1 << n);
	WR4(sc, EXTI_IMR(n), reg);

	reg = RD4(sc, EXTI_EMR(n));
	reg |= (1 << n);
	WR4(sc, EXTI_EMR(n), reg);

	reg = RD4(sc, EXTI_RTSR(n));
	reg |= (1 << n);
	WR4(sc, EXTI_RTSR(n), reg);

	reg = RD4(sc, EXTI_FTSR(n));
	reg |= (1 << n);
	WR4(sc, EXTI_FTSR(n), reg);
}

void
stm32l4_exti_init(struct stm32l4_exti_softc *sc, uint32_t base)
{

	sc->base = base;
}
