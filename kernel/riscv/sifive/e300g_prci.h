/*-
 * Copyright (c) 2017 Ruslan Bukin <br@bsdpad.com>
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

/*
 * SiFive E300 PRCI Memory Map.
 * SiFive E300 Platform Reference Manual, Version 1.0.1
 */

#ifndef _SYS_RISCV_SIFIVE_E300G_PRCI_H_
#define _SYS_RISCV_SIFIVE_E300G_PRCI_H_

/* Clock Configuration Registers */
#define	PRCI_HFROSCCFG		0x000
#define	 HFROSCDIV_S		0
#define	 HFROSCDIV_M		(0x3f << HFROSCDIV_S)
#define	 HFROSCTRIM_S		16
#define	 HFROSCTRIM_M		(0x1f << HFROSCTRIM_S)
#define	 HFROSCEN		(1 << 30)
#define	 HFROSCRDY		(1 << 31)

#define	PRCI_HFXOSCCFG		0x004
#define	PRCI_PLLCFG		0x008
#define	 PLL_LOCK		(1 << 31)
#define	 PLL_BYPASS		(1 << 18)
#define	 PLLREFSEL		(1 << 17)
#define	 PLLSEL			(1 << 16)
#define	 PLL_R_S		0
#define	 PLL_R_M		(0x3 << PLL_R_S)
#define	 PLL_R(x)		((x) << PLL_R_S)
#define	 PLL_F_S		4
#define	 PLL_F_M		(0x3f << PLL_F_S)
#define	 PLL_F(x)		((x) << PLL_F_S)
#define	 PLL_Q_S		10
#define	 PLL_Q_M		(0x2 << PLL_Q_S)
#define	 PLL_Q(x)		((x) << PLL_Q_S)
#define	PRCI_PLLOUTDIV		0x00C
#define	 PLLOUTDIV_S		0
#define	 PLLOUTDIV_M		(0x3f << PLLOUTDIV_S)
#define	 PLLOUTDIV(x)		((x) << PLLOUTDIV_S)
#define	 PLLOUTDIV_BY_1		(1 << 8)
#define	PRCI_CORECLKCFG		0x010

struct prci_softc {
	uint32_t base;
};

int e300g_prci_init(struct prci_softc *sc, uint32_t base);

#endif /* !_SYS_RISCV_SIFIVE_E300G_PRCI_H_ */
