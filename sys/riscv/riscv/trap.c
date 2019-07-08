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
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/pcpu.h>

#include <machine/frame.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#include <riscv/riscv/trap.h>

#define	TRAP_DEBUG
#undef	TRAP_DEBUG

#ifdef	TRAP_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#ifdef MDX_SCHED
static struct thread intr_thread[MAXCPU];
#endif

static void
dump_frame(struct trapframe *tf)
{
	int i;

	printf("tf->tf_ra %zx\n", tf->tf_ra);
	printf("tf->tf_sp %zx\n", tf->tf_sp);
	printf("tf->tf_gp %zx\n", tf->tf_gp);
	printf("tf->tf_tp %zx\n", tf->tf_tp);

	for (i = 0; i < 7; i++)
		printf("tf->tf_t%d %zx\n", i, tf->tf_t[i]);
	for (i = 0; i < 12; i++)
		printf("tf->tf_s%d %zx\n", i, tf->tf_s[i]);
	for (i = 0; i < 8; i++)
		printf("tf->tf_a%d %zx\n", i, tf->tf_a[i]);

	printf("tf->tf_mepc %zx\n", tf->tf_mepc);
	printf("tf->tf_mstatus %zx\n", tf->tf_mstatus);
	printf("tf->tf_mtval %zx\n", tf->tf_mtval);
	printf("tf->tf_mcause %zx\n", tf->tf_mcause);
}

static void
handle_exception(struct trapframe *tf)
{

	switch (tf->tf_mcause) {
	case EXCP_MACHINE_ECALL:
		tf->tf_mepc += 4;
		break;
	case EXCP_ILLEGAL_INSTRUCTION:
		printf("%s: illegal instruction at %zx\n",
		    __func__, tf->tf_mepc);
	default:
		dump_frame(tf);
		panic("%s: unhandled exception 0x%zx\n",
		    __func__, tf->tf_mcause);
	}
}

#ifdef MDX_SCHED
struct trapframe *
riscv_exception(struct trapframe *tf)
{
	struct thread *td;
	struct pcpu *p;
	bool released;
	bool intr;
	int irq;

	td = curthread;
	p = curpcpu;
	released = false;
	intr = false;

	/* This CPU could not pick up new threads for a moment. */
	if (td->td_idle)
		sched_cpu_avail(p, false);

	/* Switch to the interrupt thread. */
	PCPU_SET(curthread, &intr_thread[PCPU_GET(cpuid)]);
	critical_enter();

	/* Check the trapframe first before we release this thread. */
	if (tf->tf_mcause & EXCP_INTR) {
		irq = (tf->tf_mcause & EXCP_MASK);
		intr = true;
	} else
		handle_exception(tf);

	/*
	 * Check if this thread went to sleep and release it from this CPU.
	 * Note that tf can not be used after this call since this thread
	 * could be added back to run queue by another CPU in mtx_unlock()
	 * or by this cpu in riscv_intr().
	 */
	released = sched_ack(td, tf);

	if (intr)
		riscv_intr(irq);

	/* Check if this thread has no more CPU time. */
	if (!released)
		released = sched_park(td);

	/* Pickup new thread if we don't have one anymore. */
	if (released)
		td = sched_next();

	/* Switch to the new thread. */
	critical_exit();
	PCPU_SET(curthread, td);

	return (td->td_tf);
}

#else

struct trapframe *
riscv_exception(struct trapframe *tf)
{
	struct thread *td;
	bool intr;
	int irq;

	td = curthread;
	intr = false;

	curthread->td_critnest++;

	if (tf->tf_mcause & EXCP_INTR) {
		irq = (tf->tf_mcause & EXCP_MASK);
		intr = true;
	} else
		handle_exception(tf);

	td->td_tf = tf;
	if (intr)
		riscv_intr(irq);

	curthread->td_critnest--;

	return (td->td_tf);
}
#endif
