/*-
 * Copyright (c) 2021 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/callout.h>

#include <arm/raspberrypi/rp2040_clocks.h>

#define	RP2040_CLOCKS_DEBUG
#undef	RP2040_CLOCKS_DEBUG

#ifdef	RP2040_CLOCKS_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static inline bool
has_glitchless_mux(int clk_index)
{

	return (clk_index == RP2040_CLOCKS_NDX_SYS ||
	    clk_index == RP2040_CLOCKS_NDX_REF);
}

static int
rp2040_clocks_configure(struct rp2040_clocks_softc *sc,
    int clk_index, uint32_t src, uint32_t auxsrc, uint32_t src_freq,
    uint32_t freq)
{
	int delay_cycles;
	uint32_t reg;
	uint32_t div;
	int i;

	div = (uint32_t)(((uint64_t)src_freq << 8) / freq);
	if (div > RD4(sc, RP2040_CLOCKS_CLK_NDX_DIV(clk_index)))
		WR4(sc, RP2040_CLOCKS_CLK_NDX_DIV(clk_index), div);

	if (has_glitchless_mux(clk_index) &&
	    src == CLOCKS_CLK_SYS_CTRL_SRC_CLKSRC_CLK_SYS_AUX) {
		reg = RD4(sc, RP2040_CLOCKS_CLK_NDX_CTRL(clk_index));
		reg &= ~CLOCKS_CLK_SYS_CTRL_SRC_M;
		WR4(sc, RP2040_CLOCKS_CLK_NDX_CTRL(clk_index), reg);

		while (RD4(sc, RP2040_CLOCKS_CLK_NDX_SELECTED(clk_index)) !=
		    1) ;
	} else {
		reg = RD4(sc, RP2040_CLOCKS_CLK_NDX_CTRL(clk_index));
		reg &= ~CLOCKS_CLK_GPOUT0_CTRL_ENABLE;
		WR4(sc, RP2040_CLOCKS_CLK_NDX_CTRL(clk_index), reg);

		if (sc->clock_freq[clk_index] > 0) {
			delay_cycles = sc->clock_freq[RP2040_CLOCKS_NDX_SYS] /
			    sc->clock_freq[clk_index] + 1;
			for (i = 0; i < delay_cycles; i++)
				__asm __volatile ("nop");
		}
	}

	reg = RD4(sc, RP2040_CLOCKS_CLK_NDX_CTRL(clk_index));
	reg &= ~CLOCKS_CLK_SYS_CTRL_AUXSRC_M;
	reg |= auxsrc;
	WR4(sc, RP2040_CLOCKS_CLK_NDX_CTRL(clk_index), reg);

	if (has_glitchless_mux(clk_index)) {
		reg = RD4(sc, RP2040_CLOCKS_CLK_NDX_CTRL(clk_index));
		reg &= ~CLOCKS_CLK_SYS_CTRL_SRC_M;
		reg |= src;
		WR4(sc, RP2040_CLOCKS_CLK_NDX_CTRL(clk_index), reg);
		while (RD4(sc, RP2040_CLOCKS_CLK_NDX_SELECTED(clk_index)) !=
		    (1 << src)) ;
	}

	reg = RD4(sc, RP2040_CLOCKS_CLK_NDX_CTRL(clk_index));
	reg |= CLOCKS_CLK_GPOUT0_CTRL_ENABLE;
	WR4(sc, RP2040_CLOCKS_CLK_NDX_CTRL(clk_index), reg);

	WR4(sc, RP2040_CLOCKS_CLK_NDX_DIV(clk_index), div);
	sc->clock_freq[clk_index] = freq;

	return (0);
}

void
rp2040_clocks_init(struct rp2040_clocks_softc *sc, uint32_t base)
{
	uint32_t reg;

	sc->base = base;

	WR4(sc, RP2040_CLOCKS_CLK_SYS_RESUS_CTRL, 0);

	reg = RD4(sc, RP2040_CLOCKS_CLK_SYS_CTRL);
	reg &= ~CLOCKS_CLK_SYS_CTRL_SRC_M;
	WR4(sc, RP2040_CLOCKS_CLK_SYS_CTRL, reg);
	while (RD4(sc, RP2040_CLOCKS_CLK_SYS_SELECTED) != 1)
		;

	reg = RD4(sc, RP2040_CLOCKS_CLK_REF_CTRL);
	reg &= ~CLOCKS_CLK_REF_CTRL_SRC_M;
	WR4(sc, RP2040_CLOCKS_CLK_REF_CTRL, reg);
	while (RD4(sc, RP2040_CLOCKS_CLK_REF_SELECTED) != 1)
		;
}

void
rp2040_clocks_setup(struct rp2040_clocks_softc *sc)
{

	/* CLK_REF <= XOSC (12MHz) */
	rp2040_clocks_configure(sc, RP2040_CLOCKS_NDX_REF,
				CLOCKS_CLK_REF_CTRL_SRC_XOSC_CLKSRC,
				0,
				12000000,
				12000000);

	/* CLK_SYS <= PLL SYS (125MHz) */
	rp2040_clocks_configure(sc, RP2040_CLOCKS_NDX_SYS,
				CLOCKS_CLK_SYS_CTRL_SRC_CLKSRC_CLK_SYS_AUX,
				CLOCKS_CLK_SYS_CTRL_AUXSRC_CLKSRC_PLL_SYS,
				125000000,
				125000000);

	/* CLK USB <= PLL USB (48MHz) */
	rp2040_clocks_configure(sc, RP2040_CLOCKS_NDX_USB,
				0,
				CLOCKS_CLK_USB_CTRL_AUXSRC_CLKSRC_PLL_USB,
				48000000,
				48000000);

	/* CLK ADC <= PLL USB (48MHz) */
	rp2040_clocks_configure(sc, RP2040_CLOCKS_NDX_ADC,
				0,
				CLOCKS_CLK_ADC_CTRL_AUXSRC_CLKSRC_PLL_USB,
				48000000,
				48000000);

	/* CLK RTC <= PLL USB (48MHz) */
	rp2040_clocks_configure(sc, RP2040_CLOCKS_NDX_RTC,
				0,
				CLOCKS_CLK_RTC_CTRL_AUXSRC_CLKSRC_PLL_USB,
				48000000,
				46875);

	/* CLK PERI <= CLK_SYS (12MHz) */
	rp2040_clocks_configure(sc, RP2040_CLOCKS_NDX_PERI,
				0,
				CLOCKS_CLK_PERI_CTRL_AUXSRC_CLK_SYS,
				125000000,
				125000000);
}
