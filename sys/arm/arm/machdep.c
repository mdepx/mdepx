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

struct pcpu __pcpu;

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

	tf->tf_r14 = EXCP_RET_THREAD_MSP;

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
md_init(int arg)
{
	struct pcpu *pcpup;
	int cpuid;

	zero_bss();
	relocate_data();

	cpuid = 0;

	pcpup = &__pcpu;
	pcpup->pc_cpuid = cpuid;

	thread_init(cpuid);

#ifdef CONFIG_SCHED
	sched_init();
#endif

	/* Allow the app to register malloc and timer. */
	app_init();

#ifdef CONFIG_SCHED
	struct thread *td;
	td = thread_create("main", 1, 10000, 4096, main, NULL);
	if (td == NULL)
		panic("can't create the main thread\n");
	sched_enter();
#else
	main();
#endif
}
