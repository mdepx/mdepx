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

#ifndef _RISCV_GIGADEVICE_GD32V_RCU_H_
#define _RISCV_GIGADEVICE_GD32V_RCU_H_

#define	RCU_CTL		0x00	/* Control register */
#define	 CTL_PLL2STB	(1 << 29) /* PLL2 Clock Stabilization Flag */
#define	 CTL_PLL2EN	(1 << 28) /* PLL2 enable */
#define	 CTL_PLL1STB	(1 << 27) /* PLL1 Clock Stabilization Flag */
#define	 CTL_PLL1EN	(1 << 26) /* PLL1 enable */
#define	 CTL_PLLSTB	(1 << 25) /* PLL Clock Stabilization Flag */
#define	 CTL_PLLEN	(1 << 24) /* PLL enable */
#define	 CTL_CKMEN	(1 << 19) /* HXTAL Clock Monitor Enable */
#define	 CTL_HXTALBPS	(1 << 18) /* HXTAL clock bypass mode enable */
#define	 CTL_HXTALSTB	(1 << 17) /* High speed crystal oscillator (HXTAL) clock stabilization flag */
#define	 CTL_HXTALEN	(1 << 16) /* High Speed crystal oscillator (HXTAL) En*/
#define	 CTL_IRC8MEN	(1 << 0) /* Internal 8MHz RC oscillator Enable */
#define	RCU_CFG0	0x04	/* Clock configuration register 0 */
#define	 CFG0_SCSS_S	2 /* System clock switch status */
#define	 CFG0_SCSS_M		(0x3 << CFG0_SCSS_S)
#define	 CFG0_SCSS_IRC8M	(0 << CFG0_SCSS_S)
#define	 CFG0_SCSS_HXTAL	(1 << CFG0_SCSS_S)
#define	 CFG0_SCSS_CK_PLL	(2 << CFG0_SCSS_S)
#define	 CFG0_SCS_S		0	/* System clock switch */
#define	 CFG0_SCS_M		(0x3 << CFG0_SCS_S)
#define	 CFG0_SCS_IRC8M		(0 << CFG0_SCS_S)
#define	 CFG0_SCS_HXTAL		(1 << CFG0_SCS_S)
#define	 CFG0_SCS_CK_PLL	(2 << CFG0_SCS_S)
#define	 CFG0_PLLMF_S	18	/* The PLL clock multiplication factor */
#define	 CFG0_PLLMF_M	(0xf << CFG0_PLLMF_S)
#define	 CFG0_PLLMF4	(1 << 29) /* Bit 4 of PLLMF */
#define	 CFG0_AHBPSC_CK_SYS_S		4 /* AHB prescaler selection */
#define	 CFG0_AHBPSC_CK_SYS_M		(0xf << CFG0_AHBPSC_CK_SYS_S)
#define	 CFG0_AHBPSC_CK_SYS_DIV_1	(0 << CFG0_AHBPSC_CK_SYS_S)
#define	 CFG0_AHBPSC_CK_SYS_DIV_2	(8 << CFG0_AHBPSC_CK_SYS_S)
#define	 CFG0_APB2PSC_CK_AHB_S		11 /* APB2 prescaler selection */
#define	 CFG0_APB2PSC_CK_AHB_M		(0x7 << CFG0_APB2PSC_CK_AHB_S)
#define	 CFG0_APB2PSC_CK_AHB_DIV_1	(0 << CFG0_APB2PSC_CK_AHB_S)
#define	 CFG0_APB2PSC_CK_AHB_DIV_2	(4 << CFG0_APB2PSC_CK_AHB_S)
#define	 CFG0_APB1PSC_CK_AHB_S		8 /* APB1 prescaler selection */
#define	 CFG0_APB1PSC_CK_AHB_M		(0x7 << CFG0_APB1PSC_CK_AHB_S)
#define	 CFG0_APB1PSC_CK_AHB_DIV_1	(0 << CFG0_APB1PSC_CK_AHB_S)
#define	 CFG0_APB1PSC_CK_AHB_DIV_2	(4 << CFG0_APB1PSC_CK_AHB_S)
#define	RCU_INT		0x08	/* Clock interrupt register */
#define	RCU_APB2RST	0x0C	/* APB2 reset register */
#define	RCU_APB1RST	0x10	/* APB1 reset register */
#define	RCU_AHBEN	0x14	/* AHB enable register */
#define	 AHBEN_DMA1EN	(1 << 1) /* DMA1 clock enable */
#define	 AHBEN_DMA0EN	(1 << 0) /* DMA0 clock enable */
#define	RCU_APB2EN	0x18	/* APB2 enable register */
#define	 APB2EN_USART0EN	(1 << 14)	/* USART0 clock enable */
#define	 APB2EN_SPI0EN		(1 << 12)	/* SPI0 clock enable */
#define	 APB2EN_TIMER0EN	(1 << 11)	/* TIMER0 clock enable */
#define	 APB2EN_PAEN		(1 << 2)	/* GPIO port A clock enable */
#define	 APB2EN_PBEN		(1 << 3)	/* GPIO port B clock enable */
#define	 APB2EN_PCEN		(1 << 4)	/* GPIO port B clock enable */
#define	 APB2EN_PDEN		(1 << 5)	/* GPIO port B clock enable */
#define	 APB2EN_PEEN		(1 << 6)	/* GPIO port B clock enable */
#define	 APB2EN_AFEN		(1 << 0) /* Alternate function IO clk enable */
#define	RCU_APB1EN	0x1C	/* APB1 enable register */
#define	 APB1EN_I2C1EN		(1 << 22) /* I2C1 clock enable */
#define	 APB1EN_I2C0EN		(1 << 21) /* I2C0 clock enable */
#define	 APB1EN_TIMER1EN	(1 << 0) /* TIMER1 clock enable */
#define	RCU_BDCTL	0x20	/* Backup domain control register */
#define	RCU_RSTSCK	0x24	/* Reset source/clock register */
#define	RCU_AHBRST	0x28	/* AHB reset register */
#define	RCU_CFG1	0x2C	/* Clock configuration register 1 */
#define	 CFG1_PLL2MF_S		12 /* The PLL2 clock multiplication factor */
#define	 CFG1_PLL2MF_M		(0xf << CFG1_PLL2MF_S)
#define	 CFG1_PLL2MF_20		(0xf << CFG1_PLL2MF_S)
#define	 CFG1_PLL1MF_S		8 /* The PLL1 clock multiplication factor */
#define	 CFG1_PLL1MF_M		(0xf << CFG1_PLL1MF_S)
#define	 CFG1_PLL1MF_20		(0xf << CFG1_PLL1MF_S)
#define	 CFG1_PREDV1_S		4 /* PREDV1 division factor */
#define	 CFG1_PREDV1_M		(0xf << CFG1_PREDV1_S)
#define	 CFG1_PREDV1_DIV_1	(0 << CFG1_PREDV1_S)
#define	 CFG1_PREDV1_DIV_2	(1 << CFG1_PREDV1_S)
#define	 CFG1_PREDV0_S		0	/* PREDV0 division factor */
#define	 CFG1_PREDV0_M		(0xf << CFG1_PREDV0_S)
#define	 CFG1_PREDV0_DIV_1	(0 << CFG1_PREDV0_S)
#define	 CFG1_PREDV0_DIV_2	(1 << CFG1_PREDV0_S)
#define	RCU_DSV		0x34	/* Deep-sleep mode voltage register */

struct gd32v_rcu_softc {
	uint32_t base;
};

int gd32v_rcu_init(mdx_device_t dev, uint32_t base);
void gd32v_rcu_setup(mdx_device_t dev,
    uint32_t ahben, uint32_t apb1en, uint32_t apb2en);

#endif /* !_RISCV_GIGADEVICE_GD32V_RCU_H_ */
