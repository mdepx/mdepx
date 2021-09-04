/*-
 * Copyright (c) 2020 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/thread.h>
#include <sys/spinlock.h>
#include <sys/malloc.h>
#include <sys/mutex.h>
#include <sys/sem.h>
#include <sys/list.h>
#include <sys/smp.h>
#include <sys/of.h>

#include <riscv/sifive/e300g_clint.h>
#include <riscv/sifive/e300g_uart.h>

#include "board.h"

#define	CLINT_BASE		0x02000000
#define	PLIC_BASE		0x0c000000

#include <dev/uart/uart_16550.h>

extern uint8_t __riscv_boot_ap[MDX_CPU_MAX];

static struct uart_16550_softc uart_sc;
static struct clint_softc clint_sc;

struct mdx_device dev_uart = { .sc = &uart_sc };

char
uart_getchar(void)
{
	char a;

	a = mdx_uart_getc(&dev_uart);

	return (a);
}

void
board_init(void)
{

	/* Initialize malloc. */

	malloc_init();
	malloc_add_region((void *)0x80800000, 0x7800000);

	/* Once malloc has initialized, probe devices. */

	mdx_of_probe_devices();

	/* Timer. */

	e300g_clint_init(&clint_sc, (void *)CLINT_BASE, BOARD_CPU_FREQ);

	/* Release secondary core(s). */

#ifdef MDX_SCHED_SMP
	int j;

	printf("Releasing CPUs...\n");

	for (j = 0; j < MDX_CPU_MAX; j++)
		__riscv_boot_ap[j] = 1;
#endif
}
