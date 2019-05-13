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
 * Register offsets within UART memory map.
 * SiFive E300 Platform Reference Manual, Version 1.0.1
 */

#ifndef _SYS_RISCV_SIFIVE_E300G_UART_H_
#define _SYS_RISCV_SIFIVE_E300G_UART_H_

#define	UART_TXDATA	0x000 /* Transmit data register */
#define	 TXDATA_EMPTY	(1 << 31)
#define	UART_RXDATA	0x004 /* Receive data register */
#define	UART_TXCTRL	0x008 /* Transmit control register */
#define	 UART_TXEN	(1 << 0) /* Enable */
#define	UART_RXCTRL	0x00C /* Receive control register */
#define	UART_IE		0x010 /* UART interrupt enable */
#define	UART_IP		0x014 /* UART Interrupt pending */
#define	UART_DIV	0x018 /* Baud rate divisor */

struct uart_softc {
	size_t base;
};

int e300g_uart_init(struct uart_softc *sc, uint32_t base,
     uint32_t cpu_freq, uint32_t baud_rate);
void e300g_uart_putc(struct uart_softc *sc, char c);

#endif /* !_SYS_RISCV_SIFIVE_E300G_UART_H_ */
