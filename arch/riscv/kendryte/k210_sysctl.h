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


#define	K210_SYSCTL_GIT_ID	0x00	/* Git short commit id */
#define	K210_SYSCTL_CLK_FREQ	0x04	/* System clock base frequency */
#define	K210_SYSCTL_PLL0	0x08	/* PLL0 controller */
#define	 PLL0_CLKR_S		0
#define	 PLL0_CLKF_S		4
#define	 PLL0_CLKOD_S		10
#define	 PLL0_BWADJ_S		14
#define	 PLL0_RESET		(1 << 20)
#define	 PLL0_PWRD		(1 << 21)
#define	 PLL0_INTFB		(1 << 22)
#define	 PLL0_BYPASS		(1 << 23)
#define	 PLL0_TEST		(1 << 24)
#define	 PLL0_OUT_EN		(1 << 25)
#define	 PLL0_TEST_EN		(1 << 26)
#define	K210_SYSCTL_PLL1	0x0c	/* PLL1 controller */
#define	 PLL1_CLKR_S		0
#define	 PLL1_CLKF_S		4
#define	 PLL1_CLKOD_S		10
#define	 PLL1_BWADJ_S		14
#define	 PLL1_RESET		(1 << 20)
#define	 PLL1_PWRD		(1 << 21)
#define	 PLL1_INTFB		(1 << 22)
#define	 PLL1_BYPASS		(1 << 23)
#define	 PLL1_TEST		(1 << 24)
#define	 PLL1_OUT_EN		(1 << 25)
#define	K210_SYSCTL_PLL2	0x10	/* PLL2 controller */
#define	 PLL2_CLKR_S		0
#define	 PLL2_CLKF_S		4
#define	 PLL2_CLKOD_S		10
#define	 PLL2_BWADJ_S		14
#define	 PLL2_RESET		(1 << 20)
#define	 PLL2_PWRD		(1 << 21)
#define	 PLL2_INTFB		(1 << 22)
#define	 PLL2_BYPASS		(1 << 23)
#define	 PLL2_TEST		(1 << 24)
#define	 PLL2_OUT_EN		(1 << 25)
#define	 PLL2_CKIN_SEL_S	26
#define	K210_SYSCTL_RESV5	0x14	/* Reserved */
#define	K210_SYSCTL_PLL_LOCK	0x18	/* PLL lock tester */
#define	K210_SYSCTL_ROM_ERROR	0x1c	/* AXI ROM detector */
#define	K210_SYSCTL_CLK_SEL0	0x20	/* Clock select controller0 */
#define	K210_SYSCTL_CLK_SEL1	0x24	/* Clock select controller1 */
#define	K210_SYSCTL_CLK_EN_CENT	0x28	/* Central clock enable */
#define	 CENT_CLK_EN_CPU	(1 << 0)
#define	 CENT_CLK_EN_SRAM0	(1 << 1)
#define	 CENT_CLK_EN_SRAM1	(1 << 2)
#define	 CENT_CLK_EN_APB0	(1 << 3)
#define	 CENT_CLK_EN_APB1	(1 << 4)
#define	 CENT_CLK_EN_APB2	(1 << 5)
#define	K210_SYSCTL_CLK_EN_PERI	0x2c	/* Peripheral clock enable */
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
#define	K210_SYSCTL_SOFT_RESET	0x30	/* Soft reset ctrl */
#define	K210_SYSCTL_PERI_RESET	0x34	/* Peripheral reset controller */
#define	K210_SYSCTL_CLK_TH0	0x38	/* Clock threshold controller 0 */
#define	K210_SYSCTL_CLK_TH1	0x3c	/* Clock threshold controller 1 */
#define	K210_SYSCTL_CLK_TH2	0x40	/* Clock threshold controller 2 */
#define	K210_SYSCTL_CLK_TH3	0x44	/* Clock threshold controller 3 */
#define	K210_SYSCTL_CLK_TH4	0x48	/* Clock threshold controller 4 */
#define	K210_SYSCTL_CLK_TH5	0x4c	/* Clock threshold controller 5 */
#define	K210_SYSCTL_CLK_TH6	0x50	/* Clock threshold controller 6 */
#define	K210_SYSCTL_MISC	0x54	/* Miscellaneous controller */
#define	K210_SYSCTL_PERI	0x58	/* Peripheral controller */
#define	K210_SYSCTL_SPI_SLEEP	0x5c	/* SPI sleep controller */
#define	K210_SYSCTL_RESET_STATUS 0x60	/* Reset source status */
#define	K210_SYSCTL_DMA_SEL0	0x64	/* DMA handshake selector */
#define	K210_SYSCTL_DMA_SEL1	0x68	/* DMA handshake selector */
#define	K210_SYSCTL_POWER_SEL	0x6c	/* IO Power Mode Select controller */
#define	K210_SYSCTL_RESV28	0x70	/* Reserved */
#define	K210_SYSCTL_RESV29	0x74	/* Reserved */
#define	K210_SYSCTL_RESV30	0x78	/* Reserved */
#define	K210_SYSCTL_RESV31	0x7c	/* Reserved */

struct k210_sysctl_softc {
	size_t base;
};

void k210_sysctl_init(struct k210_sysctl_softc *sc, uint32_t base);

#endif /* !_RISCV_KENDRYTE_K210_SYSCTL_H_ */
