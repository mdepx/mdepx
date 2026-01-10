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

#include <arm/stm/stm32n6_rcc.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

/* Test setup */

void
stm32n6_rcc_pll1(struct stm32n6_rcc_softc *sc)
{
	uint32_t reg;

	/* PLL1 800 MHz (64 x 25 / 2) */
	reg = PLL1CFGR1_PLL1SEL_HSI_CK;
	reg |= 2 << PLL1CFGR1_PLL1DIVM_S | 25 << PLL1CFGR1_PLL1DIVN_S;
	WR4(sc, RCC_PLL1CFGR1, reg);

	/* PLL2 1000 Mhz */
	reg = PLL2CFGR1_PLL2SEL_HSI_CK;
	reg |= 8 << PLL2CFGR1_PLL2DIVM_S | 125 << PLL2CFGR1_PLL2DIVN_S;
	WR4(sc, RCC_PLL2CFGR1, reg);

	WR4(sc, RCC_CR, CR_LSI_ON | CR_HSI_ON | CR_HSE_ON | CR_PLL1_ON |
	    CR_PLL2_ON);

	/* TODO: poll for PLL ready bit. */
	udelay(10000);

	/* DCMIPP 333.3 MHz */
	reg = ICxCFGR_ICxSEL_PLL2 | ((3 - 1) << ICxCFGR_ICxINT_S);
	WR4(sc, RCC_IC17CFGR, reg);

	/* CSI 20 MHz */
	reg = ICxCFGR_ICxSEL_PLL1 | ((40 - 1) << ICxCFGR_ICxINT_S);
	WR4(sc, RCC_IC18CFGR, reg);

	/* CPU 800 MHz */
	reg = ICxCFGR_ICxSEL_PLL1 | ((1 - 1) << ICxCFGR_ICxINT_S);
	WR4(sc, RCC_IC1CFGR, reg);

	/* SYS buses 400 MHz */
	reg = ICxCFGR_ICxSEL_PLL1 | ((2 - 1) << ICxCFGR_ICxINT_S);
	WR4(sc, RCC_IC2CFGR, reg);

	/* XSPI1 200 MHz */
	reg = ICxCFGR_ICxSEL_PLL1 | ((4 - 1) << ICxCFGR_ICxINT_S);
	WR4(sc, RCC_IC3CFGR, reg);

	/* NPU 1000 MHz */
	reg = ICxCFGR_ICxSEL_PLL2 | ((1 - 1) << ICxCFGR_ICxINT_S);
	WR4(sc, RCC_IC6CFGR, reg);

	/* SRAM3456 800 MHz */
	/* TODO: move to PLL3 for 900 MHz */
	reg = ICxCFGR_ICxSEL_PLL1 | ((1 - 1) << ICxCFGR_ICxINT_S);
	WR4(sc, RCC_IC11CFGR, reg);

	/* LTDC 25 MHz typ. */
	reg = ICxCFGR_ICxSEL_PLL1 | ((32 - 1) << ICxCFGR_ICxINT_S);
	WR4(sc, RCC_IC16CFGR, reg);

	WR4(sc, RCC_CCIPR1, CCRPR1_DCMIPPSEL_IC17_CK);
	WR4(sc, RCC_CCIPR4, CCIPR4_LTDCSEL_IC16_CK);
	WR4(sc, RCC_CCIPR6, 2); //xspi1 IC3
	WR4(sc, RCC_MISCENR, 1 << 3);

	reg = 0;
	reg |= DIVENSR_ICxENS(17); /* dcmipp */
	reg |= DIVENSR_ICxENS(18); /* csi */
	reg |= DIVENSR_ICxENS(1); /* cpu */
	reg |= DIVENSR_ICxENS(2); /* sysb (ahb apb ...) */
	reg |= DIVENSR_ICxENS(3); /* xspi1 */
	reg |= DIVENSR_ICxENS(6); /* npu */
	reg |= DIVENSR_ICxENS(11); /* sram3456 */
	reg |= DIVENSR_ICxENS(16); /* ltdc */
	WR4(sc, RCC_DIVENSR, reg);

	reg = 0;
	reg |= 1 << CFGR2_HPRE_S;	/* div 2 so H sysbuses 200 MHz */
	reg |= 1 << CFGR2_TIMPRE_S;	/* div 2 so timers 200 MHz */
	reg |= 0 << CFGR2_PPRE5_S;	/* div 1 apb5 */
	reg |= 0 << CFGR2_PPRE4_S;	/* div 1 apb4 */
	reg |= 0 << CFGR2_PPRE2_S;	/* div 1 apb2 */
	reg |= 0 << CFGR2_PPRE1_S;	/* div 1 apb1 */
	WR4(sc, RCC_CFGR2, reg);

	/* Switch CPU and Sys Buses to the new clocks. */

	reg = RD4(sc, RCC_CFGR1);
	reg |= CFGR1_CPUSW_IC1_CK;
	reg |= CFGR1_SYSSW_IC2_CK;
	WR4(sc, RCC_CFGR1, reg);
}

void
stm32n6_rcc_setup(struct stm32n6_rcc_softc *sc, struct rcc_config *cfg)
{
	uint32_t reg;

	/* Note this are Set registers (not write) */

	WR4(sc, RCC_AHB1ENSR, cfg->ahb1enr);
	WR4(sc, RCC_AHB2ENSR, cfg->ahb2enr);
	WR4(sc, RCC_AHB3ENSR, cfg->ahb3enr);
	WR4(sc, RCC_AHB4ENSR, cfg->ahb4enr);
	WR4(sc, RCC_AHB5ENSR, cfg->ahb5enr);
	WR4(sc, RCC_APB1LENSR, cfg->apb1lenr);
	WR4(sc, RCC_APB1HENSR, cfg->apb1henr);
	WR4(sc, RCC_APB2ENSR, cfg->apb2enr);
	WR4(sc, RCC_APB3ENSR, cfg->apb3enr);
	WR4(sc, RCC_APB4LENSR, cfg->apb4lenr);
	WR4(sc, RCC_APB4HENSR, cfg->apb4henr);
	WR4(sc, RCC_APB5ENSR, cfg->apb5enr);
	WR4(sc, RCC_MEMENSR, cfg->memenr);

	/* Reset XSPI1/2/M. */
	reg = AHB5RSTR_XSPI1RST | AHB5RSTR_XSPI2RST | AHB5RSTR_XSPIMRST;
	WR4(sc, RCC_AHB5RSTR, reg);
	udelay(10);
	WR4(sc, RCC_AHB5RSTR, 0);

	/* Reset display/camera IP. */
	reg = APB5RSTR_LTDCRST | APB5RSTR_DCMIPPRST | APB5RSTR_CSIRST;
	WR4(sc, RCC_APB5RSTR, reg);
	udelay(10);
	WR4(sc, RCC_APB5RSTR, 0);
}

int
stm32n6_rcc_init(struct stm32n6_rcc_softc *sc, uint32_t base)
{

	sc->base = base;

	return (0);
}
