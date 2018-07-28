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

#ifndef _ARM_STM_STM32L4_RCC_H_
#define _ARM_STM_STM32L4_RCC_H_

#define	RCC_CR			0x00
#define	 CR_PLLSAIRDY		(1 << 27)
#define	 CR_PLLSAION		(1 << 26)
#define	 CR_PLLRDY		(1 << 25)
#define	 CR_PLLON		(1 << 24)
#define	 CR_CSSON		(1 << 19) /* Clock security system enable */
#define	 CR_HSEBYP		(1 << 18) /* HSE clock bypass */
#define	 CR_HSERDY		(1 << 17) /* HSE clock ready flag */
#define	 CR_HSEON		(1 << 16)
#define	 CR_HSIRDY		(1 << 10) /* HSI clock ready flag */
#define	 CR_HSION		(1 << 8)
#define	 CR_MSIRANGE_S		4 /* MSI clock ranges */
#define	 CR_MSIRANGE_M		(0xf << CR_MSIRANGE_S)
#define	 CR_MSIRANGE_100KHZ	(0 << CR_MSIRANGE_S)
#define	 CR_MSIRANGE_200KHZ	(1 << CR_MSIRANGE_S)
#define	 CR_MSIRANGE_400KHZ	(2 << CR_MSIRANGE_S)
#define	 CR_MSIRANGE_800KHZ	(3 << CR_MSIRANGE_S)
#define	 CR_MSIRANGE_1MHZ	(4 << CR_MSIRANGE_S)
#define	 CR_MSIRANGE_2MHZ	(5 << CR_MSIRANGE_S)
#define	 CR_MSIRANGE_4MHZ	(6 << CR_MSIRANGE_S)
#define	 CR_MSIRANGE_8MHZ	(7 << CR_MSIRANGE_S)
#define	 CR_MSIRANGE_16MHZ	(8 << CR_MSIRANGE_S)
#define	 CR_MSIRANGE_24MHZ	(9 << CR_MSIRANGE_S)
#define	 CR_MSIRANGE_32MHZ	(10 << CR_MSIRANGE_S)
#define	 CR_MSIRANGE_48MHZ	(11 << CR_MSIRANGE_S)
#define	 CR_MSIRGSEL		(1 << 3) /* MSI clock range selection */
#define	 CR_MSIRDY		(1 << 1) /* MSI clock ready flag */
#define	 CR_MSION		(1 << 0) /* MSI clock enable */
#define	RCC_CFGR		0x08
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
#define	 CFGR_SWS_S		2 /* System clock switch status */
#define	 CFGR_SWS_M		(0x3 << CFGR_SWS_S)
#define	 CFGR_SWS_PLL		(0x3 << CFGR_SWS_S)
#define	 CFGR_SWS_HSE		(0x2 << CFGR_SWS_S)
#define	 CFGR_SWS_HSI16		(0x1 << CFGR_SWS_S)
#define	 CFGR_SWS_MSI		(0x0 << CFGR_SWS_S)
#define	 CFGR_SW_S		0 /* System clock switch */
#define	 CFGR_SW_M		(0x3 << CFGR_SW_S)
#define	 CFGR_SW_PLL		(0x3 << CFGR_SW_S)
#define	 CFGR_SW_HSE		(0x2 << CFGR_SW_S)
#define	 CFGR_SW_HSI16		(0x1 << CFGR_SW_S)
#define	 CFGR_SW_MSI		(0x0 << CFGR_SW_S)
#define	RCC_PLLCFGR		0x0C
#define	 PLLCFGR_PLLR_S		25
#define	 PLLCFGR_PLLQ_S		21
#define	 PLLCFGR_PLLP_S		17
#define	 PLLCFGR_PLLN_S		8
#define	 PLLCFGR_PLLM_S		4
#define	RCC_PLLSAICFGR		0x10
#define	RCC_AHB1RSTR		0x28
#define	RCC_APB1RSTR		0x38
#define	 APB1RSTR_I2C3RST	(1 << 23)	/* I2C3 reset */
#define	 APB1RSTR_SPI2RST	(1 << 14)	/* SPI2 reset */
#define	RCC_AHB1ENR		0x48
#define	 AHB1ENR_TSCEN		(1 << 16) /* Touch Sensing Controller clock enable */
#define	 AHB1ENR_CRCEN		(1 << 12)
#define	 AHB1ENR_FLASHEN	(1 << 8)
#define	 AHB1ENR_DMA2EN		(1 << 1)
#define	 AHB1ENR_DMA1EN		(1 << 0)
#define	RCC_AHB2ENR		0x4C
#define	 AHB2ENR_GPIOHEN	(1 << 7)
#define	 AHB2ENR_GPIOEEN	(1 << 4)
#define	 AHB2ENR_GPIODEN	(1 << 3)
#define	 AHB2ENR_GPIOCEN	(1 << 2)
#define	 AHB2ENR_GPIOBEN	(1 << 1)
#define	 AHB2ENR_GPIOAEN	(1 << 0)
#define	RCC_AHB3ENR		0x50
#define	RCC_APB1ENR1		0x58
#define	 APB1ENR1_PWREN		(1 << 28) /* Power interface clock enable */
#define	 APB1ENR1_USART2EN	(1 << 17) /* USART2 clock enable */
#define	 APB1ENR1_I2C3EN	(1 << 23) /* I2C3 clock enable */
#define	 APB1ENR1_SPI2EN	(1 << 14) /* SPI2 clock enable */
#define	 APB1ENR1_TIM2EN	(1 << 0)
#define	RCC_APB1ENR2		0x5C
#define	RCC_APB2ENR		0x60
#define	 APB2ENR_USART1EN	(1 << 14)
#define	 APB2ENR_SPI1EN		(1 << 12) /* SPI1 clock enable */
#define	 APB2ENR_SDMMC1EN	(1 << 10)
#define	RCC_CCIPR		0x88	/* Peripherals independent clock configuration register */

struct stm32l4_rcc_softc {
	uint32_t base;
};

int stm32l4_rcc_init(struct stm32l4_rcc_softc *sc, uint32_t base);
void stm32l4_rcc_pll_configure(struct stm32l4_rcc_softc *sc,
    int pllm, int plln, int pllq, int pllp, uint8_t external, uint32_t rcc_cfgr);
int stm32l4_rcc_setup(struct stm32l4_rcc_softc *sc,
    uint32_t ahb1enr, uint32_t ahb2enr, uint32_t ahb3enr,
    uint32_t apb1enr1, uint32_t apb1enr2, uint32_t apb2enr);
void stm32l4_rcc_msi_configure(struct stm32l4_rcc_softc *sc, uint32_t freq);

#endif /* !_ARM_STM_STM32L4_RCC_H_ */
