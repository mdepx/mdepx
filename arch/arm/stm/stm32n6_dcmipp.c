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
#include <sys/sem.h>

#include <arm/stm/stm32n6_dcmipp.h>

#define	N_COLOR_PROFILES	5 /* 2856, 4000, 5000, 6500, 0 */

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static mdx_sem_t frame_sem;

#define	dprintf(...)

void
stm32n6_dcmipp_intr(void *arg, int irq)
{
	struct stm32n6_dcmipp_softc *sc;
	int pipe;

	sc = arg;

	dprintf("%s\n", __func__);

	/* TODO */
	pipe = 2;

	WR4(sc, DCMIPP_PxFCR(pipe), PxFCR_CFRAMEF);

	mdx_sem_post(&frame_sem);
}

void
stm32n6_dcmipp_status(struct stm32n6_dcmipp_softc *sc)
{

#if 0
	printf("%s: pipe0 status %x\n", __func__, RD4(sc, DCMIPP_P0SR));
	printf("%s: pipe1 status %x\n", __func__, RD4(sc, DCMIPP_P1SR));
	printf("%s: common status1 %x\n", __func__, RD4(sc, DCMIPP_CMSR1));
	printf("%s: common status2 %x\n", __func__, RD4(sc, DCMIPP_CMSR2));
	printf("%s: frame cnt %d\n", __func__, RD4(sc, DCMIPP_CMFRCR));
	printf("%s: p0 cfscr %x\n", __func__, RD4(sc, DCMIPP_P0CFSCR));
	printf("%s: p0 m0ar1 %x\n", __func__, RD4(sc, DCMIPP_P0CPPM0AR1));
#endif
}

void
stm32n6_dcmipp_setup_downsize(struct stm32n6_dcmipp_softc *sc, int pipe,
    struct stm32n6_dcmipp_downsize_config *conf)
{
	uint32_t reg;

	/* Downsize configuration. */

	reg = conf->hratio << PxDSRTIOR_HRATIO_S;
	reg |= conf->vratio << PxDSRTIOR_VRATIO_S;
	WR4(sc, DCMIPP_PxDSRTIOR(pipe), reg);

	reg = conf->hsize << PxDSSZR_HSIZE_S;
	reg |= conf->vsize << PxDSSZR_VSIZE_S;
	WR4(sc, DCMIPP_PxDSSZR(pipe), reg);

	reg = conf->hdivfactor << PxDSCR_HDIV_S;
	reg |= conf->vdivfactor << PxDSCR_VDIV_S;
	reg |= PxDSCR_ENABLE;
	WR4(sc, DCMIPP_PxDSCR(pipe), reg);
}

static int16_t
cc_reg(int32_t coeff)
{
	int64_t val;
	uint16_t tmp;

	val = coeff;

	val = (val * 256) / 100000000;

	if (val < 0) {
		tmp = ((uint16_t)val ^ 0x7FFU) + 1U;
		val = (uint16_t)(-1 * (int16_t)(tmp));
	}

	return (int16_t)val;
}

static void
to_sm(uint32_t factor, uint8_t *s0, uint8_t *m0)
{
	uint64_t val;
	int s;

	val = factor;

	val = (val * 128) / 100000000;

	s = 0;
	while (val >= 256) {
		val /= 2;
		s++;
	}

	*s0 = s;
	*m0 = val;
}

#if 0
static int32_t coeff[N_COLOR_PROFILES][3][3] = {
	{ { 151460000, -102340000, 50892000, },
	  { -85991000, 210980000, -24984000, },
	  { 25000000, -261000000, 341000000, }, },
	{ { 155134500, -69370000, 13106000, },
	  { -38671000, 167689800, -33936000, },
	  { 5546200, -66770000, 159944200, }, },
	{ { 180080000, -64840000, -15230000, },
	  { -35550000, 169920000, -34380000, },
	  { 9770000, -95700000, 185940000, }, },
	{ { 180080000, -64840000, -15230000, },
	  { -35550000, 169920000, -34380000, },
	  { 9770000, -95700000, 185940000, }, },
	{ { 0, 0, 0, }, { 0, 0, 0, }, { 0, 0, 0, }, },
};
static uint32_t ispGainR[N_COLOR_PROFILES] = \
	{ 140000000, 177000000, 220000000, 245000000, 0, };
