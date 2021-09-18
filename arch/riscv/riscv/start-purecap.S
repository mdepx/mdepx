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
	/* Pick a hart to run the boot process. */
	li	t1, 1
	cllc	ct0, cpu_lottery
	camoadd.w t0, t1, 0(ct0)

	/*
	 * We must jump to mpentry because the offset is too large to fit
	 * in a 12-bit branch immediate.
	 */
	beqz	t0, 1f
	j	mpentry
1:

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

	/* Level 1 superpages (1GiB each) */
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

	cllc		ct0, cpu_exception_handler
	cspecialw	REG_TCC, ct0

	/* Clear BSS. */
	cllc	ct0, _sbss
	cllc	ct1, _ebss
1:
	beq	t0, t1, 2f
	li	t2, 0
	csb	zero, 0(ct0)
	cincoffset ct0, ct0, 1
	j	1b
2:

	/* Load our stack pointer. */
	cllc		ct0, idle_thread_stack
	li		t1, MDX_THREAD_STACK_SIZE
	mulw		t2, t1, a0
	cincoffset	csp, ct0, t2	# Start of the current CPU stack
	csetbounds	csp, csp, t1
	cincoffset	csp, csp, t1	# End of our stack

	fence

	mv		s0, a0
	mv		s1, a1

	/* Initialize cap relocs. */
	cllc		ct0, start_purecap
	cjalr		ct0

#ifdef MDX_OF
	cspecialr	ct0, ddc
	csetoffset	ca0, ct0, s1
#ifdef MDX_RISCV_SUPERVISOR_MODE
	li		t0, DMAPBASE
	cincoffset	ca0, ca0, t0	/* Map to VA space. */
#endif
	cllc		ct0, mdx_of_install_dtbp
	cjalr		ct0
#endif

	mv		a0, s0
	clgc		cra, md_init
	cjalr		cra

	/* NOT REACHED */

ASM_ENTRY(mpentry)
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
	csetbounds	csp, csp, t1
	cincoffset	csp, csp, t1	# End of our stack

	cllc		ct0, cpu_exception_handler
	cspecialrw	cnull, REG_TCC, ct0

	cllc		ct0, md_init_secondary
	cjalr		cra, ct0 # jump to main
#else
1:
	j	1b
#endif
END(mpentry)

get_physmem:
	cllc	ct0, virt_map	/* physical address of virt_map */
	cld	t1, 0(ct0)	/* virtual address of virt_map */
	sub	t1, t1, t0	/* calculate phys->virt delta */
	li	t2, KERNBASE
	sub	s9, t2, t2	/* s9 = physmem base */
	ret

	.align 12
pagetable_l1:
	.space  PAGE_SIZE

	.align 3
virt_map:
	.quad	virt_map
cpu_lottery:
	.space 4
