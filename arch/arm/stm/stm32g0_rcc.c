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
#include <arm/stm/stm32g0_rcc.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

#if 0
void
stm32g0_rcc_usartsel_sysclk(struct stm32g0_rcc_softc *sc)
{
	uint32_t reg;

	reg = RD4(sc, RCC_CCIPR);
	WR4(sc, RCC_CCIPR, 1 << 0); /* USART0SEL SYSCLK */
	WR4(sc, RCC_CCIPR, 1 << 2); /* USART2SEL SYSCLK */
	WR4(sc, RCC_CCIPR, reg);
}

void
stm32g0_rcc_i2c1sel_hsi16(struct stm32g0_rcc_softc *sc)
{
	uint32_t reg;

	reg = CCIPR_I2C1SEL_HSI16;
	WR4(sc, RCC_CCIPR, reg);
}
#endif

void
stm32g0_rcc_hse_enable(struct stm32g0_rcc_softc *sc)
{
	uint32_t reg;

	/* Enable High-Speed External (HSE) ocsillator */
	reg = RD4(sc, RCC_CR);
	reg |= CR_HSEON;
	WR4(sc, RCC_CR, reg);

	while ((RD4(sc, RCC_CR) & CR_HSERDY) == 0)
		;
}

int
stm32g0_rcc_lsi_enable(struct stm32g0_rcc_softc *sc)
{
	int timeout;
	int reg;

	reg = RD4(sc, RCC_CSR);
	reg |= CSR_LSION;
	WR4(sc, RCC_CSR, reg);

	timeout = 1000;	/* ~480 is required on stm32l431kc */
	do {
		reg = RD4(sc, RCC_CSR);
		if (reg & CSR_LSIRDY)
			break;
	} while (timeout--);

	if (timeout <= 0) {
		printf("Can't enable LSI clock\n");
		return (-1);
	}

	return (0);
}

#if 0
stm32g0_rcc_lsi(void)
{
	stm32g0_rcc_lsi_enable(sc);

	uint32_t reg;
	WR4(sc, RCC_CFGR, CFGR_SW_LSI);
	do {
		reg = RD4(sc, RCC_CFGR);
		if ((reg & CFGR_SWS_M) == CFGR_SWS_LSI)
			break;
	} while (1);
}
#endif

void
stm32g0_rcc_pll_configure(struct stm32g0_rcc_softc *sc,
    int pllm, int plln, int pllq, int pllp, uint8_t external,
    uint32_t rcc_cfgr)
{

	/* TODO */
}

void
stm32g0_rcc_setup(struct stm32g0_rcc_softc *sc,
    struct rcc_config *cfg)
{

	WR4(sc, RCC_IOPENR, cfg->iopenr);
	WR4(sc, RCC_AHBENR, cfg->ahbenr);
	WR4(sc, RCC_APBENR1, cfg->apbenr1);
	WR4(sc, RCC_APBENR2, cfg->apbenr2);
}

void
stm32g0_rcc_bdcr_setup(struct stm32g0_rcc_softc *sc,
    uint32_t bdcr)
{

	WR4(sc, RCC_BDCR, bdcr);
}

int
stm32g0_rcc_init(struct stm32g0_rcc_softc *sc, uint32_t base)
{

	sc->base = base;

	return (0);
}
