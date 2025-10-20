/*-
 * Copyright (c) 2024 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _ARM_STM_STM32G0_EXTI_H_
#define _ARM_STM_STM32G0_EXTI_H_

#define	EXTI_RTSR1		0x00
#define	EXTI_FTSR1		0x04
#define	EXTI_SWIER1		0x08
#define	EXTI_RPR1		0x0C
#define	EXTI_FPR1		0x10
#define	_EXTI_EXTICR(x)		(0x060 + 0x4 * ((x) - 1)) /* (x = 1 to 4) */
#define	EXTI_EXTICR(x)		_EXTI_EXTICR(((x) / 4) + 1)
#define	 EXTICR_PIN_S(x)	((x % 4) * 8)
#define	 EXTICR_PA(x)		(0 << EXTICR_PIN_S(x))
#define	 EXTICR_PB(x)		(1 << EXTICR_PIN_S(x))
#define	 EXTICR_PC(x)		(2 << EXTICR_PIN_S(x))
#define	 EXTICR_PD(x)		(3 << EXTICR_PIN_S(x))
#define	 EXTICR_PF(x)		(5 << EXTICR_PIN_S(x))
#define	EXTI_IMR1		0x80
#define	EXTI_EMR1		0x84

struct exti_intr_entry {
	void (*handler) (void *arg, int raising);
	void *arg;
};

struct stm32g0_exti_softc {
	uint32_t base;
	const struct exti_intr_entry *map;
};

void stm32g0_exti_intr(void *arg, int irq);
void stm32g0_exti_install_intr_map(struct stm32g0_exti_softc *sc,
    const struct exti_intr_entry *map);
void stm32g0_exti_setup(struct stm32g0_exti_softc *sc, uint32_t port,
    uint32_t n);
void stm32g0_exti_init(struct stm32g0_exti_softc *sc, uint32_t base);

#endif /* !_ARM_STM_STM32G0_EXTI_H_ */
