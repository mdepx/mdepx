/*-
 * Copyright (c) 2024 Ruslan Bukin <br@bsdpad.com>
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

#include <riscv/artinchip/d21x_cmu.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
d21x_cmu_clk_enable(mdx_device_t dev, uint32_t clk_reg, uint32_t div)
{
	struct d21x_cmu_softc *sc;
	uint32_t reg;
	int i;

	sc = mdx_device_get_softc(dev);

	reg = (div << MOD_CLK_DIV_S);
	WR4(sc, clk_reg, reg);

	reg |= MOD_CLK_EN;
	reg |= MOD_BUS_EN;
	WR4(sc, clk_reg, reg);

	/* TODO: delay */
	for (i = 0; i < 100000; i ++) {};

	reg |= MOD_RSTN;
	WR4(sc, clk_reg, reg);

	/* TODO: delay */
	for (i = 0; i < 100000; i ++) {};
}

void
d21x_cmu_init(mdx_device_t dev, uint32_t base)
{
	struct d21x_cmu_softc *sc;

	sc = mdx_device_get_softc(dev);
	sc->base = base;
}
