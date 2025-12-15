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

#include <sys/cdefs.h>
#include <sys/systm.h>
#include <arm/stm/stm32n6_csi.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

/* Synopsis IP frequencies table */

typedef struct {
	uint32_t hsfreqrange;
	uint32_t osc_freq_target;
} snps_phy_freq;

const snps_phy_freq snps_freq[63] = {
	{ 0x00U, 460U },  /* BW_80 */
	{ 0x10U, 460U },  /* BW_90 */
	{ 0x20U, 460U },  /* BW_100 */
	{ 0x30U, 460U },  /* BW_110 */
	{ 0x01U, 460U },  /* BW_120 */
	{ 0x11U, 460U },  /* BW_130 */
	{ 0x21U, 460U },  /* BW_140 */
	{ 0x31U, 460U },  /* BW_150 */
	{ 0x02U, 460U },  /* BW_160 */
	{ 0x12U, 460U },  /* BW_170 */
	{ 0x22U, 460U },  /* BW_180 */
	{ 0x32U, 460U },  /* BW_190 */
	{ 0x03U, 460U },  /* BW_205 */
	{ 0x13U, 460U },  /* BW_220 */
	{ 0x23U, 460U },  /* BW_235 */
	{ 0x33U, 460U },  /* BW_250 */
	{ 0x04U, 460U },  /* BW_275 */
	{ 0x14U, 460U },  /* BW_300 */
	{ 0x25U, 460U },  /* BW_325 */
	{ 0x35U, 460U },  /* BW_350 */
	{ 0x05U, 460U },  /* BW_400 */
	{ 0x16U, 460U },  /* BW_450 */
	{ 0x26U, 460U },  /* BW_500 */
	{ 0x37U, 460U },  /* BW_550 */
	{ 0x07U, 460U },  /* BW_600 */
	{ 0x18U, 460U },  /* BW_650 */
	{ 0x28U, 460U },  /* BW_700 */
	{ 0x39U, 460U },  /* BW_750 */
	{ 0x09U, 460U },  /* BW_800 */
	{ 0x19U, 460U },  /* BW_850 */
	{ 0x29U, 460U },  /* BW_900 */
	{ 0x3AU, 460U },  /* BW_950 */
	{ 0x0AU, 460U },  /* BW_1000 */
	{ 0x1AU, 460U },  /* BW_1050 */
	{ 0x2AU, 460U },  /* BW_1100 */
	{ 0x3BU, 460U },  /* BW_1150 */
	{ 0x0BU, 460U },  /* BW_1200 */
	{ 0x1BU, 460U },  /* BW_1250 */
	{ 0x2BU, 460U },  /* BW_1300 */
	{ 0x3CU, 460U },  /* BW_1350 */
	{ 0x0CU, 460U },  /* BW_1400 */
	{ 0x1CU, 460U },  /* BW_1450 */
	{ 0x2CU, 460U },  /* BW_1500 */
	{ 0x3DU, 285U },  /* BW_1550 */
	{ 0x0DU, 295U },  /* BW_1600 */
	{ 0x1DU, 304U },  /* BW_1650 */
	{ 0x2EU, 313U },  /* BW_1700 */
	{ 0x3EU, 322U },  /* BW_1750 */
	{ 0x0EU, 331U },  /* BW_1800 */
	{ 0x1EU, 341U },  /* BW_1850 */
	{ 0x2FU, 350U },  /* BW_1900 */
	{ 0x3FU, 359U },  /* BW_1950 */
	{ 0x0FU, 368U },  /* BW_2000 */
	{ 0x40U, 377U },  /* BW_2050 */
	{ 0x41U, 387U },  /* BW_2100 */
	{ 0x42U, 396U },  /* BW_2150 */
	{ 0x43U, 405U },  /* BW_2200 */
	{ 0x44U, 414U },  /* BW_2250 */
	{ 0x45U, 423U },  /* BW_2300 */
	{ 0x46U, 432U },  /* BW_2350 */
	{ 0x47U, 442U },  /* BW_2400 */
	{ 0x48U, 451U },  /* BW_2450 */
	{ 0x49U, 460U },  /* BW_2500 */
};

void
stm32n6_csi_status(struct stm32n6_csi_softc *sc)
{

	printf("%s: SR0 %x\n", __func__, RD4(sc, CSI_SR0));
	printf("%s: SR1 %x\n", __func__, RD4(sc, CSI_SR1));
	printf("%s: PTSR %x\n", __func__, RD4(sc, CSI_PTSR));
	printf("%s: SPDFR %x\n", __func__, RD4(sc, CSI_SPDFR));
}

