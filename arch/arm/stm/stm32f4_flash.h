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

#ifndef _ARM_STM_STM32F4_FLASH_H_
#define _ARM_STM_STM32F4_FLASH_H_

#define	FLASH_ACR		0x00 /* Flash access control register */
#define	 ACR_LATENCY_S		0
#define	 ACR_LATENCY_M		(0xf << ACR_LATENCY_S)
#define	 ACR_LATENCY(n)		((n) << ACR_LATENCY_S)
#define	 ACR_DCEN		(1 << 10) /* Data cache enable */
#define	 ACR_ICEN		(1 << 9) /* Instruction cache enable */
#define	 ACR_PRFTEN		(1 << 8) /* Prefetch enable */

struct stm32f4_flash_softc {
	uint32_t base;
};

int stm32f4_flash_init(struct stm32f4_flash_softc *sc, uint32_t base);
void stm32f4_flash_setup(struct stm32f4_flash_softc *sc);

#endif /* !_ARM_STM_STM32F4_FLASH_H_ */
