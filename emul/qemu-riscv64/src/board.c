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
#include <machine/vmparam.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#include <dev/uart/uart_16550.h>

#include "board.h"

extern uint8_t __riscv_boot_ap[MDX_CPU_MAX];

static mdx_device_t uart;

char
uart_getchar(void)
{
	char a;

	a = mdx_uart_getc(uart);

	return (a);
}

void
board_init(void)
{
	void *addr;

	/* Initialize malloc. */

	addr = (void *)PHYS_TO_DMAP(0x90800000);

	malloc_init();
	malloc_add_region(addr, 0x7800000);

	/* Once malloc has initialized, probe devices. */
	mi_startup();

	uart = mdx_device_lookup_by_name("uart_16550", 0);

	csr_set(mie, MIE_MEIE);

	/* Release secondary core(s). */

#ifdef MDX_SCHED_SMP
	int j;

	printf("Releasing CPUs...\n");

	for (j = 0; j < MDX_CPU_MAX; j++)
		__riscv_boot_ap[j] = 1;
#endif
}
