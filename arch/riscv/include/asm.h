/*-
 * Copyright (c) 2018 Ruslan Bukin <br@bsdpad.com>
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

#ifndef	_MACHINE_ASM_H_
#define	_MACHINE_ASM_H_

#include <machine/abi.h>

#define	ENTRY(sym)					\
	.text; .globl sym; .ent sym; sym:

#define	GLOBAL(sym)					\
	.globl sym; sym:

#define	ASM_ENTRY(sym)					\
	.text; .globl sym; .type sym,@function; sym:

#define	END(sym) .size sym, . - sym

#if REG_SIZE == 4
#define	REG_S		sw
#define	REG_L		lw
#define	PTR_LA		la
#elif REG_SIZE == 8
#define	REG_S		sd
#define	REG_L		ld
#define	PTR_LA		lla
#endif

#ifndef __CHERI_PURE_CAPABILITY__

#ifdef MDX_RISCV_SUPERVISOR_MODE
#define	REG_EPC		sepc
#define	REG_CAUSE	scause
#define	REG_STATUS	sstatus
#define	REG_TVAL	stval
#define	REG_SCRATCH	sscratch
#define	REG_TVEC	stvec
#define	RET		sret
#else
#define	REG_EPC		mepc
#define	REG_CAUSE	mcause
#define	REG_STATUS	mstatus
#define	REG_TVAL	mtval
#define	REG_SCRATCH	mscratch
#define	REG_TVEC	mtvec
#define	RET		mret
#endif

#else /* __CHERI_PURE_CAPABILITY__ */

#ifdef MDX_RISCV_SUPERVISOR_MODE
#define	REG_EPC		sepcc
#define	REG_CAUSE	scause
#define	REG_STATUS	sstatus
#define	REG_TVAL	stval
#define	REG_SCRATCH	sscratchc
#define	RET		sret
#else
#define	REG_EPC		mepcc
#define	REG_CAUSE	mcause
#define	REG_STATUS	mstatus
#define	REG_TVAL	mtval
#define	REG_SCRATCH	mscratchc
#define	RET		mret
#endif

#endif

#endif	/* !_MACHINE_ASM_H_ */
