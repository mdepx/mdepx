/*-
 * Copyright (c) 2018-2020 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/pcpu.h>
#include <sys/systm.h>
#include <sys/thread.h>

#include <machine/frame.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

uint8_t intr_stack[MDX_CPU_MAX][MDX_ARM_INTR_STACK_SIZE];

void
critical_enter(void)
{
	struct thread *td;
	register_t reg;

	td = curthread;

	if (td->td_critnest == 0) {
		reg = intr_disable();
		td->td_critnest = 1;
		td->td_md.md_saved_primask = reg;
	} else
		td->td_critnest++;
}

void
critical_exit(void)
{
	struct thread *td;

	td = curthread;

	td->td_critnest--;
	if (td->td_critnest == 0)
		intr_restore(td->td_md.md_saved_primask);
}

void
cpu_idle(void)
{

	critical_enter();
	__asm __volatile(
		"dsb \n"
		"wfi \n");
	critical_exit();
}

void
cpu_nullop(void)
{

	__asm __volatile("nop");
}

void
md_setup_frame(struct trapframe *tf, void *entry,
    void *arg, void *terminate)
{
	struct hwregs *hw;

	tf->tf_r14 = EXCP_RET | EXCP_RET_FTYPE | EXCP_RET_MODE_THREAD;
	tf->tf_r14 |= EXCP_RET_SPSEL_PSP;

	/* Security Extensions only. Ignored on non-trustzone devices. */
	tf->tf_r14 |= EXCP_RET_DCRS;
	tf->tf_r14 |= EXCP_RET_RESERVED2;

#ifndef	MDX_ARM_TRUSTZONE
	tf->tf_r14 |= EXCP_RET_ES_SECURE;
	tf->tf_r14 |= EXCP_RET_S_SECURE;
#endif

	hw = (struct hwregs *)&tf->hwregs;
	hw->r0 = (uint32_t)arg;
	hw->r14 = (uint32_t)terminate;
	hw->r15 = (uint32_t)entry;
	hw->xpsr = XPSR_T;
}

void
md_thread_yield(void)
{

	__asm __volatile("svc 0");
}

void
md_thread_terminate(struct thread *td)
{

}

/*
 * The cpuid must be 0 here.
 */
void
md_init(int cpuid)
{
#ifdef MDX_CPU
	struct pcpu *pcpup;

	pcpup = &__pcpu[cpuid];
	pcpup->pc_cpuid = cpuid;
	list_init(&pcpup->pc_avail);
	pcpup->pc_stack = (uintptr_t)&intr_stack[cpuid] +
	    MDX_ARM_INTR_STACK_SIZE;
	__asm __volatile("msr msp, %0" :: "r"(pcpup->pc_stack));
#endif

#ifdef MDX_THREAD
	mdx_thread_init(cpuid);
#endif

#ifdef MDX_SCHED
	mdx_sched_init();
#endif

	/* Initialize the board (register timer and/or malloc if required). */
	board_init();

#ifdef MDX_SCHED_SMP
	mdx_sched_cpu_add(pcpup);
#endif

#ifdef MDX_THREAD
	mdx_thread_main();
#endif

	panic("md_init returned");
}
