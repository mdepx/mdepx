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

#include <sys/types.h>

#include <riscv/microsemi/core_uart.h>

#define	RD1(_sc, _reg)		\
	*(volatile uint8_t *)((_sc)->base + _reg)
#define	WR1(_sc, _reg, _val)	\
	*(volatile uint8_t *)((_sc)->base + _reg) = _val

void
core_uart_putc(struct core_uart_softc *sc, char c)
{
	int status;

	do {
		status = RD1(sc, UART_STATUS);
	} while ((status & STATUS_TXEMPTY) == 0);

	WR1(sc, UART_TXDATA, c);
}

int
core_uart_init(struct core_uart_softc *sc, uint32_t base,
    uint32_t cpu_freq, uint32_t baud_rate, uint32_t ctrl2)
{
	uint32_t reg;

	sc->base = base;

	reg = (cpu_freq / (16 * baud_rate)) - 1;
	WR1(sc, UART_CTRL1, reg & 0xff);

	reg = (reg & 0xff00) >> 5;
	reg |= ctrl2;
	WR1(sc, UART_CTRL2, reg & 0xff);

	return (0);
}
