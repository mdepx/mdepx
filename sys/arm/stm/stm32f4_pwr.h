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

#ifndef _ARM_STM_STM32F4_PWR_H_
#define _ARM_STM_STM32F4_PWR_H_

#define	PWR_CR		0x00
#define	 CR_ODEN	(1 << 16)
#define	 CR_ODSWEN	(1 << 17) /* Over-drive switching enabled. */
#define	 CR_VOS_S	14 /* Regulator voltage scaling output selection */
#define	 CR_VOS_M	(0x3 << CR_VOS_S)
#define	 CR_VOS_1	(0x3 << CR_VOS_S)
#define	 CR_VOS_2	(0x2 << CR_VOS_S)
#define	 CR_VOS_3	(0x1 << CR_VOS_S)
#define	PWR_CSR		0x04

struct stm32f4_pwr_softc {
	uint32_t base;
};

int stm32f4_pwr_init(struct stm32f4_pwr_softc *sc, uint32_t base);
void pwr_init(struct stm32f4_pwr_softc *sc);
void vos_init(struct stm32f4_pwr_softc *sc);

#endif /* !_ARM_STM_STM32F4_PWR_H_ */
