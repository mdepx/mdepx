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
#include <machine/cpuregs.h>

.macro save_registers el
	cincoffset	csp, csp, -(35 * CREG_SIZE)

	csc	cra, (0 * CREG_SIZE)(csp)
	csc	csp, (1 * CREG_SIZE)(csp)
	csc	cgp, (2 * CREG_SIZE)(csp)
	csc	ctp, (3 * CREG_SIZE)(csp)

	csc	ct0, (4 * CREG_SIZE)(csp)
	csc	ct1, (5 * CREG_SIZE)(csp)
	csc	ct2, (6 * CREG_SIZE)(csp)
	csc	ct3, (7 * CREG_SIZE)(csp)
	csc	ct4, (8 * CREG_SIZE)(csp)
	csc	ct5, (9 * CREG_SIZE)(csp)
	csc	ct6, (10 * CREG_SIZE)(csp)

	csc	cs0, (11 * CREG_SIZE)(csp)
	csc	cs1, (12 * CREG_SIZE)(csp)
	csc	cs2, (13 * CREG_SIZE)(csp)
	csc	cs3, (14 * CREG_SIZE)(csp)
	csc	cs4, (15 * CREG_SIZE)(csp)
	csc	cs5, (16 * CREG_SIZE)(csp)
	csc	cs6, (17 * CREG_SIZE)(csp)
	csc	cs7, (18 * CREG_SIZE)(csp)
	csc	cs8, (19 * CREG_SIZE)(csp)
	csc	cs9, (20 * CREG_SIZE)(csp)
	csc	cs10, (21 * CREG_SIZE)(csp)
	csc	cs11, (22 * CREG_SIZE)(csp)

	csc	ca0, (23 * CREG_SIZE)(csp)
	csc	ca1, (24 * CREG_SIZE)(csp)
	csc	ca2, (25 * CREG_SIZE)(csp)
	csc	ca3, (26 * CREG_SIZE)(csp)
	csc	ca4, (27 * CREG_SIZE)(csp)
	csc	ca5, (28 * CREG_SIZE)(csp)
	csc	ca6, (29 * CREG_SIZE)(csp)
	csc	ca7, (30 * CREG_SIZE)(csp)

	cspecialr	ct0, REG_EPC
	csc		ct0, (31 * CREG_SIZE)(csp)
	csrr		t0, REG_STATUS
	csd		t0, (32 * CREG_SIZE)(csp)
	csrr		t0, REG_TVAL
	csd		t0, (33 * CREG_SIZE)(csp)
	csrr		t0, REG_CAUSE
	csd		t0, (34 * CREG_SIZE)(csp)
.endm

.macro load_registers el
	clc		ct0, (31 * CREG_SIZE)(csp)
	cspecialw	REG_EPC, ct0
	cld		t0, (32 * CREG_SIZE)(csp)
	csrw		REG_STATUS, t0

	clc	cra, (0 * CREG_SIZE)(csp)
	clc	ctp, (3 * CREG_SIZE)(csp)

	clc	ct0, (4 * CREG_SIZE)(csp)
	clc	ct1, (5 * CREG_SIZE)(csp)
	clc	ct2, (6 * CREG_SIZE)(csp)
	clc	ct3, (7 * CREG_SIZE)(csp)
	clc	ct4, (8 * CREG_SIZE)(csp)
	clc	ct5, (9 * CREG_SIZE)(csp)
	clc	ct6, (10 * CREG_SIZE)(csp)

	clc	cs0, (11 * CREG_SIZE)(csp)
	clc	cs1, (12 * CREG_SIZE)(csp)
	clc	cs2, (13 * CREG_SIZE)(csp)
	clc	cs3, (14 * CREG_SIZE)(csp)
	clc	cs4, (15 * CREG_SIZE)(csp)
	clc	cs5, (16 * CREG_SIZE)(csp)
	clc	cs6, (17 * CREG_SIZE)(csp)
	clc	cs7, (18 * CREG_SIZE)(csp)
	clc	cs8, (19 * CREG_SIZE)(csp)
	clc	cs9, (20 * CREG_SIZE)(csp)
	clc	cs10, (21 * CREG_SIZE)(csp)
	clc	cs11, (22 * CREG_SIZE)(csp)

	clc	ca0, (23 * CREG_SIZE)(csp)
	clc	ca1, (24 * CREG_SIZE)(csp)
	clc	ca2, (25 * CREG_SIZE)(csp)
	clc	ca3, (26 * CREG_SIZE)(csp)
	clc	ca4, (27 * CREG_SIZE)(csp)
	clc	ca5, (28 * CREG_SIZE)(csp)
	clc	ca6, (29 * CREG_SIZE)(csp)
	clc	ca7, (30 * CREG_SIZE)(csp)

	cincoffset	csp, csp, (35 * CREG_SIZE)
.endm

/*
 * riscv bumblebee core (gd32v) requires 6 bits alignment for its
 * exception handler.
 */

	.align 6
ASM_ENTRY(cpu_exception_handler)
	save_registers 1
	cmove		ca0, csp
	cspecialr	csp, REG_SCRATCH
	clgc		ct0, riscv_exception
	cjalr		ct0
	cmove		csp, ca0
	load_registers 1
	RET
END(cpu_exception_handler)

#ifdef MDX_RISCV_FPE
/*
 * void fpe_state_clear(void);
 */
	.align 2
ASM_ENTRY(fpe_state_clear)
	/* Enable FPE */
	li	t0, MSTATUS_FS_INITIAL
	csrs	REG_STATUS, t0

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
	csrc	REG_STATUS, t0
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
	csrs	REG_STATUS, t0

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
	csrc	REG_STATUS, t0
	ret
END(fpe_state_restore)
#endif /* !MDX_RISCV_FPE */
