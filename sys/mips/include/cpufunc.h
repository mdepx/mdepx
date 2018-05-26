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

#define	CP0_USERLOCAL	$4,2
#define	CP0_PAGEMASK	$5,0
#define	CP0_TLB_SPEC	$5,4
#define	CP0_WIRED	$6
#define	CP0_BAD_VADDR	$8
#define	CP0_COUNT	$9
#define	CP0_COMPARE	$11
#define	CP0_STATUS	$12,0
#define	CP0_INTCTL	$12,1
#define	CP0_CAUSE	$13,0
#define	CP0_EXC_PC	$14
#define	CP0_EBASE	$15,1
#define	CP0_CONFIG	$16,0
#define	CP0_CONFIG1	$16,1
#define	CP0_CONFIG2	$16,2
#define	CP0_CONFIG3	$16,3
#define	CP0_CONFIG7	$16,7
#define	CP0_LLADDR	$17,0
#define	PMON_CSR	$17,7
#define	PMON_HIGH	$17,4
#define	PMON_LC		$17,5
#define	PMON_RC		$17,6
#define	CP0_WATCHLO	$18,0
#define	CP0_WATCHHI	$19,0
#define	CP0_ERRCTL	$26,0

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

#define	mips_rd_count()		mfc0(9, 0)
#define	mips_rd_compare()	mfc0(11, 0)
#define	mips_wr_compare(val)	mtc0(11, 0, (val))
#define	mips_rd_status()	mfc0(12, 0)
#define	mips_wr_status(val)	mtc0(12, 0, (val))
#define	mips_rd_cause()		mfc0(13, 0)
#define	mips_wr_cause(val)	mtc0(13, 0, (val))
#define	mips_wr_ebase(val)	mtc0(15, 1, (val))
#define	mips_rd_cinfo()		mfc0(15, 6)

#ifndef __ASSEMBLER__
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
#endif

#endif /* !_MACHINE_CPUFUNC_H_ */
