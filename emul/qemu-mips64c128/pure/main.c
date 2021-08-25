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
#include <sys/thread.h>
#include <sys/cheri.h>

#include <machine/frame.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#include <mips/mips/trap.h>
#include <mips/mips/timer.h>

#include <dev/uart/uart_16550.h>

extern char MipsException[], MipsExceptionEnd[];
extern char MipsTLBMiss[], MipsTLBMissEnd[];
extern char MipsCache[], MipsCacheEnd[];

#define	UART_BASE		0x180003f8
#define	UART_CLOCK_RATE		3686400
#define	DEFAULT_BAUDRATE	115200
#define	MIPS_DEFAULT_FREQ	50000000 /* 50MHz clock */

static struct mips_timer_softc timer_sc;
static struct uart_16550_softc uart_sc;

struct mdx_device uart = {.sc = &uart_sc };

void * __capability kernel_sealcap;

void cpu_reset(void);
int main(void);

static void
softintr(void *arg, struct trapframe *frame, int i)
{
	uint32_t cause;

	printf("Soft interrupt %d\n", i);

	cause = mips_rd_cause();
	cause &= ~(1 << (8 + i));
	mips_wr_cause(cause);
};

static void
hardintr_unknown(void *arg, struct trapframe *frame, int i)
{

	printf("Unknown hard interrupt %d\n", i);
}

static void
hardintr(void *arg, struct trapframe *frame, int i)
{
}

static void
mips_install_vectors(void)
{
	capability cap;
	capability a, b;

#if 0
	if (MipsCacheEnd - MipsCache > 0x80)
		panic("Cache error code is too big");
	if (MipsTLBMissEnd - MipsTLBMiss > 0x80)
		panic("TLB code is too big");
#endif

	a = MipsException;

	/*
	 * Mips exception code has two instructions only,
	 * so copy 8 bytes.
	 */
	cap = cheri_getdefault();
	b = cheri_setoffset(cap, MIPS_EXC_VEC_GENERAL);
	b = cheri_setbounds(b, 8);
	bcopy(a, b, 8);

	b = cheri_setoffset(cap, (long)CHERI_CCALL_EXC_VEC);
	b = cheri_setbounds(b, 8);
	bcopy(a, b, 8);

#if 0
	/* Install Cache Error code */
	bcopy(MipsCache, (void *)MIPS_EXC_VEC_CACHE_ERR,
	    MipsCacheEnd - MipsCache);

	/* Install TLB exception code */
	bcopy(MipsTLBMiss, (void *)MIPS_EXC_VEC_UTLB_MISS,
	    MipsTLBMissEnd - MipsTLBMiss);
	bcopy(MipsTLBMiss, (void *)MIPS_EXC_VEC_XTLB_MISS,
	    MipsTLBMissEnd - MipsTLBMiss);
#endif
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
	cap = cheri_setoffset(cap, MIPS_XKPHYS_UNCACHED_BASE + UART_BASE);
	cap = cheri_setbounds(cap, 6);

	uart_16550_init(&uart, cap, 0, UART_CLOCK_RATE);
	mdx_uart_setup(&uart, DEFAULT_BAUDRATE,
	    UART_DATABITS_5,
	    UART_STOPBITS_1,
	    UART_PARITY_NONE);
	mdx_console_register_uart(&uart);
}

void
board_init(void)
{
	capability malloc_base;
	uint32_t malloc_size;
	int status;
	int error;

	/* Setup capability-enabled JTAG UART. */
	setup_uart();

	mips_install_vectors();

	mips_setup_intr(0, softintr, NULL);
	mips_setup_intr(1, softintr, NULL);
	mips_setup_intr(2, hardintr, NULL);
	mips_setup_intr(3, hardintr_unknown, NULL);
	mips_setup_intr(4, hardintr_unknown, NULL);
	mips_setup_intr(5, hardintr_unknown, NULL);
	mips_setup_intr(6, hardintr_unknown, NULL);
	mips_setup_intr(7, mips_timer_intr, &timer_sc);

	status = mips_rd_status();
	status &= ~(MIPS_SR_IM_M);
	status |= MIPS_SR_IM_HARD(5);
	status |= MIPS_SR_IE;
	status &= ~MIPS_SR_BEV;
	status |= MIPS_SR_UX;
	status |= MIPS_SR_KX;
	status |= MIPS_SR_SX;
	mips_wr_status(status);

	malloc_base = cheri_getdefault();
	malloc_base = cheri_setoffset(malloc_base, BASE_ADDR + 0x01000000);
	malloc_size = 0x01000000;

	malloc_init();
	mdx_fl_init_datacap(malloc_base);
	malloc_add_region(malloc_base, malloc_size);

	error = mips_timer_init(&timer_sc, MIPS_DEFAULT_FREQ);
	if (error)
		printf("can't register the timer\n");

	/* Remove default capability */
	__asm __volatile("csetdefault $cnull");
}

struct token {
	int a;
};

static void
test_thr(void *arg)
{
	struct token *t;

	t = arg;

	while (1)
		printf("hi %d\n", t->a);
}

static void
test_thr1(void *arg)
{
	struct token *t;

	t = arg;

	while (1)
		printf("hi1 %d\n", t->a);
}

int
main(void)
{
	struct thread *td;
	struct token *t;

	t = malloc(sizeof(struct token));
	t->a = 1;
	td = mdx_thread_create("test", 1, 1000, 4 * 4096, test_thr, t);
	mdx_sched_add(td);

	t = malloc(sizeof(struct token));
	t->a = 2;
	td = mdx_thread_create("test1", 1, 1000, 4 * 4096, test_thr1, t);
	mdx_sched_add(td);

	while (1) {
		printf("Hello Pure Capability World\n");
		mdx_usleep(1000);
	}

	return (0);
}
