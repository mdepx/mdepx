/*-
 * Copyright (c) 2017-2021 Ruslan Bukin <br@bsdpad.com>
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

/* Core Local Interruptor (CLINT) */

#include <sys/cdefs.h>
#include <sys/pcpu.h>
#include <sys/smp.h>
#include <sys/systm.h>
#include <sys/io.h>
#include <sys/of.h>
#include <sys/cheri.h>

#include <machine/atomic.h>
#include <machine/cpuregs.h>
#include <machine/sbi.h>

#include <machine/clint.h>

extern struct clint_softc *clint_sc;

#ifdef MDX_SCHED_SMP
extern struct entry pcpu_all;

void
send_ipi(int cpumask, int ipi)
{
	struct pcpu *p;

	KASSERT(MDX_CPU_MAX <= 32, ("cpumask is 32 bit"));
	KASSERT(!list_empty(&pcpu_all), ("no cpus"));

	for (p = CONTAINER_OF(pcpu_all.next, struct pcpu, pc_all);;
	    (p = CONTAINER_OF(p->pc_all.next, struct pcpu, pc_all))) {
		if (cpumask & (1 << p->pc_cpuid)) {
			atomic_set_32(&p->pc_pending_ipis, ipi);
			clint_set_sip(p->pc_cpuid);
		}
		if (p->pc_all.next == &pcpu_all)
			break;
	}
}

void
clint_intr_software(void)
{
#ifndef MDX_RISCV_SUPERVISOR_MODE
	struct clint_softc *sc;
	int cpuid;

	sc = clint_sc;

	cpuid = PCPU_GET(cpuid);

	WR4(sc, MSIP(cpuid), 0);
#else
	csr_clear(sip, SIP_SSIP);
#endif

	ipi_handler();
}
#endif

void
clint_set_sip(int hart_id)
{
#ifndef MDX_RISCV_SUPERVISOR_MODE
	struct clint_softc *sc;

	sc = clint_sc;

	WR4(sc, MSIP(hart_id), 1);
#else
	u_long hart_mask;

	hart_mask = (1 << hart_id);
	sbi_send_ipi(&hart_mask);
#endif
}
