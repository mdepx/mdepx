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

#ifndef _ARM_STM_STM32F4_FMC_H_
#define _ARM_STM_STM32F4_FMC_H_

#define	FMC_BCR1	0x00 /*   */
#define	FMC_BCR2	0x08 /*   */
#define	FMC_BCR3	0x10 /*   */
#define	FMC_BCR4	0x18 /*   */
#define	FMC_BTR1	0x04 /*   */
#define	FMC_BTR2	0x0C /*   */
#define	FMC_BTR3	0x14 /*   */
#define	FMC_BTR4	0x1C /*   */
#define	FMC_BWTR1	0x104 /*   */
#define	FMC_BWTR2	0x10C /*   */
#define	FMC_BWTR3	0x104 /*   */
#define	FMC_BWTR4	0x10C /*   */
#define	FMC_PCR2	0x60 /*   */
#define	FMC_PCR3	0x80 /*   */
#define	FMC_PCR4	0xA0 /*   */
#define	FMC_SR2		0x64 /*   */
#define	FMC_SR3		0x84 /*   */
#define	FMC_SR4		0xA4 /*   */
#define	FMC_PMEM2	0x68 /*   */
#define	FMC_PMEM3	0x88 /*   */
#define	FMC_PMEM4	0xA8 /*   */
#define	FMC_PATT2	0x6C /*   */
#define	FMC_PATT3	0x8C /*   */
#define	FMC_PATT4	0xAC /*   */
#define	FMC_PIO4	0xB0 /*   */
#define	FMC_ECCR2	0x74 /*   */
#define	FMC_ECCR3	0x94 /*   */
#define	FMC_SDCR_1	0x140 /*   */
#define	FMC_SDCR_2	0x144 /*   */
#define	 SDCR_WP	(1 << 9) /* Write protection */
#define	 RPIPE_S	13
#define	 RPIPE_M	0x3
#define	 RBURST_S	12
#define	 RPIPE_1	1 /* One HCLK clock cycle delay */
#define	 RPIPE_0	0 /* No delay */
#define	 SDCLK_S	10
#define	 SDCLK_M	0x3
#define	 SDCLK_2	2 /* 2 x HCLK periods */
#define	 CAS_S		7
#define	 CAS_M		0x3
#define	 CAS_3		3
#define	 CAS_2		2
#define	 NB_S		6
#define	 NB_1		0
#define	 NB_4		1
#define	 MWID_S		4
#define	 MWID_M		0x3
#define	 MWID_8		0
#define	 MWID_16	1
#define	 MWID_32	2
#define	 NR_S		2
#define	 NR_M		0x3
#define	 NR_12		1
#define	 NR_13		2
#define	 NC_S		0
#define	 NC_M		0x3
#define	 NC_8		0
#define	 NC_9		1
#define	 NC_10		2
#define	FMC_SDTR1	0x148 /*   */
#define	FMC_SDTR2	0x14C /*   */
#define	 TRCD_S		24
#define	 TRP_S		20
#define	 TWR_S		16
#define	 TRC_S		12
#define	 TRAS_S		8
#define	 TXSR_S		4
#define	 TMRD_S		0
#define	FMC_SDCMR			0x150 /*   */
#define	 SDCMR_CTB1			(1 << 4) /* Command Target Bank 1 */
#define	 SDCMR_CTB2			(1 << 3) /* Command Target Bank 2 */
#define	 SDCMR_MODE_S			0
#define	 SDCMR_MODE_CLKEN		1
#define	 SDCMR_MODE_PALL		2
#define	 SDCMR_MODE_AUTO_REFRESH	3
#define	 SDCMR_MODE_LOAD		4
#define	 SDCMR_MODE_SELF_REFRESH	5
#define	 SDCMR_MODE_PWR_DOWN		6
#define	 SDCMR_NRFS_S			5
#define	 SDCMR_MRD_S			9
#define	 SDCMR_MRD_BURST_LEN_1		0x0
#define	 SDCMR_MRD_BURST_LEN_2		0x1
#define	 SDCMR_MRD_BURST_LEN_4		0x2
#define	 SDCMR_MRD_BURST_LEN_8		0x4
#define	 SDCMR_MRD_BURST_TYP_SEQ	0x0
#define	 SDCMR_MRD_BURST_TYP_INT	0x8
#define	 SDCMR_MRD_CAS_S		4
#define	 SDCMR_MRD_CAS_2		0x20
#define	 SDCMR_MRD_CAS_3		0x30
#define	 SDCMR_MRD_WBURST_SINGLE	0x200
#define	FMC_SDRTR			0x154 /*   */
#define	FMC_SDSR			0x158 /*   */
#define	 SDSR_BUSY			(1 << 5)

struct sdcr_reg {
	uint16_t rpipe:2;
	uint16_t rburst:1;	/* Burst read */
	uint16_t sdclk:2;	/* SDRAM clock configuration */
	uint16_t wp:1;		/* Write protection */
	uint16_t cas:2;		/* CAS Latency */
	uint16_t nb:1;		/* Number of internal banks */
	uint16_t mwid:2;	/* Memory data bus width */
	uint16_t nr:2;		/* Number of row address bits */
	uint16_t nc:2;		/* Number of column address bits */
};

struct sdtr_reg {
	uint32_t trcd:4;	/* Row to column delay */
	uint32_t trp:4;		/* Row precharge delay */
	uint32_t twr:4;		/* Recovery delay */
	uint32_t trc:4;		/* Row cycle delay */
	uint32_t tras:4;	/* Self refresh time */
	uint32_t txsr:4;	/* Exit Self-refresh delay */
	uint32_t tmrd:4;	/* Load Mode Register to Active */
};

struct sdcmr_reg {
	uint32_t burstlen;
};

struct sdram_bank {
	struct sdcr_reg sdcr;
	struct sdtr_reg sdtr;
	struct sdcmr_reg sdcmr;
};

struct sdram {
	uint16_t sdrtr;
	uint8_t nrfs;
	struct sdram_bank bank1;
	struct sdram_bank bank2;
};

struct stm32f4_fmc_softc {
	uint32_t base;
};

int stm32f4_fmc_init(struct stm32f4_fmc_softc *sc, uint32_t base);
void stm32f4_fmc_setup(struct stm32f4_fmc_softc *sc, const struct sdram *);

#endif /* !_ARM_STM_STM32F4_FMC_H_ */
