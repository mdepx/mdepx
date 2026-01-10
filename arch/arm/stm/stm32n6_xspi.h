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
#define	 CR_MSEL_M		(0x3 << CR_MSEL_S)
#define	 CR_MSEL_IO_3_0		(0x0 << CR_MSEL_S)
#define	 CR_MSEL_IO_7_4		(0x1 << CR_MSEL_S)
#define	 CR_MSEL_IO_11_8	(0x2 << CR_MSEL_S)
#define	 CR_MSEL_IO_15_12	(0x3 << CR_MSEL_S)
#define	 CR_MSEL_IO_7_0		(0x0 << CR_MSEL_S)	/* Octal, Dual-Quad */
#define	 CR_MSEL_IO_15_8	(0x2 << CR_MSEL_S)	/* Octal, Dual-Quad */
#define	 CR_FMODE_S	28
#define	 CR_FMODE_M	(0x3 << CR_FMODE_S)
#define	 CR_FMODE_MM	(0x3 << CR_FMODE_S) /* Memory-mapped mode */
#define	 CR_FMODE_IW	(0x0 << CR_FMODE_S) /* Indirect Write */
#define	 CR_FMODE_IR	(0x1 << CR_FMODE_S) /* Indirect Read */
#define	 CR_FMODE_ASP	(0x2 << CR_FMODE_S) /* Automatic Status Polling */
#define	 CR_PMM_S	23 /* Polling match mode */
#define	 CR_PMM_M	(1 << CR_PMM_S)
#define	 CR_PMM_AND	(0 << CR_PMM_S)
#define	 CR_PMM_OR	(1 << CR_PMM_S)
#define	 CR_APMS	(1 << 22) /* Automatic status-polling mode stop */
#define	 CR_FTHRES_S	8
#define	 CR_FTHRES_M	(0x3f << CR_FTHRES_S)
#define	 CR_TCEN	(1 << 3) /* Timeout counter enable */
#define	 CR_DMAEN	(1 << 2)
#define	 CR_EN		(1 << 0)
#define	XSPI_DCR1	0x008
#define	 DCR1_MTYP_S		24
#define	 DCR1_MTYP_M		(0x7 << DCR1_MTYP_S)
#define	 DCR1_MTYP_MICRON	(0x0 << DCR1_MTYP_S)
#define	 DCR1_MTYP_MACRONIX	(0x1 << DCR1_MTYP_S)
#define	 DCR1_MTYP_STD		(0x2 << DCR1_MTYP_S)
#define	 DCR1_MTYP_MACRONIX_RAM	(0x3 << DCR1_MTYP_S)
#define	 DCR1_MTYP_HYPERBUS_MEM	(0x4 << DCR1_MTYP_S)
#define	 DCR1_MTYP_HYPERBUS_REG	(0x5 << DCR1_MTYP_S)
#define	 DCR1_MTYP_AP		(0x6 << DCR1_MTYP_S)
#define	 DCR1_DEVSIZE_S		16
#define	 DCR1_DEVSIZE_M		(0x1f << DCR1_DEVSIZE_S)
#define	 DCR1_DEVSIZE_256M	(27 << DCR1_DEVSIZE_S)
#define	 DCR1_DEVSIZE_128M	(26 << DCR1_DEVSIZE_S)
#define	 DCR1_DEVSIZE_32M	(24 << DCR1_DEVSIZE_S)
#define	 DCR1_CSHT_S		8
#define	 DCR1_CSHT_M		(0x3f << DCR1_CSHT_S)
#define	XSPI_DCR2	0x00C
#define	XSPI_DCR3	0x010
#define	 DCR3_CSBOUND_S		16
#define	 DCR3_CSBOUND_2KB	(11 << DCR3_CSBOUND_S)
#define	XSPI_DCR4	0x014
#define	XSPI_SR		0x020
#define	 SR_FLEVEL_S	8
#define	 SR_FLEVEL_M	(0x7f << SR_FLEVEL_S)
#define	 SR_BUSY	(1 << 5)
#define	 SR_SMF		(1 << 3) /* Status match flag */
#define	 SR_TCF		(1 << 1) /* Transfer complete flag */
#define	XSPI_FCR	0x024
#define	 FCR_CTCF	(1 << 1) /* Clear transfer complete flag */
#define	XSPI_DLR	0x040
#define	XSPI_AR		0x048
#define	XSPI_DR		0x050
#define	XSPI_PSMKR	0x080
#define	XSPI_PSMAR	0x088
#define	XSPI_PIR	0x090
#define	XSPI_CCR	0x100
#define	 CCR_DQSE	(1 << 29) /* DQS enable */
#define	 CCR_DDTR	(1 << 27) /* Data double transfer rate */
#define	 CCR_DMODE_S	24
#define	 CCR_DMODE_NONE	0
#define	 CCR_DMODE_1	(1 << CCR_DMODE_S)
#define	 CCR_DMODE_2	(2 << CCR_DMODE_S)
#define	 CCR_DMODE_4	(3 << CCR_DMODE_S)
#define	 CCR_DMODE_8	(4 << CCR_DMODE_S)
#define	 CCR_DMODE_16	(5 << CCR_DMODE_S)
#define	 CCR_ADSIZE_S	12
#define	 CCR_ADSIZE_8	(0 << CCR_ADSIZE_S)
#define	 CCR_ADSIZE_16	(1 << CCR_ADSIZE_S)
#define	 CCR_ADSIZE_24	(2 << CCR_ADSIZE_S)
#define	 CCR_ADSIZE_32	(3 << CCR_ADSIZE_S)
#define	 CCR_ADDTR	(1 << 11) /* Address double transfer rate */
#define	 CCR_ADMODE_S	8
#define	 CCR_ADMODE_NONE	(0 << CCR_ADMODE_S)
#define	 CCR_ADMODE_1		(1 << CCR_ADMODE_S)
#define	 CCR_ADMODE_2		(2 << CCR_ADMODE_S)
#define	 CCR_ADMODE_4		(3 << CCR_ADMODE_S)
#define	 CCR_ADMODE_8		(4 << CCR_ADMODE_S)
#define	 CCR_ISIZE_S	4
#define	 CCR_ISIZE_M	(3 << CCR_ISIZE_S)
#define	 CCR_ISIZE_8	(0 << CCR_ISIZE_S)
#define	 CCR_ISIZE_16	(1 << CCR_ISIZE_S)
#define	 CCR_ISIZE_24	(2 << CCR_ISIZE_S)
#define	 CCR_ISIZE_32	(3 << CCR_ISIZE_S)
#define	 CCR_IDTR	(1 << 3) /* Instruction double transfer rate */
#define	 CCR_IMODE_S	0
#define	 CCR_IMODE_NONE	(0 << CCR_IMODE_S)
#define	 CCR_IMODE_1	(1 << CCR_IMODE_S)
#define	 CCR_IMODE_2	(2 << CCR_IMODE_S)
#define	 CCR_IMODE_4	(3 << CCR_IMODE_S)
#define	 CCR_IMODE_8	(4 << CCR_IMODE_S)
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
	uint32_t instruction_write;
	uint32_t instruction_read;
	uint8_t dummy_cycles;
	uint8_t wdummy_cycles;
	uint8_t prescaler;
	uint8_t dqs_en;
	uint8_t wdqs_en;
	uint32_t mem_type;
	uint32_t dev_size;
	uint8_t cs_cycles;
	uint8_t data_dtr;
	uint8_t data_lines;
	uint8_t address_dtr;
	uint8_t address_size; /* bits */
	uint8_t address_lines;
	uint8_t instruction_dtr;
	uint8_t instruction_size; /* bits */
	uint8_t instruction_lines;
	uint32_t instruction;
	uint8_t mode;
