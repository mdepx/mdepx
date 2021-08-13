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

#ifndef	_MACHINE_PCPU_H_
#define	_MACHINE_PCPU_H_

static inline struct pcpu *
get_pcpu(void)
{
	struct pcpu *pcpu;

#ifdef __CHERI_PURE_CAPABILITY__
	__asm __volatile("cmove %0, cgp" : "=&C"(pcpu));
#else
	__asm __volatile("mv %0, gp" : "=&r"(pcpu));
#endif

	return (pcpu);
}

static inline struct thread *
get_curthread(void)
{
	struct thread *td;

#ifdef __CHERI_PURE_CAPABILITY__
	__asm __volatile("clc %0, 0(cgp)" : "=&C"(td));
#else

#if __riscv_xlen == 64
	__asm __volatile("ld %0, 0(gp)" : "=&r"(td));
#else
	__asm __volatile("lw %0, 0(gp)" : "=&r"(td));
#endif

#endif

	return (td);
}

#define	curthread get_curthread()
#define	curpcpu get_pcpu()

#define	PCPU_GET(member)	(get_pcpu()->pc_ ## member)
#define	PCPU_ADD(member, value)	(get_pcpu()->pc_ ## member += (value))
#define	PCPU_INC(member)	PCPU_ADD(member, 1)
#define	PCPU_PTR(member)	(&get_pcpu()->pc_ ## member)
#define	PCPU_SET(member, value)	(get_pcpu()->pc_ ## member = (value))

#endif /* !_MACHINE_PCPU_H_ */
