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

#ifndef _MIPS_MICROCHIP_PIC32_CDAC_H_
#define	_MIPS_MICROCHIP_PIC32_CDAC_H_

#define	DACCON			0x00
#define	 DACCON_DACDAT_S	16 /* CDAC Voltage Reference Selection bits */
#define	 DACCON_DACDAT_M	(0x3f << DACCON_DACDAT_S)
#define	 DACCON_ON		(1 << 15) /* Voltage Reference Enable bit */
#define	 DACCON_DACOE		(1 << 8) /* CDAC Voltage Reference Output Enable bit */
#define	 DACCON_REFSEL_S	0 /* CDAC Voltage Reference Source Select bits */
#define	 DACCON_REFSEL_M	(0x3 << DACCON_REFSEL_S)
#define	 DACCON_REFSEL_AVDD	(0x3 << DACCON_REFSEL_S) /* Reference voltage is AVDD */
#define	 DACCON_REFSEL_AVSS	(0x2 << DACCON_REFSEL_S) /* No reference is selected ­ output is AVSS */
#define	 DACCON_REFSEL_VREF_P	(0x1 << DACCON_REFSEL_S) /* Reference voltage is the VREF+ input pin voltage */
#define	 DACCON_REFSEL_AVSS_1	(0x0 << DACCON_REFSEL_S) /* No reference is selected ­ output is AVSS */

struct pic32_cdac_softc {
	uint32_t base;
};

void pic32_cdac_init(struct pic32_cdac_softc *sc, uint32_t base);

#endif /* !_MIPS_MICROCHIP_PIC32_CDAC_H_ */
