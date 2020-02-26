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
#include <machine/cpuregs.h>

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
	csrr	sp, mscratch
	jal	riscv_exception
	mv	sp, a0
	load_registers 1
	mret
END(cpu_exception_handler)

#ifdef MDX_RISCV_FPE
/*
 * void fpe_state_clear(void);
 */
	.align 2
ASM_ENTRY(fpe_state_clear)
	/* Enable FPE */
	li	t0, MSTATUS_FS_INITIAL
	csrs	mstatus, t0

	fscsr	zero
	fcvt.d.l f0, zero
	fcvt.d.l f1, zero
	fcvt.d.l f2, zero
	fcvt.d.l f3, zero
	fcvt.d.l f4, zero
	fcvt.d.l f5, zero
	fcvt.d.l f6, zero
	fcvt.d.l f7, zero
	fcvt.d.l f8, zero
	fcvt.d.l f9, zero
	fcvt.d.l f10, zero
	fcvt.d.l f11, zero
	fcvt.d.l f12, zero
	fcvt.d.l f13, zero
	fcvt.d.l f14, zero
	fcvt.d.l f15, zero
	fcvt.d.l f16, zero
	fcvt.d.l f17, zero
	fcvt.d.l f18, zero
	fcvt.d.l f19, zero
	fcvt.d.l f20, zero
	fcvt.d.l f21, zero
	fcvt.d.l f22, zero
	fcvt.d.l f23, zero
	fcvt.d.l f24, zero
	fcvt.d.l f25, zero
	fcvt.d.l f26, zero
	fcvt.d.l f27, zero
	fcvt.d.l f28, zero
	fcvt.d.l f29, zero
	fcvt.d.l f30, zero
	fcvt.d.l f31, zero

	/* Disable FPE */
	li	t0, MSTATUS_FS_MASK
	csrc	mstatus, t0
	ret
END(fpe_state_clear)

/*
 * void fpe_state_save(uint64_t *regs);
 */
	.align 2
ASM_ENTRY(fpe_state_save)
	fsd	f0, (0 * 16)(a0)
	fsd	f1, (1 * 16)(a0)
	fsd	f2, (2 * 16)(a0)
	fsd	f3, (3 * 16)(a0)
	fsd	f4, (4 * 16)(a0)
	fsd	f5, (5 * 16)(a0)
	fsd	f6, (6 * 16)(a0)
	fsd	f7, (7 * 16)(a0)
	fsd	f8, (8 * 16)(a0)
	fsd	f9, (9 * 16)(a0)
	fsd	f10, (10 * 16)(a0)
	fsd	f11, (11 * 16)(a0)
	fsd	f12, (12 * 16)(a0)
	fsd	f13, (13 * 16)(a0)
	fsd	f14, (14 * 16)(a0)
	fsd	f15, (15 * 16)(a0)
	fsd	f16, (16 * 16)(a0)
	fsd	f17, (17 * 16)(a0)
	fsd	f18, (18 * 16)(a0)
	fsd	f19, (19 * 16)(a0)
	fsd	f20, (20 * 16)(a0)
	fsd	f21, (21 * 16)(a0)
	fsd	f22, (22 * 16)(a0)
	fsd	f23, (23 * 16)(a0)
	fsd	f24, (24 * 16)(a0)
	fsd	f25, (25 * 16)(a0)
	fsd	f26, (26 * 16)(a0)
	fsd	f27, (27 * 16)(a0)
	fsd	f28, (28 * 16)(a0)
	fsd	f29, (29 * 16)(a0)
	fsd	f30, (30 * 16)(a0)
	fsd	f31, (31 * 16)(a0)
	frcsr	t0
	sd	t0, (32 * 16)(a0)
	ret
END(fpe_state_save)

/*
 * void fpe_state_restore(uint64_t *regs);
 */
	.align 2
ASM_ENTRY(fpe_state_restore)
	/* Enable FPE */
	li	t0, MSTATUS_FS_INITIAL
	csrs	mstatus, t0

	ld	t0, (32 * 16)(a0)
	fscsr	t0
	fld	f0, (0 * 16)(a0)
	fld	f1, (1 * 16)(a0)
	fld	f2, (2 * 16)(a0)
	fld	f3, (3 * 16)(a0)
	fld	f4, (4 * 16)(a0)
	fld	f5, (5 * 16)(a0)
	fld	f6, (6 * 16)(a0)
	fld	f7, (7 * 16)(a0)
	fld	f8, (8 * 16)(a0)
	fld	f9, (9 * 16)(a0)
	fld	f10, (10 * 16)(a0)
	fld	f11, (11 * 16)(a0)
	fld	f12, (12 * 16)(a0)
	fld	f13, (13 * 16)(a0)
	fld	f14, (14 * 16)(a0)
	fld	f15, (15 * 16)(a0)
	fld	f16, (16 * 16)(a0)
	fld	f17, (17 * 16)(a0)
	fld	f18, (18 * 16)(a0)
	fld	f19, (19 * 16)(a0)
	fld	f20, (20 * 16)(a0)
	fld	f21, (21 * 16)(a0)
	fld	f22, (22 * 16)(a0)
	fld	f23, (23 * 16)(a0)
	fld	f24, (24 * 16)(a0)
	fld	f25, (25 * 16)(a0)
	fld	f26, (26 * 16)(a0)
	fld	f27, (27 * 16)(a0)
	fld	f28, (28 * 16)(a0)
	fld	f29, (29 * 16)(a0)
	fld	f30, (30 * 16)(a0)
	fld	f31, (31 * 16)(a0)

	/* Disable FPE */
	li	t0, MSTATUS_FS_MASK
	csrc	mstatus, t0
	ret
END(fpe_state_restore)
#endif /* !MDX_RISCV_FPE */
