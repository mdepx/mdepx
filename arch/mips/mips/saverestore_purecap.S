/*-
 * Copyright (c) 2018-2020 Ruslan Bukin <br@bsdpad.com>
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

#if __has_feature(capabilities)
#include <machine/cheriasm.h>
.set cheri_sysregs_accessible
#endif

.macro save_registers_purecap
	cincoffset	$c11, $c11, -(TF_SIZE)

.set noat
	csd		AT, zero, (REG_SIZE * 0)($c11)
.set at
	csd		v0, zero, (REG_SIZE * 1)($c11)
	csd		v1, zero, (REG_SIZE * 2)($c11)

	csd		a0, zero, (REG_SIZE * 3)($c11)
	csd		a1, zero, (REG_SIZE * 4)($c11)
	csd		a2, zero, (REG_SIZE * 5)($c11)
	csd		a3, zero, (REG_SIZE * 6)($c11)
#if defined(__mips_n32) || defined(__mips_n64)
	csd		a4, zero, (REG_SIZE * 7)($c11)
	csd		a5, zero, (REG_SIZE * 8)($c11)
	csd		a6, zero, (REG_SIZE * 9)($c11)
	csd		a7, zero, (REG_SIZE * 10)($c11)

	csd		t0, zero, (REG_SIZE * 11)($c11)
	csd		t1, zero, (REG_SIZE * 12)($c11)
	csd		t2, zero, (REG_SIZE * 13)($c11)
	csd		t3, zero, (REG_SIZE * 14)($c11)
#else
	csd		t0, zero, (REG_SIZE * 7)($c11)
	csd		t1, zero, (REG_SIZE * 8)($c11)
	csd		t2, zero, (REG_SIZE * 9)($c11)
	csd		t3, zero, (REG_SIZE * 10)($c11)
	csd		t4, zero, (REG_SIZE * 11)($c11)
	csd		t5, zero, (REG_SIZE * 12)($c11)
	csd		t6, zero, (REG_SIZE * 13)($c11)
	csd		t7, zero, (REG_SIZE * 14)($c11)
#endif
	csd		t8, zero, (REG_SIZE * 15)($c11)
	csd		t9, zero, (REG_SIZE * 16)($c11)

	csd		s0, zero, (REG_SIZE * 17)($c11)
	csd		s1, zero, (REG_SIZE * 18)($c11)
	csd		s2, zero, (REG_SIZE * 19)($c11)
	csd		s3, zero, (REG_SIZE * 20)($c11)
	csd		s4, zero, (REG_SIZE * 21)($c11)
	csd		s5, zero, (REG_SIZE * 22)($c11)
	csd		s6, zero, (REG_SIZE * 23)($c11)
	csd		s7, zero, (REG_SIZE * 24)($c11)

	csd		gp, zero, (REG_SIZE * 25)($c11)
	csd		sp, zero, (REG_SIZE * 26)($c11)
	csd		s8, zero, (REG_SIZE * 27)($c11)
	csd		ra, zero, (REG_SIZE * 28)($c11)

	mflo		v0
	mfhi		v1

	csd		v0, zero, (REG_SIZE * 29)($c11)
	csd		v1, zero, (REG_SIZE * 30)($c11)

	MFC0		a0, CP0_STATUS
	MFC0		a1, CP0_CAUSE
	MFC0		a2, CP0_BAD_VADDR
	MFC0		a3, CP0_EXC_PC

	csd		a0, zero, (REG_SIZE * 31)($c11)
	csd		a1, zero, (REG_SIZE * 32)($c11)
	csd		a2, zero, (REG_SIZE * 33)($c11)
	csd		a3, zero, (REG_SIZE * 34)($c11)

	csc		CHERI_REG_C1, zero, (C1)($c11)
	csc		CHERI_REG_C2, zero, (C2)($c11)
	csc		CHERI_REG_C3, zero, (C3)($c11)
	csc		CHERI_REG_C4, zero, (C4)($c11)
	csc		CHERI_REG_C5, zero, (C5)($c11)
	csc		CHERI_REG_C6, zero, (C6)($c11)
	csc		CHERI_REG_C7, zero, (C7)($c11)
	csc		CHERI_REG_C8, zero, (C8)($c11)
	csc		CHERI_REG_C9, zero, (C9)($c11)
	csc		CHERI_REG_C10, zero, (C10)($c11)
	csc		CHERI_REG_STC, zero, (STC)($c11)
	csc		CHERI_REG_C12, zero, (C12)($c11)
	csc		CHERI_REG_C13, zero, (C13)($c11)
	csc		CHERI_REG_C14, zero, (C14)($c11)
	csc		CHERI_REG_C15, zero, (C15)($c11)
	csc		CHERI_REG_C16, zero, (C16)($c11)
	csc		CHERI_REG_C17, zero, (C17)($c11)
	csc		CHERI_REG_C18, zero, (C18)($c11)
	csc		CHERI_REG_C19, zero, (C19)($c11)
	csc		CHERI_REG_C20, zero, (C20)($c11)
	csc		CHERI_REG_C21, zero, (C21)($c11)
	csc		CHERI_REG_C22, zero, (C22)($c11)
	csc		CHERI_REG_C23, zero, (C23)($c11)
	csc		CHERI_REG_C24, zero, (C24)($c11)
	csc		CHERI_REG_C25, zero, (C25)($c11)
	csc		CHERI_REG_C26, zero, (IDC)($c11)
	csc		CHERI_REG_C27, zero, (C27)($c11)
	csc		CHERI_REG_C28, zero, (C28)($c11)
	csc		CHERI_REG_C29, zero, (C29)($c11)
	csc		CHERI_REG_C30, zero, (C30)($c11)
	csc		CHERI_REG_C31, zero, (C31)($c11)
	cgetepcc	CHERI_REG_KSCRATCH
	csc		CHERI_REG_KSCRATCH, zero, (PCC)($c11)
	cgetdefault	CHERI_REG_KSCRATCH
	csc		CHERI_REG_KSCRATCH, zero, (DDC)($c11)
	cgetcause	t0
	csd		t0, zero, CAPCAUSE($c11)
.endm

.macro restore_registers_purecap
	cld		v0, zero, (REG_SIZE * 29)($c11)
	cld		v1, zero, (REG_SIZE * 30)($c11)

	mtlo		v0
	mthi		v1

	cld		k0, zero, (REG_SIZE * 31)($c11)
	cld		a3, zero, (REG_SIZE * 34)($c11)

	clc		CHERI_REG_KSCRATCH, zero, (DDC)($c11)
	csetdefault	CHERI_REG_KSCRATCH
	clc		CHERI_REG_C1, zero, (C1)($c11)
	clc		CHERI_REG_C2, zero, (C2)($c11)
	clc		CHERI_REG_C3, zero, (C3)($c11)
	clc		CHERI_REG_C4, zero, (C4)($c11)
	clc		CHERI_REG_C5, zero, (C5)($c11)
	clc		CHERI_REG_C6, zero, (C6)($c11)
	clc		CHERI_REG_C7, zero, (C7)($c11)
	clc		CHERI_REG_C8, zero, (C8)($c11)
	clc		CHERI_REG_C9, zero, (C9)($c11)
	clc		CHERI_REG_C10, zero, (C10)($c11)
	/* $c11 (sp) is already restored */
	clc		CHERI_REG_C12, zero, (C12)($c11)
	clc		CHERI_REG_C13, zero, (C13)($c11)
	clc		CHERI_REG_C14, zero, (C14)($c11)
	clc		CHERI_REG_C15, zero, (C15)($c11)
	clc		CHERI_REG_C16, zero, (C16)($c11)
	clc		CHERI_REG_C17, zero, (C17)($c11)
	clc		CHERI_REG_C18, zero, (C18)($c11)
	clc		CHERI_REG_C19, zero, (C19)($c11)
	clc		CHERI_REG_C20, zero, (C20)($c11)
	clc		CHERI_REG_C21, zero, (C21)($c11)
	clc		CHERI_REG_C22, zero, (C22)($c11)
	clc		CHERI_REG_C23, zero, (C23)($c11)
	clc		CHERI_REG_C24, zero, (C24)($c11)
	clc		CHERI_REG_C25, zero, (C25)($c11)
	clc		CHERI_REG_C26, zero, (IDC)($c11)
	/* C27 (KSTRATCH) restored after EPCC. */
	clc		CHERI_REG_C28, zero, (C28)($c11)
	clc		CHERI_REG_C29, zero, (C29)($c11)
	clc		CHERI_REG_C30, zero, (C30)($c11)
	clc		CHERI_REG_C31, zero, (C31)($c11)

	clc		CHERI_REG_KSCRATCH, zero, (PCC)($c11)
	csetepcc	CHERI_REG_KSCRATCH
	clc		CHERI_REG_C27, zero, (C27)($c11)

