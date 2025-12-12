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

#ifndef _ARM_STM_STM32N6_XSPI_H_
#define _ARM_STM_STM32N6_XSPI_H_

#define	XSPI_CR		0x000
#define	 CR_MSEL_S	30
#define	 CR_MSEL_M	(0x3 << CR_MSEL_S)
#define	 CR_FMODE_S	28
#define	 CR_FMODE_M	(0x3 << CR_FMODE_S)
#define	 CR_FMODE_MM	(0x3 << CR_FMODE_S) /* Memory-mapped mode */
#define	 CR_FMODE_IW	(0x0 << CR_FMODE_S) /* Indirect Write */
#define	 CR_FMODE_IR	(0x1 << CR_FMODE_S) /* Indirect Read */
#define	 CR_FMODE_ASP	(0x2 << CR_FMODE_S) /* Automatic Status Polling */
#define	 CR_FTHRES_S	8
#define	 CR_FTHRES_M	(0x3f << CR_FTHRES_S)
#define	 CR_TCEN	(1 << 3) /* Timeout counter enable */
#define	 CR_DMAEN	(1 << 2)
#define	 CR_EN		(1 << 0)
#define	XSPI_DCR1	0x008
#define	XSPI_DCR2	0x00C
#define	XSPI_DCR3	0x010
#define	XSPI_DCR4	0x014
#define	XSPI_SR		0x020
#define	XSPI_FCR	0x024
#define	XSPI_DLR	0x040
#define	XSPI_AR		0x048
#define	XSPI_DR		0x050
#define	XSPI_PSMKR	0x080
#define	XSPI_PSMAR	0x088
#define	XSPI_PIR	0x090
#define	XSPI_CCR	0x100
#define	 CCR_DQSE	(1 << 29) /* DQS enable */
#define	XSPI_TCR	0x108
#define	XSPI_IR		0x110
#define	XSPI_ABR	0x120
#define	XSPI_LPTR	0x130
#define	XSPI_WPCCR	0x140
#define	XSPI_WPTCR	0x148
#define	XSPI_WPIR	0x150
#define	XSPI_WPABR	0x160
#define	XSPI_WCCR	0x180
#define	XSPI_WTCR	0x188
#define	XSPI_WIR	0x190
#define	XSPI_WABR	0x1A0
#define	XSPI_HLCR	0x200
#define	XSPI_CALFCR	0x210
#define	XSPI_CALMR	0x218
#define	XSPI_CALSOR	0x220
#define	XSPI_CALSIR	0x228

struct xspi_config {
	int buswidth;
};

struct stm32n6_xspi_softc {
	uint32_t base;
};

void stm32n6_xspi_setup(struct stm32n6_xspi_softc *sc, struct xspi_config *);
int stm32n6_xspi_init(struct stm32n6_xspi_softc *sc, uint32_t base);

#endif /* !_ARM_STM_STM32N6_XSPI_H_ */
