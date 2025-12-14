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

#ifndef _ARM_STM_STM32N6_PWR_H_
#define _ARM_STM_STM32N6_PWR_H_

#define	PWR_SVMCR1	0x34
#define	 SVMCR1_VDDIO4SV	(1 << 8)
#define	PWR_SVMCR3	0x3C
#define	 SVMCR3_VDDIO3VRSEL	(1 << 26)
#define	 SVMCR3_VDDIO2VRSEL	(1 << 25)
#define	 SVMCR3_VDDIOVRSEL	(1 << 24)
#define	 SVMCR3_VDDIO3RDY	(1 << 17)
#define	 SVMCR3_VDDIO2RDY	(1 << 16)
#define	 SVMCR3_VDDIO3SV	(1 << 9)
#define	 SVMCR3_VDDIO2SV	(1 << 8)
#define	 SVMCR3_VDDIO3VMEN	(1 << 1)
#define	 SVMCR3_VDDIO2VMEN	(1 << 0)

struct stm32n6_pwr_softc {
	uint32_t base;
};

void stm32n6_pwr_setup_vddio23_1v8(struct stm32n6_pwr_softc *sc);
int stm32n6_pwr_init(struct stm32n6_pwr_softc *sc, uint32_t base);
void stm32n6_pwr_setup_vddio4_3v3(struct stm32n6_pwr_softc *sc, int enable);

#endif /* !_ARM_STM_STM32N6_PWR_H_ */
