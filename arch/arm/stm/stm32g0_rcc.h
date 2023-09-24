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

#ifndef _ARM_STM_STM32G0_RCC_H_
#define _ARM_STM_STM32G0_RCC_H_

#define	RCC_CR			0x00
#define	 CR_PLLRDY		(1 << 25)
#define	 CR_PLLON		(1 << 24)
#define	 CR_CSSON		(1 << 19) /* Clock security system enable */
#define	 CR_HSEBYP		(1 << 18) /* HSE clock bypass */
#define	 CR_HSERDY		(1 << 17) /* HSE clock ready flag */
#define	 CR_HSEON		(1 << 16)
#define	 CR_HSIRDY		(1 << 10) /* HSI clock ready flag */
#define	 CR_HSIKERON		(1 << 9) /* HSI16 always enable for peripheral kernels. */
#define	 CR_HSION		(1 << 8)
#define	RCC_ICSCR		0x04
#define	RCC_CFGR		0x08
#define	RCC_PLLCFGR		0x0C
#define	 PLLCFGR_PLLR_S		25
#define	 PLLCFGR_PLLQ_S		21
#define	 PLLCFGR_PLLP_S		17
#define	 PLLCFGR_PLLN_S		8
#define	 PLLCFGR_PLLM_S		4
#define	RCC_CIER		0x18
#define	RCC_CIFR		0x1C
#define	RCC_CICR		0x20
#define	RCC_IOPRSTR		0x24
#define	RCC_AHBRSTR		0x28
#define	RCC_APBRSTR1		0x2C
#define	RCC_APBRSTR2		0x30
#define	RCC_IOPENR		0x34
#define	 IOPENR_GPIOFEN		(1 << 5)
#define	 IOPENR_GPIOEEN		(1 << 4)
#define	 IOPENR_GPIODEN		(1 << 3)
#define	 IOPENR_GPIOCEN		(1 << 2)
#define	 IOPENR_GPIOBEN		(1 << 1)
#define	 IOPENR_GPIOAEN		(1 << 0)
#define	RCC_AHBENR		0x38
#define	 AHBENR_CRCEN		(1 << 12)
#define	 AHBENR_FLASHEN		(1 << 8)
#define	 AHBENR_DMA2EN		(1 << 1)
#define	 AHBENR_DMA1EN		(1 << 0)
#define	RCC_APBENR1		0x3C
#define	 APBENR1_PWREN		(1 << 28) /* Power interface clock enable */
#define	 APBENR1_DBGEN		(1 << 27) /* Debug support clock enable */
#define	 APBENR1_I2C3EN		(1 << 23) /* I2C3 clock enable */
#define	 APBENR1_I2C2EN		(1 << 22) /* I2C2 clock enable */
#define	 APBENR1_I2C1EN		(1 << 21) /* I2C2 clock enable */
#define	 APBENR1_USART4EN	(1 << 19) /* USART3 clock enable */
#define	 APBENR1_USART3EN	(1 << 18) /* USART3 clock enable */
#define	 APBENR1_USART2EN	(1 << 17) /* USART2 clock enable */
#define	 APBENR1_SPI3EN		(1 << 15) /* SPI3 clock enable */
#define	 APBENR1_SPI2EN		(1 << 14) /* SPI2 clock enable */
#define	 APBENR1_USBEN		(1 << 13) /* USB clock enable */
#define	 APBENR1_WWDGEN		(1 << 11) /* Window watchdog clock enable */
#define	 APBENR1_RTCAPBEN	(1 << 10) /* RTC APB clock enable */
#define	 APBENR1_USART6EN	(1 << 9) /* USART6 clock enable */
#define	 APBENR1_USART5EN	(1 << 8) /* USART5 clock enable */
#define	 APBENR1_TIM7EN		(1 << 5)
#define	 APBENR1_TIM6EN		(1 << 4)
#define	 APBENR1_TIM4EN		(1 << 2)
#define	 APBENR1_TIM3EN		(1 << 1)
#define	RCC_APBENR2		0x40
#define	 APBENR2_ADCEN		(1 << 20) /* ADC clock enable */
#define	 APBENR2_TIM17EN	(1 << 18)
#define	 APBENR2_TIM16EN	(1 << 17)
#define	 APBENR2_TIM15EN	(1 << 16)
#define	 APBENR2_TIM14EN	(1 << 15)
#define	 APBENR2_USART1EN	(1 << 14) /* USART1 clock enable */
#define	 APBENR2_SPI1EN		(1 << 12) /* SPI1 clock enable */
#define	 APBENR2_TIM1EN		(1 << 11) /* TIM1 timer clock enable */
#define	 APBENR2_SYSCFGEN	(1 << 0)
#define	RCC_IOPSMENR		0x44
#define	RCC_AHBSMENR		0x48
#define	RCC_APBSMENR1		0x4C
#define	RCC_APBSMENR2		0x50
#define	RCC_CCIPR		0x54
#define	RCC_CCIPR2		0x58
#define	 CCIPR2_DSISEL		(1 << 12) /* PLLDSICLK is selected */
#define	RCC_BDCR		0x5C
#define	 BDCR_RTCEN		(1 << 15)	/* RTC clock enable */
#define	 BDCR_RTCSEL_S		8	/* RTC clock source selection */
#define	 BDCR_RTCSEL_M		(0x3 << BDCR_RTCSEL_S)
#define	 BDCR_RTCSEL_LSE	(1 << BDCR_RTCSEL_S) /* LSE oscillator clock */
#define	 BDCR_RTCSEL_LSI	(2 << BDCR_RTCSEL_S) /* LSI oscillator clock */
#define	 BDCR_RTCSEL_HSE	(3 << BDCR_RTCSEL_S) /* HSE oscillator clock divided by 32 */
#define	RCC_CSR			0x60
#define	 CSR_LSION		(1 << 0) /* LSI oscillator enable */
#define	 CSR_LSIRDY		(1 << 1) /* LSI oscillator ready */

