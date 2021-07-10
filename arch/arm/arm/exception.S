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

#ifdef MDX_ARM_THUMB_CM0
	.cpu cortex-m0
#else
	.cpu cortex-m4
#endif
	.syntax unified
	.thumb

	.globl exception
	.text
	.thumb_func
	.type exception, function
exception:
	mrs	r0, psp
	mrs	r1, ipsr		/* r1 = exception number. */
	mrs	r3, primask		/* r3 = primask. */
#ifndef MDX_ARM_THUMB_CM0
	stmdb	r0!, {r3-r11, r14}	/* Save primask, r4-r11 and r14. */
#else
	subs	r0, r0, 40
	str	r3, [r0, #0]
	str	r4, [r0, #4]
	str	r5, [r0, #8]
	str	r6, [r0, #12]
	str	r7, [r0, #16]
	mov	r7, r8
	str	r7, [r0, #20]
	mov	r7, r9
	str	r7, [r0, #24]
	mov	r7, r10
	str	r7, [r0, #28]
	mov	r7, r11
	str	r7, [r0, #32]
	mov	r7, r14
	str	r7, [r0, #36]
#endif
	cpsid	i			/* Disable interrupts. */

	bl	arm_exception		/* Branch link to arm_exception() */
#ifndef MDX_ARM_THUMB_CM0
	ldmia	r0!, {r3-r11, r14}	/* Load primask, r4-r11 and r14. */
#else
	ldr	r3, [r0, #0]
	ldr	r4, [r0, #4]
	ldr	r5, [r0, #8]
	ldr	r6, [r0, #12]
	ldr	r7, [r0, #20]
	mov	r8, r7
	ldr	r7, [r0, #24]
	mov	r9, r7
	ldr	r7, [r0, #28]
	mov	r10, r7
	ldr	r7, [r0, #32]
	mov	r11, r7
	ldr	r7, [r0, #36]
	mov	r14, r7
	ldr	r7, [r0, #16]
	adds	r0, r0, 40
#endif
	msr 	psp, r0			/* Restore sp. */
	msr	primask, r3		/* Restore interrupts. */
	bx	r14

	.globl __start
	.text
	.thumb_func
	.type __start, function
__start:

	/* Clear BSS */
	ldr	r1, =_sbss
	ldr	r2, =_ebss
#ifndef MDX_ARM_THUMB_CM0
	mov	r3, 0
#else
	movs	r3, 0
#endif
1:
	cmp	r1, r2
	beq	2f
#ifndef MDX_ARM_THUMB_CM0
	strb	r3, [r1], #1
#else
	strb	r3, [r1]
	adds	r1, #1
#endif
	b	1b
2:

	/* Relocate data */
	ldr	r0, =_smem
	ldr	r1, =_sdata
	ldr	r2, =_edata
1:
	cmp	r1, r2
	beq	2f
#ifndef MDX_ARM_THUMB_CM0
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
#else
	ldrb	r3, [r0]
	adds	r0, #1
	strb	r3, [r1]
	adds	r1, #1
#endif
	b	1b
2:

	/* Move msp to psp and switch thread mode to use psp. */
	mrs	r0, msp
	msr	psp, r0
	movs	r0, 2
	msr	control, r0

#ifndef MDX_ARM_THUMB_CM0
	mov	r0, 0	/* cpuid */
#else
	movs	r0, 0	/* cpuid */
#endif

	bl	md_init

	/* NOT REACHED */

#ifdef MDX_ARM_VFP
	.globl save_fpu_context
	.text
	.thumb_func
	.type save_fpu_context, function
save_fpu_context:
	vstmia  r0!, {s0-s31}		/* Save FPU registers. */
	vmrs	r1, fpscr		/* Read FPSCR. */
	str	r1, [r0]		/* Save FPSCR. */
	bx	lr

	.globl restore_fpu_context
	.text
	.thumb_func
	.type restore_fpu_context, function
restore_fpu_context:
	vldmia  r0!, {s0-s31}		/* Restore FPU registers. */
	ldr	r1, [r0]		/* Load FPSCR. */
	vmsr	fpscr, r1		/* Restore FPSCR. */
	bx	lr
#endif
