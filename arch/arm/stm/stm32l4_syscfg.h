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

#ifndef _ARM_STM_STM32L4_SYSCFG_H_
#define _ARM_STM_STM32L4_SYSCFG_H_

#define	SYSCFG_MEMRMP		0x00	/* Memory remap register */
#define	SYSCFG_CFGR1		0x04	/* SYSCFG configuration register 1 */
#define	SYSCFG_EXTICR1		0x08	/* External interrupt configuration register 1 */
#define	SYSCFG_EXTICR2		0x0C	/* External interrupt configuration register 2 */
#define	SYSCFG_EXTICR3		0x10	/* External interrupt configuration register 3 */
#define	SYSCFG_EXTICR4		0x14	/* External interrupt configuration register 4 */
#define	SYSCFG_EXTICR(n)	(0x8 + ((n) - 1) * 0x4)
#define	SYSCFG_SCSR		0x18	/* SYSCFG SRAM2 control and status register */
#define	SYSCFG_CFGR2		0x1C	/* SYSCFG configuration register 2 */
#define	SYSCFG_SWPR		0x20	/* SYSCFG SRAM2 write protection register */
#define	SYSCFG_SKR		0x24	/* SYSCFG SRAM2 key register */

struct stm32l4_syscfg_softc {
	uint32_t base;
};

void stm32l4_syscfg_init(struct stm32l4_syscfg_softc *sc, uint32_t base);
void stm32l4_syscfg_eth_rmii(struct stm32l4_syscfg_softc *sc);
void stm32l4_syscfg_setup(struct stm32l4_syscfg_softc *sc, int port, int pin);

#endif /* !_ARM_STM_STM32L4_SYSCFG_H_ */
