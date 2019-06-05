/*-
 * Copyright (c) 2018 Ruslan Bukin <br@bsdpad.com>
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
 * ("CTSRD"), as part of the DARPA CRASH research programme.
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
#include <machine/frame.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>
#include <mips/mips/timer.h>

#define	TIMER_DEBUG
#undef	TIMER_DEBUG

#ifdef	TIMER_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

void
mips_timer_intr(void *arg, struct trapframe *frame, int irq)
{
	struct mips_timer_softc *sc;

	dprintf("%s\n", __func__);

	sc = arg;

	if (sc->usleep > 0)
		sc->usleep = 0;

	mips_wr_compare(-1);

	callout_callback(&sc->mt);
}

void
mips_timer_usleep(struct mips_timer_softc *sc, uint32_t usec)
{
	uint32_t intr;
	uint32_t reg;
	uint8_t flag;
	uint32_t ticks;

	sc->usleep = usec;

	ticks = usec * (sc->frequency / 1000000);

	flag = 0;

	while (((volatile uint32_t *)&sc->usleep)[0] > 0) {
		intr = intr_disable();

		if (flag == 0) {
			reg = mips_rd_count();
			reg += ticks;
			mips_wr_compare(reg);
			flag = 1;
		}

		__asm("wait");

		intr_restore(intr);
	}
}

void
mips_timer_udelay(struct mips_timer_softc *sc, uint32_t usec)
{
	uint32_t counts, delta;
	uint32_t first,  last;

	counts = 0;
	delta = 0;
	first = mips_rd_count();
	while (counts < usec) {
		last = mips_rd_count();

		if (last > first)
			delta += last - first;
		else
			delta += last + (0xffffffff - first) + 1;

		first = last;

		counts += delta / (sc->frequency / 1000000);
		delta %= (sc->frequency / 1000000);
	}
}

static uint32_t
timer_count(void *arg)
{
	uint32_t count;

	count = mips_rd_count();

	dprintf("%s: count %u\n", __func__, count);

	return (count);
}

static void
timer_stop(void *arg)
{

	dprintf("%s\n", __func__);

	mips_wr_compare(-1);
}

static void
timer_start(void *arg, uint32_t ticks)
{
	struct mips_timer_softc *sc;
	uint32_t reg;

	dprintf("%s: ticks %u\n", __func__, ticks);

	sc = arg;
	if (ticks < sc->minticks)
		ticks = sc->minticks;

#ifdef	CONFIG_QEMU
	uint32_t new;
	int diff;

	do {
		reg = mips_rd_count() + ticks;

		/*
		 * Note QEMU could context-switch here on the HOST OS.
		 */

		mips_wr_compare(reg);

		/* Check if QEMU context switched. */
		new = mips_rd_count();
		diff = reg - new;
	} while (diff < 0);
#else
	reg = mips_rd_count() + ticks;
	mips_wr_compare(reg);
#endif

}

void
mips_timer_init(struct mips_timer_softc *sc, uint32_t freq,
    uint32_t minticks)
{
	uint32_t reg;

	reg = mips_rd_config7();
	if ((reg & CONFIG7_WII) == 0)
		printf("%s: can't initialize timer\n", __func__);

	sc->minticks = minticks;
	sc->frequency = freq;
	sc->mt.start = timer_start;
	sc->mt.stop = timer_stop;
	sc->mt.count = timer_count;
	sc->mt.width = 0xffffffff;
	sc->mt.arg = sc;

	mips_wr_compare(-1);

	callout_register(&sc->mt);
}
