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
#define	MIPS_SR_IM_SOFT_0	MIPS_SR_IM(0)
#define	MIPS_SR_IM_SOFT_1	MIPS_SR_IM(1)
#define	MIPS_SR_IM_HARD_0	MIPS_SR_IM(2)
#define	MIPS_SR_IM_HARD_1	MIPS_SR_IM(3)
#define	MIPS_SR_IM_HARD_2	MIPS_SR_IM(4)
#define	MIPS_SR_IM_HARD_3	MIPS_SR_IM(5)
#define	MIPS_SR_IM_HARD_4	MIPS_SR_IM(6)
#define	MIPS_SR_IM_HARD_5	MIPS_SR_IM(7)
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

#endif /* !_MACHINE_CPUREGS_H_ */
