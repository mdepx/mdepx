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

#ifndef _RISCV_KENDRYTE_K210_SYSCTL_H_
#define _RISCV_KENDRYTE_K210_SYSCTL_H_

#define	K210_SYSCTL_CLK_EN_CENT	0x28	/* Central clock enable */
#define	 CENT_CLK_EN_CPU	(1 << 0)
#define	 CENT_CLK_EN_SRAM0	(1 << 1)
#define	 CENT_CLK_EN_SRAM1	(1 << 2)
#define	 CENT_CLK_EN_APB0	(1 << 3)
#define	 CENT_CLK_EN_APB1	(1 << 4)
#define	 CENT_CLK_EN_APB2	(1 << 5)
#define	K210_SYSCTL_CLK_EN_PERI	0x2C	/* Peripheral clock enable */
#define	 PERI_CLK_EN_ROM	(1 << 0)
#define	 PERI_CLK_EN_DMA	(1 << 1)
#define	 PERI_CLK_EN_AI		(1 << 2)
#define	 PERI_CLK_EN_DVP	(1 << 3)
#define	 PERI_CLK_EN_FFT	(1 << 4)
#define	 PERI_CLK_EN_GPIO	(1 << 5)
#define	 PERI_CLK_EN_SPI0	(1 << 6)
#define	 PERI_CLK_EN_SPI1	(1 << 7)
#define	 PERI_CLK_EN_SPI2	(1 << 8)
#define	 PERI_CLK_EN_SPI3	(1 << 9)
#define	 PERI_CLK_EN_I2S0	(1 << 10)
#define	 PERI_CLK_EN_I2S1	(1 << 11)
#define	 PERI_CLK_EN_I2S2	(1 << 12)
#define	 PERI_CLK_EN_I2C0	(1 << 13)
#define	 PERI_CLK_EN_I2C1	(1 << 14)
#define	 PERI_CLK_EN_I2C2	(1 << 15)
#define	 PERI_CLK_EN_UART1	(1 << 16)
#define	 PERI_CLK_EN_UART2	(1 << 17)
#define	 PERI_CLK_EN_UART3	(1 << 18)
#define	 PERI_CLK_EN_AES	(1 << 19)
#define	 PERI_CLK_EN_FPIOA	(1 << 20)
#define	 PERI_CLK_EN_TIMER0	(1 << 21)
#define	 PERI_CLK_EN_TIMER1	(1 << 22)
#define	 PERI_CLK_EN_TIMER2	(1 << 23)
#define	 PERI_CLK_EN_WDT0	(1 << 24)
#define	 PERI_CLK_EN_WDT1	(1 << 25)
#define	 PERI_CLK_EN_SHA	(1 << 26)
#define	 PERI_CLK_EN_OTP	(1 << 27)
#define	 PERI_CLK_EN_RESERVED0	(1 << 28)
#define	 PERI_CLK_EN_RTC	(1 << 29)
#define	 PERI_CLK_EN_RESERVED1	(1 << 30)
#define	 PERI_CLK_EN_RESERVED2	(1 << 31)

struct k210_sysctl_softc {
	size_t base;
};

void k210_sysctl_init(struct k210_sysctl_softc *sc, uint32_t base);

#endif /* !_RISCV_KENDRYTE_K210_SYSCTL_H_ */
