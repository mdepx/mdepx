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
#include <arm/stm/stm32f4_flash.h>
#include <arm/stm/stm32f4_pwr.h>
#include <arm/stm/stm32f4_rcc.h>

#define	RD4(_sc, _reg)		*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
stm32f4_rcc_pllsai(struct stm32f4_rcc_softc *sc)
{
	uint32_t reg;

	reg = (192 << 6) | (7 << 24) | (4 << 28);
	WR4(sc, RCC_PLLSAICFGR, reg);

	reg = (0x2 << DCKCFGR_PLLSAIDIVR_S);
	WR4(sc, RCC_DCKCFGR, reg);

	reg = RD4(sc, RCC_CR);
	reg |= CR_PLLSAION;
	WR4(sc, RCC_CR, reg);
	while ((RD4(sc, RCC_CR) & CR_PLLSAIRDY) == 0)
		;
}

void
stm32f4_rcc_pll_configure(struct stm32f4_rcc_softc *sc,
    int pllm, int plln, int pllq, int pllp, uint8_t external,
    uint32_t rcc_cfgr)
{
	uint32_t reg;

	if (external) {
		/* Use external ocsillator */
		reg = RD4(sc, RCC_CR);
		reg |= CR_HSEON;
		WR4(sc, RCC_CR, reg);
		while ((RD4(sc, RCC_CR) & CR_HSERDY) == 0)
			;
	} else {
		reg = RD4(sc, RCC_CR);
		reg |= (CR_HSION);
		WR4(sc, RCC_CR, reg);
		while ((RD4(sc, RCC_CR) & CR_HSIRDY) == 0)
			;
	}

	WR4(sc, RCC_CFGR, rcc_cfgr);

	reg = (pllm << PLLCFGR_PLLM_S);
	reg |= (plln << PLLCFGR_PLLN_S);
	reg |= (pllq << PLLCFGR_PLLQ_S);
	reg |= (pllp << PLLCFGR_PLLP_S);
	if (external)
		reg |= PLLCFGR_PLLSRC_HSE;
	WR4(sc, RCC_PLLCFGR, reg);

	reg = RD4(sc, RCC_CR);
	reg |= CR_PLLON;
	WR4(sc, RCC_CR, reg);
	while ((RD4(sc, RCC_CR) & CR_PLLRDY) == 0)
		;
}

int
stm32f4_rcc_setup(struct stm32f4_rcc_softc *sc, uint32_t ahb1enr,
    uint32_t ahb2enr, uint32_t ahb3enr, uint32_t apb1enr,
    uint32_t apb2enr)
{
	uint32_t reg;

	reg = RD4(sc, RCC_CFGR);
	reg &= ~(CFGR_SW_M);
	reg |= CFGR_SW_PLLP;
	WR4(sc, RCC_CFGR, reg);

	while ((RD4(sc, RCC_CFGR) & CFGR_SWS_M) != CFGR_SWS_PLL)
		;

	WR4(sc, RCC_AHB1ENR, ahb1enr);
	WR4(sc, RCC_AHB2ENR, ahb2enr);
	WR4(sc, RCC_AHB3ENR, ahb3enr);
	WR4(sc, RCC_APB1ENR, apb1enr);
	WR4(sc, RCC_APB2ENR, apb2enr);

	return (0);
}

int
stm32f4_rcc_init(struct stm32f4_rcc_softc *sc, uint32_t base)
{

	sc->base = base;

	return (0);
}
