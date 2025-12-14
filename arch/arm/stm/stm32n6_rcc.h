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

#ifndef _ARM_STM_STM32N6_RCC_H_
#define _ARM_STM_STM32N6_RCC_H_

#define	RCC_CR			0x00 /* RCC control register */
#define	RCC_BUSENR		0x244
#define	 BUSENR_APB2EN		(1 << 9) /* APB2 enable */
#define	 BUSENR_AHB4EN		(1 << 6) /* AHB4 enable */
#define	RCC_AHB1ENR		0x250 /* RCC AHB1 enable register */
#define	RCC_AHB2ENR		0x254 /* RCC AHB2 enable register */
#define	RCC_AHB3ENR		0x258 /* RCC AHB3 enable register */
#define	RCC_AHB4ENR		0x25C /* RCC AHB4 enable register */
#define	RCC_AHB5ENR		0x260 /* RCC AHB5 enable register */
#define	RCC_APB1LENR		0x264 /* RCC APB1L enable register */
#define	RCC_APB1HENR		0x268 /* RCC APB1H enable register */
#define	RCC_APB2ENR		0x26C /* RCC APB2 enable register */
#define	RCC_APB3ENR		0x270 /* RCC APB2 enable register */
#define	RCC_APB4LENR		0x274 /* RCC APB4L enable register */
#define	RCC_APB4HENR		0x278 /* RCC APB4H enable register */
#define	RCC_APB5ENR		0x27C /* RCC APB5 enable register */
#define	RCC_MEMENSR		0xA4C
#define	 MEMENSR_BOOTROMENS	(1 << 12)
#define	 MEMENSR_VENCRAMENS	(1 << 11)
#define	 MEMENSR_CACHEAXIRAMENS	(1 << 10)
#define	 MEMENSR_FLEXRAMENS	(1 << 9)
#define	 MEMENSR_AXISRAM2ENS	(1 << 8)
#define	 MEMENSR_AXISRAM1ENS	(1 << 7)
#define	 MEMENSR_BKPSRAMENS	(1 << 6)
#define	 MEMENSR_AHBSRAM2ENS	(1 << 5)
#define	 MEMENSR_AHBSRAM1ENS	(1 << 4)
#define	 MEMENSR_AXISRAM6ENS	(1 << 3)
#define	 MEMENSR_AXISRAM5ENS	(1 << 2)
#define	 MEMENSR_AXISRAM4ENS	(1 << 1)
#define	 MEMENSR_AXISRAM3ENS	(1 << 0)
#define	RCC_AHB1ENSR		0xA50
#define	RCC_AHB2ENSR		0xA54
#define	 APB2ENSR_TIM1EN	(1 << 0) /* TIM1 clock enable */
#define	 APB2ENSR_USART1EN	(1 << 4) /* USART1 clock enable */
#define	RCC_AHB3ENSR		0xA58
#define	RCC_AHB4ENSR		0xA5C
#define	 AHB4ENSR_PWREN		(1 << 18) /* PWR enable */
#define	 AHB4ENSR_GPIOQEN	(1 << 16) /* GPIO Q enable */
#define	 AHB4ENSR_GPIOPEN	(1 << 15) /* GPIO P enable */
#define	 AHB4ENSR_GPIOOEN	(1 << 14) /* GPIO O enable */
#define	 AHB4ENSR_GPIONEN	(1 << 13) /* GPIO N enable */
#define	 AHB4ENSR_GPIOHEN	(1 << 7) /* GPIO H enable */
#define	 AHB4ENSR_GPIOGEN	(1 << 6) /* GPIO G enable */
#define	 AHB4ENSR_GPIOFEN	(1 << 5) /* GPIO F enable */
#define	 AHB4ENSR_GPIOEEN	(1 << 4) /* GPIO E enable */
#define	 AHB4ENSR_GPIODEN	(1 << 3) /* GPIO D enable */
#define	 AHB4ENSR_GPIOCEN	(1 << 2) /* GPIO C enable */
#define	 AHB4ENSR_GPIOBEN	(1 << 1) /* GPIO B enable */
#define	 AHB4ENSR_GPIOAEN	(1 << 0) /* GPIO A enable */
#define	RCC_AHB5ENSR		0xA60
#define	 AHB5ENSR_NPUENS	(1 << 31) /* NPU enable */
#define	 AHB5ENSR_CACHEAXIENS	(1 << 30)
#define	 AHB5ENSR_GPU2DENS	(1 << 20)
#define	 AHB5ENSR_GFXMMUENS	(1 << 19)
#define	 AHB5ENSR_XSPI3ENS	(1 << 17)
#define	 AHB5ENSR_XSPIMENS	(1 << 13)
#define	 AHB5ENSR_XSPI2ENS	(1 << 12)
#define	 AHB5ENSR_XSPI1ENS	(1 << 5)
#define	 AHB5ENSR_FMCENS	(1 << 4)
#define	 AHB5ENSR_JPEGENS	(1 << 3)
#define	 AHB5ENSR_DMA2DENS	(1 << 1)
#define	RCC_APB1LENSR		0xA64
#define	 APB1LENSR_I2C1ENS	(1 << 21) /* I2C1 enable */
#define	 APB1LENSR_UART5ENS	(1 << 20)
#define	 APB1LENSR_USART2ENS	(1 << 17) /* USART2 enable */
#define	 APB1LENSR_TIM2ENS	(1 << 0) /* TIM2 enable */
#define	RCC_APB1HENSR		0xA68
#define	RCC_APB2ENSR		0xA6C
#define	 APB2ENSR_TIM1ENS	(1 << 0) /* TIM1 enable */
#define	RCC_APB3ENSR		0xA70
#define	RCC_APB4LENSR		0xA74
#define	RCC_APB4HENSR		0xA78
#define	RCC_APB5ENSR		0xA7C
#define	 APB5ENSR_LTDCEN	(1 << 1)
#define	 APB5ENSR_DCMIPPENS	(1 << 2)
#define	 APB5ENSR_GFXTIMENS	(1 << 4)
#define	 APB5ENSR_VENCENS	(1 << 5)
#define	 APB5ENSR_CSIENS	(1 << 6)

struct rcc_config {
	uint32_t ahb1enr;
	uint32_t ahb2enr;
	uint32_t ahb3enr;
	uint32_t ahb4enr;
	uint32_t ahb5enr;
	uint32_t apb1lenr;
	uint32_t apb1henr;
	uint32_t apb2enr;
	uint32_t apb3enr;
	uint32_t apb4lenr;
	uint32_t apb4henr;
	uint32_t apb5enr;
	uint32_t memenr;
};

struct stm32n6_rcc_softc {
	uint32_t base;
};

int stm32n6_rcc_init(struct stm32n6_rcc_softc *sc, uint32_t base);
void stm32n6_rcc_setup(struct stm32n6_rcc_softc *sc,
    struct rcc_config *cfg);

#endif /* !_ARM_STM_STM32N6_RCC_H_ */
