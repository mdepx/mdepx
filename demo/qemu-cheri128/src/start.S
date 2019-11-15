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

#include <machine/regdef.h>
#include <machine/cpuregs.h>
#include <machine/asm.h>
#include <machine/cherireg.h>
#include <machine/cheriasm.h>

#define	MIPS_CP0_REG_REVOKE		$30
#define	MIPS_CP0_REG_REVOKE_BASE	2
#define	MIPS_CP0_REG_REVOKE_BOUND	3
#define	MIPS_CP0_REG_REVOKE_PERMS	4

.set cheri_sysregs_accessible

	.set noreorder
	.text
ASM_ENTRY(_start)
	mtc0    zero, CP0_CAUSE

	mfc0	t0, CP0_STATUS
	li	t1, 0x40000000		/* no float */
	or	t0, t0, t1
	li	t1, 0x4			/* Clear ELR */
	nor	t1, t1, t1
	and	t0, t0, t1

	li	t1, 0x3 << 3
	not	t1, t1
	and	t0, t0, t1		/* Set KSU = 0 */

	li	t1, 0x7 << 5
	or	t0, t0, t1		/* Set UX, SX, and KSX */
	or	t0, t0, (3 << 8)	/* Set user interrupt bits */
	mtc0	t0, CP0_STATUS

	nop; nop; nop; nop; nop; nop; nop; nop;

	CHERI_LOCORE_ROOT_CAPS
	/*
	 * Create the parent kernel sealing capablity.
	 */
	cmove		CHERI_REG_C27, CHERI_REG_C28
	REG_LI		t0, CHERI_SEALCAP_KERNEL_BASE
	csetoffset	CHERI_REG_C27, CHERI_REG_C27, t0
	REG_LI		t0, CHERI_SEALCAP_KERNEL_LENGTH
	csetbounds	CHERI_REG_C27, CHERI_REG_C27, t0
	REG_LI		t0, CHERI_SEALCAP_KERNEL_PERMS
	candperm	CHERI_REG_C27, CHERI_REG_C27, t0
	PTR_LA		t0, _C_LABEL(kernel_sealcap)
	csc		CHERI_REG_C27, t0, 0($ddc)

	/* Set the stack pointer. */
	dla	t0, idle_thread_stack
	dli	t1, MDX_THREAD_STACK_SIZE
	add	sp, t0, t1

	li	a0, 0
	dla	t0, md_init
	jr	t0
	nop

ENTRY(cpu_reset)

	/* Reset */
	li	t0, 0x1f000500
	li	t1, 0x42
	sw	t1, 0(t0)

	/* NOT REACHED */
1:
	j	1b
	nop
END(cpu_reset)
