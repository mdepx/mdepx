/*-
 * Copyright (c) 2018-2025 Ruslan Bukin <br@bsdpad.com>
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

#include <dev/display/panel.h>
#include <arm/stm/stm32n6_ltdc.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static void
stm32n6_ltdc_layer(struct stm32n6_ltdc_softc *sc,
    const struct layer_info *info, int i)
{
	int nbytes_per_pixel;
	uint32_t reg;

	printf("layer->base %x\n", info->base);

	reg = (info->hsync + info->hbp) |
	    ((info->hsync + info->hbp + info->width - 1) << 16);
	WR4(sc, LTDC_LWHPCR(i), reg);

	reg = (info->vsync + info->vbp) |
	    ((info->vsync + info->vbp + info->height - 1) << 16);
	WR4(sc, LTDC_LWVPCR(i), reg);

	if (info->bpp == 32)
		WR4(sc, LTDC_LPFCR(i), LPFCR_PF_ARGB8888);
	else if (info->bpp == 24)
		WR4(sc, LTDC_LPFCR(i), LPFCR_PF_RGB888);
	else if (info->bpp == 16)
		WR4(sc, LTDC_LPFCR(i), LPFCR_PF_RGB565);
	else
		panic("implement me");

	WR4(sc, LTDC_LCFBAR(i), info->base);
	WR4(sc, LTDC_LCFBLNR(i), info->height);
	nbytes_per_pixel = info->bpp / 8;
	reg = ((info->width * nbytes_per_pixel) << 16);
	reg |= ((info->width * nbytes_per_pixel) + 7);
	WR4(sc, LTDC_LCFBLR(i), reg);

	WR4(sc, LTDC_LBLCR(i), 0);
	WR4(sc, LTDC_LCACR(i), info->alpha);

	/* Enable the layer */
	WR4(sc, LTDC_LRCR(i), LRCR_IMR | LRCR_GRMSK);
	WR4(sc, LTDC_LxCR(i), LCR_LEN);
}

static void
stm32n6_ltdc_conf(struct stm32n6_ltdc_softc *sc,
    const struct layer_info *info, int i)
{
	uint32_t reg;
	uint32_t val;

	reg = ((info->hsync - 1) << SSCR_HSW_S);
	reg |= ((info->vsync - 1) << SSCR_VSW_S);
	WR4(sc, LTDC_SSCR, reg);

	reg = ((info->hsync + info->hbp - 1) << BPCR_AHBP_S);
	reg |= ((info->vsync + info->vbp - 1) << BPCR_AVBP_S);
	WR4(sc, LTDC_BPCR, reg);

	reg = ((info->vsync + info->vbp + info->height - 1) << AWCR_AAH_S);
	reg |= ((info->hsync + info->hbp + info->width - 1) << AWCR_AAW_S);
	WR4(sc, LTDC_AWCR, reg);

	val = info->vsync + info->vbp + info->height + info->vfp - 1;
	reg = val << TWCR_TOTALH_S;
	val = info->hsync + info->hbp + info->width + info->hfp - 1;
	reg |= val << TWCR_TOTALW_S;
	WR4(sc, LTDC_TWCR, reg);

	/* Background color */
	WR4(sc, LTDC_BCCR, 0);

	stm32n6_ltdc_layer(sc, info, i);

	WR4(sc, LTDC_SRCR, SRCR_IMR);
	WR4(sc, LTDC_LRCR(i), LRCR_IMR);

	reg = RD4(sc, LTDC_GCR);
	//reg |= (GCR_HSPOL | GCR_VSPOL | GCR_DEPOL | GCR_PCPOL);
	WR4(sc, LTDC_GCR, reg);

	reg = RD4(sc, LTDC_GCR);
	reg |= (GCR_LTDCEN);
	WR4(sc, LTDC_GCR, reg);

	WR4(sc, LTDC_SRCR, SRCR_IMR);
	WR4(sc, LTDC_LRCR(i), LRCR_IMR);
}

void
stm32n6_ltdc_setup(struct stm32n6_ltdc_softc *sc,
    const struct layer_info *info, uint8_t nlayers)
{
	int i;

	for (i = 0; i < nlayers; i++)
		stm32n6_ltdc_conf(sc, &info[i], i);
}

int
stm32n6_ltdc_init(struct stm32n6_ltdc_softc *sc, uint32_t base)
{

	sc->base = base;

	return (0);
}
