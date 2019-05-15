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

static void
dump_frame(struct trapframe *tf)
{
	int i;

	printf("tf->tf_ra %x\n", tf->tf_ra);
	printf("tf->tf_sp %x\n", tf->tf_sp);
	printf("tf->tf_gp %x\n", tf->tf_gp);
	printf("tf->tf_tp %x\n", tf->tf_tp);

	for (i = 0; i < 7; i++)
		printf("tf->tf_t%d %x\n", i, tf->tf_t[i]);
	for (i = 0; i < 12; i++)
		printf("tf->tf_s%d %x\n", i, tf->tf_s[i]);
	for (i = 0; i < 8; i++)
		printf("tf->tf_a%d %x\n", i, tf->tf_a[i]);

	printf("tf->tf_mepc %x\n", tf->tf_mepc);
	printf("tf->tf_mstatus %x\n", tf->tf_mstatus);
	printf("tf->tf_mtval %x\n", tf->tf_mtval);
	printf("tf->tf_mcause %x\n", tf->tf_mcause);
}

static void
handle_exception(struct trapframe *tf)
{

	switch (tf->tf_mcause) {
	case EXCP_MACHINE_ECALL:
		tf->tf_mepc += 4;
		break;
	case EXCP_ILLEGAL_INSTRUCTION:
		printf("%s: illegal instruction at %x\n",
		    __func__, tf->tf_mepc);
	default:
		dump_frame(tf);
		panic("%s: unhandled exception 0x%x\n",
		    __func__, tf->tf_mcause);
	}
}

struct trapframe *
riscv_exception(struct trapframe *tf)
{
	struct trapframe *ret;
	struct thread *td;
	int irq;

	td = curthread;
	td->td_critnest++;

#ifdef	CONFIG_SCHED
#if 0
	/*
	 * Unsubscribe from notifications since this CPU
	 * just returned back from sleeping.
	 */
	struct pcpu *p;
	p = curpcpu;
	if (td->td_idle) {
		sched_lock();
		list_remove(&p->pc_node);
		sched_unlock();
	}
#endif

	/* Check if this thread went to sleep */
	sched_park(tf);
#endif

	if (tf->tf_mcause & EXCP_INTR) {
		irq = (tf->tf_mcause & EXCP_MASK);
		riscv_intr(tf, irq);
	} else
		handle_exception(tf);

#ifdef	CONFIG_SCHED
	ret = sched_next(tf);
#else
	ret = tf;
#endif

	td = curthread;
	td->td_critnest--;

	return (ret);
}
