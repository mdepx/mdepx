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

#include <mips/microchip/pic32_uart.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

uint8_t
pic32_getc(struct pic32_uart_softc *sc)
{
	uint8_t c;

	if ((RD4(sc, UART_STA) & STA_URXDA) == 0)
		return (0);

	c = RD4(sc, UART_RXREG);

	return (c);
}

void
pic32_putc(struct pic32_uart_softc *sc, char c)
{

	while (RD4(sc, UART_STA) & STA_UTXBF)
		;

	WR4(sc, UART_TXREG, c & 0xff);
}

void
pic32_uart_init(struct pic32_uart_softc *sc, uint32_t base,
    uint32_t speed, uint32_t cpu_freq, uint32_t div)
{
	uint32_t reg;
	uint32_t freq;

	sc->base = base;

	WR4(sc, UART_MODE, 0);
	WR4(sc, UART_STA, 0);

	freq = (cpu_freq / (div * 8));
	freq += speed;
	freq /= (speed);
	freq /= 2;
	freq -= 1;
	WR4(sc, UART_BRG, freq);

	reg = (STA_URXEN | STA_UTXEN);
	WR4(sc, UART_STA, reg);

	WR4(sc, UART_MODE, MODE_ON);
}
