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

#ifndef _ARM_STM_STM32F4_DSI_H_
#define _ARM_STM_STM32F4_DSI_H_

#define	DSI_VR		0x0000 /* DSI Host Version Register */
#define	DSI_CR		0x0004 /* DSI Host Control Register */
#define	 CR_EN		(1 << 0) /* DSI Host is enabled */
#define	DSI_CCR		0x0008 /* DSI Host Clock Control Register */
#define	 CCR_TOCKDIV_S	8 /* Timeout Clock Division */
#define	 CCR_TOCKDIV_M	0xff
#define	 CCR_TXECKDIV_S	0 /* TX Escape Clock Division */
#define	 CCR_TXECKDIV_M	0xff
#define	DSI_LVCIDR	0x000C /* DSI Host LTDC VCID Register */
#define	 LVCIDR_VCID_S	0 /* Virtual Channel ID */
#define	 LVCIDR_VCID_M	0x3
#define	DSI_LCOLCR	0x0010 /* DSI Host LTDC Color Coding Register */
#define	 LCOLCR_LPE	(1 << 8) /* Loosely Packet Enable */
#define	 LCOLCR_COLC_S	0 /* Color Coding */
#define	 LCOLCR_COLC_M	0xf
#define	 LCOLCR_COLC_16_1	0x0 /* 16-bit */
#define	 LCOLCR_COLC_16_2	0x1 /* 16-bit */
#define	 LCOLCR_COLC_16_3	0x2 /* 16-bit */
#define	 LCOLCR_COLC_18_1	0x3 /* 18-bit */
#define	 LCOLCR_COLC_18_2	0x4 /* 18-bit */
#define	 LCOLCR_COLC_24		0x5 /* 24-bit */
#define	DSI_LPCR	0x0014 /* DSI Host LTDC Polarity Configuration Register */
#define	 LPCR_HSP	(1 << 2) /* HSYNC Polarity */
#define	 LPCR_VSP	(1 << 1) /* VSYNC Polarity */
#define	 LPCR_DEP	(1 << 0) /* Data Enable Polarity */
#define	DSI_LPMCR	0x0018 /* DSI Host Low-Power mode Configuration Register */
#define	 LPMCR_LPSIZE_S		16 /* Largest Packet Size */
#define	 LPMCR_LPSIZE_M		0xff
#define	 LPMCR_VLPSIZE_S	0 /* VACT Largest Packet Size */
#define	 LPMCR_VLPSIZE_M	0xff
#define	DSI_PCR		0x002C /* DSI Host Protocol Configuration Register */
#define	DSI_GVCIDR	0x0030 /* DSI Host Generic VCID Register */
#define	DSI_MCR		0x0034 /* DSI Host mode Configuration Register */
#define	 MCR_CMDM	(1 << 0) /* Command mode. Reset value */
#define	DSI_VMCR	0x0038 /* DSI Host Video mode Configuration Register */
#define	 VMCR_LPCE	(1 << 15) /* Low-Power Command Enable */
#define	 VMCR_FBTAAE	(1 << 14) /* Frame Bus-Turn-Around Acknowledge Enable */
#define	 VMCR_LPHFPE	(1 << 13) /* Low-Power Horizontal Front-Porch Enable */
#define	 VMCR_LPHBPE	(1 << 12) /* Low-Power Horizontal Back-Porch Enable */
#define	 VMCR_LPVAE	(1 << 11) /* Low-Power Vertical Active Enable */
#define	 VMCR_LPVFPE	(1 << 10) /* Low-power Vertical Front-porch Enable */
#define	 VMCR_LPVBPE	(1 << 9) /* Low-power Vertical Back-Porch Enable */
#define	 VMCR_LPVSAE	(1 << 8) /* Low-Power Vertical Sync Active Enable */
#define	 VMCR_VMT_S	0 /* Video mode Type */
#define	 VMCR_VMT_M	0x3
#define	 VMCR_VMT_PULS	0
#define	 VMCR_VMT_EVNT	1
#define	 VMCR_VMT_BRST	2
#define	DSI_VPCR	0x003C /* DSI Host Video Packet Configuration Register */
#define	 VPCR_VPSIZE_S	0 /* Video Packet Size */
#define	 VPCR_VPSISE_M	0x3fff
#define	DSI_VCCR	0x0040 /* DSI Host Video Chunks Configuration Register */
#define	 VCCR_NUMC_S	0 /* Number of Chunks */
#define	 VCCR_NUMC_M	0x1fff
#define	DSI_VNPCR	0x0044 /* DSI Host Video Null Packet Configuration Register */
#define	 VNPCR_NPSIZE_S	0 /* Null Packet Size */
#define	 VNPCR_NPSIZE_M	0x1fff
#define	DSI_VHSACR	0x0048 /* DSI Host Video HSA Configuration Register */
#define	DSI_VHBPCR	0x004C /* DSI Host Video HBP Configuration Register */
#define	DSI_VLCR	0x0050 /* DSI Host Video Line Configuration Register */
#define	DSI_VVSACR	0x0054 /* DSI Host Video VSA Configuration Register */
#define	DSI_VVBPCR	0x0058 /* DSI Host Video VBP Configuration Register */
#define	DSI_VVFPCR	0x005C /* DSI Host Video VFP Configuration Register */
#define	DSI_VVACR	0x0060 /* DSI Host Video VA Configuration Register */
#define	DSI_LCCR	0x0064 /* DSI Host LTDC Command Configuration Register */
#define	DSI_CMCR	0x0068 /* DSI Host Command mode Configuration Register */
#define	DSI_GHCR	0x006C /* DSI Host Generic Header Configuration Register */
#define	 GHCR_WCMSB_S	16 /* WordCount MSB */
#define	 GHCR_WCMSB_M	0xff
#define	 GHCR_WCLSB_S	8 /* WordCount LSB */
#define	 GHCR_WCLSB_M	0xff
#define	 GHCR_VCID_S	6 /* Channel */
#define	 GHCR_VCID_M	0x3
#define	 GHCR_DT_S	0 /* Type */
#define	 GHCR_DT_M	0x3f
#define	DSI_GPDR	0x0070 /* DSI Host Generic Payload Data Register */
#define	DSI_GPSR	0x0074 /* DSI Host Generic Packet Status Register */
#define	 GPSR_CMDFF	(1 << 1) /* Command FIFO Full */
#define	 GPSR_CMDFE	(1 << 0) /* Command FIFO Empty */
#define	DSI_TCCR0	0x0078 /* DSI Host Timeout Counter Configuration Register 0 */
#define	DSI_TCCR1	0x007C /* DSI Host Timeout Counter Configuration Register 1 */
#define	DSI_TCCR2	0x0080 /* DSI Host Timeout Counter Configuration Register 2 */
#define	DSI_TCCR3	0x0084 /* DSI Host Timeout Counter Configuration Register 3 */
#define	DSI_TCCR4	0x0088 /* DSI Host Timeout Counter Configuration Register 4 */
#define	DSI_TCCR5	0x008C /* DSI Host Timeout Counter Configuration Register 5 */
#define	DSI_CLCR	0x0094 /* DSI Host Clock Lane Configuration Register */
#define	 CLCR_ACR	(1 << 1) /* Automatic Clock lane Control */
#define	 CLCR_DPCC	(1 << 0) /* D-PHY Clock Control */
#define	DSI_CLTCR	0x0098 /* DSI Host Clock Lane Timer Configuration Register */
#define	DSI_DLTCR	0x009C /* DSI Host Data Lane Timer Configuration Register */
#define	DSI_PCTLR	0x00A0 /* DSI Host PHY Control Register */
#define	 PCTLR_CKE	(1 << 2) /* Clock Enable */
#define	 PCTLR_DEN	(1 << 1) /* Digital Enable */
#define	DSI_PCONFR	0x00A4 /* DSI Host PHY Configuration Register */
#define	 SW_TIME_S	8 /* Stop Wait Time */
#define	 SW_TIME_M	0xff
#define	 PCONFR_NL_S	0 /* Number of Lanes */
#define	 PCONFR_NL_M	0x3
#define	 PCONFR_NL_1	(0 << PCONFR_NL_S)
#define	 PCONFR_NL_2	(1 << PCONFR_NL_S)	/* Reset value */
#define	DSI_PUCR	0x00A8 /* DSI Host PHY ULPS Control Register */
#define	DSI_PTTCR	0x00AC /* DSI Host PHY TX Triggers Configuration Register */
#define	DSI_PSR		0x00B0 /* DSI Host PHY Status Register */
#define	DSI_ISR0	0x00BC /* DSI Host Interrupt & Status Register 0 */
#define	DSI_ISR1	0x00C0 /* DSI Host Interrupt & Status Register 1 */
#define	DSI_IER0	0x00C4 /* DSI Host Interrupt Enable Register 0 */
#define	DSI_IER1	0x00C8 /* DSI Host Interrupt Enable Register 1 */
#define	DSI_FIR0	0x00D8 /* DSI Host Force Interrupt Register 0 */
#define	DSI_FIR1	0x00DC /* DSI Host Force Interrupt Register 1 */
#define	DSI_VSCR	0x0100 /* DSI Host Video Shadow Control Register */
#define	DSI_LCVCIDR	0x010C /* DSI Host LTDC Current VCID Register */
#define	DSI_LCCCR	0x0110 /* DSI Host LTDC Current Color Coding Register */
#define	DSI_LPMCCR	0x0118 /* DSI Host Low-Power mode Current Configuration */
#define	DSI_VMCCR	0x0138 /* DSI Host Video mode Current Configuration Register */
#define	DSI_VPCCR	0x013C /* DSI Host Video Packet Current Configuration Register */
#define	DSI_VCCCR	0x0140 /* DSI Host Video Chunks Current Configuration Register */
#define	DSI_VNPCCR	0x0144 /* DSI Host Video Null Packet Current Configuration Register */
#define	DSI_VHSACCR	0x0148 /* DSI Host Video HSA Current Configuration Register */
#define	DSI_VHBPCCR	0x014C /* DSI Host Video HBP Current Configuration Register */
#define	DSI_VLCCR	0x0150 /* DSI Host Video Line Current Configuration Register (DSI_VLCCR) */
#define	DSI_VVSACCR	0x0154 /* DSI Host Video VSA Current Configuration Register */
#define	DSI_VVBPCCR	0x0158 /* DSI Host Video VBP Current Configuration Register */
#define	DSI_VVFPCCR	0x015C /* DSI Host Video VFP Current Configuration Register */
#define	DSI_VVACCR	0x0160 /* DSI Host Video VA Current Configuration Register */
#define	DSI_WCFGR	0x0400 /* DSI Wrapper Configuration Register */
#define	 WCFGR_VSPOL	(1 << 7) /* VSync Polarity */
#define	 WCFGR_AR	(1 << 6) /* Automatic Refresh */
#define	 WCFGR_TEPOL	(1 << 5) /* TE Polarity */
#define	 WCFGR_TESRC	(1 << 4) /* TE Source */
#define	 WCFGR_COLMUX_S	1 /* Color Multiplexing */
#define	 WCFGR_COLMUX_M	0x7
#define	 COLMUX_16_1	0x0 /* 16-bit */
#define	 COLMUX_16_2	0x1 /* 16-bit */
#define	 COLMUX_16_3	0x2 /* 16-bit */
#define	 COLMUX_18_1	0x3 /* 18-bit */
#define	 COLMUX_18_2	0x4 /* 18-bit */
#define	 COLMUX_24	0x5 /* 24-bit */
#define	 WCFGR_DSIM	(1 << 0) /* DSI mode */
#define	DSI_WCR		0x0404 /* DSI Wrapper Control Register */
#define	 WCR_DSIEN	(1 << 3) /* DSI Enable */
#define	 WCR_LTDCEN	(1 << 2) /* LTDC Enable */
#define	 WCR_SHTDN	(1 << 1) /* Shutdown */
#define	 WCR_COLM	(1 << 0) /* Color mode */
#define	DSI_WIER	0x0408 /* DSI Wrapper Interrupt Enable Register */
#define	DSI_WISR	0x040C /* DSI Wrapper Interrupt & Status Register */
#define	 WISR_RRIF	(1 << 13) /* Regulator Ready Interrupt Flag */
#define	 WISR_RRS	(1 << 12) /* Regulator Ready Status */
#define	 WISR_PLLUIF	(1 << 10) /* PLL Unlock Interrupt Flag */
#define	 WISR_PLLLIF	(1 << 9) /* PLL Lock Interrupt Flag */
#define	 WISR_PLLLS	(1 << 8) /* PLL Lock Status */
#define	 WISR_BUSY	(1 << 2) /* Busy Flag */
#define	 WISR_ERIF	(1 << 1) /* End of Refresh Interrupt Flag */
#define	 WISR_TEIF	(1 << 0) /* Tearing Effect Interrupt Flag */
#define	DSI_WIFCR	0x0410 /* DSI Wrapper Interrupt Flag Clear Register */
#define	DSI_WPCR0	0x0418 /* DSI Wrapper PHY Configuration Register 0 */
#define	 WPCR0_UIX4_S	0 /* Unit Interval multiplied by 4 */
#define	 WPCR0_UIX4_M	0x3f
#define	DSI_WPCR1	0x041C /* DSI Wrapper PHY Configuration Register 1 */
#define	DSI_WPCR2	0x0420 /* DSI Wrapper PHY Configuration Register 2 */
#define	DSI_WPCR3	0x0424 /* DSI Wrapper PHY Configuration Register 3 */
#define	DSI_WPCR4	0x0428 /* DSI Wrapper PHY Configuration Register 4 */
#define	DSI_WRPCR	0x0430 /* DSI Wrapper Regulator and PLL Control Register */
#define	 WRPCR_REGEN	(1 << 24)	/* Regulator Enable */
#define	 WRPCR_ODF_S	16		/* PLL Output Division Factor */
#define	 WRPCR_ODF_M	0x3
#define	 WRPCR_IDF_S	11		/* PLL Input Division Factor */
#define	 WRPCR_IDF_M	0xf
#define	 WRPCR_NDIV_S	2		/* PLL Loop Division Factor */
#define	 WRPCR_NDIV_M	0x7f
#define	 WRPCR_PLLEN	(1 << 0)	/* PLL Enable */

struct stm32f4_dsi_softc {
	uint32_t base;
};

int stm32f4_dsi_init(struct stm32f4_dsi_softc *sc,
    dsi_device_t *dsi_dev, uint32_t base);
void stm32f4_dsi_setup(struct stm32f4_dsi_softc *sc,
    const struct layer_info *);

#endif /* !_ARM_STM_STM32F4_DSI_H_ */
