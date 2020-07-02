/*-
 * Copyright (c) 2020 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _RISCV_GIGADEVICE_GD32V_RTC_H_
#define _RISCV_GIGADEVICE_GD32V_RTC_H_

#define	RTC_INTEN	0x00 /* RTC interrupt enable register */
#define	RTC_CTL		0x04 /* RTC control register */
#define	RTC_PSCH	0x08 /* RTC prescaler high register */
#define	RTC_PSCL	0x0C /* RTC prescaler low register */
#define	RTC_DIVH	0x10 /* RTC divider high register */
#define	RTC_DIVL	0x14 /* RTC divider low register */
#define	RTC_CNTH	0x18 /* RTC counter high register */
#define	RTC_CNTL	0x1C /* RTC counter low register */
#define	RTC_ALRMH	0x20 /* RTC alarm high register */
#define	RTC_ALRML	0x24 /* RTC alarm low register */

struct gd32v_rtc_softc {
	uint32_t base;
};

int gd32v_rtc_init(mdx_device_t dev, uint32_t base);

#endif /* !_RISCV_GIGADEVICE_GD32V_RTC_H_ */
