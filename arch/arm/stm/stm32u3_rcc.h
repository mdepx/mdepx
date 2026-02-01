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

#ifndef _ARM_STM_STM32U3_RCC_H_
#define _ARM_STM_STM32U3_RCC_H_

#define	RCC_CR			0x00
#define	 CR_HSI48ON		(1 << 14) /* HSI48 clock enable */
#define	RCC_AHB1ENR1		0x88
#define	 AHB1ENR1_SRAM1EN	(1 << 31) /* SRAM1 clock enable */
#define	 AHB1ENR1_FLASHEN	(1 << 8) /* FLASH clock enable */
#define	RCC_AHB2ENR1		0x8C
#define	 AHB2ENR1_SRAM2EN	(1 << 30) /* SRAM2 clock enable */
#define	 AHB2ENR1_GPIOHEN	(1 << 7) /* I/O port clock enable */
#define	 AHB2ENR1_GPIOGEN	(1 << 6) /* I/O port clock enable */
#define	 AHB2ENR1_GPIOEEN	(1 << 4) /* I/O port clock enable */
#define	 AHB2ENR1_GPIODEN	(1 << 3) /* I/O port clock enable */
#define	 AHB2ENR1_GPIOCEN	(1 << 2) /* I/O port clock enable */
#define	 AHB2ENR1_GPIOBEN	(1 << 1) /* I/O port clock enable */
#define	 AHB2ENR1_GPIOAEN	(1 << 0) /* I/O port clock enable */
#define	RCC_AHB2ENR2		0x90
#define	RCC_AHB1ENR2		0x94
#define	 AHB1ENR2_PWREN		(1 << 2) /* PWR clock enable */
#define	RCC_APB1ENR1		0x9C
#define	RCC_APB1ENR2		0xA0
#define	RCC_APB2ENR		0xA4
#define	 APB2ENR_USB1EN		(1 << 24) /* USB1 clock enable */
#define	 APB2ENR_TIM1EN		(1 << 11) /* TIM1 clock enable */
#define	 APB2ENR_USART1EN	(1 << 14) /* USART1 clock enable */
#define	RCC_APB3ENR		0xA8

struct rcc_config {
	uint32_t ahb1enr1;
	uint32_t ahb2enr1;
	uint32_t ahb2enr2;
	uint32_t ahb1enr2;
	uint32_t apb1enr1;
	uint32_t apb1enr2;
	uint32_t apb2enr;
	uint32_t apb3enr;
};

struct stm32u3_rcc_softc {
	uint32_t base;
};

int stm32u3_rcc_init(struct stm32u3_rcc_softc *sc, uint32_t base);
void stm32u3_rcc_setup(struct stm32u3_rcc_softc *sc,
    struct rcc_config *cfg);

#endif /* !_ARM_STM_STM32U3_RCC_H_ */
