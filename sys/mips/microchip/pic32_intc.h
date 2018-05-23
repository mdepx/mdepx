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

#ifndef _MIPS_MICROCHIP_PIC32_INTC_H_
#define _MIPS_MICROCHIP_PIC32_INTC_H_

#define	INTC_INTCON	0x000
#define	INTC_PRISS	0x010
#define	INTC_INTSTAT	0x020
#define	INTC_IPTMR	0x030
#define	INTC_IFS(n)	(0x040 + ((n) * 0x10))
#define	INTC_IEC(n)	(0x0C0 + ((n) * 0x10))
#define	INTC_IPC(n)	(0x140 + ((n) * 0x10))
#define	INTC_OFF(n)	(0x540 + ((n) * 0x10))

struct pic32_intc_softc {
	uint32_t base;
};

struct pic32_intr {
	uint8_t flag;
	uint8_t ctrl;
	uint8_t prio;
	uint8_t sub_prio;
	uint8_t flag_bit;
	uint8_t ctrl_bit;
	uint8_t prio_bit;
	uint8_t sub_prio_bit;
};

void pic32_intc_init(struct pic32_intc_softc *sc, uint32_t base);
void pic32_intc_enable(struct pic32_intc_softc *sc, struct pic32_intr *intr, uint8_t prio);
void pic32_intc_clear_pending(struct pic32_intc_softc *sc, struct pic32_intr *intr);

#endif /* !_MIPS_MICROCHIP_PIC32_INTC_H_ */
