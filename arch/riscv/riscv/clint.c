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

struct clint_softc *clint_sc;

#if defined(MDX_OF) && !defined(MDX_RISCV_SUPERVISOR_MODE)
static struct mdx_compat_data clint_compat_data[] = {
	{ "riscv,clint0" },
	{ NULL },
};
#endif

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
clint_init(struct clint_softc *sc, capability base,
    uint32_t frequency)
{
	int error;

	if (clint_sc != NULL)
		return (MDX_EEXIST);
	clint_sc = sc;

	sc->base = base;

	error = clint_timer_init(sc, frequency);

	return (error);
}

#ifdef MDX_OF
static int
clint_get_freq(uint32_t *freq)
{
	int cpus_node;
	int error;

	cpus_node = mdx_of_node_by_path("/cpus");
	if (cpus_node < 0)
		return (MDX_ERROR);

	error = mdx_of_get_prop32(cpus_node, "timebase-frequency", freq, NULL);

	return (error);
}

#ifndef MDX_RISCV_SUPERVISOR_MODE
static int
clint_probe(mdx_device_t dev)
{

	if (!mdx_of_is_compatible(dev, "riscv,clint0"))
		return (MDX_ERROR);

	return (MDX_OK);
}

static int
clint_attach(mdx_device_t dev)
{
	struct clint_softc *sc;
	size_t base, size;
	capability cap;
	uint32_t freq;
	int error;

	if (clint_sc != NULL)
		return (MDX_EEXIST);

	sc = mdx_device_get_softc(dev);
	sc->dev = dev;

	error = mdx_of_get_reg(dev, 0, &base, &size);
	if (error)
		return (error);

	cap = mdx_getdefault();
	cap = mdx_setoffset(cap, base);
	cap = mdx_setbounds(cap, size);
	sc->base = cap;

	error = clint_get_freq(&freq);
	if (error != 0)
		return (error);

	error = clint_init(sc, cap, freq);

	return (error);
}

static struct mdx_device_ops clint_ops = {
	.probe = clint_probe,
	.attach = clint_attach,
};

static mdx_driver_t clint_driver = {
	"clint",
	&clint_ops,
	sizeof(struct clint_softc),
	clint_compat_data,
};

DRIVER_MODULE_ORDERED(clint, clint_driver, SI_ORDER_THIRD);
#else
static void
mdx_clint_sysinit(void *unused)
{
	struct clint_softc *sc;
	uint32_t freq;
	int error;

	error = clint_get_freq(&freq);
	if (error != 0) {
		printf("%s: could not get timer frequency\n", __func__);
		return;
	}

	sc = zalloc(sizeof(struct clint_softc));

	clint_init(sc, NULL, freq);
}

SYSINIT(clint, SI_SUB_DRIVERS, SI_ORDER_FIRST, mdx_clint_sysinit, NULL);
#endif
#endif
