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
#include <sys/cheric.h>

#include <machine/frame.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#include <mips/mips/timer.h>
#include <mips/mips/trap.h>

#include <dev/uart/uart_16550.h>

extern char MipsException[], MipsExceptionEnd[];
extern char MipsTLBMiss[], MipsTLBMissEnd[];
extern char MipsCache[], MipsCacheEnd[];

#define	UART_BASE		0x180003f8
#define	UART_CLOCK_RATE		3686400
#define	DEFAULT_BAUDRATE	115200
#define	MIPS_DEFAULT_FREQ	1000000

static struct mips_timer_softc timer_sc;
static struct uart_16550_softc uart_sc;

struct mdx_device uart = {.sc = &uart_sc };

void * __capability kernel_sealcap;

void cpu_reset(void);
int main(void);

struct thread main_thread;
/* CHERI requires __CHERI_CAPABILITY_WIDTH__/8 byte alignment. */
uint8_t main_thread_stack[MDX_THREAD_STACK_SIZE] __aligned(16);

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

static void
mips_install_vectors(void)
{

#if 0
	if (MipsCacheEnd - MipsCache > 0x80)
		panic("Cache error code is too big");
	if (MipsTLBMissEnd - MipsTLBMiss > 0x80)
		panic("TLB code is too big");
#endif

	/* Install exception code. */
	bcopy(MipsException, (void *)MIPS_EXC_VEC_GENERAL,
	    MipsExceptionEnd - MipsException);

	bcopy(MipsException, (void *)CHERI_CCALL_EXC_VEC,
	    MipsExceptionEnd - MipsException);

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
board_init(void)
{
	uint64_t malloc_base;
	uint32_t status;
	int malloc_size;

	/* Setup capability-enabled JTAG UART. */
	setup_uart();

	printf("pcc == %#lp\n", cheri_getpcc());

	mips_install_vectors();

	mips_setup_intr(0, softintr, NULL);
	mips_setup_intr(1, softintr, NULL);
	mips_setup_intr(2, hardintr, NULL);
	mips_setup_intr(3, hardintr_unknown, NULL);
	mips_setup_intr(4, hardintr_unknown, NULL);
	mips_setup_intr(5, hardintr_unknown, NULL);
	mips_setup_intr(6, hardintr_unknown, NULL);
	mips_setup_intr(7, mips_timer_intr, &timer_sc);

	malloc_init();
	malloc_base = BASE_ADDR + 0x01000000;
	malloc_size = 0x01000000;
	malloc_add_region((void *)malloc_base, malloc_size);

	status = mips_rd_status();
	status &= ~(MIPS_SR_IM_M);
	status |= MIPS_SR_IM_HARD(5);
	status |= MIPS_SR_IE;
	status &= ~MIPS_SR_BEV;
	status |= MIPS_SR_UX;
	status |= MIPS_SR_KX;
	status |= MIPS_SR_SX;
	mips_wr_status(status);

	mips_timer_init(&timer_sc, MIPS_DEFAULT_FREQ);
}

static void
test_thr(void)
{

	while (1)
		printf("hi\n");
}

int
main(void)
{
	struct thread *td;

	td = mdx_thread_create("test", 1, 10000,
	    4096, test_thr, (void *)0);
	td->td_index = 0;
	mdx_sched_add(td);

	while (1)
		printf("Hello Hybrid Capability World\n");

	return (0);
}
