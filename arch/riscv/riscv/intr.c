/*-
 * Copyright (c) 2019-2020 Ruslan Bukin <br@bsdpad.com>
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

#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/thread.h>

#include <machine/frame.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#include <riscv/riscv/trap.h>
#include <riscv/include/intr.h>
#include <riscv/include/clic.h>
#include <riscv/include/clint.h>
#include <riscv/include/plic.h>

#define	INTR_DEBUG
#undef	INTR_DEBUG

#ifdef	INTR_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

void
riscv_intr(int irq)
{

#ifdef MDX_RISCV_CLIC
	if (irq >= 16) {
		clic_intr(irq);
		return;
	}
#endif

	switch (irq) {
	case IRQ_TIMER_SUPERVISOR:
	case IRQ_TIMER_MACHINE:
		clint_intr();
		break;
#ifdef MDX_SCHED_SMP
	case IRQ_SOFTWARE_MACHINE:
		clint_intr_software();
		break;
#endif
#ifdef MDX_RISCV_PLIC
	case IRQ_EXTERNAL_SUPERVISOR:
		plic_intr();
		break;
#endif
	default:
		printf("%s%d: Unhandled interrupt %d\n",
		    __func__, PCPU_GET(cpuid), irq);
		break;
	}
}
