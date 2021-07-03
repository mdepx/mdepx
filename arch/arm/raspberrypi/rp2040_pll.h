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

#ifndef	_RP2040_PLL_H_
#define	_RP2040_PLL_H_

#define	RP2040_PLL_CS		0x0
#define	 PLL_CS_LOCK		(1 << 31) /* PLL is locked */
#define	RP2040_PLL_PWR		0x4
#define	 PLL_PWR_VCOPD		(1 << 5) /* PLL VCO powerdown */
#define	 PLL_PWR_POSTDIVPD	(1 << 3) /* PLL post divider powerdown */
#define  PLL_PWD_DSMPD		(1 << 2) /* PLL DSM powerdown */
#define	 PLL_PWR_PD		(1 << 0) /* PLL powerdown */
#define	RP2040_PLL_FBDIV_INT	0x8
#define	RP2040_PLL_PRIM		0xc
#define	 PLL_PRIM_POSTDIV1_S	16
#define	 PLL_PRIM_POSTDIV2_S	12

struct rp2040_pll_softc {
	uint32_t base;
};

int
rp2040_pll_init(struct rp2040_pll_softc *sc, uint32_t base,
    uint32_t refdiv, uint32_t vco_freq, uint32_t post_div1,
    uint8_t post_div2);

#endif /* !_RP2040_PLL_H_ */
