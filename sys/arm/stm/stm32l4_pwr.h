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

#ifndef _ARM_STM_STM32L4_PWR_H_
#define _ARM_STM_STM32L4_PWR_H_

#define	PWR_CR1		0x00
#define	PWR_CR2		0x04
#define	PWR_CR3		0x08
#define	PWR_CR4		0x0C
#define	PWR_SR1		0x10
#define	PWR_SR2		0x14
#define	PWR_SCR		0x18
#define	PWR_PUCRA	0x20
#define	PWR_PDCRA	0x24
#define	PWR_PUCRB	0x28
#define	PWR_PDCRB	0x2C
#define	PWR_PUCRC	0x30
#define	PWR_PDCRC	0x34
#define	PWR_PUCRD	0x38
#define	PWR_PDCRD	0x3C
#define	PWR_PUCRE	0x40
#define	PWR_PDCRE	0x44
#define	PWR_PUCRH	0x58
#define	PWR_PDCRH	0x5C

struct stm32l4_pwr_softc {
	uint32_t base;
};

int stm32l4_pwr_init(struct stm32l4_pwr_softc *sc, uint32_t base);
void stm32l4_pwr_setup(struct stm32l4_pwr_softc *sc);

#endif /* !_ARM_STM_STM32L4_PWR_H_ */
