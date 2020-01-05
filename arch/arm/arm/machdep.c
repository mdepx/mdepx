/*-
 * Copyright (c) 2018-2019 Ruslan Bukin <br@bsdpad.com>
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

#include <machine/pcpu.h>
#include <machine/frame.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#ifndef MDX_THREAD_DYNAMIC_ALLOC
extern struct thread main_thread;
extern uint8_t main_thread_stack[MDX_THREAD_STACK_SIZE];
#endif

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
md_setup_frame(struct trapframe *tf, void *entry,
    void *arg, void *terminate)
{
	struct hwregs *hw;

	tf->tf_r14 = EXCP_RET | EXCP_RET_FTYPE | EXCP_RET_MODE_THREAD;

	/* Security Extensions only. Ignored on non-trustzone devices. */
	tf->tf_r14 |= EXCP_RET_DCRS;

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

void
md_init(int cpuid)
{
#ifdef MDX_CPU
	struct pcpu *pcpup;

	pcpup = &__pcpu[cpuid];
	pcpup->pc_cpuid = cpuid;
#endif

#ifdef MDX_THREAD
	mdx_thread_init(cpuid);
#endif

#ifdef MDX_SCHED
	mdx_sched_init();
#endif

	/* Allow the app to register malloc and timer. */
	app_init();

#ifdef MDX_SCHED
	struct thread *td;

#ifndef MDX_THREAD_DYNAMIC_ALLOC
	td = &main_thread;
	td->td_stack = (uint8_t *)main_thread_stack;
	td->td_stack_size = MDX_THREAD_STACK_SIZE;
	mdx_thread_setup(td, "main", 1, 10000, main, NULL);
#else
	td = mdx_thread_create("main", 1, 10000,
	    MDX_THREAD_STACK_SIZE, main, NULL);
	if (td == NULL)
		panic("can't create the main thread\n");
#endif

	mdx_sched_add(td);
	mdx_sched_cpu_add(pcpup);
	mdx_sched_cpu_avail(pcpup, true);

	mdx_sched_enter();
#else
	main();
#endif

	panic("md_init returned");
}
