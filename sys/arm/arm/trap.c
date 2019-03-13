/*-
 * Copyright (c) 2018 Ruslan Bukin <br@bsdpad.com>
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

#include <machine/cpuregs.h>
#include <machine/frame.h>

#include <arm/arm/nvic.h>

struct trapframe *arm_exception(struct trapframe *tf, int irq);

static void
dump_frame(struct trapframe *tf)
{

	printf("tf->hwregs->r0 == %x\n", tf->hwregs.r0);
	printf("tf->hwregs->r1 == %x\n", tf->hwregs.r1);
	printf("tf->hwregs->r2 == %x\n", tf->hwregs.r2);
	printf("tf->hwregs->r3 == %x\n", tf->hwregs.r3);
	printf("tf->hwregs->r12 == %x\n", tf->hwregs.r12);
	printf("tf->hwregs->r14 == %x\n", tf->hwregs.r14);
	printf("tf->hwregs->r15 == %x\n", tf->hwregs.r15);

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

	switch (exc_code) {
	case EXCP_SVCALL:
		break;
	case EXCP_HARD_FAULT:
		dump_frame(tf);
		panic("Hardfault");
	default:
		dump_frame(tf);
		panic("unhandled exception %d", exc_code);
	}
}

struct trapframe *
arm_exception(struct trapframe *tf, int exc_code)
{
	uint32_t irq;

	curthread->td_critnest++;

	if (exc_code >= 16) {
		irq = exc_code - 16;
		arm_nvic_intr(irq, tf);
	} else
		handle_exception(tf, exc_code);

	curthread->td_critnest--;

	return (sched_next(tf));
}
