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

enum {
	HWTYPE_NONE,
	HWTYPE_STM32F4,
	HWTYPE_STM32F7,
};

struct pllcfgr_s {
	int pllm;
	int plln;
	int pllq;
};

static void
rcc_reset(struct stm32f4_rcc_softc *sc)
{
	uint32_t reg;

/* Enable the Internal High Speed clock (HSI) */

	reg = RD4(sc, RCC_CR);
	reg |= (HSION);
	WR4(sc, RCC_CR, reg);

/* Reset CFGR register */

	WR4(sc, RCC_CFGR, 0);

/* Reset HSION, HSEON, CSSON and PLLON bits */

	reg = RD4(sc, RCC_CR);
	reg &= ~(HSION | HSEON | CSSON | PLLON);
	WR4(sc, RCC_CR, reg);

/* Reset PLLCFGR register to reset default */

	//WR4(sc, RCC_PLLCFGR, 0x24003010);

/* Reset HSEBYP bit */

	reg = RD4(sc, RCC_CR);
	reg &= ~(HSEBYP);
	WR4(sc, RCC_CR, reg);

/* Disable all interrupts */

	WR4(sc, RCC_CIR, 0);
}

int
stm32f4_rcc_setup(struct stm32f4_rcc_softc *sc, struct stm32f4_pwr_softc *pwr_sc,
    struct stm32f4_flash_softc *flash_sc)
{
	struct pllcfgr_s pllcfgr;
	uint32_t reg;

	rcc_reset(sc);

	//reg = RD4(sc, RCC_CR);
	//reg |= (HSION);
	//WR4(sc, RCC_CR, reg);
	//while ((RD4(sc, RCC_CR) & HSIRDY) == 0)
	//	;

	/* Use external ocsillator */
	reg = RD4(sc, RCC_CR);
	reg |= HSEON;
	WR4(sc, RCC_CR, reg);
	while ((RD4(sc, RCC_CR) & HSERDY) == 0)
		;

	reg = RD4(sc, RCC_APB1ENR);
	reg |= (PWREN);
	WR4(sc, RCC_APB1ENR, reg);

	stm32f4_vos_setup(pwr_sc);

	reg = RD4(sc, RCC_CFGR);
	reg |= (4 << 13); //PPRE2 div by 2
	reg |= (5 << 10); //PPRE1 div by 4
	//reg |= (6 << 10); //PPRE1 div by 8
	//reg |= (7 << 10); //PPRE1 div by 16
	reg &= ~(0xf << 4); // AHB prescaler
	//reg |= (10 << 4); /* div by 8 */
	//reg |= (8 << 4); /* div by 2 */
	WR4(sc, RCC_CFGR, reg);

	pllcfgr.pllm = 0;
	pllcfgr.plln = 0;
	pllcfgr.pllq = 0;

	switch (sc->hwtype) {
	case HWTYPE_STM32F4:
		pllcfgr.pllm = 8;
		pllcfgr.plln = 336;
		pllcfgr.pllq = 7;
		break;
	case HWTYPE_STM32F7:
		pllcfgr.pllm = 25;
		pllcfgr.plln = 432;
		pllcfgr.pllq = 9;
		break;
	default:
		printf("unknown pll");
	};

	reg = (pllcfgr.pllm << 0); //PLLM
	reg |= (pllcfgr.plln << 6); //PLLN
	reg |= (0 << 16); //PLLP = 2
	reg |= (pllcfgr.pllq << 24); //PLLQ
	reg |= (1 << 22); //PLLSRC - HSE
	WR4(sc, RCC_PLLCFGR, reg);

	reg = RD4(sc, RCC_CR);
	reg |= PLLON;
	WR4(sc, RCC_CR, reg);
	while ((RD4(sc, RCC_CR) & PLLRDY) == 0)
		;

