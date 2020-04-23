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

#if __has_feature(capabilities)
static inline uint8_t
mips_cap_ioread_uint8(capability cap, size_t offset)
{
	uint8_t v;
	__asm__ __volatile__ ("clb %[v], %[offset],  0(%[cap])"
		: [v] "=r" (v)
		: [cap] "C" (cap), [offset] "r" (offset));
	return (v);
}

static inline void
mips_cap_iowrite_uint8(capability cap, size_t offset, uint8_t v)
{
	__asm__ __volatile__ ("csb %[v], %[offset],  0(%[cap])"
		:: [cap] "C" (cap), [offset] "r" (offset), [v] "r" (v));
}

#define	RD1(_sc, _reg)		\
	mips_cap_ioread_uint8((_sc)->base, _reg)
#define	WR1(_sc, _reg, _val)	\
	mips_cap_iowrite_uint8((_sc)->base, _reg, _val)
#else

#define	RD1(_sc, _reg)		\
	*(volatile uint8_t *)((_sc)->base + (_reg << (_sc)->reg_shift))
#define	WR1(_sc, _reg, _val)	\
	*(volatile uint8_t *)((_sc)->base + (_reg << (_sc)->reg_shift)) = _val

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + (_reg << (_sc)->reg_shift))
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + (_reg << (_sc)->reg_shift)) = _val

#define	UART_READ(_sc, _reg)	(_sc)->reg_shift == 2 ?	\
	RD4((_sc), (_reg)) : RD1((_sc), (_reg))
#define	UART_WRITE(_sc, _reg, _val)	(_sc)->reg_shift == 2 ?	\
	(WR4((_sc), (_reg), (_val))) : (WR1((_sc), (_reg), (_val)))

#endif

char
uart_16550_getc(struct uart_16550_softc *sc)
{
	int status;
	char c;

	do {
		status = UART_READ(sc, REG_LSR);
	} while ((status & LSR_RXRDY) == 0);

	c = UART_READ(sc, REG_DATA);

	return (c);
}

void
uart_16550_putc(struct uart_16550_softc *sc, char c)
{
	int status;

	do {
		status = UART_READ(sc, REG_LSR);
	} while ((status & LSR_THRE) == 0);

	UART_WRITE(sc, REG_DATA, c);
}

void
uart_16550_init(struct uart_16550_softc *sc, capability base,
    uint8_t reg_shift)
{

	sc->base = base;
	sc->reg_shift = reg_shift;
}

void
uart_16550_configure(struct uart_16550_softc *sc,
    uint32_t bus_freq,
    uint32_t baud_rate,
    uart_bitwidth_t bitwidth,
    uart_stopbit_t stopbit,
    uart_parity_t parity)
{
	uint32_t reg;
	int divisor;

	divisor = bus_freq / (16 * baud_rate);

	UART_WRITE(sc, REG_IER, 0);

	reg = UART_READ(sc, REG_LCR);
	reg |= LCR_DLAB;
	UART_WRITE(sc, REG_LCR, reg);

	UART_WRITE(sc, REG_DLL, (divisor & 0xff));
	UART_WRITE(sc, REG_DLM, ((divisor >> 8) & 0xff));

	switch (bitwidth) {
	case UART_BITWIDTH_8:
		reg |= LCR_BITS_8;
		break;
	case UART_BITWIDTH_7:
		reg |= LCR_BITS_7;
		break;
	case UART_BITWIDTH_6:
		reg |= LCR_BITS_6;
		break;
	case UART_BITWIDTH_5:
		reg |= LCR_BITS_5;
		break;
	}

	switch (stopbit) {
	case UART_STOP_1:
	case UART_STOP_1_5:
		/* TODO: check this. */
		break;
	case UART_STOP_2:
		reg |= LCR_STOPB;
		break;
	}

	switch (parity) {
	case UART_PARITY_NONE:
		reg |= LCR_PARITY_NONE;
		break;
	case UART_PARITY_ODD:
		reg |= LCR_PARITY_ODD;
		break;
	case UART_PARITY_EVEN:
		reg |= LCR_PARITY_EVEN;
		break;
	}
	UART_WRITE(sc, REG_LCR, reg);

	reg = UART_READ(sc, REG_LCR);
	reg &= ~LCR_DLAB;
	UART_WRITE(sc, REG_LCR, reg);
}
