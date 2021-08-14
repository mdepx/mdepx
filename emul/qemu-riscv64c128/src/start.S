/*-
 * Copyright (c) 2021 Ruslan Bukin <br@bsdpad.com>
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

#if __CHERI_PURE_CAPABILITY__
# The CPU starts in hybrid, and the boot initialisation has to be in hybrid.
.option nocapmode
#endif /* __CHERI_PURE_CAPABILITY__ */

ASM_ENTRY(_start)
	fence.i
	fence

#ifdef __CHERI_PURE_CAPABILITY__
	# W^X for PCC and DDC
	cspecialrw	ct0, ddc, cnull
	li		t1, (1 << 1)
	not		t1, t1
	candperm	ct0, ct0, t1
	cspecialrw	cnull, ddc, ct0

	cspecialrw	ct0, pcc, cnull
	li		t1, ((1 << 3) | (1 << 5))
	not		t1, t1
	candperm	ct0, ct0, t1
	la		t2, _mstart
	csetoffset	ct0, ct0, t2
#ifdef __riscv_xcheri_mode_dependent_jumps
	jr.cap		ct0
#else
	cjr		ct0
#endif
#endif

_mstart:
	li	x1, 0

	//la	t0, cpu_exception_handler
	//csrw	mtvec, t0

	//li	t0, 0
	//bne	a0, t0, mpentry

#if 0
	la	t0, cpu_lottery
	li	t1, 1
	amoadd.w t0, t1, 0(t0)
	bnez	t0, mpentry
#endif

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

	/* Init our stack pointer. */
	la		t0, idle_thread_stack
	li		t1, MDX_THREAD_STACK_SIZE
	mulw		t2, t1, a0
	add		t2, t2, t0	# Start of the current CPU stack
	add		sp, t2, t1	# End of the current CPU stack

#ifdef __CHERI_PURE_CAPABILITY__
	# From now on, do purecap ABI but in hybrid mode.
	cfromptr	csp, ddc, sp	# Get DDC
	sub		t2, sp, t2	# Length of the stack
	csetbounds	csp, csp, t2
	cincoffset	csp, csp, t2

	cspecialrw	ct0, pcc, cnull # Get PCC
	li		t1, 1
	csetflags	ct0, ct0, t1

	# Initialise the captable.
	# ct0 is still PCC
	la		t1, start_purecap
	csetoffset	ct0, ct0, t1
#ifdef __riscv_xcheri_mode_dependent_jumps
	jalr.cap	cra, ct0
#else
	cjalr		cra, ct0
#endif

	# From now on, the DDC is always null
	//cmove		ct0, cnull
	//cspecialrw	cnull, ddc, ct0 # Write DDC

	# Set up mtcc and jump to main in purecap mode
	cspecialrw	ct0, pcc, cnull # Get PCC

	li		t1, 1
	csetflags	ct0, ct0, t1 # set cap mode on

	la		t1, cpu_exception_handler
	csetoffset	ct0, ct0, t1
	cspecialrw	cnull, mtcc, ct0 # Write MTCC

	la		t1, md_init
	csetoffset	ct0, ct0, t1
#ifdef __riscv_xcheri_mode_dependent_jumps
	jalr.cap	cra, ct0 # jump to main
#else
	cjalr		cra, ct0 # jump to main
#endif
#endif

	fence

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

cpu_lottery:
	.space 4

#if __CHERI_PURE_CAPABILITY__
.option capmode
#endif /* __CHERI_PURE_CAPABILITY__ */
