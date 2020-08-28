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

#define	MIPS_CP0_REG_REVOKE		$30
#define	MIPS_CP0_REG_REVOKE_BASE	2
#define	MIPS_CP0_REG_REVOKE_BOUND	3
#define	MIPS_CP0_REG_REVOKE_PERMS	4

	.set noreorder
	.text
ASM_ENTRY(_start)
	mtc0    zero, CP0_CAUSE

	mfc0	t0, $12
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
	mtc0	t0, $12

	nop; nop; nop; nop; nop; nop; nop; nop;

	/* Zero BSS */
	li	t2, 0
	dla	t0, _sbss
	nop
	dla	t1, _ebss
	nop
1:
	beq	t0, t1, 2f
	nop
	sb	t2, 0(t0)
	addi	t0, t0, 1
	j	1b
	nop
2:

	CClearLo	0xfffe
	CClearHi	0xffff

	/* W^X for pointers: ensure $c0 is never executable. */
	cgetdefault	$c3
	dli		t0, ~(1 << 1)
	candperm	$c3, $c3, t0
	csetdefault	$c3

	# Implement W^X for pointers: trim store permissions from $pcc to
	# prevent it from becoming writable in the future.
	cgetpcc		$c12
	dli		t0, ~((1 << 3) | (1 << 5))
	candperm	$c12, $c12, t0

	# Jump to write-free $pcc.
	dla		t0, jump_target
	csetoffset	$c12, $c12, t0
	cjr		$c12
	nop

jump_target:

	dmtc0  zero, MIPS_CP0_REG_REVOKE, MIPS_CP0_REG_REVOKE_PERMS
	dmtc0  zero, MIPS_CP0_REG_REVOKE, MIPS_CP0_REG_REVOKE_BASE
	dmtc0  zero, MIPS_CP0_REG_REVOKE, MIPS_CP0_REG_REVOKE_BOUND

	dla	t0, idle_thread_stack
	dla	t1, MDX_THREAD_STACK_SIZE

	cgetdefault	$c11
	cincoffset	$c11, $c11, t0
	csetboundsexact	$c11, $c11, t1
	cincoffset	$c11, $c11, t1

	/* Relocate capabilities */
	cgetpcc		$c12
	dla		t0, crt_init_globals
	csetoffset	$c12, $c12, t0
	csetbounds	$idc, $c11, zero # need non null
	cjalr		$c12, $c17
	cmove		$c18, $idc
	cmove		$idc, $c3

	/* Call into main */
	cgetpcc		$c12
	li		a0, 0
	dla		t0, md_init
	csetoffset	$c12, $c12, t0
	csetbounds	$idc, $c11, zero # need non null
	cjalr		$c12, $c17
	cmove		$c18, $idc
	cmove		$idc, $c3

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
