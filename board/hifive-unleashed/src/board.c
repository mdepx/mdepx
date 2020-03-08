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

#include <sys/cdefs.h>
#include <sys/console.h>
#include <sys/list.h>
#include <sys/malloc.h>
#include <sys/mutex.h>
#include <sys/pcpu.h>
#include <sys/sem.h>
#include <sys/smp.h>
#include <sys/spinlock.h>
#include <sys/systm.h>
#include <sys/thread.h>

#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#include <riscv/sifive/e300g_clint.h>
#include <riscv/sifive/e300g_uart.h>

#include "board.h"

#define	CLINT_BASE		0x02000000
#define	UART_BASE		0x10000000
#define	UART_CLOCK_RATE		3686400
#define	DEFAULT_BAUDRATE	115200

static struct uart_softc uart_sc;
static struct clint_softc clint_sc;

extern uint8_t __riscv_boot_ap[2];
extern uint32_t _sbss;
extern uint32_t _ebss;

#ifdef MDX_SCHED_SMP
uint8_t mp_release __section(".data") = 0;
#endif

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

	malloc_init();
	malloc_add_region(0x80800000, 0x7800000);

	e300g_uart_init(&uart_sc, 0x10010000,
	    500000000, DEFAULT_BAUDRATE);
	mdx_console_register(uart_putchar, (void *)&uart_sc);

	e300g_clint_init(&clint_sc, CLINT_BASE, BOARD_TIMER_FREQ);

#ifdef MDX_SCHED_SMP
	int j;
	printf("Releasing CPUs...\n");
	for (j = 2; j < 5; j++)
		__riscv_boot_ap[j] = 1;
#endif
}
