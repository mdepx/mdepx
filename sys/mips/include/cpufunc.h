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

#ifndef	_MACHINE_CPUFUNC_H_
#define	_MACHINE_CPUFUNC_H_

#include <machine/cpuregs.h>

#ifdef __mips_n64
#define mtc0(reg, sel, value)						\
	__asm __volatile(						\
		"dmtc0   %z0, $%1, %2"					\
		:: "r" ((uint64_t) (value)), "K" (reg), "K" (sel))

#define mfc0(reg, sel)						\
({								\
	uint64_t __value;					\
	__asm __volatile(					\
		"dmfc0 %0, $%1, %2"				\
		: "=r" (__value) : "K" (reg), "K" (sel));	\
	__value;						\
})
#else
#define mtc0(reg, sel, value)						\
	__asm __volatile(						\
		"mtc0   %z0, $%1, %2"					\
		:: "r" ((unsigned) (value)), "K" (reg), "K" (sel))

#define mfc0(reg, sel)						\
({								\
	uint32_t __value;					\
	__asm __volatile(					\
		"mfc0 %0, $%1, %2"				\
		: "=r" (__value) : "K" (reg), "K" (sel));	\
	__value;						\
})
#endif

#define	mips_wr_index(val)	mtc0(0, 0, (val))
#define	mips_rd_index()		mfc0(0, 0)
#define	mips_wr_entrylo0(val)	mtc0(2, 0, (val))
#define	mips_wr_entrylo1(val)	mtc0(3, 0, (val))
#define	mips_wr_pagemask(val)	mtc0(5, 0, (val))
#define	mips_rd_wired()		mfc0(6, 0)
#define	mips_rd_badvaddr()	mfc0(8, 0)
#define	mips_rd_count()		mfc0(9, 0)
#define	mips_wr_entryhi(val)	mtc0(10, 0, (val))
#define	mips_rd_entryhi()	mfc0(10, 0)
#define	mips_rd_compare()	mfc0(11, 0)
#define	mips_wr_compare(val)	mtc0(11, 0, (val))
#define	mips_rd_status()	mfc0(12, 0)
#define	mips_wr_status(val)	mtc0(12, 0, (val))
#define	mips_rd_cause()		mfc0(13, 0)
#define	mips_wr_cause(val)	mtc0(13, 0, (val))
#define	mips_wr_ebase(val)	mtc0(15, 1, (val))
#define	mips_rd_cinfo()		mfc0(15, 6)
#define	mips_rd_config0()	mfc0(16, 0)
#define	mips_wr_config0(val)	mtc0(16, 0, (val))
#define	mips_rd_config7()	mfc0(16, 7)
#define	 CONFIG7_WII		(1 << 31)	/* Wait IE Ignore bit */

static __inline void
intr_enable(void)
{
	register_t status;

	status = mips_rd_status();
	status |= MIPS_SR_IE;
	mips_wr_status(status);
};

static __inline register_t
intr_disable(void)
{
	register_t reg;

	reg = mips_rd_status();
	mips_wr_status(reg & ~MIPS_SR_IE);

	return (reg & MIPS_SR_IE);
};

static __inline void
intr_restore(register_t reg)
{

	if (reg == MIPS_SR_IE)
		intr_enable();
};

#endif /* !_MACHINE_CPUFUNC_H_ */
