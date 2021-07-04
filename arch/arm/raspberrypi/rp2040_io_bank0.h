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

#ifndef	_RP2040_IO_BANK0_H_
#define	_RP2040_IO_BANK0_H_

#define	RP2040_IO_BANK0_GPIO_STATUS(n)	(0x000 + 0x8 * (n))	/* n: 0..29 */
#define	RP2040_IO_BANK0_GPIO_CTRL(n)	(0x004 + 0x8 * (n))
#define	 IO_BANK0_GPIO_CTRL_FUNCSEL_S	0
#define	 IO_BANK0_GPIO_CTRL_FUNCSEL_M	(0x1f << IO_BANK0_GPIO_CTRL_FUNCSEL_S)
#define	 IO_BANK0_GPIO0_CTRL_FUNCSEL_UART0_TX	2
#define	 IO_BANK0_GPIO1_CTRL_FUNCSEL_UART0_RX	2
#define	 IO_BANK0_GPIO4_CTRL_FUNCSEL_UART1_TX	2
#define	 IO_BANK0_GPIO5_CTRL_FUNCSEL_UART1_RX	2
#define	RP2040_IO_BANK0_INTR(n)		(0x0f0 + 0x4 * (n))	/* n: 0..3 */
#define	RP2040_IO_BANK0_PROC0_INTE(n)	(0x100 * 0x4 * (n))
#define	RP2040_IO_BANK0_PROC0_INTF(n)	(0x110 * 0x4 * (n))
#define	RP2040_IO_BANK0_PROC0_INTS(n)	(0x120 * 0x4 * (n))
#define	RP2040_IO_BANK0_PROC1_INTE(n)	(0x130 * 0x4 * (n))
#define	RP2040_IO_BANK0_PROC1_INTF(n)	(0x140 * 0x4 * (n))
#define	RP2040_IO_BANK0_PROC1_INTS(n)	(0x150 * 0x4 * (n))
#define	RP2040_IO_BANK0_DORMANT_WAKE_INTE(n)	(0x160 * 0x4 * (n))
#define	RP2040_IO_BANK0_DORMANT_WAKE_INTF(n)	(0x170 * 0x4 * (n))
#define	RP2040_IO_BANK0_DORMANT_WAKE_INTS(n)	(0x180 * 0x4 * (n))

struct rp2040_io_bank0_softc {
	uint32_t base;
};

void rp2040_io_bank0_init(struct rp2040_io_bank0_softc *sc, uint32_t base);
void rp2040_io_bank0_funcsel(struct rp2040_io_bank0_softc *sc, int pin,
    int funcsel);

#endif /* !_RP2040_IO_BANK0_H_ */
