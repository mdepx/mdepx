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

#ifndef _MIPS_MICROCHIP_PIC32MM_PPS_H_
#define	_MIPS_MICROCHIP_PIC32MM_PPS_H_

#define	SYSKEY		0xBF803B30

#if 1
/* Peripheral pin select */
#define	PPS_RPCON		(0x80)

#define	PPS_RPINR(n)		(0xA0 + 0x10 * (n - 1))
#define	PPS_RPOR(n)		(0x190 + 0x10 * (n))
#endif

#if 0
/* MZEF */
#define	PPS_U2RXR	0x070
#define	PPS_U4RXR	0x080
#define	 RPD1		0x0
#define	 RPG9		0x1
#define	 RPB14		0x2
#define	 RPD0		0x3
#define	 RPB6		0x5
#define	 RPD5		0x6
#define	 RPB2		0x7
#define	 RPF3		0x8
#define	 RPF13_1	0x9
#define	 RPF2_1		0xb
#define	 RPC2_1		0xc
#define	 RPE8_1		0xd

#define	RPB0		5
/* Peripheral Pin Select Output Register Map */
#define	PPS_RPG8R	0x2A0
#define	PPS_RPG9R	0x2A4
#define	PPS_RPD4R	0x1D0
#define	 NO_CONNECT	0b0000
#define	 U3RTS		0b0001
#define	 U4TX		0b0010
#define	 U6TX		0b0100
#define	 SS1		0b0101
#define	 SS3		0b0111
#define	 SS4		0b1000
#define	 SS5_1		0b1001
#define	 SDO6_1		0b1010
#define	 OC5		0b1011
#define	 OC8		0b1100
#define	 C1OUT		0b1110
#define	 REFCLKO3	0b1111
#endif

struct pic32_pps_softc {
	uint32_t base;
};

void pic32_pps_init(struct pic32_pps_softc *sc, uint32_t base);
void pic32_pps_write(struct pic32_pps_softc *sc, uint32_t reg, uint32_t val);
void pic32_pps_rpinr(struct pic32_pps_softc *sc, uint32_t reg, uint32_t val);
void pic32_pps_rpor(struct pic32_pps_softc *sc, uint8_t rpor, uint32_t val);

#endif /* !_MIPS_MICROCHIP_PIC32MM_PPS_H_ */
