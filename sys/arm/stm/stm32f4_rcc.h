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

#ifndef _ARM_STM_STM32F4_RCC_H_
#define _ARM_STM_STM32F4_RCC_H_

#define	RCC_CR			0x00
#define	 CR_PLLSAIRDY		(1 << 29)
#define	 CR_PLLSAION		(1 << 28)
#define	 CR_HSEBYP		(1 << 18) /* HSE clock bypass */
#define	 CR_HSION		(1 << 0)
#define	 CR_HSEON		(1 << 16)
#define	 CR_PLLI2SRDY		(1 << 27) /* PLLI2S clock ready flag */
#define	 CR_PLLI2SON		(1 << 26) /* PLLI2S enable */
#define	 CR_PLLON		(1 << 24)
#define	 CR_CSSON		(1 << 19) /* Clock security system enable */
#define	 CR_PLLRDY		(1 << 25)
#define	 CR_HSERDY		(1 << 17) /* HSE clock ready flag */
#define	 CR_HSIRDY		(1 << 1) /* HSI clock ready flag */
#define	RCC_PLLCFGR		0x04
#define	 PLLCFGR_PLLR_S		28
#define	 PLLCFGR_PLLQ_S		24
#define	 PLLCFGR_PLLSRC_HSE	(1 << 22)
#define	 PLLCFGR_PLLP_S		16
#define	 PLLCFGR_PLLN_S		6
#define	 PLLCFGR_PLLM_S		0
#define	RCC_CFGR		0x08
#define	 CFGR_PPRE2_S		13 /* APB high-speed prescaler (APB2) */
#define	 CFGR_PPRE2_M		(0x7 << CFGR_PPRE2_S)
#define	 CFGR_PPRE2_2		(0x4 << CFGR_PPRE2_S)
#define	 CFGR_PPRE2_4		(0x5 << CFGR_PPRE2_S)
#define	 CFGR_PPRE2_8		(0x6 << CFGR_PPRE2_S)
#define	 CFGR_PPRE2_16		(0x7 << CFGR_PPRE2_S)
#define	 CFGR_PPRE1_S		10 /* APB Low speed prescaler (APB1) */
#define	 CFGR_PPRE1_M		(0x7 << CFGR_PPRE1_S)
#define	 CFGR_PPRE1_2		(0x4 << CFGR_PPRE1_S)
#define	 CFGR_PPRE1_4		(0x5 << CFGR_PPRE1_S)
#define	 CFGR_PPRE1_8		(0x6 << CFGR_PPRE1_S)
#define	 CFGR_PPRE1_16		(0x7 << CFGR_PPRE1_S)
#define	 CFGR_HPRE_S		4 /* AHB prescaler */
#define	 CFGR_HPRE_M		(0xf << CFGR_HPRE_S)
#define	 CFGR_SWS_S		2 /* System clock switch status */
#define	 CFGR_SWS_M		(0x3 << CFGR_SWS_S)
#define	 CFGR_SWS_PLL		(0x2 << CFGR_SWS_S)
#define	 CFGR_SWS_HSE		(0x1 << CFGR_SWS_S)
#define	 CFGR_SWS_HSI		(0x0 << CFGR_SWS_S)
#define	 CFGR_SW_S		0 /* System clock switch */
#define	 CFGR_SW_M		(0x3 << CFGR_SW_S)
#define	 CFGR_SW_PLLR		(0x3 << CFGR_SW_S)
#define	 CFGR_SW_PLLP		(0x2 << CFGR_SW_S)
#define	 CFGR_SW_HSE		(0x1 << CFGR_SW_S)
#define	 CFGR_SW_HSI		(0x0 << CFGR_SW_S)
#define	RCC_CIR			0x0C
#define	RCC_AHB1RSTR		0x10
#define	 ETHMACRST		(1 << 25) /* Ethernet MAC reset */
#define	RCC_APB1RSTR		0x20
#define	 I2C3RST		(1 << 23)	/* I2C3 reset */
#define	 SPI2RST		(1 << 14)	/* SPI2 reset */
#define	RCC_AHB1ENR		0x30
#define	 ETHMACPTPEN		(1 << 28) /* Ethernet PTP clock enable */
#define	 ETHMACRXEN		(1 << 27) /* Ethernet Reception clock enable */
#define	 ETHMACTXEN		(1 << 26) /* Ethernet Transmission clock enable */
#define	 ETHMACEN		(1 << 25) /* Ethernet MAC clock enable */
#define	 DMA2EN			(1 << 22)
#define	 DMA1EN			(1 << 21)
#define	 GPIOKEN		(1 << 10)
#define	 GPIOJEN		(1 << 9)
#define	 GPIOIEN		(1 << 8)
#define	 GPIOHEN		(1 << 7)
#define	 GPIOGEN		(1 << 6)
#define	 GPIOFEN		(1 << 5)
#define	 GPIOEEN		(1 << 4)
#define	 GPIODEN		(1 << 3)
#define	 GPIOCEN		(1 << 2)
#define	 GPIOBEN		(1 << 1)
#define	 GPIOAEN		(1 << 0)
#define	 OTGHSULPIEN		(1 << 30) /* USB OTG HSULPI clock enable */
#define	 OTGHSEN		(1 << 29) /* USB OTG HS clock enable */
#define	RCC_AHB2ENR		0x34
#define	 OTGFSEN		(1 << 7) /* USB OTG FS clock enable */
#define	RCC_AHB3ENR		0x38
#define	 FMCEN			(1 << 0) /* FMC module clock enabled */
#define	RCC_APB1ENR		0x40
#define	 PWREN			(1 << 28) /* Power interface clock enable */
#define	 I2C3EN			(1 << 23) /* I2C3 clock enable */
#define	 SPI2EN			(1 << 14) /* SPI2 clock enable */
#define	 TIM2EN			(1 << 0)
#define	RCC_APB2ENR		0x44
#define	 DSIEN			(1 << 27) /* DSI clocks enable */
#define	 LTDCEN			(1 << 26) /* LTDC clock enable */
#define	 SAI2EN			(1 << 23) /* SAI2 clock enable */
#define	 SPI5EN			(1 << 20)
#define	 SYSCFGEN		(1 << 14) /* System configuration controller clock enable */
#define	 SDMMC1EN		(1 << 11)
#define	 USART6EN		(1 << 5)
#define	 USART1EN		(1 << 4)
#define	 TIM1EN			(1 << 0)
#define	RCC_PLLI2SCFGR		0x84
#define	 PLLI2SR_S		28
#define	 PLLI2SQ_S		24
#define	 PLLI2SP_S		16
#define	 PLLI2SN_S		6
#define	RCC_PLLSAICFGR		0x88
#define	 PLLSAICFGR_PLLSAIR_S	28	/* PLLSAI division factor for LCD clock */
#define	 PLLSAICFGR_PLLSAIR_M	(0x7 << PLLSAICFGR_PLLSAIR_S)
#define	 PLLSAICFGR_PLLSAIQ_S	24	/* PLLSAI division factor for SAI clock */
#define	 PLLSAICFGR_PLLSAIQ_M	(0xf << PLLSAICFGR_PLLSAIQ_S)
#define	 PLLSAICFGR_PLLSAIP_S	16	/* PLLSAI division factor for 48MHz clock */
#define	 PLLSAICFGR_PLLSAIP_M	(0x3 << PLLSAICFGR_PLLSAIP_S)
#define	 PLLSAICFGR_PLLSAIN_S	6	/* PLLSAI division factor for VCO */
#define	 PLLSAICFGR_PLLSAIN_M	(0x1ff << PLLSAICFGR_PLLSAIN_S)

