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
	addi	sp, sp, -(35 * 4)

	sw	ra, (0 * 4)(sp)
	sw	sp, (1 * 4)(sp)
	sw	gp, (2 * 4)(sp)
	sw	tp, (3 * 4)(sp)

	sw	t0, (4 * 4)(sp)
	sw	t1, (5 * 4)(sp)
	sw	t2, (6 * 4)(sp)
	sw	t3, (7 * 4)(sp)
	sw	t4, (8 * 4)(sp)
	sw	t5, (9 * 4)(sp)
	sw	t6, (10 * 4)(sp)

	sw	s0, (11 * 4)(sp)
	sw	s1, (12 * 4)(sp)
	sw	s2, (13 * 4)(sp)
	sw	s3, (14 * 4)(sp)
	sw	s4, (15 * 4)(sp)
	sw	s5, (16 * 4)(sp)
	sw	s6, (17 * 4)(sp)
	sw	s7, (18 * 4)(sp)
	sw	s8, (19 * 4)(sp)
	sw	s9, (20 * 4)(sp)
	sw	s10, (21 * 4)(sp)
	sw	s11, (22 * 4)(sp)

	sw	a0, (23 * 4)(sp)
	sw	a1, (24 * 4)(sp)
	sw	a2, (25 * 4)(sp)
	sw	a3, (26 * 4)(sp)
	sw	a4, (27 * 4)(sp)
	sw	a5, (28 * 4)(sp)
	sw	a6, (29 * 4)(sp)
	sw	a7, (30 * 4)(sp)

	csrr	t0, mepc
	sw	t0, (31 * 4)(sp)
	csrr	t0, mstatus
	sw	t0, (32 * 4)(sp)
	csrr	t0, mbadaddr
	sw	t0, (33 * 4)(sp)
	csrr	t0, mcause
	sw	t0, (34 * 4)(sp)
.endm

.macro load_registers el
	lw	t0, (32 * 4)(sp)
	csrw	mstatus, t0

	lw	t0, (31 * 4)(sp)
	csrw	mepc, t0

	lw	ra, (0 * 4)(sp)
	lw	gp, (2 * 4)(sp)
	lw	tp, (3 * 4)(sp)

	lw	t0, (4 * 4)(sp)
	lw	t1, (5 * 4)(sp)
	lw	t2, (6 * 4)(sp)
	lw	t3, (7 * 4)(sp)
	lw	t4, (8 * 4)(sp)
	lw	t5, (9 * 4)(sp)
	lw	t6, (10 * 4)(sp)

	lw	s0, (11 * 4)(sp)
	lw	s1, (12 * 4)(sp)
	lw	s2, (13 * 4)(sp)
	lw	s3, (14 * 4)(sp)
	lw	s4, (15 * 4)(sp)
	lw	s5, (16 * 4)(sp)
	lw	s6, (17 * 4)(sp)
	lw	s7, (18 * 4)(sp)
	lw	s8, (19 * 4)(sp)
	lw	s9, (20 * 4)(sp)
	lw	s10, (21 * 4)(sp)
	lw	s11, (22 * 4)(sp)

	lw	a0, (23 * 4)(sp)
	lw	a1, (24 * 4)(sp)
	lw	a2, (25 * 4)(sp)
	lw	a3, (26 * 4)(sp)
	lw	a4, (27 * 4)(sp)
	lw	a5, (28 * 4)(sp)
	lw	a6, (29 * 4)(sp)
	lw	a7, (30 * 4)(sp)

	addi	sp, sp, (35 * 4)
.endm

ASM_ENTRY(cpu_exception_handler)
	save_registers 1
	mv a0, sp
	jal riscv_exception
	load_registers 1
	mret
END(cpu_exception_handler)
