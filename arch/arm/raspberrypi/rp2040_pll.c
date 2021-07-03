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

#include <arm/raspberrypi/rp2040_pll.h>

#define	RP2040_PLL_DEBUG
#undef	RP2040_PLL_DEBUG

#ifdef	RP2040_PLL_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

int
rp2040_pll_init(struct rp2040_pll_softc *sc, uint32_t base,
    uint32_t refdiv, uint32_t vco_freq, uint32_t post_div1,
    uint8_t post_div2)
{
	uint32_t ref_mhz;
	uint32_t fbdiv;
	uint32_t reg;
	int timeout;

	sc->base = base;

	/* Turn PLL off */
	WR4(sc, RP2040_PLL_PWR, 0xffffffff);
	WR4(sc, RP2040_PLL_FBDIV_INT, 0);

	/* Configure PLL */
	WR4(sc, RP2040_PLL_CS, refdiv);

	ref_mhz = 12000000 / refdiv; /* board OSC */
	fbdiv = vco_freq / ref_mhz;
	WR4(sc, RP2040_PLL_FBDIV_INT, fbdiv);

	/* Turn PLL on */
	reg = RD4(sc, RP2040_PLL_PWR);
	reg &= ~(PLL_PWR_PD | PLL_PWR_VCOPD);
	WR4(sc, RP2040_PLL_PWR, reg);

	timeout = 10000;

	do {
		reg = RD4(sc, RP2040_PLL_CS);
		if (reg & PLL_CS_LOCK)
			break;
	} while (timeout--);

	if (timeout <= 0) {
		printf("%s: could not initialize PLL\n", __func__);
		return (-1);
	}

	/* Configure post dividers */
	reg = post_div1 << PLL_PRIM_POSTDIV1_S;
	reg |= post_div2 << PLL_PRIM_POSTDIV2_S;
	WR4(sc, RP2040_PLL_PRIM, reg);

	reg = RD4(sc, RP2040_PLL_PWR);
	reg &= ~PLL_PWR_POSTDIVPD;
	WR4(sc, RP2040_PLL_PWR, reg);

	return (0);
}
