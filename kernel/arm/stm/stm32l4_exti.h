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

#ifndef _ARM_STM_STM32L4_EXTI_H_
#define _ARM_STM_STM32L4_EXTI_H_

#include <machine/frame.h>

#define	EXTI_IMR(n)	((n) < 32 ? 0x00 : 0x20)	/* Interrupt mask register */
#define	EXTI_EMR(n)	((n) < 32 ? 0x04 : 0x24)	/* Event mask register */
#define	EXTI_RTSR(n)	((n) < 32 ? 0x08 : 0x28)	/* Rising trigger selection register */
#define	EXTI_FTSR(n)	((n) < 32 ? 0x0C : 0x2C)	/* Falling trigger selection register */
#define	EXTI_SWIER(n)	((n) < 32 ? 0x10 : 0x30)	/* Software interrupt event register */
#define	EXTI_PR(n)	((n) < 32 ? 0x14 : 0x34)	/* Pending register */

#define	EXTI_NUM_INT	39

struct exti_intr_entry {
	void (*handler) (void *arg);
	void *arg;
};

struct stm32l4_exti_softc {
	uint32_t base;
	const struct exti_intr_entry *map;
};

void stm32l4_exti_init(struct stm32l4_exti_softc *sc, uint32_t base);
void stm32l4_exti_intr(void *arg, struct trapframe *tf, int irq);
void stm32l4_exti_install_intr_map(struct stm32l4_exti_softc *sc,
    const struct exti_intr_entry *map);
void stm32l4_exti_setup(struct stm32l4_exti_softc *sc, uint32_t n);

#endif /* !_ARM_STM_STM32L4_EXTI_H_ */
