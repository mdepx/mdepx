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

#ifndef _ARM_NXP_LPC43XX_CGU_H_
#define _ARM_NXP_LPC43XX_CGU_H_

#define	CGU_FREQ_MON		0x014 /* Frequency monitor register */
#define	CGU_XTAL_OSC_CTRL	0x018 /* Crystal oscillator control */
#define	 XTAL_POWER_DOWN	(1 << 0)
#define	CGU_PLL0USB_STAT	0x01C /* PLL0USB status register */
#define	CGU_PLL0USB_CTRL	0x020 /* PLL0USB control register */
#define	 CTRL_PD		(1 << 0) /* PLL0 power down */
#define	 CTRL_BYPASS		(1 << 1) /* Input clock bypass control */
#define	 CTRL_DIRECTI		(1 << 2) /* PLL0 direct input */
#define	 CTRL_DIRECTO		(1 << 3) /* PLL0 direct output */
#define	 CTRL_CLKEN		(1 << 4) /* PLL0 clock enable */
#define	 CTRL_FRM		(1 << 6) /* Free running mode */
#define	 CTRL_AUTOBLOCK		(1 << 11) /* Clock autoblocking */
#define	 CTRL_CLK_SEL_S		24 /* Clock source selection. */
#define	 CTRL_CLK_SEL_M		(0x1f << CTRL_CLK_SEL_S)
#define	 CTRL_CLK_SEL_32KHZ_OSC		0x00 /* 32 kHz oscillator */
#define	 CTRL_CLK_SEL_IRC		0x01 /* IRC (default) */
#define	 CTRL_CLK_SEL_ENET_RX_CLK	0x02
#define	 CTRL_CLK_SEL_ENET_TX_CLK	0x03
#define	 CTRL_CLK_SEL_GP_CLKIN		0x04
#define	 CTRL_CLK_SELCRYSTAL_OSC	0x06
#define	 CTRL_CLK_SEL_PLL1		0x09
#define	 CTRL_CLK_SEL_IDIVA		0x0C
#define	 CTRL_CLK_SEL_IDIVB		0x0D
#define	 CTRL_CLK_SEL_IDIVC		0x0E
#define	 CTRL_CLK_SEL_IDIVD		0x0F
#define	 CTRL_CLK_SEL_IDIVE		0x10
#define	CGU_PLL0USB_MDIV	0x024 /* PLL0USB M-divider register */
#define	CGU_PLL0USB_NP_DIV	0x028 /* PLL0USB N/P-divider */
#define	CGU_PLL0AUDIO_STAT	0x02C /* PLL0AUDIO status register */
#define	CGU_PLL0AUDIO_CTRL	0x030 /* PLL0AUDIO control register */
#define	CGU_PLL0AUDIO_MDIV	0x034 /* PLL0AUDIO M-divider */
#define	CGU_PLL0AUDIO_NP_DIV	0x038 /* PLL0AUDIO N/P-divider */
#define	CGU_PLL0AUDIO_FRAC	0x03C /* PLL0AUDIO fractional */
#define	CGU_PLL1_STAT		0x040 /* PLL1 status register */
#define	CGU_PLL1_CTRL		0x044 /* PLL1 control register */
#define	CGU_IDIVA_CTRL		0x048 /* Integer divider A control */
#define	CGU_IDIVB_CTRL		0x04C /* Integer divider B control */
#define	CGU_IDIVC_CTRL		0x050 /* Integer divider C control */
#define	CGU_IDIVD_CTRL		0x054 /* Integer divider D control */
#define	CGU_IDIVE_CTRL		0x058 /* Integer divider E control */
#define	CGU_BASE_SAFE_CLK	0x05C /* Output stage 0 control */
#define	CGU_BASE_USB0_CLK	0x060 /* Output stage 1 control */
#define	CGU_BASE_PERIPH_CLK	0x064 /* Output stage 2 control */
#define	CGU_BASE_USB1_CLK	0x068 /* Output stage 3 control */
#define	CGU_BASE_M4_CLK		0x06C /* Output stage 4 control */
#define	CGU_BASE_SPIFI_CLK	0x070 /* Output stage 5 control */
#define	CGU_BASE_SPI_CLK	0x074 /* Output stage 6 control */
#define	CGU_BASE_PHY_RX_CLK	0x078 /* Output stage 7 control */
#define	CGU_BASE_PHY_TX_CLK	0x07C /* Output stage 8 control */
#define	CGU_BASE_APB1_CLK	0x080 /* Output stage 9 control */
#define	CGU_BASE_APB3_CLK	0x084 /* Output stage 10 control */
#define	CGU_BASE_LCD_CLK	0x088 /* Output stage 11 control */
#define	CGU_BASE_ADCHS_CLK	0x08C /* Output stage 12 control */
#define	CGU_BASE_SDIO_CLK	0x090 /* Output stage 13 control */
#define	CGU_BASE_SSP0_CLK	0x094 /* Output stage 14 control */
#define	CGU_BASE_SSP1_CLK	0x098 /* Output stage 15 control */
#define	CGU_BASE_UART0_CLK	0x09C /* Output stage 16 control */
#define	CGU_BASE_UART1_CLK	0x0A0 /* Output stage 17 control */
#define	CGU_BASE_UART2_CLK	0x0A4 /* Output stage 18 control */
#define	CGU_BASE_UART3_CLK	0x0A8 /* Output stage 19 control */
#define	CGU_BASE_OUT_CLK	0x0AC /* Output stage 20 control */
#define	CGU_OUTCLK_21_CTRL	0x0B0 /* Reserved output stages */
#define	CGU_OUTCLK_22_CTRL	0x0B4 /* Reserved output stages */
#define	CGU_OUTCLK_23_CTRL	0x0B8 /* Reserved output stages */
#define	CGU_OUTCLK_24_CTRL	0x0BC /* Reserved output stages */
#define	CGU_BASE_AUDIO_CLK	0x0C0 /* Output stage 25 control */
#define	CGU_BASE_CGU_OUT0_CLK	0x0C4 /* Output stage 26 control */
#define	CGU_BASE_CGU_OUT1_CLK	0x0C8 /* Output stage 27 control */

struct cgu_softc {
	size_t base;
};

#endif /* !_ARM_NXP_LPC43XX_CGU_H_ */
