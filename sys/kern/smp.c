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

#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/spinlock.h>
#include <sys/smp.h>

#include <machine/atomic.h>
#include <machine/frame.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>
#include <machine/smp.h>

static void (*smp_tryst_func)(void *arg);
static void *smp_tryst_arg;
static int smp_tryst_ncpus;
static uint32_t smp_tryst_room[2];
static struct spinlock smp_lock;

void
ipi_handler(void)
{
	uint32_t ipi_bitmap;
	uint32_t ipi;
	int bit;

	ipi_bitmap = atomic_readandclear_int(PCPU_PTR(pending_ipis));
	if (ipi_bitmap == 0)
		return;

	while ((bit = ffs(ipi_bitmap))) {
		bit -= 1;
		ipi = (1 << bit);
		ipi_bitmap &= ~ipi;

		switch (ipi) {
		case IPI_IPI:
			break;
		case IPI_TRYST:
			smp_tryst_action();
			break;
		default:
			break;
		}
	}
}

void
smp_tryst_action(void)
{
	void (*local_func)(void *);
	void *local_arg;

	atomic_add_acq_int(&smp_tryst_room[0], 1);

	/* Wait all the CPUs to enter the room. */
	while (smp_tryst_room[0] < smp_tryst_ncpus)
		;

	local_func = smp_tryst_func;
	local_arg = smp_tryst_arg;
	local_func(local_arg);

	/* Leave the room. */
	atomic_add_rel_int(&smp_tryst_room[1], 1);
}

void
smp_tryst_cpus(uint32_t cpus, void (*fn), void *arg)
{
	int cpuid;
	int ncpus;
	int i;

	ncpus = 0;
	for (i = 0; i < MAXCPU; i++)
		if (cpus & (1 << i))
			ncpus += 1;

	if (ncpus == 0)
		panic("smp_tryst_cpus with cpu mask == 0");

	critical_enter();
	while (!sl_trylock(&smp_lock))
		ipi_handler();

	smp_tryst_func = fn;
	smp_tryst_arg = arg;
	smp_tryst_ncpus = ncpus;
	smp_tryst_room[1] = 0;
	atomic_store_rel_int(&smp_tryst_room[0], 0);

	cpuid = PCPU_GET(cpuid);
	send_ipi(cpus & ~(1 << cpuid), IPI_TRYST);

	if (cpus & (1 << cpuid))
		smp_tryst_action();

	while (atomic_load_acq_int(&smp_tryst_room[1]) < ncpus)
		;

	sl_unlock(&smp_lock);
	critical_exit();
}

void
smp_init(void)
{

	sl_init(&smp_lock);
}