static uint32_t ispGainG[N_COLOR_PROFILES] = \
	{ 100000000, 100000000, 100000000, 100000000, 0, };
static uint32_t ispGainB[N_COLOR_PROFILES] = \
	{ 275000000, 235000000, 180000000, 155000000, 0, };
#else
static int32_t coeff[N_COLOR_PROFILES][3][3] = {
	{ { 159760000, -9780000, -49990000, },
	  { -45530000, 171540000, -26000000, },
	  { -3300000, -110120000, 213430000, }, },
	{ { 164670000, -20970000, -43700000, },
	  { -51330000, 178670000, -27339999, },
	  { -12490000, -48170000, 160670000, }, },
	{ { 150570000, 2440000, -53010000, },
	  { -37350000, 193760000, -56420000, },
	  { -11100000, -35490000, 146590000, }, },
	{ { 0, 0, 0, }, { 0, 0, 0, }, { 0, 0, 0, }, },
	{ { 0, 0, 0, }, { 0, 0, 0, }, { 0, 0, 0, }, },
};
static uint32_t ispGainR[N_COLOR_PROFILES] = \
	{ 137000000, 182000000, 244000000, 0, 0, };
static uint32_t ispGainG[N_COLOR_PROFILES] = \
	{ 100000000, 100000000, 100000000, 0, 0, };
static uint32_t ispGainB[N_COLOR_PROFILES] = \
	{ 287000000, 212000000, 143000000, 0, 0, };
#endif

static void
stm32n6_dcmipp_exposure(struct stm32n6_dcmipp_softc *sc, int pipe, int i)
{
	uint32_t reg;
	uint8_t s, m;

	to_sm(ispGainG[i], &s, &m);
	reg = s << EXCR2_SHFG_S | m << EXCR2_MULTG_S;
	to_sm(ispGainB[i], &s, &m);
	reg |= s << EXCR2_SHFB_S | m << EXCR2_MULTB_S;
	WR4(sc, DCMIPP_PxEXCR2(pipe), reg);

	to_sm(ispGainR[i], &s, &m);
	reg = s << EXCR1_SHFR_S | m << EXCR1_MULTR_S | EXCR1_ENABLE;
	WR4(sc, DCMIPP_PxEXCR1(pipe), reg);
}

static void
stm32n6_dcmipp_color_conv(struct stm32n6_dcmipp_softc *sc, int p, int i)
{
	int32_t rr, rg, rb;
	int32_t gr, gg, gb;
	int32_t br, bg, bb;

	rr = cc_reg(coeff[i][0][0]);
	rg = cc_reg(coeff[i][0][1]);
	rb = cc_reg(coeff[i][0][2]);
	gr = cc_reg(coeff[i][1][0]);
	gg = cc_reg(coeff[i][1][1]);
	gb = cc_reg(coeff[i][1][2]);
	br = cc_reg(coeff[i][2][0]);
	bg = cc_reg(coeff[i][2][1]);
	bb = cc_reg(coeff[i][2][2]);

	WR4(sc, DCMIPP_PxCCRR1(p), rg << CCRR1_RG_S | rr << CCRR1_RR_S);
	WR4(sc, DCMIPP_PxCCRR2(p),  0 << CCRR2_RA_S | rb << CCRR2_RB_S);
	WR4(sc, DCMIPP_PxCCGR1(p), gg << CCGR1_GG_S | gr << CCGR1_GR_S);
	WR4(sc, DCMIPP_PxCCGR2(p),  0 << CCGR2_GA_S | gb << CCGR2_GB_S);
	WR4(sc, DCMIPP_PxCCBR1(p), bg << CCBR1_BG_S | br << CCBR1_BR_S);
	WR4(sc, DCMIPP_PxCCBR2(p),  0 << CCBR2_BA_S | bb << CCBR2_BB_S);
	WR4(sc, DCMIPP_PxCCCR(p), CCCR_ENABLE | CCCR_TYPE);
}

void
stm32n6_dcmipp_setup(struct stm32n6_dcmipp_softc *sc,
    struct stm32n6_dcmipp_pipe_config *pconf)
{
	uint32_t reg;
	int profile_id;
	int i;

	i = pconf->pipe_id;