	/* SAI LCD */
	switch(sc->hwtype) {
	case HWTYPE_STM32F4:
		reg = (192 << 6) | (7 << 24) | (4 << 28);
		WR4(sc, RCC_PLLSAICFGR, reg);

		reg = (0x2 << PLLSAIDIVR_S);
		WR4(sc, RCC_DCKCFGR, reg);

		break;
	case HWTYPE_STM32F7:
		reg = (192 << 6) | (4 << 24) | (5 << 28);
		WR4(sc, RCC_PLLSAICFGR, reg);

#if 1
#define	 PLLI2SN_M	(0x1ff << PLLI2SN_S)
#define	 PLLI2SQ_M	(0xf << PLLI2SQ_S)

		/* (25000000/25)*429/(2*19) = 11289473 hz I2S */

		reg = RD4(sc, RCC_PLLI2SCFGR);
		reg &= ~(PLLI2SN_M);
		reg = (429 << PLLI2SN_S);
		reg &= ~(PLLI2SQ_M);
		reg |= (2 << PLLI2SQ_S);
		WR4(sc, RCC_PLLI2SCFGR, reg);

		reg = (0x1 << PLLSAIDIVR_S);
		reg |= (SAI2SEL_PLLI2S << SAI2SEL_S);
		reg |= (8 << PLLI2SDIV_S);
		WR4(sc, RCC_DCKCFGR, reg);
#endif

		break;
	default:
		printf("unknown hwtype\n");
		return (-1);
	}

	if (sc->hwtype == HWTYPE_STM32F7) {
		//PCLK2
		reg = (0 << USART6SEL_S);
		reg |= (0 << I2C3SEL_S);
		reg |= (SDMMCSEL_PLL48 << SDMMCSEL_S);
		WR4(sc, RCC_DCKCFGR2, reg);
	}

	reg = RD4(sc, RCC_CR);
	reg |= PLLSAION;
	WR4(sc, RCC_CR, reg);
	while ((RD4(sc, RCC_CR) & PLLSAIRDY) == 0)
		;

#if 1
	reg = RD4(sc, RCC_CR);
	reg |= CR_PLLI2SON;
	WR4(sc, RCC_CR, reg);
	while ((RD4(sc, RCC_CR) & CR_PLLI2SRDY) == 0)
		;
#endif

	stm32f4_pwr_setup(pwr_sc);
	stm32f4_flash_setup(flash_sc);

	reg = RD4(sc, RCC_CFGR);
	reg &= ~(3 << 0);
	reg |= (2 << 0); //PLL used as the system clock
	WR4(sc, RCC_CFGR, reg);

	while (((RD4(sc, RCC_CFGR) >> 2) & 2) != 2)
		;

	/* AHB */
	reg = RD4(sc, RCC_AHB1ENR);
	reg |= (RCCAEN | RCCBEN | RCCCEN);
	reg |= (RCCDEN | RCCEEN | RCCFEN);
	reg |= (RCCGEN | RCCHEN | RCCIEN);
	reg |= (RCCJEN | RCCKEN);
	reg |= (OTGHSEN);
	reg |= (DMA1EN | DMA2EN);
	reg |= (ETHMACRXEN | ETHMACTXEN | ETHMACEN);
	//reg |= (ETHMACPTPEN);
	WR4(sc, RCC_AHB1ENR, reg);

	reg = RD4(sc, RCC_AHB2ENR);
	reg |= (OTGFSEN);
	WR4(sc, RCC_AHB2ENR, reg);

	reg = RD4(sc, RCC_AHB3ENR);
	reg |= (FMCEN);
	WR4(sc, RCC_AHB3ENR, reg);

	/* APB */
	reg = RD4(sc, RCC_APB1ENR);
	reg |= (TIM2EN);
	reg |= (I2C3EN);
	reg |= (SPI2EN);
	WR4(sc, RCC_APB1ENR, reg);

	reg = RD4(sc, RCC_APB2ENR);
	reg |= (USART1EN);
	reg |= (USART6EN);
	reg |= (SDMMC1EN);
	reg |= (TIM1EN);
	reg |= (SPI5EN);
	reg |= (LTDCEN);
	reg |= (DSIEN);
	reg |= (SAI2EN);
	reg |= (SYSCFGEN);
	WR4(sc, RCC_APB2ENR, reg);

	/* Reset I2C3 */
	reg = RD4(sc, RCC_APB1RSTR);
	reg |= (I2C3RST);
	reg |= (SPI2RST);
	WR4(sc, RCC_APB1RSTR, reg);

	reg = RD4(sc, RCC_APB1RSTR);
	reg &= ~(I2C3RST);
	reg &= ~(SPI2RST);
	WR4(sc, RCC_APB1RSTR, reg);

	return (0);
}

int
stm32f4_rcc_init(struct stm32f4_rcc_softc *sc, uint32_t base)
{

	sc->base = base;
	sc->hwtype = HWTYPE_STM32F4;

	return (0);
}
