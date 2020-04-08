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

#include <machine/cpuregs.h>
#include <machine/frame.h>
#include <machine/scs.h>
#include <machine/vfp.h>

#include <arm/arm/nvic.h>

struct trapframe *arm_exception(struct trapframe *tf, int irq);

#ifdef MDX_SCHED
static struct thread intr_thread[MDX_CPU_MAX];
#endif

#ifdef MDX_SCHED_SMP
#error Add support
#endif

uint8_t intr_stack[MDX_CPU_MAX][MDX_ARM_INTR_STACK_SIZE];

void save_fpu_context(struct vfp_state *vfp);
void restore_fpu_context(struct vfp_state *vfp);

static void
dump_frame(struct trapframe *tf)
{

	printf("tf %p\n", tf);
	printf("tf->hwregs->r0 == %x\n", tf->hwregs.r0);
	printf("tf->hwregs->r1 == %x\n", tf->hwregs.r1);
	printf("tf->hwregs->r2 == %x\n", tf->hwregs.r2);
	printf("tf->hwregs->r3 == %x\n", tf->hwregs.r3);
	printf("tf->hwregs->r12 == %x\n", tf->hwregs.r12);
	printf("tf->hwregs->r14 (lr) == %x\n", tf->hwregs.r14);
	printf("tf->hwregs->r15 (pc) == %x\n", tf->hwregs.r15);

	printf("tf->tf_r4 == %x\n", tf->tf_r4);
	printf("tf->tf_r5 == %x\n", tf->tf_r5);
	printf("tf->tf_r6 == %x\n", tf->tf_r6);
	printf("tf->tf_r7 == %x\n", tf->tf_r7);
	printf("tf->tf_r8 == %x\n", tf->tf_r8);
	printf("tf->tf_r9 == %x\n", tf->tf_r9);
	printf("tf->tf_r10 == %x\n", tf->tf_r10);
	printf("tf->tf_r11 == %x\n", tf->tf_r11);
	printf("tf->tf_r14 == %x\n", tf->tf_r14);
}

static void
handle_exception(struct trapframe *tf, int exc_code)
{
	uint32_t reg;
	uint16_t ufsr;
#ifdef MDX_ARM_VFP
	struct pcb *pcb;
#endif

	switch (exc_code) {
	case EXCP_SVCALL:
		break;
	case EXCP_HARD_FAULT:
		reg = SCS_RD4(SCB_HFSR);
		if (reg & HFSR_FORCED) {
			ufsr = SCS_RD2(SCB_UFSR);
			if (ufsr & UFSR_NOCP) {
#ifdef MDX_ARM_VFP
				pcb = &curthread->td_pcb;
				if ((pcb->pcb_flags & PCB_FLAGS_FPU) == 0) {
					vfp_control(true);
					pcb->pcb_flags |= PCB_FLAGS_FPU;
					break;
				}
#else
				printf("VFP is not available\n");
#endif
			}
		}
		dump_frame(tf);
		panic("Hardfault (HFSR %x)", reg);
	default:
		dump_frame(tf);
		panic("unhandled exception %d", exc_code);
	}
}

#ifdef MDX_SCHED

static bool
save_fpu(struct thread *td)
{
	bool fpu_is_enabled;

	fpu_is_enabled = false;

#ifdef MDX_ARM_VFP
	struct pcb *pcb;
	pcb = &td->td_pcb;
	if (pcb->pcb_flags & PCB_FLAGS_FPU) {
		fpu_is_enabled = true;
		save_fpu_context(&pcb->pcb_vfp);
	}
#endif

	return (fpu_is_enabled);
}

static void
restore_fpu(struct thread *td, bool fpu_was_enabled)
{
#ifdef MDX_ARM_VFP
	struct pcb *pcb;

	pcb = &td->td_pcb;
	if (pcb->pcb_flags & PCB_FLAGS_FPU) {
		if (fpu_was_enabled == false)
			vfp_control(true);
		restore_fpu_context(&pcb->pcb_vfp);
	} else
		if (fpu_was_enabled == true)
			vfp_control(false);
#endif
}

struct trapframe *
arm_exception(struct trapframe *tf, int exc_code)
{
	struct thread *td;
	bool fpu_was_enabled;
	bool released;
	bool intr;
	uint32_t irq;

	td = curthread;
	fpu_was_enabled = false;
	released = false;
	intr = false;

	/*
	 * Save the frame address.
	 * TODO: compare thread's stack base and tf.
	 */
	td->td_tf = tf;

	/*
	 * Any thread that is leaving the CPU could be added back to
	 * the run queue by interrupt handlers, that means we have to
	 * ACK and release it before processing interrupts.
	 */
	if (exc_code >= 16) {
		/* We will handle the interrupt later. */
		irq = exc_code - 16;
		intr = true;
	} else
		handle_exception(tf, exc_code);

	switch (td->td_state) {
	case TD_STATE_RUNNING:
		/* Current thread is still running. Do not switch. */
		break;
	case TD_STATE_TERMINATING:
		/* This thread has finished work. */
		mdx_thread_terminate_cleanup(td);
		released = true;
		break;
	case TD_STATE_SEM_WAIT:
	case TD_STATE_SLEEPING:
		fpu_was_enabled = save_fpu(td);
		td->td_state = TD_STATE_ACK;
		/* Note that this thread can now be used by another CPU. */
		released = true;
		break;
	case TD_STATE_YIELDING:
	case TD_STATE_READY:
		fpu_was_enabled = save_fpu(td);
		mdx_sched_add(td);
		released = true;
		break;
	case TD_STATE_WAKEUP:
	default:
		panic("unknown state %d, thread name %s",
		    td->td_state, td->td_name);
		break;
	}

	/*
	 * Switch to the interrupt thread if we don't have
	 * a thread anymore.
	 */
	if (released)
		PCPU_SET(curthread, &intr_thread[PCPU_GET(cpuid)]);

	curthread->td_critnest++;

	if (intr)
		arm_nvic_intr(irq, NULL);

	if (released) {
		/* We don't have a thread to run. Pick a next one. */
		td = mdx_sched_next();
		restore_fpu(td, fpu_was_enabled);
	}

	curthread->td_critnest--;

	/* Switch to the new thread. */
	if (released)
		PCPU_SET(curthread, td);

	return (td->td_tf);
}

#else

struct trapframe *
arm_exception(struct trapframe *tf, int exc_code)
{
	struct thread *td;
	uint32_t irq;

	td = curthread;
	td->td_tf = tf;
	td->td_critnest++;

	if (exc_code >= 16) {
		irq = exc_code - 16;
		arm_nvic_intr(irq, tf);
	} else
		handle_exception(tf, exc_code);

	td->td_critnest--;

	return (td->td_tf);
}

#endif
