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
#include <sys/endian.h>
#include <sys/systm.h>
#include <sys/malloc.h>

#include <machine/frame.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>
#include <machine/cheric.h>

#include <dev/uart/uart_16550.h>

extern char MipsException[], MipsExceptionEnd[];
extern char MipsTLBMiss[], MipsTLBMissEnd[];
extern char MipsCache[], MipsCacheEnd[];

#define	USEC_TO_TICKS(n)	(100 * (n))	/* 100MHz clock. */
#define	UART_BASE		0x180003f8
#define	UART_CLOCK_RATE		3686400
#define	DEFAULT_BAUDRATE	115200
#define	MIPS_DEFAULT_FREQ	1000000

static struct uart_16550_softc uart_sc;

void * __capability kernel_sealcap;

void cpu_reset(void);
int main(void);

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

void
udelay(uint32_t usec)
{

}

static inline void __unused
trace_enable(void)
{

	__asm __volatile("li $zero, 0xbeef");
}

static inline void __unused
trace_disable(void)
{

	__asm __volatile("li $zero, 0xdead");
}

static void
setup_uart(void)
{
	capability cap;

	cap = cheri_getdefault();

	/* Remove default capability */
	__asm __volatile("csetdefault $cnull");

	cap = cheri_setoffset(cap, MIPS_XKPHYS_UNCACHED_BASE + UART_BASE);
	cap = cheri_csetbounds(cap, 6);

	uart_16550_init(&uart_sc, cap, UART_CLOCK_RATE, DEFAULT_BAUDRATE, 0);
	console_register(uart_putchar, (void *)&uart_sc);
}

int
main(void)
{

	/* Setup capability-enabled JTAG UART. */
	setup_uart();

	while (1)
		printf("Hello Pure Capability World\n");

	return (0);
}
