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
#include <machine/pte.h>
#include <machine/cpuregs.h>
#include <machine/vmparam.h>
#include <machine/param.h>

	.globl	kernbase
	.set	kernbase, KERNBASE

	.globl	dmapbase
	.set	dmapbase, DMAPBASE

	.text

ASM_ENTRY(_start)
	fence.i
	fence

#ifdef __CHERI_PURE_CAPABILITY__
.option push
.option nocapmode

	/*
	 * The boot loader may enter in non-capmode even though
	 * our ELF header requests capmode.  The instructions below
	 * should work in both modes.
	 */
	lla		t0, 1f
	cspecialr	ct1, pcc
	csetaddr	ct1, ct1, t0
	li		t0, 1
	csetflags	ct1, ct1, t0
#ifdef __riscv_xcheri_mode_dependent_jumps
	jr.cap	ct1
#else
	cjr	ct1
#endif
1:
.option pop
#endif

_mstart:

	nop

	li	x1, 0
	li	x2, 0

	/* Save a0,a1 with hartid/DTB */
	mv		s4, a0
	mv		s5, a1

	li	t0, 0
	bne	a0, t0, mpentry

#if 0
	la	t0, cpu_lottery
	li	t1, 1
	amoadd.w t0, t1, 0(t0)
	bnez	t0, mpentry
#endif

	/* Clear BSS. */
	cllc	ct0, _sbss
	cllc	ct1, _ebss
1:
	beq	t0, t1, 2f
	li	t2, 0
	//sb	t2, 0(t0)
	//addi	t0, t0, 1
	csb	zero, 0(ct0)
	cincoffset ct0, ct0, 1
	j	1b
2:

#ifdef MDX_RISCV_SUPERVISOR_MODE
	/*
	 * Page tables setup
	 *  a0 - modulep or zero
	 *  a1 - zero or dtbp
	 */
pagetables:
	/*
	 * Make a direct mapping:
	 * 0xffffffd000000000 (va) -> 0 (pa),
	 * using 4 x L1 superpages (1GiB each)
	 */

	/* Physical base address. */
	li	s9, 0

	/* Level 1 superpages (512 x 1GiB) */
	cllc	cs1, pagetable_l1

	li	s2, DMAPBASE
	srli	s2, s2, L1_SHIFT
	andi	s2, s2, 0x1ff

	li	t0, PTE_SIZE
	mulw	s2, s2, t0

	li	t0, (PTE_KERN | PTE_KERN_CAP | PTE_X)
	li	t1, 0			/* Start ppn */
	li	t2, 4			/* Build 4 entries */
	add	t3, t1, t2		/* ppn limit */
1:
	slli	t2, t1, PTE_PPN2_S	/* << PTE_PPN2_S */
	or	t4, t0, t2
#ifdef __CHERI_PURE_CAPABILITY__
	cincoffset cs3, cs1, s2
	csd	t4, 0(cs3)
#else
	add	s3, s1, s2
	sd	t4, 0(s3)		/* Store PTE entry to position */
#endif

	addi	s2, s2, PTE_SIZE
	addi	t1, t1, 1
	bltu	t1, t3, 1b

	/* Page tables END */

	/* Setup supervisor trap vector */
2:
#ifdef __CHERI_PURE_CAPABILITY__
	cllc	ct0, va
	li	t1, DMAPBASE
	cincoffset	ct0, ct0, t1
	cspecialw REG_TCC, ct0
#else
	lla	t0, va
	sub	t0, t0, s9
	li	t1, DMAPBASE
	add	t0, t0, t1
	csrw	stvec, t0
#endif

	/* Start trace. */
	//slti zero, zero, 0x1b

	/* Set page tables base register */
#ifdef __CHERI_PURE_CAPABILITY__
	cllc	cs2, pagetable_l1
#else
	lla	s2, pagetable_l1
#endif
	srli	s2, s2, PAGE_SHIFT
	li	t0, SATP_MODE_SV39
	or	s2, s2, t0
	sfence.vma
	csrw	satp, s2

	.align 2
