/*-
 * Copyright (c) 2019-2020 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/smp.h>
#include <sys/systm.h>
#include <sys/thread.h>

#include <machine/atomic.h>
#include <machine/frame.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

/* Interrupt stack */
static uint8_t intr_stack[MDX_CPU_MAX][MDX_RISCV_INTR_STACK_SIZE];
uint8_t __riscv_boot_ap[MDX_CPU_MAX];

#ifdef MDX_CPU
static uint32_t ncpus;
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

void __weak_symbol
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
#ifdef MDX_RISCV_SUPERVISOR_MODE
	tf->tf_status = SSTATUS_SPIE | (SSTATUS_SPP);
#else
	tf->tf_mstatus = MSTATUS_MPIE | (MSTATUS_MPP_MASK << MSTATUS_MPP_SHIFT);
#endif
	tf->tf_epc = (register_t)entry;
	tf->tf_a[0] = (register_t)arg;
	tf->tf_ra = (register_t)terminate;
}

void
md_thread_yield(void)
{

#ifdef MDX_SUPERVISOR_MODE
	__asm __volatile("ebreak");
#else
	__asm __volatile("ecall");
#endif
}

void
md_thread_terminate(struct thread *td)
{

}

#ifdef MDX_SCHED_SMP
void
md_init_secondary(int hart)
{
	struct pcpu *pcpup;
	int cpu;

	cpu = atomic_fetchadd_int(&ncpus, 1);

	pcpup = &__pcpu[cpu];
	pcpup->pc_cpuid = hart;
	pcpup->pc_stack = (uintptr_t)&intr_stack[hart] +
	    MDX_RISCV_INTR_STACK_SIZE;
	list_init(&pcpup->pc_avail);
	__asm __volatile("mv gp, %0" :: "r"(pcpup));
#ifdef MDX_RISCV_SUPERVISOR_MODE
	csr_write(sscratch, pcpup->pc_stack);
#else
	csr_write(mscratch, pcpup->pc_stack);
#endif

	csr_set(mie, MIE_MSIE);

	csr_clear(mie, MIE_MTIE);
	csr_clear(mip, MIP_MTIP);

	mdx_thread_init(hart);

	mdx_sched_cpu_add(pcpup);
	mdx_sched_cpu_avail(pcpup, true);

	intr_enable();
	mdx_sched_enter();
}
#endif

/*
 * The hart must be 0 here.
 * Use md_init_secondary() for harts > 0.
 */
void
md_init(int hart)
{
#ifdef MDX_CPU
	struct pcpu *pcpup;

	ncpus = 0;

	pcpup = &__pcpu[ncpus++];
	pcpup->pc_cpuid = hart;
	pcpup->pc_stack = (uintptr_t)&intr_stack[hart] +
	    MDX_RISCV_INTR_STACK_SIZE;
	list_init(&pcpup->pc_avail);
	__asm __volatile("mv gp, %0" :: "r"(pcpup));
#ifdef MDX_RISCV_SUPERVISOR_MODE
	csr_write(sscratch, pcpup->pc_stack);
#else
	csr_write(mscratch, pcpup->pc_stack);
#endif
#endif

#ifdef MDX_THREAD
	mdx_thread_init(hart);
#endif

#ifdef MDX_SCHED
	mdx_sched_init();
#ifdef MDX_SCHED_SMP
	smp_init();
#endif
#endif

	/* Enable supervisor software interrupts. */
#ifdef MDX_SCHED_SMP
#ifdef MDX_RISCV_SUPERVISOR_MODE
	csr_set(sie, SIE_SSIE);
#else
	csr_set(mie, MIE_MSIE);
#endif
#endif

	/* Initialize the board (register timer and/or malloc if required). */
	board_init();

#ifdef MDX_SCHED_SMP
	mdx_sched_cpu_add(pcpup);
#endif

	intr_enable();

#ifdef MDX_THREAD
	mdx_thread_main();
#endif

	panic("md_init returned");
}
