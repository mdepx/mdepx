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

#ifndef _ARM_STM_STM32L4_RTC_H_
#define _ARM_STM_STM32L4_RTC_H_

#define	RTC_TR		0x00
#define	RTC_DR		0x04
#define	RTC_CR		0x08
#define	RTC_ISR		0x0C
#define	RTC_PRER	0x10
#define	RTC_WUTR	0x14
#define	RTC_ALRMAR	0x1C
#define	RTC_ALRMBR	0x20
#define	RTC_WPR		0x24
#define	RTC_SSR		0x28
#define	RTC_SHIFTR	0x2C
#define	RTC_TSTR	0x30
#define	RTC_TSDR	0x34
#define	RTC_TSSSR	0x38
#define	RTC_CALR	0x3C
#define	RTC_TAMPCR	0x40
#define	RTC_ALRMASSR	0x44
#define	RTC_ALRMBSSR	0x48
#define	RTC_OR		0x4C
#define	RTC_BKPR(n)	(0x50 + 0x4 * (n))

struct stm32l4_rtc_softc {
	uint32_t base;
};

void stm32l4_rtc_init(struct stm32l4_rtc_softc *sc, uint32_t base);

#endif /* !_ARM_STM_STM32L4_RTC_H_ */