va:
#endif /* !MDX_RISCV_SUPERVISOR_MODE */

	/* Restore a0, a1 */
	mv	a0, s4
	mv	a1, s5

	/* Init our stack pointer. */
	cllc		ct0, idle_thread_stack
	li		t1, MDX_THREAD_STACK_SIZE
	mulw		t2, t1, a0
	//add		t2, t2, t0	# Start of the current CPU stack
	cincoffset	csp, ct0, t2

#ifdef __CHERI_PURE_CAPABILITY__
	# From now on, do purecap ABI but in hybrid mode.
	//cfromptr	csp, ddc, t2	# Get DDC
	csetbounds	csp, csp, t1
	cincoffset	csp, csp, t1

	cspecialrw	ct0, pcc, cnull # Get PCC
	cllc		ct0, start_purecap
	li		t1, 1
	csetflags	ct0, ct0, t1

	# Initialise the captable.
	# ct0 is still PCC
	//csetoffset	ct0, ct0, t1
#ifdef __riscv_xcheri_mode_dependent_jumps
	aaa
	jalr.cap	cra, ct0
#else
	cjalr		ct0
#endif

	/* Restore a0, a1 */
	mv	a0, s4
	mv	a1, s5

	# From now on, the DDC is always null
	//cmove		ct0, cnull
	//cspecialrw	cnull, ddc, ct0 # Write DDC

	# Set up REG_TCC and jump to main in purecap mode
	cspecialrw	ct0, pcc, cnull # Get PCC

	cllc		ct0, cpu_exception_handler
	li		t1, 1
	csetflags	ct0, ct0, t1 # set cap mode on

	//csetoffset	ct0, ct0, t1
	//cspecialrw	cnull, REG_TCC, ct0 # Write MTCC
	cspecialw	REG_TCC, ct0

	clgc		cra, md_init
	cjalr		cra

	//la		t1, md_init
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

	/* Restore a0, a1 */
	mv	a0, s4
	mv	a1, s5

#ifdef MDX_SCHED_SMP
	cllc	ct0, __riscv_boot_ap
	cincoffset ct0, ct0, a0

1:
	clb	t1, 0(ct0)
	beqz	t1, 1b

#ifdef MDX_RISCV_SUPERVISOR_MODE
	/* Setup supervisor trap vector */
	cllc		ct0, mpva
	li		t1, DMAPBASE
	cincoffset	ct0, ct0, t1
	cspecialw	REG_TCC, ct0

	/* Set page tables base register */
	cllc	cs2, pagetable_l1
	srli	s2, s2, PAGE_SHIFT
	li	t0, SATP_MODE_SV39
	or	s2, s2, t0
	sfence.vma
	csrw	satp, s2

	.align 2
mpva:
#endif

	cllc		ct0, idle_thread_stack
	li		t1, MDX_THREAD_STACK_SIZE
	mulw		t2, t1, a0
	cincoffset	csp, ct0, t2	# Start of the current CPU stack

#ifdef __CHERI_PURE_CAPABILITY__
	# From now on, do purecap ABI but in hybrid mode.
	//cfromptr	csp, ddc, t2	# Get DDC
	csetbounds	csp, csp, t1
	cincoffset	csp, csp, t1
#endif

	# Set up REG_TCC and jump to main in purecap mode
	//cspecialrw	ct0, pcc, cnull # Get PCC
	cllc		ct0, cpu_exception_handler
	li		t1, 1
	csetflags	ct0, ct0, t1 # set cap mode on

	cspecialrw	cnull, REG_TCC, ct0 # Write MTCC

	cllc		ct0, md_init_secondary
	//csetoffset	ct0, ct0, t1
#ifdef __riscv_xcheri_mode_dependent_jumps
	jalr.cap	cra, ct0 # jump to main
#else
	cjalr		cra, ct0 # jump to main
#endif

	fence

	j	md_init_secondary
#else
1:
	j	1b
#endif
END(mpentry)

	.align 12
pagetable_l1:
	.space  PAGE_SIZE

	.align 3
virt_map:
	.quad	virt_map
cpu_lottery:
	.space 4

#if __CHERI_PURE_CAPABILITY__
.option capmode
#endif /* __CHERI_PURE_CAPABILITY__ */
