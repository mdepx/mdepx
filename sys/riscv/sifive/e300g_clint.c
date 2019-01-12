/*-
 * Copyright (c) 2017 Ruslan Bukin <br@bsdpad.com>
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

/* Coreplex-Local Interrupts (CLINT) */

#include <sys/types.h>

#include <riscv/include/machdep.h>
#include <riscv/sifive/e300g_clint.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static int
clint_mtime(struct clint_softc *sc)
{
	uint32_t reg;

	reg = RD4(sc, MTIME);

	return (reg);
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
e300g_clint_init(struct clint_softc *sc, uint32_t base)
{

	sc->base = base;

	return (0);
}
