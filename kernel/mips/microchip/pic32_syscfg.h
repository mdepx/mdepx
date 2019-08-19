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

#ifndef _MIPS_MICROCHIP_PIC32_SYSCFG_H_
#define	_MIPS_MICROCHIP_PIC32_SYSCFG_H_

#include <sys/types.h>

#define	CFGCON	0x000
#define	DEVID	0x020
#define	SYSKEY	0x030
#define	CFGEBIA	0x0C0
#define	CFGEBIC	0x0D0
#define	CFGPG	0x0E0
#define	CFGCON2	0x0F0
#define	CFGMPLL	0x100
#define	 MPLLIDIV_S	0		/* MPLL Input Divider bits */
#define	 MPLLIDIV_M	(0x3f << MPLLIDIV_S)
#define	 INTVREFCON_S	6		/* Internal DDRVREF Control bits */
#define	 INTVREFCON_INT	(0x3 << INTVREFCON_S)
#define	 INTVREFCON_M	(0x3 << INTVREFCON_S)
#define	 MPLLMULT_S	8		/* MPLL Multiplier bits */
#define	 MPLLMULT_M	(0xff << MPLLMULT_S)
#define	 MPLLVREGDIS	(1 << 22)	/* MPLL Voltage regulator Disable bit */
#define	 MPLLVREGRDY	(1 << 23)	/* MPLL Voltage Regulator Ready bit */
#define	 MPLLODIV1_S	24		/* MPLL Output Divider 1 bits */
#define	 MPLLODIV1_M	(0x7 << MPLLODIV1_S)
#define	 MPLLODIV2_S	27		/* MPLL Output Divider 2 bits */
#define	 MPLLODIV2_M	(0x7 << MPLLODIV2_S)
#define	 MPLLDIS	(1 << 30)	/* MPLL Disable bit */
#define	 MPLLRDY	(1 << 31)	/* Memory PLL Status bit */

/* vars */
#define	MPLL_IDIV_INIT		0x03
#define	MPLL_MULT_INIT		0x32
#define	MPLL_ODIV1_INIT		0x02
#define	MPLL_ODIV2_INIT		0x01

struct pic32_syscfg_softc {
	uint32_t base;
};

void pic32_syscfg_init(struct pic32_syscfg_softc *sc, uint32_t base);

#endif /* !_MIPS_MICROCHIP_PIC32_SYSCFG_H_ */
