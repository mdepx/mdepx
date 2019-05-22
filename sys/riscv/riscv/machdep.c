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
#include <sys/smp.h>

#include <machine/atomic.h>
#include <machine/frame.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>
#include <machine/smp.h>

static struct pcpu __pcpu[MAXCPU];
static uint32_t ncpus;
static size_t cpu_stacks[MAXCPU][4096]; /* Interrupt stack */

uint8_t __riscv_boot_ap[MAXCPU];
uint8_t secondary_stacks[MAXCPU][4096]; /* Idle thread stacks */

void
critical_enter(void)
{
	struct thread *td;
	register_t reg;

	td = curthread;

	if (td->td_critnest == 0) {
		reg = intr_disable();
		td->td_critnest = 1;
		td->td_md.md_saved_sstatus_ie = reg;
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
		intr_restore(td->td_md.md_saved_sstatus_ie);
}

void
cpu_idle(void)
{

	critical_enter();
	__asm __volatile(
	    "fence \n"
	    "wfi   \n");
	critical_exit();
}

void
md_setup_frame(struct trapframe *tf, void *entry,
    void *arg, void *terminate)
{

	bzero(tf, sizeof(struct trapframe));
	tf->tf_mstatus = MSTATUS_MPIE | (MSTATUS_MPP_MASK << MSTATUS_MPP_SHIFT);
	tf->tf_mepc = (register_t)entry;
	tf->tf_a[0] = (register_t)arg;
	tf->tf_ra = (register_t)terminate;
}

void
md_thread_yield(void)
{

	__asm __volatile("ecall");
}

void
md_thread_terminate(struct thread *td)
{

}

void
md_init_secondary(int hart)
{
	struct pcpu *pcpup;
	int cpu;

	cpu = atomic_fetchadd_int(&ncpus, 1);

	pcpup = &__pcpu[cpu];
	pcpup->pc_cpuid = hart;
	pcpup->pc_stack = (uintptr_t)&cpu_stacks[hart] + 4096 * 8;
	__asm __volatile("mv gp, %0" :: "r"(pcpup));
	csr_write(mscratch, pcpup->pc_stack);

	csr_set(mie, MIE_MSIE);

	csr_clear(mie, MIE_MTIE);
	csr_clear(mip, MIP_MTIP);

	thread_init(hart);
	sched_cpu_add(pcpup);

	intr_enable();
	sched_enter();
}

void
md_init(int hart)
{
	struct pcpu *pcpup;
	struct thread *td;

	ncpus = 0;

	pcpup = &__pcpu[ncpus++];
	pcpup->pc_cpuid = hart;
	pcpup->pc_stack = (uintptr_t)&cpu_stacks[hart] + 4096 * 8;
	__asm __volatile("mv gp, %0" :: "r"(pcpup));
	csr_write(mscratch, pcpup->pc_stack);

	thread_init(hart);
	sched_init();
	smp_init();

	csr_set(mie, MIE_MSIE);

	/* Allow the app to register malloc and timer. */
	app_init();

	td = thread_create("main", 1, 10000, 4096, main, NULL);
	if (td == NULL)
		panic("can't create the main thread\n");

	sched_cpu_add(pcpup);
	intr_enable();
	sched_enter();
}
