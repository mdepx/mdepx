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

.macro save_registers el
	addi	sp, sp, -(35 * REG_SIZE)

	REG_S	ra, (0 * REG_SIZE)(sp)
	REG_S	sp, (1 * REG_SIZE)(sp)
	REG_S	gp, (2 * REG_SIZE)(sp)
	REG_S	tp, (3 * REG_SIZE)(sp)

	REG_S	t0, (4 * REG_SIZE)(sp)
	REG_S	t1, (5 * REG_SIZE)(sp)
	REG_S	t2, (6 * REG_SIZE)(sp)
	REG_S	t3, (7 * REG_SIZE)(sp)
	REG_S	t4, (8 * REG_SIZE)(sp)
	REG_S	t5, (9 * REG_SIZE)(sp)
	REG_S	t6, (10 * REG_SIZE)(sp)

	REG_S	s0, (11 * REG_SIZE)(sp)
	REG_S	s1, (12 * REG_SIZE)(sp)
	REG_S	s2, (13 * REG_SIZE)(sp)
	REG_S	s3, (14 * REG_SIZE)(sp)
	REG_S	s4, (15 * REG_SIZE)(sp)
	REG_S	s5, (16 * REG_SIZE)(sp)
	REG_S	s6, (17 * REG_SIZE)(sp)
	REG_S	s7, (18 * REG_SIZE)(sp)
	REG_S	s8, (19 * REG_SIZE)(sp)
	REG_S	s9, (20 * REG_SIZE)(sp)
	REG_S	s10, (21 * REG_SIZE)(sp)
	REG_S	s11, (22 * REG_SIZE)(sp)

	REG_S	a0, (23 * REG_SIZE)(sp)
	REG_S	a1, (24 * REG_SIZE)(sp)
	REG_S	a2, (25 * REG_SIZE)(sp)
	REG_S	a3, (26 * REG_SIZE)(sp)
	REG_S	a4, (27 * REG_SIZE)(sp)
	REG_S	a5, (28 * REG_SIZE)(sp)
	REG_S	a6, (29 * REG_SIZE)(sp)
	REG_S	a7, (30 * REG_SIZE)(sp)

	csrr	t0, mepc
	REG_S	t0, (31 * REG_SIZE)(sp)
	csrr	t0, mstatus
	REG_S	t0, (32 * REG_SIZE)(sp)
	csrr	t0, mbadaddr
	REG_S	t0, (33 * REG_SIZE)(sp)
	csrr	t0, mcause
	REG_S	t0, (34 * REG_SIZE)(sp)
.endm

.macro load_registers el
	REG_L	t0, (31 * REG_SIZE)(sp)
	csrw	mepc, t0
	REG_L	t0, (32 * REG_SIZE)(sp)
	csrw	mstatus, t0

	REG_L	ra, (0 * REG_SIZE)(sp)
	REG_L	tp, (3 * REG_SIZE)(sp)

	REG_L	t0, (4 * REG_SIZE)(sp)
	REG_L	t1, (5 * REG_SIZE)(sp)
	REG_L	t2, (6 * REG_SIZE)(sp)
	REG_L	t3, (7 * REG_SIZE)(sp)
	REG_L	t4, (8 * REG_SIZE)(sp)
	REG_L	t5, (9 * REG_SIZE)(sp)
	REG_L	t6, (10 * REG_SIZE)(sp)

	REG_L	s0, (11 * REG_SIZE)(sp)
	REG_L	s1, (12 * REG_SIZE)(sp)
	REG_L	s2, (13 * REG_SIZE)(sp)
	REG_L	s3, (14 * REG_SIZE)(sp)
	REG_L	s4, (15 * REG_SIZE)(sp)
	REG_L	s5, (16 * REG_SIZE)(sp)
	REG_L	s6, (17 * REG_SIZE)(sp)
	REG_L	s7, (18 * REG_SIZE)(sp)
	REG_L	s8, (19 * REG_SIZE)(sp)
	REG_L	s9, (20 * REG_SIZE)(sp)
	REG_L	s10, (21 * REG_SIZE)(sp)
	REG_L	s11, (22 * REG_SIZE)(sp)

	REG_L	a0, (23 * REG_SIZE)(sp)
	REG_L	a1, (24 * REG_SIZE)(sp)
	REG_L	a2, (25 * REG_SIZE)(sp)
	REG_L	a3, (26 * REG_SIZE)(sp)
	REG_L	a4, (27 * REG_SIZE)(sp)
	REG_L	a5, (28 * REG_SIZE)(sp)
	REG_L	a6, (29 * REG_SIZE)(sp)
	REG_L	a7, (30 * REG_SIZE)(sp)

	addi	sp, sp, (35 * REG_SIZE)
.endm

	.align 2
ASM_ENTRY(cpu_exception_handler)
	save_registers 1
	mv	a0, sp
	jal	riscv_exception
	mv	sp, a0
	load_registers 1
	mret
END(cpu_exception_handler)
