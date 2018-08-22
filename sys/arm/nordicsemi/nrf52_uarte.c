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
#include <sys/systm.h>

#include "nrf52_uarte.h"

#define	RD4(_sc, _reg)		*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
uarte_intr(void *arg, uint32_t irqno)
{
	struct uarte_softc *sc;

	sc = arg;

	WR4(sc, EVENTS_RXDRDY, 0);
	WR4(sc, RXD_MAXCNT, 8);
	WR4(sc, TASKS_STARTRX, 1);
}

void
uarte_putc(struct uarte_softc *sc, char ch)
{
	int timeout;

	WR4(sc, EVENTS_ENDTX, 0);
	WR4(sc, TXD_PTR, (uint32_t)&ch);
	WR4(sc, TXD_MAXCNT, 1);
	WR4(sc, TASKS_STARTTX, 1);

	timeout = 1000;
	do {
		if (RD4(sc, EVENTS_ENDTX) != 0) {
			WR4(sc, EVENTS_ENDTX, 0);
			break;
		}
	} while (--timeout);
}

static void
uarte_start(struct uarte_softc *sc)
{

	WR4(sc, INTENSET, RXDRDY);

	WR4(sc, ENABLE, 0);
	switch (sc->baudrate) {
	case 115200:
		WR4(sc, BAUDRATE, BAUD_115200);
		break;
	default:
		panic("Unsupported baud rate");
	}
	WR4(sc, PSELTXD, sc->pin_tx);
	WR4(sc, PSELRXD, sc->pin_rx);
	WR4(sc, CONFIG, 0);
	WR4(sc, ENABLE, ENABLE_EN);

	WR4(sc, RXD_PTR, (uint32_t)&sc->rx_data[0]);
	WR4(sc, RXD_MAXCNT, 8);

	WR4(sc, TASKS_STARTRX, 1);
}

void
uarte_init(struct uarte_softc *sc, uint32_t base,
    uint8_t pin_tx, uint8_t pin_rx,
    uint32_t baudrate)
{

	sc->base = base;
	sc->pin_tx = pin_tx;
	sc->pin_rx = pin_rx;
	sc->baudrate = baudrate;

	uarte_start(sc);
}