	reg = RD4(sc, DCMIPP_CMCR);
	switch (pconf->pipe_id) {
	case 1:
		reg |= CMCR_PSFC_PIPE1;
		break;
	case 2:
		reg |= CMCR_PSFC_PIPE2;
		break;
	};
	WR4(sc, DCMIPP_CMCR, reg);

	WR4(sc, DCMIPP_PxPPM0PR(i), pconf->pitch << PxPPM0PR_PITCH_S);
	WR4(sc, DCMIPP_PxPPM0AR1(i), pconf->base_addr);

	switch (pconf->format) {
	case DCMIPP_FORMAT_RGB565:
		WR4(sc, DCMIPP_PxPPCR(i), PxPPCR_FORMAT_RGB565);
		break;
	case DCMIPP_FORMAT_RGB888:
		WR4(sc, DCMIPP_PxPPCR(i), PxPPCR_FORMAT_RGB888_YUV444_1);
		break;
	};

	if (pconf->gamma_en)
		WR4(sc, DCMIPP_PxGMCR(i), PxGMCR_EN);

	if (pconf->debayer) {
		/* Debayering / demosaicing. */
		reg = DMCR_EN;
		reg |= 2 << DMCR_PEAK_S;
		reg |= 4 << DMCR_LINEV_S;
		reg |= 4 << DMCR_LINEH_S;
		reg |= 6 << DMCR_EDGE_S;
		reg |= DMCR_TYPE_RGGB;
		WR4(sc, DCMIPP_PxDMCR(i), reg);
	}

	if (pconf->black_level_calibration) {
		/* Black Level Calibration. */
		reg = BLCCR_ENABLE;
		reg |= 12 << BLCCR_BLCR_S;
		reg |= 12 << BLCCR_BLCG_S;
		reg |= 12 << BLCCR_BLCB_S;
		WR4(sc, DCMIPP_PxBLCCR(i), reg);
	}

	profile_id = 1;
	if (pconf->color_conv)
		stm32n6_dcmipp_color_conv(sc, i, profile_id);

	if (pconf->exposure)
		stm32n6_dcmipp_exposure(sc, i, profile_id);

#if 0
	/* TODO: crop. */
	WR4(sc, DCMIPP_PxCRSTR(i), 400 | 400 << 16);
	WR4(sc, DCMIPP_PxCRSZR(i), 480 | 480 << 16 | 0 << 31);
#endif

	/* Decimation */
	if (pconf->decimation)
		WR4(sc, DCMIPP_PxDCCR(i), 2 << 1 | 2 << 3 | 1 << 0);

	/* Start Capture */
	reg = PxFSCR_PIPEN;
	/* reg |= PxFSCR_PIPEDIFF; */
	reg |= pconf->vc << PxFSCR_VC_S;
	reg |= pconf->dtida << PxFSCR_DTIDA_S;
	WR4(sc, DCMIPP_PxFSCR(i), reg);

	switch (pconf->flow_type) {
	case DCMIPP_FLOW_TYPE_CONTINUOUS:
		/* Request transfer immediately. */
		WR4(sc, DCMIPP_PxFCTCR(i), PxFCTCR_CPTREQ);
		break;
	case DCMIPP_FLOW_TYPE_SNAPSHOT:
		WR4(sc, DCMIPP_PxFCTCR(i), PxFCTCR_CPTMODE_SS);
		break;
	}
}

void
stm32n6_dcmipp_pipe2_frame_request(struct stm32n6_dcmipp_softc *sc, int pipe)
{
	uint32_t reg;

	mdx_sem_init(&frame_sem, 0);

	WR4(sc, DCMIPP_PxIER(pipe), PxIER_FRAMEIE);

	reg = RD4(sc, DCMIPP_PxFCTCR(pipe));
	reg |= PxFCTCR_CPTREQ;
	WR4(sc, DCMIPP_PxFCTCR(pipe), reg);

	mdx_sem_wait(&frame_sem);

	WR4(sc, DCMIPP_PxIER(pipe), 0);
}

void
stm32n6_dcmipp_init(struct stm32n6_dcmipp_softc *sc, uint32_t base)
{

	sc->base = base;

	/* Disable parallel interface. */
	WR4(sc, DCMIPP_PRCR, 0);

	/* Switch input to CSI2. */
	WR4(sc, DCMIPP_CMCR, CMCR_INSEL_CSI2);
}
