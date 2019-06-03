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

	.data
	.space 4

	.set noreorder
	.text
ASM_ENTRY(_start)
	mtc0    zero, CP0_CAUSE

#if 0
	li	t0, MIPS_SR_BEV | MIPS_SR_SR
	li	t1, MIPS_SR_CU(1)
	or	t1, MIPS_SR_FR
	or	t1, MIPS_SR_KX | MIPS_SR_SX | MIPS_SR_UX

	mfc0	t2, CP0_STATUS
	and	t2, t0
	or	t2, t1
	mtc0	t2, CP0_STATUS
	ssnop
	ssnop
	ssnop
	ssnop
	ehb
#endif

#if 0
	li	t0, 0x3
	mtc0	t0, CP0_CONFIG
	ssnop
	ssnop
	ssnop
	ssnop
	ehb
#endif

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

	/* Load stack pointer */
	dla	t0, secondary_stacks
	add	t0, t0, 4096
	move	sp, t0
	nop
	li	a0, 0
	dla	t0, md_init
	jalr	t0
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
