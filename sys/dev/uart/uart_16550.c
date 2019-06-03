/*-
 * Copyright (c) 2019 Ruslan Bukin <br@bsdpad.com>
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

#include <dev/uart/uart_16550.h>

#define	RD1(_sc, _reg)		\
	*(volatile uint8_t *)((_sc)->base + (_reg << (_sc)->reg_shift))
#define	WR1(_sc, _reg, _val)	\
	*(volatile uint8_t *)((_sc)->base + (_reg << (_sc)->reg_shift)) = _val

char
uart_16550_getc(struct uart_16550_softc *sc)
{
	int status;
	char c;

	do {
		status = RD1(sc, REG_LSR);
	} while ((status & LSR_RXRDY) == 0);

	c = RD1(sc, REG_DATA);

	return (c);
}

void
uart_16550_putc(struct uart_16550_softc *sc, char c)
{
	int status;

	do {
		status = RD1(sc, REG_LSR);
	} while ((status & LSR_THRE) == 0);

	WR1(sc, REG_DATA, c);
}

int
uart_16550_init(struct uart_16550_softc *sc, size_t base,
    uint32_t uart_freq, uint32_t baud_rate, uint8_t reg_shift)
{
	uint32_t reg;
	int divisor;

	sc->base = base;
	sc->reg_shift = reg_shift;

	divisor = uart_freq / (16 * baud_rate);

	WR1(sc, REG_IER, 0);

	reg = RD1(sc, REG_LCR);
	reg |= LCR_DLAB;
	WR1(sc, REG_LCR, reg);

	WR1(sc, REG_DLL, (divisor & 0xff));
	WR1(sc, REG_DLM, ((divisor >> 8) & 0xff));

	reg = RD1(sc, REG_LCR);
	reg &= ~LCR_DLAB;
	WR1(sc, REG_LCR, reg);

	return (0);
}
