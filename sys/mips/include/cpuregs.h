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

#ifndef _MACHINE_CPUREGS_H_
#define	_MACHINE_CPUREGS_H_

#ifdef __mips_n64
#define	MIPS_XKPHYS_UNCACHED_BASE	0x9000000000000000ULL
#endif

/* CP0 status register */
#define	MIPS_SR_IE	(1 << 0)
#define	MIPS_SR_EXL	(1 << 1)
#define	MIPS_SR_ERL	(1 << 2)
#define	MIPS_SR_R0	(1 << 3) /* Read-only */
#define	MIPS_SR_UM	(1 << 4)
#define	MIPS_SR_KSU_S	3
#define	MIPS_SR_KSU_M	(0x3 << MIPS_SR_KSU_S)
#define	MIPS_SR_UX	5
#define	MIPS_SR_SX	6
#define	MIPS_SR_KX	7
#define	MIPS_SR_IM_S	8
#define	MIPS_SR_IM_M	(0xff << MIPS_SR_IM_S)
#define	MIPS_SR_IM(n)	(1 << (MIPS_SR_IM_S + (n)))
#define	MIPS_SR_IM_SOFT(n)	MIPS_SR_IM((n))
#define	MIPS_SR_IM_HARD(n)	MIPS_SR_IM(2 + (n))
#define	MIPS_SR_NMI	(1 << 19)
#define	MIPS_SR_SR	(1 << 20)
#define	MIPS_SR_RS	(1 << 21)
#define	MIPS_SR_BEV	(1 << 22)
#define	MIPS_SR_PX	(1 << 23)
#define	MIPS_SR_MX	(1 << 24)
#define	MIPS_SR_RE	(1 << 25)
#define	MIPS_SR_FR	(1 << 26)
#define	MIPS_SR_RP	(1 << 27)
#define	MIPS_SR_CU(n)	(1 << (28 + (n)))

/* CP0 cause register */
#define	MIPS_CR_EXC_CODE_S	2
#define	MIPS_CR_EXC_CODE_M	(0x1f << MIPS_CR_EXC_CODE_S)
#define	 MIPS_CR_EXC_CODE_INT		0 /* Interrupt */
#define	 MIPS_CR_EXC_CODE_MOD		1 /* TLB modification exception */
#define	 MIPS_CR_EXC_CODE_TLBL		2 /* TLB exception (load or instruction fetch) */
#define	 MIPS_CR_EXC_CODE_TLBS		3 /* TLB exception (store) */
#define	 MIPS_CR_EXC_CODE_ADEL		4 /* Address error exception (load or instruction fetch) */
#define	 MIPS_CR_EXC_CODE_ADES		5 /* Address error exception (store) */
#define	 MIPS_CR_EXC_CODE_IBE		6 /* Bus error exception (instruction fetch) */
#define	 MIPS_CR_EXC_CODE_DBE		7 /* Bus error exception (data reference: load or store) */
#define	 MIPS_CR_EXC_CODE_SYS		8 /* Syscall exception */
#define	 MIPS_CR_EXC_CODE_BP		9 /* Breakpoint exception */
#define	 MIPS_CR_EXC_CODE_RI		10 /* Reserved instruction exception */
#define	 MIPS_CR_EXC_CODE_CPU		11 /* Coprocessor Unusable exception */
#define	 MIPS_CR_EXC_CODE_OV		12 /* Arithmetic Overflow exception */
#define	 MIPS_CR_EXC_CODE_TR		13 /* Trap exception */
#define	 MIPS_CR_EXC_CODE_FPE		15 /* Floating point exception */
#define	 MIPS_CR_EXC_CODE_C2E		18 /* Reserved for precise Coprocessor 2 exceptions */
#define	 MIPS_CR_EXC_CODE_MDMX		22 /* Reserved for MDMX Unusable Exception in MIPS64 */
#define	 MIPS_CR_EXC_CODE_WATCH		23 /* Reference to WatchHi/WatchLo address */
#define	 MIPS_CR_EXC_CODE_MCHECK	24 /* Machine check */
#define	 MIPS_CR_EXC_CODE_CACHE_ERR	30 /* Cache error */
#define	MIPS_CR_IP_S		8
#define	MIPS_CR_IP_M		(0xff << MIPS_CR_IP_S)
#define	MIPS_CR_IP(n)		(1 << (MIPS_CR_IP_S + (n)))
#define	MIPS_CR_IP_SOFT(n)	MIPS_CR_IP((n))
#define	MIPS_CR_IP_HARD(n)	MIPS_CR_IP(2 + (n))
#define	MIPS_CR_WP		(1 << 22)
#define	MIPS_CR_IV		(1 << 23)
#define	MIPS_CR_CE_S		28
#define	MIPS_CR_CE_M		(0x3 << MIPS_CR_CE_S)
#define	MIPS_CR_BD		(1 << 31)

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

#endif /* !_MACHINE_CPUREGS_H_ */
