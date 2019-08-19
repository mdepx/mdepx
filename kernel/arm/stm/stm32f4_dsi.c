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

#include <sys/cdefs.h>
#include <dev/display/panel.h>
#include <dev/display/dsi.h>

#include <arm/stm/stm32f4_dsi.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static void
stm32f4_dsi_short(dsi_device_t *dev, uint8_t vchid,
    uint8_t data_type, uint8_t data0, uint8_t data1)
{
	struct stm32f4_dsi_softc *sc;
	uint32_t reg;

	sc = dev->arg;

	/* Wait FIFO empty flag */
	while ((RD4(sc, DSI_GPSR) & GPSR_CMDFE) != 1)
		;

	reg = (data_type << GHCR_DT_S);
	reg |= (vchid << GHCR_VCID_S);
	reg |= (data0 << GHCR_WCLSB_S);
	reg |= (data1 << GHCR_WCMSB_S);
	WR4(sc, DSI_GHCR, reg);
}

static void
stm32f4_dsi_long(dsi_device_t *dev, uint8_t vchid, uint32_t data_type,
    const uint8_t *params, uint32_t nparams)
{
	struct stm32f4_dsi_softc *sc;
	uint32_t data0;
	uint32_t data1;
	uint32_t reg;
	int i;

	sc = dev->arg;

	/* Wait FIFO empty flag */
	while ((RD4(sc, DSI_GPSR) & GPSR_CMDFE) != 1)
		;
	
	for (i = 0; i < nparams; i += 4) {
		reg = params[i];
		if ((i + 1) < nparams)
			reg |= (params[i + 1] << 8);
		if ((i + 2) < nparams)
			reg |= (params[i + 2] << 16);
		if ((i + 3) < nparams)
			reg |= (params[i + 3] << 24);
		WR4(sc, DSI_GPDR, reg);
	}

	data0 = nparams & 0xff;
	data1 = (nparams >> 8) & 0xff;

	reg = (data_type << GHCR_DT_S);
	reg |= (vchid << GHCR_VCID_S);
	reg |= (data0 << GHCR_WCLSB_S);
	reg |= (data1 << GHCR_WCMSB_S);
	WR4(sc, DSI_GHCR, reg);
}

