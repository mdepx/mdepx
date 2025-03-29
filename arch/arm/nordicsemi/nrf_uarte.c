/*-
 * Copyright (c) 2018-2025 Ruslan Bukin <br@bsdpad.com>
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

#include <dev/uart/uart.h>

#include "nrf_uarte.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

#ifdef MDX_OF
static struct mdx_compat_data nrf_uarte_compat_data[] = {
	{ "nordic,nrf-uarte" },
	{ NULL },
};
#endif

void
nrf_uarte_intr(void *arg, int irq)
{
	struct nrf_uarte_softc *sc;

	sc = arg;

	if (sc->cb != NULL)
		sc->cb(sc->rx_data[0], sc->cb_arg);

	WR4(sc, UARTE_EVENTS_RXDRDY, 0);
	WR4(sc, UARTE_RXD_MAXCNT, 8);
	WR4(sc, UARTE_TASKS_STARTRX, 1);
}

static void
nrf_uarte_putc(mdx_device_t dev, int c)
{
	struct nrf_uarte_softc *sc;
	int timeout;

	sc = mdx_device_get_softc(dev);

	WR4(sc, UARTE_EVENTS_ENDTX, 0);
	WR4(sc, UARTE_TXD_PTR, (uint32_t)&c);
	WR4(sc, UARTE_TXD_MAXCNT, 1);
	WR4(sc, UARTE_TASKS_STARTTX, 1);

	timeout = 1000;
	do {
		if (RD4(sc, UARTE_EVENTS_ENDTX) != 0) {
			WR4(sc, UARTE_EVENTS_ENDTX, 0);
			break;
		}
	} while (--timeout);
}

static void
nrf_uarte_start(struct nrf_uarte_softc *sc)
{

	WR4(sc, UARTE_INTENSET, INTENSET_RXDRDY);

	WR4(sc, UARTE_ENABLE, 0);
	switch (sc->baudrate) {
	case 115200:
		WR4(sc, UARTE_BAUDRATE, BAUD_115200);
		break;
	default:
		panic("Unsupported baud rate");
	}
	WR4(sc, UARTE_PSEL_TXD, sc->pin_tx);
	WR4(sc, UARTE_PSEL_RXD, sc->pin_rx);
	WR4(sc, UARTE_CONFIG, 0);
	WR4(sc, UARTE_ENABLE, ENABLE_EN);

	WR4(sc, UARTE_RXD_PTR, (uint32_t)&sc->rx_data[0]);
	WR4(sc, UARTE_RXD_MAXCNT, 8);

	WR4(sc, UARTE_TASKS_STARTRX, 1);
}

void
nrf_uarte_register_callback(mdx_device_t dev,
    void (*func)(int c, void *arg), void *arg)
{
	struct nrf_uarte_softc *sc;

	sc = mdx_device_get_softc(dev);

	sc->cb = func;
	sc->cb_arg = arg;
}

static void
nrf_uarte_setup(mdx_device_t dev, int baudrate, enum uart_databits databits,
    enum uart_stopbits stopbits, enum uart_parity parity)
{
	struct nrf_uarte_softc *sc;

	sc = mdx_device_get_softc(dev);

	/* TODO: use all the arguments. */

	sc->baudrate = baudrate;
	nrf_uarte_start(sc);
}


static struct mdx_uart_ops nrf_uarte_uart_ops = {
	.putc = nrf_uarte_putc,
	.setup = nrf_uarte_setup,
};

void
nrf_uarte_init(mdx_device_t dev, uint32_t base, uint8_t pin_tx, uint8_t pin_rx)
{
	struct nrf_uarte_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(*sc));
	sc->base = base;
	sc->pin_tx = pin_tx;
	sc->pin_rx = pin_rx;

	dev->ops = &nrf_uarte_uart_ops;
}

#ifdef MDX_OF
static int
nrf_uarte_probe(mdx_device_t dev)
{

	if (!mdx_of_is_compatible(dev, "nordic,nrf-uarte"))
		return (MDX_ERROR);

	return (MDX_OK);
}

static int
nrf_uarte_attach(mdx_device_t dev)
{
	struct nrf_uarte_softc *sc;
	int error;

	sc = mdx_device_get_softc(dev);

	error = mdx_of_get_reg(dev, 0, &sc->base, NULL);
	if (error)
		return (error);

	error = mdx_of_dev_get_prop32(dev, "tx-pin", &sc->pin_tx, NULL);
	if (error)
		return (error);

	error = mdx_of_dev_get_prop32(dev, "rx-pin", &sc->pin_rx, NULL);
	if (error)
		return (error);

	error = mdx_of_dev_get_prop32(dev, "current-speed", &sc->baudrate,
	    NULL);
	if (error)
		return (error);

	dev->ops = &nrf_uarte_uart_ops;

	mdx_of_setup_intr(dev, 0, nrf_uarte_intr, sc);

	mdx_uart_setup(dev, sc->baudrate, UART_DATABITS_8, UART_STOPBITS_1,
	    UART_PARITY_NONE);

	return (0);
}

static struct mdx_device_ops nrf_uarte_ops = {
	.probe = nrf_uarte_probe,
	.attach = nrf_uarte_attach,
};

static mdx_driver_t nrf_uarte_driver = {
	"nrf_uarte",
	&nrf_uarte_ops,
	sizeof(struct nrf_uarte_softc),
	nrf_uarte_compat_data,
};

DRIVER_MODULE_ORDERED(nrf_uarte, nrf_uarte_driver, SI_ORDER_SECOND);
#endif