#define	XSPI_MODE_INDIRECT_WRITE	0
#define	XSPI_MODE_INDIRECT_READ		1
#define	XSPI_MODE_AUTO_POLLING		2
#define	XSPI_MODE_MEMORY_MAPPED		3
};

struct stm32n6_xspi_poll_cfg {
	uint32_t match_value;
	uint32_t match_mask;
	uint32_t match_mode;
	uint32_t interval;
	uint32_t auto_stop;
	uint32_t instruction;
};

struct stm32n6_xspi_softc {
	uint32_t base;
	int dopi;
};

int stm32n6_xspi_setup(struct stm32n6_xspi_softc *sc, struct xspi_config *conf);
void stm32n6_xspi_init(struct stm32n6_xspi_softc *sc, uint32_t base);
int stm32n6_xspi_write_reg(struct stm32n6_xspi_softc *sc, int addr, int val);
int stm32n6_xspi_transmit(struct stm32n6_xspi_softc *sc, int addr, uint8_t *val,
    int len);
int stm32n6_xspi_receive(struct stm32n6_xspi_softc *sc, int addr, uint8_t *val,
    int len);
int stm32n6_xspi_autopoll(struct stm32n6_xspi_softc *sc,
    struct stm32n6_xspi_poll_cfg *poll_cfg);

#endif /* !_ARM_STM_STM32N6_XSPI_H_ */
