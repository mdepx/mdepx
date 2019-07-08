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
#include <sys/mutex.h>
#include <sys/sem.h>
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

extern uint8_t __riscv_boot_ap[MDX_SMP_MAXCPU];

#ifdef MDX_SCHED
static struct mtx m __unused;
static struct mtx m1 __unused;
static sem_t sem;
#endif

static struct spinlock l1;
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

#ifdef MDX_SCHED
static void __unused
test_thr(void *arg)
{

	while (1) {
		if (!mtx_timedlock(&m, 1000))
			continue;
		printf("cpu%d: hello from thread%04d cn %d mstatus %x\n",
		    PCPU_GET(cpuid), (size_t)arg, curthread->td_critnest,
		    csr_read(mstatus));
		mtx_unlock(&m);

		raw_sleep(1000);
	}
}

static void __unused
test_thr1(void *arg)
{

	while (1) {
		printf("cpu%d: hello from thread%d\n",
		    PCPU_GET(cpuid), (size_t)arg);
		raw_sleep(10000000);
	}
}

static void __unused
test_thr2(void *arg)
{

	while (1) {
		printf("cpu%d: hello from thread%d\n",
		    PCPU_GET(cpuid), (size_t)arg);
		raw_sleep(5000000);
	}
}

static void __unused
test_m0(void *arg)
{

	while (1) {
		if (mtx_timedlock(&m1, 50000) == 0) {
			printf("again %ld\n", (uint64_t)arg);
			continue;
		}
		printf("test_m0 acuired the mutex\n");
		mtx_unlock(&m1);
	}
}

static void __unused
test_m1(void *arg)
{

	while (1) {
		if (mtx_timedlock(&m1, 50000) == 0) {
			printf("again %ld\n", (uint64_t)arg);
			continue;
		}
		printf("test_m1 acuired the mutex\n");
		mtx_unlock(&m1);
	}
}

static void __unused
test_m2(void *arg)
{

	while (1) {
		if (mtx_timedlock(&m1, 50000) == 0) {
			printf("again %ld\n", (uint64_t)arg);
			continue;
		}
		printf("test_m2 acuired the mutex\n");
		mtx_unlock(&m1);
	}
}
#endif

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

#ifdef MDX_SCHED
	sem_init(&sem, 1);
	mtx_init(&m);
#endif

	e300g_clint_init(&clint_sc, CLINT_BASE);

	printf("Hello World\n");

	return (0);
}

static void __unused
test(void *arg)
{

	printf("%d", PCPU_GET(cpuid));
}

static void __unused
hello(void *arg)
{

	if (PCPU_GET(cpuid) == 0)
		printf("\n\nhello\n\n");
}

int
main(void)
{
	int j;

	printf("cpu%d: pcpu size %d\n", PCPU_GET(cpuid), sizeof(struct pcpu));
	sl_init(&l1);

#ifdef MDX_SMP
	printf("Releasing CPUs...\n");
	for (j = 0; j < 8; j++)
		__riscv_boot_ap[j] = 1;
#endif

	/* Some testing routines. */
#if 0
	struct thread *td;
	size_t i;
	for (i = 0; i < 20; i++) {
		td = thread_create("test", 1, USEC_TO_TICKS(1000),
		    4096, test_thr, (void *)i);
		if (td == NULL)
			break;
		printf("td %p created\n", td);
	}
#endif

#ifdef MDX_SCHED
	struct thread *td;
	size_t i;
	for (i = 1; i < 500; i++) {
		td = thread_create("test", 1, (USEC_TO_TICKS(1000) * i),
		    4096, test_thr, (void *)i);
		if (td == NULL)
			break;
		td->td_index = i;
	}
#endif

#if 0
	thread_create("test", 1, USEC_TO_TICKS(1000),
	    4096, test_thr1, (void *)0);
	thread_create("test", 1, USEC_TO_TICKS(2000),
	    4096, test_thr2, (void *)1);
	thread_create("test", 1, USEC_TO_TICKS(2000),
	    4096, test_thr2, (void *)2);
#endif

#if 0
	struct thread *td;
	td = thread_create("test1", 1, USEC_TO_TICKS(500),
	    4096, test_m0, (void *)0);
	td->td_index = 0;
	td = thread_create("test2", 1, USEC_TO_TICKS(500),
	    4096, test_m1, (void *)1);
	td->td_index = 1;
	td = thread_create("test3", 1, USEC_TO_TICKS(500),
	    4096, test_m2, (void *)2);
	td->td_index = 2;
#endif

#if 1
	for (j = 0; j < 100; j++)
		callout_set(&c1[j], USEC_TO_TICKS(100000) * j, cb,
		    (void *)&c1[j]);
#endif

#ifdef MDX_SMP
	char a;
	while (1) {
		a = uart_16550_getc(&uart_sc);
		if (a == 0x61) {
			critical_enter();
			smp_tryst_cpus(0xf, hello, NULL);
			critical_exit();
		}
	}
#endif

	while (1) {
		printf("Hello world\n");
		raw_sleep(USEC_TO_TICKS(1000000));
	}

	return (0);
}
