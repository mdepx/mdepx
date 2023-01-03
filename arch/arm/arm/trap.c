/*-
 * Copyright (c) 2018-2023 Ruslan Bukin <br@bsdpad.com>
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
#ifndef MDX_ARM_THUMB_CM0
	printf("tf->tf_r8 == %x\n", tf->tf_r8);
	printf("tf->tf_r9 == %x\n", tf->tf_r9);
	printf("tf->tf_r10 == %x\n", tf->tf_r10);
	printf("tf->tf_r11 == %x\n", tf->tf_r11);
#endif
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
	case 16 ... 1024:
		arm_nvic_intr(exc_code - 16);
		break;
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
					printf("VFP enabled for the thread.\n");
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

#ifdef MDX_ARM_VFP
static bool
fpu_check_and_save(struct thread *td)
{
	struct pcb *pcb;

	pcb = &td->td_pcb;

	if (pcb->pcb_flags & PCB_FLAGS_FPU) {
		switch (td->td_state) {
		case TD_STATE_SEM_WAIT:
		case TD_STATE_SLEEPING:
		case TD_STATE_YIELDING:
		case TD_STATE_READY:
			save_fpu_context(&pcb->pcb_vfp);
			break;
		}

		return (true);
	}

	return (false);
}

static void
restore_fpu(struct thread *td, bool fpu_was_enabled)
{
	struct pcb *pcb;

	pcb = &td->td_pcb;
	if (pcb->pcb_flags & PCB_FLAGS_FPU) {
		if (fpu_was_enabled == false)
			vfp_control(true);
		restore_fpu_context(&pcb->pcb_vfp);
	} else
		if (fpu_was_enabled == true)
			vfp_control(false);
}
#endif

/* Non-SMP version of exception handler. */

struct trapframe *
arm_exception(struct trapframe *tf, int exc_code)
{
	struct thread *td;
	bool released;
#ifdef MDX_ARM_VFP
	bool fpu_was_enabled;
#endif

	td = curthread;

	curthread->td_critnest++;
	if (curthread->td_critnest > 1) {
		/* Interrupts are disabled, so this must be nested exception.*/
		KASSERT(exc_code < 16, ("Nested interrupt is not expected."));
		handle_exception(tf, exc_code);
		curthread->td_critnest--;
		return (tf);
	}

	/*
	 * Save the frame address.
	 * TODO: compare thread's stack base and tf.
	 */
	td->td_tf = tf;

	/* Set ACK flag to the thread that is going to terminate or sleep. */
	released = mdx_sched_ack(td);

	/*
	 * A thread that is leaving CPU could be added back to the run queue by
	 * timer/sem timeout callback in arm_nvic_intr() by this CPU.
	 */
	handle_exception(tf, exc_code);

	if (!released)
		released = mdx_sched_park(td);

	if (released) {
		/* We don't have a thread to run. Pick the next one. */
#ifdef MDX_ARM_VFP
		fpu_was_enabled = fpu_check_and_save(td);
#endif
		td = mdx_sched_next();
#ifdef MDX_ARM_VFP
		restore_fpu(td, fpu_was_enabled);
#endif
	}

	/* Switch to the new (old) thread. */
	curthread->td_critnest--;
	PCPU_SET(curthread, td);

	return (td->td_tf);
}

#else /* !MDX_SCHED */

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
		arm_nvic_intr(irq);
	} else
		handle_exception(tf, exc_code);

	td->td_critnest--;

	return (td->td_tf);
}

#endif
