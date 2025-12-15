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

/* Test setup */

void
stm32n6_dcmipp_setup(struct stm32n6_dcmipp_softc *sc)
{
	uint32_t reg;

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
	reg |= 7 << DMCR_PEAK_S;
	reg |= 7 << DMCR_LINEV_S;
	reg |= 7 << DMCR_LINEH_S;
	reg |= 7 << DMCR_EDGE_S;
	reg |= DMCR_TYPE_RGGB;
	WR4(sc, DCMIPP_P1DMCR, reg);

/* TODO */
#define	IMX335_RAW10	0x2b
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
