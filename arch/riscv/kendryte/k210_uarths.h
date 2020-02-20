/*-
 * Copyright (c) 2020 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _RISCV_KENDRYTE_K210_UARTHS_H_
#define _RISCV_KENDRYTE_K210_UARTHS_H_

#define	K210_UARTHS_TXFIFO	0x00
#define	 UARTHS_TXFIFO_DATA_S	0
#define	 UARTHS_TXFIFO_DATA_M	(0xff << UARTHS_TXFIFO_DATA_S)
#define	 UARTHS_TXFIFO_FULL	(1 << 31)
#define	K210_UARTHS_RXFIFO	0x04
#define	K210_UARTHS_TXCTRL	0x08
#define	 UARTHS_TXCTRL_TXEN	(1 << 0)
#define	 UARTHS_TXCTRL_TXWM_S	16
#define	 UARTHS_TXCTRL_TXWM_M	(0xffff << UARTHS_TXCTRL_TXWM_S)
#define	K210_UARTHS_RXCTRL	0x0c
#define	 UARTHS_RXCTRL_RXEN	(1 << 0)
#define	 UARTHS_RXCTRL_RXWM_S	16
#define	 UARTHS_RXCTRL_RXWM_M	(0xffff << UARTHS_RXCTRL_RXWM_S)
#define	K210_UARTHS_IE		0x10
#define	K210_UARTHS_IP		0x14
#define	 UARTHS_IP_TXWM		(1 << 0)
#define	 UARTHS_IP_RXWM		(1 << 1)
#define	K210_UARTHS_DIV		0x18

struct k210_uarths_softc {
	size_t base;
};

void k210_uarths_init(struct k210_uarths_softc *sc, uint32_t base);
void k210_uarths_putc(struct k210_uarths_softc *sc, char c);

#endif /* !_RISCV_KENDRYTE_K210_UARTHS_H_ */
