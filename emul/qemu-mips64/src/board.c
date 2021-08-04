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
#include <sys/thread.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/mutex.h>

#include <machine/frame.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#include <mips/mips/timer.h>
#include <mips/mips/trap.h>

#include <dev/uart/uart_16550.h>

#include "board.h"

extern char MipsException[], MipsExceptionEnd[];
extern char MipsTLBMiss[], MipsTLBMissEnd[];
extern char MipsCache[], MipsCacheEnd[];

static struct mips_timer_softc timer_sc;
static struct uart_16550_softc uart_sc;

struct mdx_device dev_uart = { .sc = &uart_sc };

#define	UART_BASE		0x180003f8
#define	UART_CLOCK_RATE		3686400
#define	DEFAULT_BAUDRATE	115200

void cpu_reset(void);

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

	mips_timer_udelay(&timer_sc, usec);
}

static void
mips_install_vectors(void)
{

	if (MipsCacheEnd - MipsCache > 0x80)
		panic("Cache error code is too big");
	if (MipsTLBMissEnd - MipsTLBMiss > 0x80)
		panic("TLB code is too big");

	/* Install exception code. */
	bcopy(MipsException, (void *)MIPS_EXC_VEC_GENERAL,
	    MipsExceptionEnd - MipsException);

	/* Install Cache Error code */
	bcopy(MipsCache, (void *)MIPS_EXC_VEC_CACHE_ERR,
	    MipsCacheEnd - MipsCache);

	/* Install TLB exception code */
	bcopy(MipsTLBMiss, (void *)MIPS_EXC_VEC_UTLB_MISS,
	    MipsTLBMissEnd - MipsTLBMiss);
	bcopy(MipsTLBMiss, (void *)MIPS_EXC_VEC_XTLB_MISS,
	    MipsTLBMissEnd - MipsTLBMiss);
}

void
board_init(void)
{
	uint64_t malloc_base;
	uint32_t status;
	int malloc_size;

	uart_16550_init(&dev_uart, (void *)(UART_BASE | 0xffffffffa0000000), 0,
	    UART_CLOCK_RATE);
	mdx_uart_setup(&dev_uart, DEFAULT_BAUDRATE,
	    UART_DATABITS_5,
	    UART_STOPBITS_1,
	    UART_PARITY_NONE);
	mdx_console_register_uart(&dev_uart);

	mips_install_vectors();

	mips_setup_intr(0, softintr, NULL);
	mips_setup_intr(1, softintr, NULL);
	mips_setup_intr(2, hardintr, NULL);
	mips_setup_intr(3, hardintr_unknown, NULL);
	mips_setup_intr(4, hardintr_unknown, NULL);
	mips_setup_intr(5, hardintr_unknown, NULL);
	mips_setup_intr(6, hardintr_unknown, NULL);
	mips_setup_intr(7, mips_timer_intr, &timer_sc);

	mips_timer_init(&timer_sc, MIPS_CPU_FREQ);

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
}