static void
stm32n6_csi_phy_write(struct stm32n6_csi_softc *sc, uint32_t reg_msb,
    uint32_t reg_lsb, uint32_t val)
{
	uint32_t reg;

	reg = RD4(sc, CSI_PTCR1);
	reg |= PTCR1_TWM;
	WR4(sc, CSI_PTCR1, reg);

	reg = RD4(sc, CSI_PTCR0);
	reg |= PTCR0_TCKEN;
	WR4(sc, CSI_PTCR0, reg);

	reg = RD4(sc, CSI_PTCR1);
	reg |= PTCR1_TWM;
	WR4(sc, CSI_PTCR1, reg);

	WR4(sc, CSI_PTCR0, 0);
	WR4(sc, CSI_PTCR1, 0);

	reg = RD4(sc, CSI_PTCR1);
	reg |= reg_msb & 0xFF;
	WR4(sc, CSI_PTCR1, reg);

	reg = RD4(sc, CSI_PTCR0);
	reg |= PTCR0_TCKEN;
	WR4(sc, CSI_PTCR0, reg);

	WR4(sc, CSI_PTCR0, 0);

	reg = RD4(sc, CSI_PTCR1);
	reg |= PTCR1_TWM;
	WR4(sc, CSI_PTCR1, reg);

	reg = RD4(sc, CSI_PTCR0);
	reg |= PTCR0_TCKEN;
	WR4(sc, CSI_PTCR0, reg);

	reg = RD4(sc, CSI_PTCR1);
	reg |= PTCR1_TWM | (reg_lsb & 0xFF);
	WR4(sc, CSI_PTCR1, reg);

	WR4(sc, CSI_PTCR0, 0);
	WR4(sc, CSI_PTCR1, 0);

	reg = RD4(sc, CSI_PTCR1);
	reg |= val & 0xFF;
	WR4(sc, CSI_PTCR1, reg);

	reg = RD4(sc, CSI_PTCR0);
	reg |= PTCR0_TCKEN;
	WR4(sc, CSI_PTCR0, reg);

	WR4(sc, CSI_PTCR0, 0);
}

void
stm32n6_csi_init(struct stm32n6_csi_softc *sc, uint32_t base)
{
	uint16_t osc_freq;
	uint32_t reg;
	int freq_id;
	int f;

	sc->base = base;

	WR4(sc, CSI_CR, 0);

	/* NB: line merger config could go here, before CR_CSIEN */

	WR4(sc, CSI_CR, CR_CSIEN);

	WR4(sc, CSI_PRCR, 0);
	WR4(sc, CSI_PCR, 0);
	WR4(sc, CSI_PTCR0, PTCR0_TCKEN);

	/* No sleep delay before scheduler. */
	udelay(1000);

	WR4(sc, CSI_PTCR0, 0);

	f = 28;	/* 24MHz */
	freq_id = 44;

	reg = f << PFCR_CCFR_S | snps_freq[freq_id].hsfreqrange << PFCR_HSFR_S;
	WR4(sc, CSI_PFCR, reg);

	osc_freq = snps_freq[freq_id].osc_freq_target;

	stm32n6_csi_phy_write(sc, 0x00, 0x08, 0x38);
	stm32n6_csi_phy_write(sc, 0x00, 0xe4, 0x11);
	stm32n6_csi_phy_write(sc, 0x00, 0xe3, osc_freq >> 8);
	stm32n6_csi_phy_write(sc, 0x00, 0xe3, osc_freq & 0xFF);

	reg = RD4(sc, CSI_PFCR);
	reg |= PFCR_DLD_TX;
	WR4(sc, CSI_PFCR, reg);

	/* Virtual channel 0 config */
	WR4(sc, CSI_VCxCFGR1(0), VCxCFGR1_ALLDT | (3 << 8));

	WR4(sc, CSI_PCR, PCR_CLEN | PCR_DL0EN | PCR_DL1EN | PCR_PWRDOWN);
	WR4(sc, CSI_PRCR, PRCR_PEN);
	WR4(sc, CSI_PMCR, 0);

	/* Virtual channel 0 start */
	WR4(sc, CSI_CR, CR_VC0START | CR_CSIEN);
}
