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

#define	CLINT_DEBUG
#undef	CLINT_DEBUG

#ifdef	CLINT_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

extern struct clint_softc *clint_sc;

void
clint_intr(void)
{
	struct clint_softc *sc;

	dprintf("%s\n", __func__);

	sc = clint_sc;

	csr_clear_tie();

	mdx_callout_callback(&sc->mt);
}

static void
clint_stop(void *arg)
{

	csr_clear_tie();
	csr_clear_tip();
}

#if __riscv_xlen == 64
static void
clint_set_timer(struct clint_softc *sc, int cpuid, uint64_t new)
{

#ifndef MDX_RISCV_SUPERVISOR_MODE
	WR8(sc, MTIMECMP(cpuid), new);
#else
	sbi_set_timer(new);
#endif
}
#endif

static void
clint_start(void *arg, uint32_t ticks)
{
	struct clint_softc *sc;
	int cpuid;

	sc = arg;

	cpuid = PCPU_GET(cpuid);

	dprintf("%s: ticks %u\n", __func__, ticks);
	dprintf("%s%d: ticks %u\n", __func__, cpuid, ticks);

#if __riscv_xlen == 64
	uint64_t val;
	uint64_t new;

	val = csr_read64(time);
	new = val + ticks;
	clint_set_timer(sc, cpuid, new);
#else
	/*
	 * Machine-mode only.
	 * TODO: support for 32-bit supervisor-mode.
	 */
	uint32_t low, high;
	uint32_t new;

	low = RD4(sc, MTIME);
	high = RD4(sc, MTIME + 0x4);
	new = low + ticks;
	dprintf("%s%d: ticks %u, low %u, high %u, new %u\n",
	    __func__, PCPU_GET(cpuid), ticks, low, high, new);
	if (new < low)
		high += 1;
	WR4(sc, MTIMECMP(cpuid) + 0x4, high);
	WR4(sc, MTIMECMP(cpuid), new);
#endif

	csr_set_tie();
}

ticks_t
clint_mtime(void *arg)
{
	ticks_t low;

#ifdef MDX_RISCV_SUPERVISOR_MODE
#if __riscv_xlen == 64
	low = csr_read(time);
#else
	low = csr_read64(time);
#endif
#else
	struct clint_softc *sc;

	sc = arg;

#if __riscv_xlen == 64
	low = RD4(sc, MTIME);
#else
	low = RD8(sc, MTIME);
#endif
#endif

	return (low);
}

void
clint_udelay(struct clint_softc *sc,
    uint32_t usec, uint32_t osc_freq)
{
	ticks_t first, last;
	ticks_t delta;
	ticks_t counts;

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

int
clint_timer_init(struct clint_softc *sc, uint32_t frequency)
{

	sc->mt.start = clint_start;
	sc->mt.stop = clint_stop;
	sc->mt.count = clint_mtime;
	sc->mt.maxcnt = 0xffffffff;
	sc->mt.frequency = frequency;
	sc->mt.arg = sc;

	mdx_callout_register(&sc->mt);

	clint_stop(sc);

	return (0);
}
