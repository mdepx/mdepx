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

#include <machine/asm.h>

ASM_ENTRY(_start)
	fence.i
	fence

	la	t0, cpu_exception_handler
	csrw	mtvec, t0

	/*
	 * CPU0 (E51) does not support lr/sc.
	 * Don't use it in SMP environment.
	 */

#ifdef MDX_SCHED_SMP
	li	t0, 1
#else
	li	t0, 0
#endif
	bne	a0, t0, mpentry

	/* Clear BSS. */
	la	t0, _sbss
	la	t1, _ebss
1:
	beq	t0, t1, 2f
	li	t2, 0
	sb	t2, 0(t0)
	addi	t0, t0, 1
	j	1b
2:

#ifdef MDX_SCHED_SMP
	/* Release CPUs */
	la	t0, mp_release
	li	t1, 1
	sb	t1, 0(t0)
#endif

	/* Load our stack pointer. */
	la	t0, idle_thread_stack
	li	t1, MDX_THREAD_STACK_SIZE
	mulw	t2, t1, a0
	add	t2, t2, t1
	add	sp, t2, t0

	fence

	j	md_init

	/* NOT REACHED */

ASM_ENTRY(mpentry)
#ifdef MDX_SCHED_SMP
	/* Spin until CPU1 clears BSS. */
1:
	la	t0, mp_release
	lb	t1, 0(t0)
	beqz	t1, 1b

	/* Spin until CPU1 release the core. */
	la	t0, __riscv_boot_ap
	add	t0, t0, a0

1:
	lb	t1, 0(t0)
	beqz	t1, 1b

	la	t0, idle_thread_stack
	li	t1, MDX_THREAD_STACK_SIZE
	mulw	t2, t1, a0
	add	t2, t2, t1
	add	sp, t2, t0

	fence

	j	md_init_secondary
#else
1:
	j	1b
#endif
END(mpentry)
