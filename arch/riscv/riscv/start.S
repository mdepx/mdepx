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

#include <machine/asm.h>

ASM_ENTRY(_start)
	fence.i
	fence

	la	t0, cpu_exception_handler
	csrw	REG_TVEC, t0

	/* Pick a hart to run the boot process. */
	la	t0, cpu_lottery
	li	t1, 1
	amoadd.w t0, t1, 0(t0)

	/*
	 * We must jump to mpentry because the offset is too large to fit
	 * in a 12-bit branch immediate.
	 */
	beqz	t0, 1f
	j	mpentry
1:

	/* Clear BSS. */
	la	t0, _sbss	# start of BSS section
	la	t1, _ebss	# end of BSS section
1:
	beq	t0, t1, 2f
	li	t2, 0
	sb	t2, 0(t0)
	addi	t0, t0, 1
	j	1b
2:

	/* Load our stack pointer. */
	la	t0, idle_thread_stack
	li	t1, MDX_THREAD_STACK_SIZE
	mulw	t2, t1, a0	# Offset to start of our stack
	add	t2, t2, t0	# Start of our stack
	add	sp, t2, t1	# End of our stack

	fence

	mv	s0, a0
	mv	s1, a1

#ifdef MDX_OF
	mv	a0, a1
	lla	t0, mdx_of_install_dtbp
	jalr	t0
#endif

	mv	a0, s0
	j	md_init

	/* NOT REACHED */

ASM_ENTRY(mpentry)
#ifdef MDX_SCHED_SMP
	la	t0, __riscv_boot_ap
	add	t0, t0, a0

1:
	lb	t1, 0(t0)
	beqz	t1, 1b

	la	t0, idle_thread_stack
	li	t1, MDX_THREAD_STACK_SIZE
	mulw	t2, t1, a0	# Offset to start of our stack
	add	t2, t2, t0	# Start of our stack
	add	sp, t2, t1	# End of our stack

	fence

	j	md_init_secondary
#else
1:
	j	1b
#endif
END(mpentry)

	.align 3
cpu_lottery:
	.space 4