#if 0
#define	 CFGR_PPRE2_S		11 /* APB high-speed prescaler (APB2) */
#define	 CFGR_PPRE2_M		(0x7 << CFGR_PPRE2_S)
#define	 CFGR_PPRE2_2		(0x4 << CFGR_PPRE2_S)
#define	 CFGR_PPRE2_4		(0x5 << CFGR_PPRE2_S)
#define	 CFGR_PPRE2_8		(0x6 << CFGR_PPRE2_S)
#define	 CFGR_PPRE2_16		(0x7 << CFGR_PPRE2_S)
#define	 CFGR_PPRE1_S		8 /* APB Low speed prescaler (APB1) */
#define	 CFGR_PPRE1_M		(0x7 << CFGR_PPRE1_S)
#define	 CFGR_PPRE1_2		(0x4 << CFGR_PPRE1_S)
#define	 CFGR_PPRE1_4		(0x5 << CFGR_PPRE1_S)
#define	 CFGR_PPRE1_8		(0x6 << CFGR_PPRE1_S)
#define	 CFGR_PPRE1_16		(0x7 << CFGR_PPRE1_S)
#define	 CFGR_HPRE_S		4 /* AHB prescaler */
#define	 CFGR_HPRE_M		(0xf << CFGR_HPRE_S)
#endif

#define	 CFGR_SWS_S		3 /* System clock switch status */
#define	 CFGR_SWS_M		(0x7 << CFGR_SWS_S)
#define	 CFGR_SWS_LSE		(0x4 << CFGR_SWS_S)
#define	 CFGR_SWS_LSI		(0x3 << CFGR_SWS_S)
#define	 CFGR_SWS_PLLRCLK	(0x2 << CFGR_SWS_S)
#define	 CFGR_SWS_HSE		(0x1 << CFGR_SWS_S)
#define	 CFGR_SWS_HSISYS	(0x0 << CFGR_SWS_S)

#define	 CFGR_SW_S		0 /* System clock switch */
#define	 CFGR_SW_M		(0x7 << CFGR_SW_S)
#define	 CFGR_SW_LSE		(0x4 << CFGR_SW_S)
#define	 CFGR_SW_LSI		(0x3 << CFGR_SW_S)
#define	 CFGR_SW_PLLRCLK	(0x2 << CFGR_SW_S)
#define	 CFGR_SW_HSE		(0x1 << CFGR_SW_S)
#define	 CFGR_SW_HSISYS		(0x0 << CFGR_SW_S)

#if 0
#define	RCC_APB1RSTR		0x38
#define	 APB1RSTR_I2C3RST	(1 << 23)	/* I2C3 reset */
#define	 APB1RSTR_SPI2RST	(1 << 14)	/* SPI2 reset */
#endif

struct rcc_config {
	uint32_t ahbenr;
	uint32_t apbenr1;
	uint32_t apbenr2;
	uint32_t iopenr;
};

struct stm32g0_rcc_softc {
	uint32_t base;
};

int stm32g0_rcc_init(struct stm32g0_rcc_softc *sc, uint32_t base);
void stm32g0_rcc_pll_configure(struct stm32g0_rcc_softc *sc,
    int pllm, int plln, int pllq, int pllp, uint8_t external,
    uint32_t rcc_cfgr);
void stm32g0_rcc_setup(struct stm32g0_rcc_softc *sc, struct rcc_config *cfg);
void stm32g0_rcc_msi_configure(struct stm32g0_rcc_softc *sc, uint32_t freq);
int stm32g0_rcc_lsi_enable(struct stm32g0_rcc_softc *sc);
void stm32g0_rcc_bdcr_setup(struct stm32g0_rcc_softc *sc, uint32_t bdcr);
void stm32g0_rcc_hse_enable(struct stm32g0_rcc_softc *sc);
void stm32g0_rcc_usartsel_sysclk(struct stm32g0_rcc_softc *sc);

#endif /* !_ARM_STM_STM32G0_RCC_H_ */