.set noat
	cld		AT, zero, (REG_SIZE * 0)($c11)
.set at
	cld		v0, zero, (REG_SIZE * 1)($c11)
	cld		v1, zero, (REG_SIZE * 2)($c11)

	cld		a0, zero, (REG_SIZE * 3)($c11)
	cld		a1, zero, (REG_SIZE * 4)($c11)
	cld		a2, zero, (REG_SIZE * 5)($c11)
	cld		a3, zero, (REG_SIZE * 6)($c11)
#if defined(__mips_n32) || defined(__mips_n64)
	cld		a4, zero, (REG_SIZE * 7)($c11)
	cld		a5, zero, (REG_SIZE * 8)($c11)
	cld		a6, zero, (REG_SIZE * 9)($c11)
	cld		a7, zero, (REG_SIZE * 10)($c11)

	cld		t0, zero, (REG_SIZE * 11)($c11)
	cld		t1, zero, (REG_SIZE * 12)($c11)
	cld		t2, zero, (REG_SIZE * 13)($c11)
	cld		t3, zero, (REG_SIZE * 14)($c11)
#else
	cld		t0, zero, (REG_SIZE * 7)($c11)
	cld		t1, zero, (REG_SIZE * 8)($c11)
	cld		t2, zero, (REG_SIZE * 9)($c11)
	cld		t3, zero, (REG_SIZE * 10)($c11)

	cld		t4, zero, (REG_SIZE * 11)($c11)
	cld		t5, zero, (REG_SIZE * 12)($c11)
	cld		t6, zero, (REG_SIZE * 13)($c11)
	cld		t7, zero, (REG_SIZE * 14)($c11)
#endif
	cld		t8, zero, (REG_SIZE * 15)($c11)
	cld		t9, zero, (REG_SIZE * 16)($c11)

	cld		s0, zero, (REG_SIZE * 17)($c11)
	cld		s1, zero, (REG_SIZE * 18)($c11)
	cld		s2, zero, (REG_SIZE * 19)($c11)
	cld		s3, zero, (REG_SIZE * 20)($c11)
	cld		s4, zero, (REG_SIZE * 21)($c11)
	cld		s5, zero, (REG_SIZE * 22)($c11)
	cld		s6, zero, (REG_SIZE * 23)($c11)
	cld		s7, zero, (REG_SIZE * 24)($c11)

	cld		s8, zero, (REG_SIZE * 27)($c11)
	cld		ra, zero, (REG_SIZE * 28)($c11)

	cincoffset	$c11, $c11, (TF_SIZE)
.endm
