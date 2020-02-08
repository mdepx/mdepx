/*-
 * Copyright (c) 2017-2019 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/systm.h>
#include <sys/smp.h>

#include <machine/pcpu.h>
#include <machine/atomic.h>
#include <machine/cpuregs.h>

#include <riscv/sifive/e300g_clint.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

#define	CLINT_DEBUG
#undef	CLINT_DEBUG

#ifdef	CLINT_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

static struct clint_softc *clint_sc;

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
	struct clint_softc *sc;
	int cpuid;

	sc = clint_sc;

	cpuid = PCPU_GET(cpuid);

	WR4(sc, MSIP(cpuid), 0);

	ipi_handler();
}
#endif

void
clint_set_sip(int hart_id)
{
	struct clint_softc *sc;

	sc = clint_sc;

	WR4(sc, MSIP(hart_id), 1);
}

void
clint_intr(void)
{
	struct clint_softc *sc;

	dprintf("%s\n", __func__);

	sc = clint_sc;

	csr_clear(mie, MIE_MTIE);

	mdx_callout_callback(&sc->mt);
}

static void
clint_stop(void *arg)
{

	csr_clear(mie, MIE_MTIE);
	csr_clear(mip, MIP_MTIP);
}

static void
clint_start(void *arg, uint32_t ticks)
{
	struct clint_softc *sc;
	uint32_t low, high;
	uint32_t new;
	int cpuid;

	sc = arg;

	cpuid = PCPU_GET(cpuid);

	dprintf("%s: ticks %u\n", __func__, ticks);
	dprintf("%s%d: ticks %u\n", __func__, cpuid, ticks);
#if 0
	if (ticks > 4094967295)
		panic("ticks %u\n", ticks);
	if (ticks == 0)
		panic("ticks %u\n", ticks);
#endif

	low = RD4(sc, MTIME);
	high = RD4(sc, MTIME + 0x4);
	new = low + ticks;
	dprintf("%s%d: ticks %u, low %u, high %u, new %u\n",
	    __func__, PCPU_GET(cpuid), ticks, low, high, new);
	if (new < low)
		high += 1;
	WR4(sc, MTIMECMP(cpuid) + 0x4, high);
	WR4(sc, MTIMECMP(cpuid), new);

	csr_set(mie, MIE_MTIE);
}

static uint32_t
clint_mtime(void *arg)
{
	struct clint_softc *sc;
	uint32_t low;

	sc = arg;

	low = RD4(sc, MTIME);

	return (low);
}

void
clint_udelay(struct clint_softc *sc,
    uint32_t usec, uint32_t osc_freq)
{
	uint32_t first, last;
	uint32_t delta;
	uint32_t counts;

	counts = 0;
	first = clint_mtime(sc);
	while (counts < usec) {
		last = clint_mtime(sc);
		if (last > first)
			delta = last - first;
		else
			delta = last + (0xffffffff - first) + 1;

		counts += (delta * (1000000 / osc_freq));

		first = last;
	}
}

static int
clint_measure_cpu_freq(struct clint_softc *sc, uint32_t osc_freq, int n)
{
	u_long start_mcycle, delta_mcycle;
	u_long start_mtime, delta_mtime;
	u_long mtime_freq;
	u_long tmp;
	int freq;

	tmp = clint_mtime(sc);
	do {
		start_mtime = clint_mtime(sc);
	} while (start_mtime == tmp);

	start_mcycle = csr_read(mcycle);

	do {
		delta_mtime = (clint_mtime(sc) - start_mtime);
	} while (delta_mtime < n);

	delta_mcycle = (csr_read(mcycle) - start_mcycle);

	mtime_freq = osc_freq;

	freq = (delta_mcycle / delta_mtime) * mtime_freq;
	freq += ((delta_mcycle % delta_mtime) * mtime_freq) / delta_mtime;

	return (freq);
}

int
clint_get_cpu_freq(struct clint_softc *sc, uint32_t osc_freq)
{
	int cpu_freq;

	/* warm up I$ */
	clint_measure_cpu_freq(sc, osc_freq, 1);

	cpu_freq = clint_measure_cpu_freq(sc, osc_freq, 10);

	return (cpu_freq);
}

int
e300g_clint_init(struct clint_softc *sc, uint32_t base,
    uint32_t frequency)
{

	clint_sc = sc;
	sc->base = base;

	sc->mt.start = clint_start;
	sc->mt.stop = clint_stop;
	sc->mt.count = clint_mtime;
	sc->mt.maxcnt = 0xffffffff;
	sc->mt.frequency = frequency;
	sc->mt.usec_to_ticks = mdx_callout_usec_to_ticks;
	sc->mt.arg = sc;

	mdx_callout_register(&sc->mt);

	csr_clear(mie, MIE_MTIE);
	csr_clear(mip, MIP_MTIP);

	return (0);
}
