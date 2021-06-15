/*-
 * Copyright (c) 2019-2021 Ruslan Bukin <br@bsdpad.com>
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

#ifdef MDX_RISCV_SUPERVISOR_MODE

static __inline register_t
intr_disable(void)
{
	register_t reg;

	__asm __volatile(
		"csrrci %0, sstatus, %1"
		: "=&r" (reg) : "i" (SSTATUS_SIE)
	);

	return (reg & (SSTATUS_SIE));
}

static __inline void
intr_restore(register_t reg)
{

	__asm __volatile(
		"csrs sstatus, %0"
		:: "r" (reg)
	);
}

static __inline void
intr_enable(void)
{

	__asm __volatile(
		"csrsi sstatus, %0"
		:: "i" (SSTATUS_SIE)
	);
}

#else /* !MDX_RISCV_SUPERVISOR_MODE */

static __inline register_t
intr_disable(void)
{
	register_t reg;

	__asm __volatile(
		"csrrci %0, mstatus, %1"
		: "=&r" (reg) : "i" (MSTATUS_MIE)
	);

	return (reg & (MSTATUS_MIE));
}

static __inline void
intr_restore(register_t reg)
{

	__asm __volatile(
		"csrs mstatus, %0"
		:: "r" (reg)
	);
}

static __inline void
intr_enable(void)
{

	__asm __volatile(
		"csrsi mstatus, %0"
		:: "i" (MSTATUS_MIE)
	);
}

#endif

#endif /* !_MACHINE_CPUFUNC_H_ */
