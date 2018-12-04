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

#ifndef _MIPS_MICROCHIP_PIC32_CCP_H_
#define _MIPS_MICROCHIP_PIC32_CCP_H_

/* Capture/Compare/PWM timer module. */

#define	CCPCON1	0x00
#define	 CCPCON1_ON	(1 << 15)
#define	 CCPCON1_T32	(1 << 5)	/* 32-Bit Time Base Select bit */
#define	CCPCON2	0x10
#define	CCPCON3	0x20
#define	CCPSTAT	0x30
#define	CCPTMR	0x40
#define	CCPPR	0x50
#define	CCPRA	0x60
#define	CCPRB	0x70
#define	CCPBUF	0x80

struct pic32_ccp_softc {
	uint32_t base;
};

void pic32_ccp_init(struct pic32_ccp_softc *sc, uint32_t base);
void pic32_ccp_delay(struct pic32_ccp_softc *sc, uint32_t usec);
uint32_t pic32_ccp_counts(struct pic32_ccp_softc *sc);

#endif /* !_MIPS_MICROCHIP_PIC32_CCP_H_ */
