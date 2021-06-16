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
#include <sys/console.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/thread.h>

#include <machine/cpufunc.h>
#include <machine/cpuregs.h>
#include <machine/scs.h>

#include <dev/intc/intc.h>
#include <dev/uart/uart.h>

#include <arm/arm/nvic.h>
#include <arm/raspberrypi/rp2040.h>

static struct rp2040_timer_softc timer_sc;
static struct rp2040_xosc_softc xosc_sc;
static struct rp2040_clocks_softc clocks_sc;
static struct rp2040_resets_softc resets_sc;
static struct rp2040_io_bank0_softc io_bank0_sc;
static struct rp2040_uart_softc uart_sc;
static struct arm_nvic_softc nvic_sc;

struct mdx_device dev_nvic = { .sc = &nvic_sc };
struct mdx_device dev_uart = { .sc = &uart_sc };

void
udelay(uint32_t usec)
{
	int i;

	/* TODO: implement me */

	for (i = 0; i < usec * 100; i++)
		;
}

void
usleep(uint32_t usec)
{

	mdx_usleep(usec);
}

void
board_init(void)
{

	malloc_init();
	malloc_add_region((void *)0x20020000, 0x20000);

	rp2040_xosc_init(&xosc_sc, RP2040_XOSC_BASE);
	rp2040_clocks_init(&clocks_sc, RP2040_CLOCKS_BASE);

	rp2040_resets_init(&resets_sc, RP2040_RESETS_BASE);
	rp2040_resets_reset(&resets_sc, RESETS_RESET_IOBANK0, 0);
	rp2040_resets_reset(&resets_sc, RESETS_RESET_UART0, 0);
	rp2040_resets_reset(&resets_sc, RESETS_RESET_TIMER, 0);

	rp2040_io_bank0_init(&io_bank0_sc, RP2040_IO_BANK0_BASE);
	rp2040_io_bank0_funcsel(&io_bank0_sc, 0,
	    IO_BANK0_GPIO0_CTRL_FUNCSEL_UART_TX);
	rp2040_io_bank0_funcsel(&io_bank0_sc, 1,
	    IO_BANK0_GPIO1_CTRL_FUNCSEL_UART_RX);

	arm_nvic_init(&dev_nvic, BASE_NVIC);
	mdx_intc_setup(&dev_nvic, 0, rp2040_timer_intr, &timer_sc);
	mdx_intc_enable(&dev_nvic, 0);

	rp2040_uart_init(&dev_uart, RP2040_UART0_BASE);
	mdx_uart_setup(&dev_uart, 115200, UART_DATABITS_8, UART_STOPBITS_1,
	    UART_PARITY_NONE);
	mdx_console_register_uart(&dev_uart);

	rp2040_timer_init(&timer_sc, RP2040_TIMER_BASE, 20000);

	printf("%s: initialized\n", __func__);
}
