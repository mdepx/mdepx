/*-
 * Copyright (c) 2018-2020 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/console.h>
#include <sys/types.h>
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/malloc.h>

#include <machine/cpufunc.h>
#include <machine/cpuregs.h>

#include <riscv/sifive/e300g.h>

struct clint_softc clint_sc;
struct aon_softc aon_sc;
struct spi_softc spi1_sc;
struct prci_softc prci_sc;
struct gpio_softc gpio_sc;
struct uart_softc uart_sc;

#define	BOARD_OSC_FREQ	32768

void
udelay(uint32_t usec)
{

	clint_udelay(&clint_sc, usec, BOARD_OSC_FREQ);
}

static void
uart_putchar(int c, void *arg)
{
	struct uart_softc *sc;

	sc = arg;

	if (sc == NULL)
		return;

	if (c == '\n')
		e300g_uart_putc(sc, '\r');

	e300g_uart_putc(sc, c);
}

void
board_init(void)
{
	uint32_t cpu_freq;

	e300g_clint_init(&clint_sc, CLINT_BASE, BOARD_OSC_FREQ);
	e300g_aon_init(&aon_sc, AON_BASE);
	e300g_prci_init(&prci_sc, PRCI_BASE);
	e300g_spi_init(&spi1_sc, SPI1_BASE);

	e300g_gpio_init(&gpio_sc, GPIO_BASE);
	e300g_iof0_enable(&gpio_sc, IOF0_UART0_RX | IOF0_UART0_TX);

	cpu_freq = clint_get_cpu_freq(&clint_sc, BOARD_OSC_FREQ);
	e300g_uart_init(&uart_sc, UART0_BASE, cpu_freq, 115200);
	mdx_console_register(uart_putchar, (void *)&uart_sc);
}