#define	RCC_DCKCFGR			0x8C
#define	 DCKCFGR_PLLSAIDIVR_S		16
#define	 DCKCFGR_SAI2SEL_S		22
#define	 DCKCFGR_SAI2SEL_PLLI2S		1
#define	 DCKCFGR_PLLI2SDIV_S		0
#define	RCC_DCKCFGR2			0x90
#define	 DCKCFGR2_I2C3SEL_S		20
#define	 DCKCFGR2_USART6SEL_S		10
#define	 DCKCFGR2_SDMMCSEL_S		28 /* SDMMC clock source selection */
#define	 DCKCFGR2_SDMMCSEL_PLL48	0
#define	 DCKCFGR2_SDMMCSEL_SYS		1

struct stm32f4_rcc_softc {
	uint32_t base;
};

int stm32f4_rcc_init(struct stm32f4_rcc_softc *sc, uint32_t base);
void stm32f4_rcc_pll_configure(struct stm32f4_rcc_softc *sc,
    int pllm, int plln, int pllq, int pllp, uint8_t external, uint32_t rcc_cfgr);
int stm32f4_rcc_setup(struct stm32f4_rcc_softc *sc, uint32_t ahb1enr,  
    uint32_t ahb2enr, uint32_t ahb3enr, uint32_t apb1enr,
    uint32_t apb2enr);
void stm32f4_rcc_pllsai(struct stm32f4_rcc_softc *sc,
    uint32_t sain, uint32_t saiq, uint32_t sair);
void stm32f4_rcc_eth_reset(struct stm32f4_rcc_softc *sc);

#endif /* !_ARM_STM_STM32F4_RCC_H_ */
