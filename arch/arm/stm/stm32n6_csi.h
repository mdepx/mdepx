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

#ifndef _ARM_STM_STM32N6_CSI_H_
#define _ARM_STM_STM32N6_CSI_H_

/* CSI-2 Host */

#define	CSI_CR		0x00
#define	 CR_VC1START	(1 << 6)
#define	 CR_VC0START	(1 << 2) /* Virtual channel 0 start */
#define	 CR_CSIEN	(1 << 0) /* CSI-2 enable */
#define	CSI_PCR		0x04
#define	 PCR_DL1EN	(1 << 3) /* D-PHY_RX data lane 1 enable */
#define	 PCR_DL0EN	(1 << 2) /* D-PHY_RX data lane 0 enable */
#define	 PCR_CLEN	(1 << 1) /* Clock lane enable */
#define	 PCR_PWRDOWN	(1 << 0) /* Power down */
#define	CSI_VCxCFGR1(x)	(0x10 + 0x10 * (x))
#define	 VCxCFGR1_ALLDT	(1 << 0) /* All data types enable for this channel */
#define	CSI_VCxCFGR2(x)	(0x14 + 0x10 * (x))
#define	CSI_VCxCFGR3(x)	(0x18 + 0x10 * (x))
#define	CSI_VCxCFGR4(x)	(0x1C + 0x10 * (x))
#define	CSI_LBxCFGR(x)	(0x50 + 0x04 * (x))
#define	CSI_TIMxCFGR(x)	(0x60 + 0x04 * (x))
#define	CSI_LMCFGR	0x70
#define	CSI_PRGITR	0x74
#define	CSI_WDR		0x78
#define	CSI_IER0	0x80
#define	CSI_IER1	0x84
#define	CSI_SR0		0x90
#define	CSI_SR1		0x94
#define	CSI_FCR0	0x100
#define	CSI_FCR1	0x104
#define	CSI_SPDFR	0x110
#define	CSI_ERR1	0x114
#define	CSI_ERR2	0x118

/* Phy Registers */

#define	CSI_PRCR	0x1000
#define	 PRCR_PEN	(1 << 1)
#define	CSI_PMCR	0x1004
#define	CSI_PFCR	0x1008
#define	 PFCR_DLD_TX	(1 << 16) /* Data lane direction of lane 0 */
#define	CSI_PTCR0	0x1010
#define	 PTCR0_TCKEN	(1 << 0) /* Test-interface clken for the TDI bus */
#define	CSI_PTCR1	0x1014
#define	 PTCR1_TWM	(1 << 16) /* Test-interface write mode selector */
#define	CSI_PTSR	0x1018

struct stm32n6_csi_softc {
	uint32_t base;
};

void stm32n6_csi_init(struct stm32n6_csi_softc *sc, uint32_t base);
void stm32n6_csi_status(struct stm32n6_csi_softc *sc);

#endif /* !_ARM_STM_STM32N6_CSI_H_ */
