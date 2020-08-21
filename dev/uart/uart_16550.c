/*-
 * Copyright (c) 2019-2020 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/cheri.h>
#include <sys/io.h>

#include <dev/uart/uart_16550.h>

#define	RD1(_sc, _reg)		\
	mdx_ioread_uint8((_sc)->base, (_reg << (_sc)->reg_shift))
#define	WR1(_sc, _reg, _val)	\
	mdx_iowrite_uint8((_sc)->base, (_reg << (_sc)->reg_shift), _val)

#define	RD4(_sc, _reg)		\
	mdx_ioread_uint32((_sc)->base, (_reg << (_sc)->reg_shift))
#define	WR4(_sc, _reg, _val)	\
	mdx_iowrite_uint32((_sc)->base, (_reg << (_sc)->reg_shift), _val)

#define	UART_READ(_sc, _reg)	(_sc)->reg_shift == 2 ?	\
	RD4((_sc), (_reg)) : RD1((_sc), (_reg))
#define	UART_WRITE(_sc, _reg, _val)	(_sc)->reg_shift == 2 ?	\
	(WR4((_sc), (_reg), (_val))) : (WR1((_sc), (_reg), (_val)))

static bool
uart_16550_rxready(mdx_device_t dev)
{
	struct uart_16550_softc *sc;
	int status;

	sc = mdx_device_get_softc(dev);

	status = UART_READ(sc, REG_LSR);
	if (status & LSR_RXRDY)
		return (true);

	return (false);
}

static int
uart_16550_getc(mdx_device_t dev)
{
	struct uart_16550_softc *sc;
	int status;
	int c;

	sc = mdx_device_get_softc(dev);

	do {
		status = UART_READ(sc, REG_LSR);
	} while ((status & LSR_RXRDY) == 0);

	c = UART_READ(sc, REG_DATA);

	return (c);
}

static void
uart_16550_putc(mdx_device_t dev, int c)
{
	struct uart_16550_softc *sc;
	int status;

	sc = mdx_device_get_softc(dev);

	do {
		status = UART_READ(sc, REG_LSR);
	} while ((status & LSR_THRE) == 0);

	UART_WRITE(sc, REG_DATA, c);
}

static void
uart_16550_setup(mdx_device_t dev, int baud_rate,
    uart_databits_t databits,
    uart_stopbits_t stopbits,
    uart_parity_t parity)
{
	struct uart_16550_softc *sc;
	uint32_t reg;
	int divisor;

	sc = mdx_device_get_softc(dev);

	divisor = sc->bus_freq / (16 * baud_rate);

	UART_WRITE(sc, REG_IER, 0);

	reg = UART_READ(sc, REG_LCR);
	reg |= LCR_DLAB;
	UART_WRITE(sc, REG_LCR, reg);

	UART_WRITE(sc, REG_DLL, (divisor & 0xff));
	UART_WRITE(sc, REG_DLM, ((divisor >> 8) & 0xff));

	switch (databits) {
	case UART_DATABITS_8:
		reg |= LCR_BITS_8;
		break;
	case UART_DATABITS_7:
		reg |= LCR_BITS_7;
		break;
	case UART_DATABITS_6:
		reg |= LCR_BITS_6;
		break;
	case UART_DATABITS_5:
		reg |= LCR_BITS_5;
		break;
	}

	switch (stopbits) {
	case UART_STOPBITS_1:
	case UART_STOPBITS_1_5:
		/* TODO: check this. */
		break;
	case UART_STOPBITS_2:
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

static struct mdx_uart_ops uart_16550_ops = {
	.putc = uart_16550_putc,
	.getc = uart_16550_getc,
	.rxready = uart_16550_rxready,
	.setup = uart_16550_setup,
};

void
uart_16550_init(mdx_device_t dev, capability base,
    uint8_t reg_shift, uint32_t bus_freq)
{
	struct uart_16550_softc *sc;

	sc = mdx_device_get_softc(dev);
	sc->base = base;
	sc->reg_shift = reg_shift;
	sc->bus_freq = bus_freq;

	dev->ops = &uart_16550_ops;
}
