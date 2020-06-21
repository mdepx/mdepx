/*-
 * Copyright (c) 2020 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/systm.h>

#include "gd32v_rcu.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
gd32v_rcu_setup(mdx_device_t dev,
    uint32_t ahben, uint32_t apb1en, uint32_t apb2en)
{
	struct gd32v_rcu_softc *sc;

	sc = mdx_device_get_softc(dev);

	WR4(sc, RCU_AHBEN, ahben);
	WR4(sc, RCU_APB1EN, apb1en);
	WR4(sc, RCU_APB2EN, apb2en);
}

void
gd32v_rcu_dump(mdx_device_t dev)
{
	struct gd32v_rcu_softc *sc;

	sc = mdx_device_get_softc(dev);

	printf("%s: ctl %x\n", __func__, RD4(sc, RCU_CTL));
	printf("%s: cfg0 %x\n", __func__, RD4(sc, RCU_CFG0));
	printf("%s: cfg1 %x\n", __func__, RD4(sc, RCU_CFG1));
}

static void
gd32v_rcu_108mhz(struct gd32v_rcu_softc *sc)
{
	uint32_t reg;

	reg = RD4(sc, RCU_CTL);
	reg |= CTL_HXTALEN;
	WR4(sc, RCU_CTL, reg);

	do {
		reg = RD4(sc, RCU_CTL);
	} while ((reg & CTL_HXTALSTB) == 0);

	reg = RD4(sc, RCU_CFG0);
	reg &= ~CFG0_AHBPSC_CK_SYS_M;
	reg |= CFG0_AHBPSC_CK_SYS_DIV_1;
	reg |= CFG0_APB2PSC_CK_AHB_DIV_1;
	reg |= CFG0_APB1PSC_CK_AHB_DIV_2;
	WR4(sc, RCU_CFG0, reg);

	reg &= CFG0_PLLMF_M;
	reg &= CFG0_PLLMF4;
	WR4(sc, RCU_CFG0, reg);

	/* 8 MHz hxtal */
	reg = RD4(sc, RCU_CFG1);
	reg |= CFG1_PREDV0_DIV_2 | CFG1_PREDV1_DIV_2;
	reg |= CFG1_PLL1MF_20 | CFG1_PLL2MF_20;
	WR4(sc, RCU_CFG1, reg);

	reg = RD4(sc, RCU_CTL);
	reg |= CTL_PLL1EN;
	WR4(sc, RCU_CTL, reg);

	do {
		reg = RD4(sc, RCU_CTL);
	} while ((reg & CTL_PLL1STB) == 0);

	reg = RD4(sc, RCU_CTL);
	reg |= CTL_PLL2EN;
	WR4(sc, RCU_CTL, reg);

	do {
		reg = RD4(sc, RCU_CTL);
	} while ((reg & CTL_PLL2STB) == 0);

	reg = RD4(sc, RCU_CTL);
	reg |= CTL_PLLEN;
	WR4(sc, RCU_CTL, reg);

	do {
		reg = RD4(sc, RCU_CTL);
	} while ((reg & CTL_PLLSTB) == 0);

	reg = RD4(sc, RCU_CFG0);
	reg &= ~CFG0_SCS_M;
	reg |= CFG0_SCS_CK_PLL;
	WR4(sc, RCU_CFG0, reg);

	do {
		reg = RD4(sc, RCU_CFG0);
	} while ((reg & CFG0_SCSS_M) != CFG0_SCSS_CK_PLL);
}

static void
gd32v_rcu_configure(mdx_device_t dev)
{
	struct gd32v_rcu_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	reg = RD4(sc, RCU_CTL);
	reg |= CTL_IRC8MEN;
	WR4(sc, RCU_CTL, reg);

	reg &= ~(CTL_HXTALEN | CTL_CKMEN | CTL_PLLEN);
	WR4(sc, RCU_CTL, reg);

	reg &= ~(CTL_HXTALBPS);
	WR4(sc, RCU_CTL, reg);

	WR4(sc, RCU_CFG1, 0);

	reg = RD4(sc, RCU_CTL);
	reg &= ~(CTL_PLLEN | CTL_PLL1EN | CTL_PLL2EN | CTL_CKMEN | CTL_HXTALEN);
	WR4(sc, RCU_CTL, reg);
	WR4(sc, RCU_INT, 0x00FF0000);

	gd32v_rcu_108mhz(sc);
}

int
gd32v_rcu_init(mdx_device_t dev, uint32_t base)
{
	struct gd32v_rcu_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(struct gd32v_rcu_softc));
	sc->base = base;

	gd32v_rcu_configure(dev);

	return (0);
}
