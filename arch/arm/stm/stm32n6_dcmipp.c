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
#include <arm/stm/stm32n6_dcmipp.h>

#define	N_COLOR_PROFILES	5 /* 2856, 4000, 5000, 6500, 0 */

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
stm32n6_dcmipp_status(struct stm32n6_dcmipp_softc *sc)
{

	printf("%s: pipe0 status %x\n", __func__, RD4(sc, DCMIPP_P0SR));
	printf("%s: pipe1 status %x\n", __func__, RD4(sc, DCMIPP_P1SR));
	printf("%s: common status1 %x\n", __func__, RD4(sc, DCMIPP_CMSR1));
	printf("%s: common status2 %x\n", __func__, RD4(sc, DCMIPP_CMSR2));
	printf("%s: frame cnt %d\n", __func__, RD4(sc, DCMIPP_CMFRCR));
	printf("%s: p0 cfscr %x\n", __func__, RD4(sc, DCMIPP_P0CFSCR));
	printf("%s: p0 m0ar1 %x\n", __func__, RD4(sc, DCMIPP_P0CPPM0AR1));
}

void
stm32n6_dcmipp_setup_downsize(struct stm32n6_dcmipp_softc *sc,
    struct stm32n6_dcmipp_downsize_config *conf)
{
	uint32_t reg;

	/* Downsize configuration. */

	reg = conf->hratio << P1DSRTIOR_HRATIO_S;
	reg |= conf->vratio << P1DSRTIOR_VRATIO_S;
	WR4(sc, DCMIPP_P1DSRTIOR, reg);

	reg = conf->hsize << P1DSSZR_HSIZE_S;
	reg |= conf->vsize << P1DSSZR_VSIZE_S;
	WR4(sc, DCMIPP_P1DSSZR, reg);

	reg = conf->hdivfactor << P1DSCR_HDIV_S;
	reg |= conf->vdivfactor << P1DSCR_VDIV_S;
	reg |= P1DSCR_ENABLE;
	WR4(sc, DCMIPP_P1DSCR, reg);
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

static int32_t coeff[N_COLOR_PROFILES][3][3] =  {
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
	{ { 0, 0, 0, },
	  { 0, 0, 0, },
	  { 0, 0, 0, }, },
};
static uint32_t ispGainR[N_COLOR_PROFILES] = \
	{ 140000000, 177000000, 220000000, 245000000, 0, };
static uint32_t ispGainG[N_COLOR_PROFILES] = \
	{ 100000000, 100000000, 100000000, 100000000, 0, };
static uint32_t ispGainB[N_COLOR_PROFILES] = \
	{ 275000000, 235000000, 180000000, 155000000, 0, };

static void
stm32n6_dcmipp_exposure(struct stm32n6_dcmipp_softc *sc, int i)
{
	uint32_t reg;
	uint8_t s, m;

	to_sm(ispGainG[i], &s, &m);
	reg = s << EXCR2_SHFG_S | m << EXCR2_MULTG_S;
	to_sm(ispGainB[i], &s, &m);
	reg |= s << EXCR2_SHFB_S | m << EXCR2_MULTB_S;
	WR4(sc, DCMIPP_P1EXCR2, reg);

	to_sm(ispGainR[i], &s, &m);
	reg = s << EXCR1_SHFR_S | m << EXCR1_MULTR_S | EXCR1_ENABLE;
	WR4(sc, DCMIPP_P1EXCR1, reg);
}

static void
stm32n6_dcmipp_color_conv(struct stm32n6_dcmipp_softc *sc, int i)
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

	WR4(sc, DCMIPP_P1CCRR1, rg << CCRR1_RG_S | rr << CCRR1_RR_S);
	WR4(sc, DCMIPP_P1CCRR2,  0 << CCRR2_RA_S | rb << CCRR2_RB_S);
	WR4(sc, DCMIPP_P1CCGR1, gg << CCGR1_GG_S | gr << CCGR1_GR_S);
	WR4(sc, DCMIPP_P1CCGR2,  0 << CCGR2_GA_S | gb << CCGR2_GB_S);
	WR4(sc, DCMIPP_P1CCBR1, bg << CCBR1_BG_S | br << CCBR1_BR_S);
	WR4(sc, DCMIPP_P1CCBR2,  0 << CCBR2_BA_S | bb << CCBR2_BB_S);
	WR4(sc, DCMIPP_P1CCCR, CCCR_ENABLE | CCCR_TYPE);
}

void
stm32n6_dcmipp_setup(struct stm32n6_dcmipp_softc *sc)
{
	uint32_t reg;
	int profile_id;

	/* Disable parallel interface. */
	WR4(sc, DCMIPP_PRCR, 0);

	/* Switch input to CSI2. */
	WR4(sc, DCMIPP_CMCR, CMCR_INSEL_CSI2 | CMCR_PSFC_PIPE1);
	WR4(sc, DCMIPP_P1PPM0PR, 1600 << P1PPM0PR_PITCH_S);

	/* Format and gamma */
	WR4(sc, DCMIPP_P1PPCR, P1PPCR_FORMAT_RGB565);
	WR4(sc, DCMIPP_P1GMCR, P1GMCR_EN);

	/* Debayering / demosaicing. */
	reg = DMCR_EN;
	reg |= 2 << DMCR_PEAK_S;
	reg |= 4 << DMCR_LINEV_S;
	reg |= 4 << DMCR_LINEH_S;
	reg |= 6 << DMCR_EDGE_S;
	reg |= DMCR_TYPE_RGGB;
	WR4(sc, DCMIPP_P1DMCR, reg);

	/* Black Level Calibration. */
	reg = BLCCR_ENABLE;
	reg |= 12 << BLCCR_BLCR_S;
	reg |= 12 << BLCCR_BLCG_S;
	reg |= 12 << BLCCR_BLCB_S;
	WR4(sc, DCMIPP_P1BLCCR, reg);

	/* Color Conversion */
	profile_id = 1;
	stm32n6_dcmipp_color_conv(sc, profile_id);
	stm32n6_dcmipp_exposure(sc, profile_id);

/* TODO */
#define	IMX335_RAW10	0x2B
#define	IMX335_RAW12	0x2C

	/* Start Capture */
	WR4(sc, DCMIPP_P1PPM0AR1, 0x34200000);
	reg = P1FSCR_PIPEN;
	reg |= 0 << P1FSCR_VC_S;
	reg |= IMX335_RAW10 << P1FSCR_DTIDA_S;
	WR4(sc, DCMIPP_P1FSCR, reg);
	WR4(sc, DCMIPP_P1FCTCR, P1FCTCR_CPTREQ);
}

void
stm32n6_dcmipp_init(struct stm32n6_dcmipp_softc *sc, uint32_t base)
{

	sc->base = base;
}
