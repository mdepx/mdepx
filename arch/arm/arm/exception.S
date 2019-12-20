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

	.cpu cortex-m4
	.syntax unified
	.thumb

	.globl exception
	.text
	.thumb_func
	.type exception, function
exception:
	mrs	r1, ipsr		/* r1 = exception number. */
	mrs	r3, primask		/* r3 = primask. */
	stmdb	sp!, {r3-r11, r14}	/* Save primask, r4-r11 and r14. */
	cpsid	i			/* Disable interrupts. */
	mov	r0, sp			/* r0 = trapframe. */

	/* Setup interrupt stack. */
	ldr	r4, =intr_stack		/* r4 = interrupt stack (bottom) */
	add	r4, r4, MDX_ARM_INTR_STACK_SIZE	/* interrupt stack (top) */
	msr	msp, r4

	bl	arm_exception		/* Branch link to arm_exception() */
	ldmia	r0!, {r3-r11, r14}	/* Load primask, r4-r11 and r14. */
	msr 	msp, r0			/* Restore sp. */
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
	mov	r3, 0
1:
	cmp	r1, r2
	beq	2f
	strb	r3, [r1], #1
	b	1b
2:

	/* Relocate data */
	ldr	r0, =_smem
	ldr	r1, =_sdata
	ldr	r2, =_edata
1:
	cmp	r1, r2
	beq	2f
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
	b	1b
2:

	mov	r0, 0	/* cpuid */
	bl	md_init
