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

#ifndef _MIPS_MICROCHIP_PIC32_ADC_H_
#define _MIPS_MICROCHIP_PIC32_ADC_H_

#define	ADCBUF(n)		(0x000 + (n) * 0x10)
#define	ADCON1			(0x100)
#define	 ADCON1_ON		(1 << 15)	/* ADC Operating Mode bit */
#define	 ADCON1_SIDL		(1 << 13)	/* ADC Stop in Idle Mode bit */
#define	 ADCON1_FORM_S		8	/* Data Output Format bits */
#define	 ADCON1_FORM_M		(0x7 << ADCON1_FORM_S)
#define	 ADCON1_FORM_SF32	(0x7 << ADCON1_FORM_S)	/* Signed Fractional 32-bit */
#define	 ADCON1_FORM_F32	(0x6 << ADCON1_FORM_S)	/* Fractional 32-bit */
#define	 ADCON1_FORM_SI32	(0x5 << ADCON1_FORM_S)	/* Signed Integer 32-bit */
#define	 ADCON1_FORM_I32	(0x4 << ADCON1_FORM_S)	/* Integer 32-bit */
#define	 ADCON1_FORM_SF16	(0x3 << ADCON1_FORM_S)	/* Signed Fractional 16-bit */
#define	 ADCON1_FORM_F16	(0x2 << ADCON1_FORM_S)	/* Fractional 16-bit */
#define	 ADCON1_FORM_SI16	(0x1 << ADCON1_FORM_S)	/* Signed Integer 16-bit */
#define	 ADCON1_FORM_I16	(0x0 << ADCON1_FORM_S)	/* Integer 16-bit */
#define	 ADCON1_SSRC_S		8 /* Conversion Trigger Source Select bits */
#define	 ADCON1_SSRC_M		(0xf << ADCON1_SSRC_S)
#define	 ADCON1_MODE12		(1 << 3) /* 12-Bit Operation Mode bit */
#define	 ADCON1_ASAM		(1 << 2) /* ADC Sample Auto-Start bit */
#define	 ADCON1_SAMP		(1 << 1) /* ADC Sample Enable bit */
#define	 ADCON1_DONE		(1 << 0) /* ADC Conversion Status bit */
#define	ADCON2			(0x110)
#define	 ADCON2_CSCNA		(1 << 10) /* Scan Mode bit */
#define	 ADCON2_BUFREGEN	(1 << 11) /* ADC Buffer Register Enable bit */
#define	ADCON3			(0x120)
#define	ADCHS			(0x140)
#define	 ADCHS_CH0NA_S		5	/* Negative Input Select bits */
#define	 ADCHS_CH0NA_M		(0x7 << ADCHS_CH0NA_S)
#define	 ADCHS_CH0SA_S		0	/* Positive Input Select bits */
#define	 ADCHS_CH0SA_M		(0x1f << ADCHS_CH0SA_S)
#define	ADCSS			(0x150)
#define	ADCON5			(0x170)
#define	 ADCON5_ASEN		(1 << 15) /* Auto-Scan Enable bit */
#define	ADCHIT			(0x180)

struct pic32_adc_softc {
	uint32_t base;
};

void pic32_adc_init(struct pic32_adc_softc *sc, uint32_t base);
uint32_t pic32_adc_convert(struct pic32_adc_softc *sc, int channel);

#endif /* !_MIPS_MICROCHIP_PIC32_ADC_H_ */