void
stm32f4_dsi_setup(struct stm32f4_dsi_softc *sc,
    const struct layer_info *info)
{
	uint32_t HorizontalSyncActive;
	uint32_t HorizontalBackPorch;
	uint32_t HorizontalLine;
	uint32_t lane_byte_clk_khz;
	uint32_t lcd_clock;
	uint32_t val;
	int reg;

	printf("DSI version: %x\n", RD4(sc, DSI_VR));
 
	/* Enable the regulator */
	reg = RD4(sc, DSI_WRPCR);
	reg |= (WRPCR_REGEN);
	WR4(sc, DSI_WRPCR, reg);

	/* Wait ready */
	/* TODO: timeout ? */
	while ((RD4(sc, DSI_WISR) & WISR_RRS) == 0)
		continue;

	/* Configure PLL */
	reg = RD4(sc, DSI_WRPCR);
	reg &= ~(WRPCR_NDIV_M << WRPCR_NDIV_S);
	reg |= (125 << WRPCR_NDIV_S);
	reg &= ~(WRPCR_ODF_M << WRPCR_ODF_S);
	reg |= (0 << WRPCR_ODF_S);
	reg &= ~(WRPCR_IDF_M << WRPCR_IDF_S);
	reg |= (2 << WRPCR_IDF_S);
	WR4(sc, DSI_WRPCR, reg);

	/* Enable the PLL */
	reg = RD4(sc, DSI_WRPCR);
	reg |= (WRPCR_PLLEN);
	WR4(sc, DSI_WRPCR, reg);

	/* Wait the PLL to be ready */
	/* TODO: timeout ? */
	while ((RD4(sc, DSI_WISR) & WISR_PLLLS) == 0)
		continue;

	/* Enable PHY clock and digital data */
	reg = (PCTLR_CKE | PCTLR_DEN);
	WR4(sc, DSI_PCTLR, reg);

	/* Lanes configuration */
	reg = (CLCR_DPCC | CLCR_ACR); /* High speed, automatic lane control */
	WR4(sc, DSI_CLCR, reg);

	/* 2 lanes */
	reg = (PCONFR_NL_2 << PCONFR_NL_S);
	WR4(sc, DSI_PCONFR, reg);

	reg = RD4(sc, DSI_CCR);
	reg &= ~(CCR_TXECKDIV_M << CCR_TXECKDIV_S);
	reg |= (4 << CCR_TXECKDIV_S);
	WR4(sc, DSI_CCR, reg);

	/*
	 * Formula:
	 * tempIDF = (PLLIDF > 0) ? PLLIDF : 1;
	 * (4000000 * tempIDF * (1 << PLLODF)) / ((HSE_VALUE/1000) * PLLNDIV);
	 */

	val = (4000000 * 2 * (1 << 0)) / ((8000000/1000) * 125);

	reg = RD4(sc, DSI_WPCR0);
	reg &= ~(WPCR0_UIX4_M << WPCR0_UIX4_S);
	reg |= (val << WPCR0_UIX4_S);
	WR4(sc, DSI_WPCR0, reg);

	/* Video mode */
	reg = RD4(sc, DSI_MCR);
	reg &= ~(MCR_CMDM);	/* Video mode */
	WR4(sc, DSI_MCR, reg);

	reg = RD4(sc, DSI_WCFGR);
	reg &= ~(WCFGR_DSIM);	/* Video mode */
	WR4(sc, DSI_WCFGR, reg);

	reg = RD4(sc, DSI_VMCR);
	reg &= ~(VMCR_VMT_M << VMCR_VMT_S);
	reg |= (VMCR_VMT_BRST << VMCR_VMT_S);
	WR4(sc, DSI_VMCR, reg);

	reg = (info->width << VPCR_VPSIZE_S);
	WR4(sc, DSI_VPCR, reg);

	reg = (0 << VCCR_NUMC_S); /* Number of Chunks */
	WR4(sc, DSI_VCCR, reg);

	reg = (0xfff << VNPCR_NPSIZE_S);
	WR4(sc, DSI_VNPCR, reg);

	reg = (0 << LVCIDR_VCID_S); /* Virtual Channel ID */
	WR4(sc, DSI_LVCIDR, reg);

	reg = 0; /* All active high */
	WR4(sc, DSI_LPCR, reg);

	switch (info->bpp) {
	case 24:
		reg = (LCOLCR_COLC_24 << LCOLCR_COLC_S);
		break;
	case 16:
		reg = (LCOLCR_COLC_16_1 << LCOLCR_COLC_S);
		break;
	default:
		printf("Error: can't configure LCOLCR\n");
	}
	WR4(sc, DSI_LCOLCR, reg);

	reg = RD4(sc, DSI_WCFGR);
	reg &= ~(WCFGR_COLMUX_M << WCFGR_COLMUX_S);
	switch (info->bpp) {
	case 24:
		reg |= (COLMUX_24 << WCFGR_COLMUX_S);
		break;
	case 16:
		reg |= (COLMUX_16_1 << WCFGR_COLMUX_S);
		break;
	default:
		printf("Error: can't configure COLMUX\n");
	}
	WR4(sc, DSI_WCFGR, reg);

	/* TODO: calculate this */
	lane_byte_clk_khz = 62500;
	lcd_clock = 4000;

	HorizontalLine = ((info->width + info->hsync + info->hbp + info->hfp)
	    * lane_byte_clk_khz) / lcd_clock;
	HorizontalBackPorch = (info->hbp * lane_byte_clk_khz) / lcd_clock;
	HorizontalSyncActive = (info->hsync * lane_byte_clk_khz) / lcd_clock;

	WR4(sc, DSI_VHBPCR, HorizontalBackPorch);
	WR4(sc, DSI_VHSACR, HorizontalSyncActive);
	WR4(sc, DSI_VLCR, HorizontalLine);

	WR4(sc, DSI_VVSACR, info->vsync);	/* Vertical sync active */
	WR4(sc, DSI_VVBPCR, info->vbp);		/* Vertical back porch */
	WR4(sc, DSI_VVFPCR, info->vfp);		/* Vertical Front Porch */
	WR4(sc, DSI_VVACR, info->height);	/* Vertical Active */

	/* Low power configuration */
	reg = RD4(sc, DSI_VMCR);
	reg |= VMCR_LPCE;
	WR4(sc, DSI_VMCR, reg);

	reg = (64 << LPMCR_VLPSIZE_S);
	reg |= (64 << LPMCR_LPSIZE_S);
	WR4(sc, DSI_LPMCR, reg);

	reg = RD4(sc, DSI_VMCR);
	reg |= VMCR_LPHFPE;
	reg |= VMCR_LPHBPE;
	reg |= VMCR_LPVAE;
	reg |= VMCR_LPVFPE;
	reg |= VMCR_LPVBPE;
	reg |= VMCR_LPVSAE;
	/* reg |= VMCR_FBTAAE; */
	WR4(sc, DSI_VMCR, reg);

	/* Enable the DSI */
	RD4(sc, DSI_CR);
	reg |= CR_EN;
	WR4(sc, DSI_CR, reg);

	reg = RD4(sc, DSI_WCR);
	reg |= WCR_DSIEN;
	/* reg |= (1 << 0); */ /* 8 color mode */
	WR4(sc, DSI_WCR, reg);

	printf("DSI configuration done\n");

	reg = RD4(sc, DSI_WCR);
	reg |= WCR_LTDCEN;
	WR4(sc, DSI_WCR, reg);
}

int
stm32f4_dsi_init(struct stm32f4_dsi_softc *sc,
    dsi_device_t *dsi_dev, uint32_t base)
{

	sc->base = base;

	dsi_dev->dsi_short = stm32f4_dsi_short;
	dsi_dev->dsi_long = stm32f4_dsi_long;
	dsi_dev->arg = sc;

	return (0);
}
