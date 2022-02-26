/*-
 * Copyright (c) 2019-2021 Ruslan Bukin <br@bsdpad.com>
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

#include <riscv/riscv/trap.h>

#define	TRAP_DEBUG
#undef	TRAP_DEBUG

#ifdef	TRAP_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#ifdef MDX_SCHED
static struct thread intr_thread[MDX_CPU_MAX];
#endif

#ifdef MDX_RISCV_FPE
void fpe_state_clear(void);
void fpe_state_save(uint64_t *);
void fpe_state_restore(uint64_t *);
#endif

static void
dump_frame(struct trapframe *tf)
{
	int i;

	printf("tf->tf_ra %#lp\n", tf->tf_ra);
	printf("tf->tf_sp %#lp\n", tf->tf_sp);
	printf("tf->tf_gp %#lp\n", tf->tf_gp);
	printf("tf->tf_tp %#lp\n", tf->tf_tp);

	for (i = 0; i < 7; i++)
		printf("tf->tf_t%d %#lp\n", i, tf->tf_t[i]);
	for (i = 0; i < 12; i++)
		printf("tf->tf_s%d %#lp\n", i, tf->tf_s[i]);
	for (i = 0; i < 8; i++)
		printf("tf->tf_a%d %#lp\n", i, tf->tf_a[i]);

	printf("tf->tf_epc %#lp\n", tf->tf_epc);
#if __has_feature(capabilities)
	printf("tf->tf_ddc %#lp\n", tf->tf_ddc);
#endif
	printf("tf->tf_status %jx\n", tf->tf_status);
	printf("tf->tf_tval %jx\n", tf->tf_tval);
	printf("tf->tf_cause %jx\n", tf->tf_cause);
}

static void
handle_exception(struct trapframe *tf)
{
#ifdef MDX_RISCV_FPE
	struct pcb *pcb;
#endif

	switch (tf->tf_cause & EXCP_MASK) {
	case EXCP_MACHINE_ECALL:
		tf->tf_epc += 4;
		break;
	case EXCP_BREAKPOINT:
		tf->tf_epc += 2;
		break;
	case EXCP_ILLEGAL_INSTRUCTION:
#ifdef MDX_RISCV_FPE
		pcb = &curthread->td_pcb;
		if ((pcb->pcb_flags & PCB_FLAGS_FPE_ENABLED) == 0) {
			fpe_state_clear();
			tf->tf_status &= ~SSTATUS_FS_MASK;
			tf->tf_status |= SSTATUS_FS_CLEAN;
			pcb->pcb_flags |= PCB_FLAGS_FPE_ENABLED;
			break;
		}
#endif
		printf("%s: illegal instruction at %zx\n",
		    __func__, tf->tf_epc);
	default:
		dump_frame(tf);
		panic("%s: unhandled exception 0x%zx\n",
		    __func__, tf->tf_cause);
	}
}

#ifdef MDX_SCHED
#ifdef MDX_RISCV_FPE
static bool
fpe_check_and_save(struct thread *td)
{
	struct pcb *pcb;

	switch (td->td_state) {
	case TD_STATE_SEM_WAIT:
	case TD_STATE_SLEEPING:
	case TD_STATE_YIELDING:
	case TD_STATE_READY:
		pcb = &td->td_pcb;
		if (pcb->pcb_flags & PCB_FLAGS_FPE_ENABLED)
			fpe_state_save(&pcb->pcb_x[0][0]);
		return (true);
	}

	return (false);
}
#endif
struct trapframe *
riscv_exception(struct trapframe *tf)
{
	struct thread *td;
	bool released, fpe_saved;
	bool intr;
	int irq;
#ifdef MDX_RISCV_FPE
	struct pcb *pcb;
#endif

	td = curthread;
	released = false;
	intr = false;
	fpe_saved = false;

	td->td_tf = tf;

#ifdef MDX_SCHED_SMP
	/* This CPU could not pick up new threads for a moment. */
	if (td->td_idle)
		mdx_sched_cpu_avail(curpcpu, false);
#endif

#ifdef MDX_RISCV_FPE
	/*
	 * Check if the thread will be released later in mdx_sched_ack()
	 * and save any floating point registers before the call.
	 */
	if (fpe_check_and_save(td))
		fpe_saved = true;
#endif

	/* Process the trapframe first before we release this thread. */
	if (tf->tf_cause & EXCP_INTR) {
		irq = (tf->tf_cause & EXCP_MASK);
		intr = true;
	} else
		handle_exception(tf);

	/*
	 * Check if this thread went to sleep and release it from this CPU.
	 * Note that td and tf can not be used after this call since this
	 * thread could be added back to the run queue by another CPU in
	 * mdx_mutex_unlock() or by this cpu in riscv_intr().
	 *
	 * Note this is needed in both interrupt/exception cases.
	 */
	released = mdx_sched_ack(td);

	/*
	 * Switch to the interrupt thread (stack is already replaced).
	 */
	PCPU_SET(curthread, &intr_thread[PCPU_GET(cpuid)]);
	curthread->td_critnest++;

	/*
	 * Service the interrupt. This includes timer interrupt, which means
	 * this should be done before the call to mdx_sched_park().
	 */
	if (intr)
		riscv_intr(irq);

	/* Check if this thread has no more CPU time. */
	if (!released) {
#ifdef MDX_RISCV_FPE
		/*
		 * Check if the thread will be released in mdx_sched_park()
		 * and save any floating point registers before the call.
		 */
		if (!fpe_saved)
			fpe_check_and_save(td);
#endif
		released = mdx_sched_park(td);
	}

	/* Pickup new thread if we don't have one anymore. */
	if (released) {
		td = mdx_sched_next();
#ifdef MDX_RISCV_FPE
		pcb = &td->td_pcb;
		if (pcb->pcb_flags & PCB_FLAGS_FPE_ENABLED)
			fpe_state_restore(&pcb->pcb_x[0][0]);
#endif
	}

	/* Switch to the new thread. */
	curthread->td_critnest--;
	PCPU_SET(curthread, td);

	return (td->td_tf);
}

#else /* !MDX_SCHED */

struct trapframe *
riscv_exception(struct trapframe *tf)
{
	struct thread *td;
	bool intr;
	int irq;

	td = curthread;
	intr = false;

	curthread->td_critnest++;

	if (tf->tf_cause & EXCP_INTR) {
		irq = (tf->tf_cause & EXCP_MASK);
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
