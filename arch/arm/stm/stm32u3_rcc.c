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
#include <sys/systm.h>

#include <arm/stm/stm32u3_rcc.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

#define	dprintf(fmt, ...)

void
stm32u3_rcc_setup(struct stm32u3_rcc_softc *sc, struct rcc_config *cfg)
{
	uint32_t reg;

	dprintf("%s: CR %x\n", __func__, RD4(sc, RCC_CR));
	dprintf("%s: CFGR1 %x\n", __func__, RD4(sc, RCC_CFGR1));
	dprintf("%s: CFGR2 %x\n", __func__, RD4(sc, RCC_CFGR2));
	dprintf("%s: ICSCR1 %x\n", __func__, RD4(sc, RCC_ICSCR1));

	/* Switch to HSI. */

	reg = RD4(sc, RCC_CR);
	reg |= CR_HSION;
	WR4(sc, RCC_CR, reg);

	do {
		reg = RD4(sc, RCC_CR);
		if (reg & CR_HSIRDY)
			break;
	} while (1);

	/* Switch to HSI */
	WR4(sc, RCC_CFGR1, 1);

	/* Enable HSI48 for USB. */
	reg = RD4(sc, RCC_CR);
	reg |= CR_HSI48ON;
	WR4(sc, RCC_CR, reg);

	WR4(sc, RCC_AHB1ENR1, cfg->ahb1enr1);
	WR4(sc, RCC_AHB2ENR1, cfg->ahb2enr1);
	WR4(sc, RCC_AHB2ENR2, cfg->ahb2enr2);
	WR4(sc, RCC_AHB1ENR2, cfg->ahb1enr2);

	WR4(sc, RCC_APB1ENR1, cfg->apb1enr1);
	WR4(sc, RCC_APB1ENR2, cfg->apb1enr2);
	WR4(sc, RCC_APB2ENR, cfg->apb2enr);
	WR4(sc, RCC_APB3ENR, cfg->apb3enr);
}

int
stm32u3_rcc_init(struct stm32u3_rcc_softc *sc, uint32_t base)
{

	sc->base = base;

	return (0);
}
