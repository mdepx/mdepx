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

#include <sys/cdefs.h>

#include "max32660_uart.h"

#define	RD4(_sc, _reg)		*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
max32660_uart_intr(void *arg, uint32_t irqno)
{

}

void
max32660_uart_putc(struct max32660_uart_softc *sc, char ch)
{
	uint32_t reg;

	do {
		reg = RD4(sc, UART_STAT);
	} while (reg & STAT_TX_FULL);

	WR4(sc, UART_FIFO, ch);
}

static void
max32660_uart_baud(struct max32660_uart_softc *sc,
    uint32_t uart_freq, uint32_t baud_rate)
{
	uint64_t dbaud;
	uint32_t clk_div;
	uint32_t reg;

	clk_div = (uart_freq / (128 * baud_rate));

	dbaud = ((uart_freq * 1000) / (128 * baud_rate)) - 1000;
	dbaud *= 128;
	dbaud /= 1000;

	reg = BAUD0_CLKDIV_128;
	reg |= (clk_div << BAUD0_IBAUD_S);
	WR4(sc, UART_BAUD0, reg);

	reg = (dbaud << BAUD1_DBAUD_S);
	WR4(sc, UART_BAUD1, reg);
}

void
max32660_uart_init(struct max32660_uart_softc *sc, uint32_t base,
    uint32_t uart_freq, uint32_t baud_rate)
{
	uint32_t reg;

	sc->base = base;

	max32660_uart_baud(sc, uart_freq, baud_rate);

	reg = CTRL0_EN | CTRL0_SIZE_8BITS;
	WR4(sc, UART_CTRL0, reg);
}
