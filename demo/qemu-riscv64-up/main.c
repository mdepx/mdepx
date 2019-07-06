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

#include <sys/cdefs.h>
#include <sys/console.h>
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/spinlock.h>
#include <sys/malloc.h>
#include <sys/list.h>
#include <sys/pcpu.h>
#include <sys/smp.h>

#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#include <dev/uart/uart_16550.h>
#include <riscv/sifive/e300g_clint.h>
#include <riscv/sifive/e300g_uart.h>

#define	CLINT_BASE		0x02000000
#define	UART_BASE		0x10000000
#define	UART_CLOCK_RATE		3686400
#define	DEFAULT_BAUDRATE	115200
#define	USEC_TO_TICKS(n)	(10 * (n))

static struct uart_16550_softc uart_sc;
static struct clint_softc clint_sc;

extern uint8_t __riscv_boot_ap[2];
extern uint32_t _sbss;
extern uint32_t _ebss;

static struct callout c1[1000] __unused;

static void
uart_putchar(int c, void *arg)
{
	struct uart_16550_softc *sc;

	sc = arg;

	if (sc == NULL)
		return;

	if (c == '\n')
		uart_16550_putc(sc, '\r');

	uart_16550_putc(sc, c);
}

static void __unused
cb(void *arg)
{
	struct callout *c;
	uint32_t ticks;

	c = arg;

	ticks = c->ticks_orig;
	ticks /= 2;
	printf("ticks %d\n", ticks);
	callout_set_locked(c, ticks, cb, (void *)c);
}

int
app_init(void)
{

	malloc_init();
	malloc_add_region(0x80800000, 0x7800000);

	uart_16550_init(&uart_sc, UART_BASE,
	    UART_CLOCK_RATE, DEFAULT_BAUDRATE, 0);
	console_register(uart_putchar, (void *)&uart_sc);

	e300g_clint_init(&clint_sc, CLINT_BASE);

	printf("Hello World\n");

	return (0);
}

int
main(void)
{
	int j;

	printf("cpu%d: pcpu size %d\n", PCPU_GET(cpuid), sizeof(struct pcpu));

	for (j = 0; j < 100; j++)
		callout_set(&c1[j], USEC_TO_TICKS(100000) * j, cb,
		    (void *)&c1[j]);

	while (1) {
		printf("Hello world\n");
		raw_sleep(USEC_TO_TICKS(1000000));
	}

	return (0);
}
