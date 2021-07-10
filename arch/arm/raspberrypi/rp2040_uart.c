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

#include <sys/cdefs.h>
#include <sys/of.h>
#include <sys/systm.h>
#include <sys/thread.h>

#include <dev/uart/uart.h>

#include "rp2040_uart.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
rp2040_uart_intr(void *arg, int irq)
{

	printf("%s\n", __func__);
}

static void
rp2040_uart_start(struct rp2040_uart_softc *sc)
{
	int ibrd;
	int fbrd;

	/*
	 * (12000000 / (16 * 115200)) = 6.510
	 * 0.510 * 64 = 32.64
	 */

	switch (sc->baudrate) {
	case 115200:
		ibrd = 6;
		fbrd = 32;
		break;
	default:
		panic("unknown baudrate\n");
	};

	WR4(sc, RP2040_UART_UARTIBRD, ibrd);
	WR4(sc, RP2040_UART_UARTFBRD, fbrd);
	WR4(sc, RP2040_UART_UARTLCR_H,
	    UART_UARTLCR_H_WLEN_8BIT | UART_UARTLCR_H_FEN);
	WR4(sc, RP2040_UART_UARTCR,
	    UART_UARTCR_UARTEN | UART_UARTCR_TXE | UART_UARTCR_RXE);
}

static void
rp2040_uart_putc(mdx_device_t dev, int c)
{
	struct rp2040_uart_softc *sc;

	sc = mdx_device_get_softc(dev);

	while (RD4(sc, RP2040_UART_UARTFR) & UART_UARTFR_TXFF)
		cpu_nullop();

	WR4(sc, RP2040_UART_UARTDR, c);
}

static void
rp2040_uart_setup(mdx_device_t dev, int baudrate, enum uart_databits databits,
    enum uart_stopbits stopbits, enum uart_parity parity)
{
	struct rp2040_uart_softc *sc;

	sc = mdx_device_get_softc(dev);

	/* TODO: use all the arguments. */

	sc->baudrate = baudrate;

	rp2040_uart_start(sc);
}

static struct mdx_uart_ops rp2040_uart_uart_ops = {
	.putc = rp2040_uart_putc,
	.setup = rp2040_uart_setup,
};

void
rp2040_uart_init(mdx_device_t dev, uint32_t base)
{
	struct rp2040_uart_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(*sc));
	sc->base = base;

	dev->ops = &rp2040_uart_uart_ops;
}
