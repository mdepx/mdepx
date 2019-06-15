/*-
 * Copyright (c) 2018 Ruslan Bukin <br@bsdpad.com>
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
 * ("CTSRD"), as part of the DARPA CRASH research programme.
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

#include <machine/abi.h>
#include <machine/frame.h>

.macro save_registers
	addi	sp, sp, -(TF_SIZE)

.set noat
	REG_S	AT, (REG_SIZE * 0)(sp)
.set at
	REG_S	v0, (REG_SIZE * 1)(sp)
	REG_S	v1, (REG_SIZE * 2)(sp)

	REG_S	a0, (REG_SIZE * 3)(sp)
	REG_S	a1, (REG_SIZE * 4)(sp)
	REG_S	a2, (REG_SIZE * 5)(sp)
	REG_S	a3, (REG_SIZE * 6)(sp)
#if defined(__mips_n32) || defined(__mips_n64)
	REG_S	a4, (REG_SIZE * 7)(sp)
	REG_S	a5, (REG_SIZE * 8)(sp)
	REG_S	a6, (REG_SIZE * 9)(sp)
	REG_S	a7, (REG_SIZE * 10)(sp)

	REG_S	t0, (REG_SIZE * 11)(sp)
	REG_S	t1, (REG_SIZE * 12)(sp)
	REG_S	t2, (REG_SIZE * 13)(sp)
	REG_S	t3, (REG_SIZE * 14)(sp)
#else
	REG_S	t0, (REG_SIZE * 7)(sp)
	REG_S	t1, (REG_SIZE * 8)(sp)
	REG_S	t2, (REG_SIZE * 9)(sp)
	REG_S	t3, (REG_SIZE * 10)(sp)
	REG_S	t4, (REG_SIZE * 11)(sp)
	REG_S	t5, (REG_SIZE * 12)(sp)
	REG_S	t6, (REG_SIZE * 13)(sp)
	REG_S	t7, (REG_SIZE * 14)(sp)
#endif
	REG_S	t8, (REG_SIZE * 15)(sp)
	REG_S	t9, (REG_SIZE * 16)(sp)

	REG_S	s0, (REG_SIZE * 17)(sp)
	REG_S	s1, (REG_SIZE * 18)(sp)
	REG_S	s2, (REG_SIZE * 19)(sp)
	REG_S	s3, (REG_SIZE * 20)(sp)
	REG_S	s4, (REG_SIZE * 21)(sp)
	REG_S	s5, (REG_SIZE * 22)(sp)
	REG_S	s6, (REG_SIZE * 23)(sp)
	REG_S	s7, (REG_SIZE * 24)(sp)

	REG_S	gp, (REG_SIZE * 25)(sp)
	REG_S	sp, (REG_SIZE * 26)(sp)
	REG_S	s8, (REG_SIZE * 27)(sp)
	REG_S	ra, (REG_SIZE * 28)(sp)

	mflo	v0
	mfhi	v1

	REG_S	v0, (REG_SIZE * 29)(sp)
	REG_S	v1, (REG_SIZE * 30)(sp)

	MFC0	a0, CP0_STATUS
	MFC0	a1, CP0_CAUSE
	MFC0	a2, CP0_BAD_VADDR
	MFC0	a3, CP0_EXC_PC

	REG_S	a0, (REG_SIZE * 31)(sp)
	REG_S	a1, (REG_SIZE * 32)(sp)
	REG_S	a2, (REG_SIZE * 33)(sp)
	REG_S	a3, (REG_SIZE * 34)(sp)
.endm

.macro restore_registers
	REG_L	v0, (REG_SIZE * 29)(sp)
	REG_L	v1, (REG_SIZE * 30)(sp)

	mtlo	v0
	mthi	v1

	REG_L	k0, (REG_SIZE * 31)(sp)
	REG_L	a3, (REG_SIZE * 34)(sp)

	MTC0	a3, CP0_EXC_PC

.set noat
	REG_L	AT, (REG_SIZE * 0)(sp)
.set at
	REG_L	v0, (REG_SIZE * 1)(sp)
	REG_L	v1, (REG_SIZE * 2)(sp)

	REG_L	a0, (REG_SIZE * 3)(sp)
	REG_L	a1, (REG_SIZE * 4)(sp)
	REG_L	a2, (REG_SIZE * 5)(sp)
	REG_L	a3, (REG_SIZE * 6)(sp)
#if defined(__mips_n32) || defined(__mips_n64)
	REG_L	a4, (REG_SIZE * 7)(sp)
	REG_L	a5, (REG_SIZE * 8)(sp)
	REG_L	a6, (REG_SIZE * 9)(sp)
	REG_L	a7, (REG_SIZE * 10)(sp)

	REG_L	t0, (REG_SIZE * 11)(sp)
	REG_L	t1, (REG_SIZE * 12)(sp)
	REG_L	t2, (REG_SIZE * 13)(sp)
	REG_L	t3, (REG_SIZE * 14)(sp)
#else
	REG_L	t0, (REG_SIZE * 7)(sp)
	REG_L	t1, (REG_SIZE * 8)(sp)
	REG_L	t2, (REG_SIZE * 9)(sp)
	REG_L	t3, (REG_SIZE * 10)(sp)

	REG_L	t4, (REG_SIZE * 11)(sp)
	REG_L	t5, (REG_SIZE * 12)(sp)
	REG_L	t6, (REG_SIZE * 13)(sp)
	REG_L	t7, (REG_SIZE * 14)(sp)
#endif
	REG_L	t8, (REG_SIZE * 15)(sp)
	REG_L	t9, (REG_SIZE * 16)(sp)

	REG_L	s0, (REG_SIZE * 17)(sp)
	REG_L	s1, (REG_SIZE * 18)(sp)
	REG_L	s2, (REG_SIZE * 19)(sp)
	REG_L	s3, (REG_SIZE * 20)(sp)
	REG_L	s4, (REG_SIZE * 21)(sp)
	REG_L	s5, (REG_SIZE * 22)(sp)
	REG_L	s6, (REG_SIZE * 23)(sp)
	REG_L	s7, (REG_SIZE * 24)(sp)

	REG_L	s8, (REG_SIZE * 27)(sp)
	REG_L	ra, (REG_SIZE * 28)(sp)

	addi	sp, sp, (TF_SIZE)
.endm
