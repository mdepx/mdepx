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
#include <machine/cpufunc.h>

#if __has_feature(capabilities)
#include <machine/cheric.h>
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
#ifdef MDX_MIPS_QEMU
void
cpu_idle(void)
{

	/*
	 * Older mipses require interrupts turned on during sleep.
	 * This is a case in qemu Malta platform.
	 */
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
#if __has_feature(capabilities)

#ifdef __CHERI_PURE_CAPABILITY__
	tf->tf_pcc = entry;
#else
	capability cap;
	cap = cheri_getkcc();
	cap = cheri_setoffset(cap, (uintptr_t)entry);
	/* TODO: set bounds. */
	tf->tf_pcc = cap;

	/* Set some default data capability for this thread. */
	cap = cheri_getdefault();
	tf->tf_c[0] = cap;
#endif

#else /* !__has_feature(capabilities) */
	tf->tf_ra = (uintptr_t)terminate;
	tf->tf_pc = (uintptr_t)entry;
	tf->tf_a[0] = (uintptr_t)arg;
#endif
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
#ifdef __CHERI_PURE_CAPABILITY__
	__asm __volatile("csetkr1c %0" :: "r"(pcpup));
#else
	__asm __volatile("move $28, %0" :: "r"(pcpup));
#endif
#endif

#ifdef MDX_THREAD
	mdx_thread_init(cpuid);
#endif

#ifdef MDX_SCHED
	mdx_sched_init();
#endif

	/* Initialize the board (register timer and/or malloc if required). */
	board_init();

#ifdef MDX_THREAD
	mdx_thread_main();
#endif

	panic("md_init returned");
}
