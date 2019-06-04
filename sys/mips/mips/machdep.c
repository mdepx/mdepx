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
#include <sys/pcpu.h>

#include <machine/frame.h>
#include <machine/cpufunc.h>

uint8_t secondary_stacks[MAXCPU][4096]; /* Idle thread stacks */
struct pcpu __pcpu[MAXCPU];

void
critical_enter(void)
{
	struct thread *td;
	register_t reg;

	td = curthread;

	if (td->td_critnest == 0) {
		reg = intr_disable();
		td->td_critnest = 1;
		td->td_md.md_saved_intr = reg;
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
		intr_restore(td->td_md.md_saved_intr);
}

/*
 * This should be based on WII bit in Config7 register, however some
 * implementations (e.g. BERI CPU) do not set WII bit correctly.
 */
#ifdef CONFIG_CPU_IDLE_IE
void
cpu_idle(void)
{

	__asm __volatile("wait");
}
#else
void
cpu_idle(void)
{

	critical_enter();
	__asm __volatile("wait");
	critical_exit();
}
#endif

void
md_setup_frame(struct trapframe *tf, void *entry,
    void *arg, void *terminate)
{

	tf->tf_ra = (uintptr_t)terminate;
	tf->tf_pc = (uintptr_t)entry;
	tf->tf_a[0] = (uintptr_t)arg;
}

void
md_thread_yield(void)
{

	__asm __volatile("syscall");
}

void
md_thread_terminate(struct thread *td)
{

}

void
md_init(int cpuid)
{
	struct pcpu *pcpup;

	cpuid = 0;

	pcpup = &__pcpu[cpuid];
	pcpup->pc_cpuid = cpuid;
	__asm __volatile("move $28, %0" :: "r"(pcpup));

	thread_init(cpuid);

#ifdef CONFIG_SCHED
	sched_init();
#endif

	/* Allow the app to register malloc and timer. */
	app_init();

#ifdef CONFIG_SCHED
	struct thread *td;
	td = thread_create("main", 1, 50000000, 4096, main, NULL);
	if (td == NULL)
		panic("can't create the main thread\n");
	sched_enter();
#else
	main();
#endif

	panic("md_init returned");
}
