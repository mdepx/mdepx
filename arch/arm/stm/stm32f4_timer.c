/*-
 * Copyright (c) 2018, 2020 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/callout.h>

#include <machine/frame.h>

#include <arm/stm/stm32f4_timer.h>

#define	TIM_DEBUG
#undef	TIM_DEBUG

#ifdef	TIM_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
stm32f4_timer_intr(void *arg, struct trapframe *tf, int irq)
{
	struct stm32f4_timer_softc *sc;
	uint32_t reg;

	sc = arg;

	dprintf("%s\n", __func__);

	reg = RD4(sc, TIM_DIER);
	reg &= ~DIER_CC1IE;
	WR4(sc, TIM_DIER, reg);

	reg = RD4(sc, TIM_SR);
	reg &= ~SR_CC1IF;
	WR4(sc, TIM_SR, reg);

	mdx_callout_callback(&sc->mt);
}

void
stm32f4_timer_udelay(struct stm32f4_timer_softc *sc, uint32_t ticks)
{

	/* TODO */
}

#if 0
void
stm32f4_timer_usleep(struct stm32f4_timer_softc *sc, uint32_t ticks)
{
	uint32_t primask;

	__asm __volatile( "mrs     %0, primask\n"
	    : "=&r" (primask) :: "memory");

	__asm __volatile("cpsid i");

	stm32f4_timer_setup(sc, ticks);

	__asm __volatile("wfi");

	__asm __volatile( "msr     primask, %0\n"
	    :: "r" (primask) : "memory");
}
#endif

static void
stm32f4_timer_start(void *arg, uint32_t ticks)
{
	struct stm32f4_timer_softc *sc;
	uint32_t reg;
	uint16_t val;

	sc = arg;

	dprintf("%s: %d\n", __func__, ticks);

	val = RD4(sc, TIM_CNT);
	val += ticks;
	WR4(sc, TIM_CCR1, val);

	reg = RD4(sc, TIM_CR1);
	reg |= CR1_CEN;
	WR4(sc, TIM_CR1, reg);

	reg = RD4(sc, TIM_DIER);
	reg |= DIER_CC1IE;
	WR4(sc, TIM_DIER, reg);
}

static void
stm32f4_timer_stop(void *arg)
{
	struct stm32f4_timer_softc *sc;
	uint32_t reg;

	sc = arg;

	reg = RD4(sc, TIM_CR1);
	reg &= ~CR1_CEN;
	WR4(sc, TIM_CR1, reg);
}

static uint32_t
stm32f4_timer_count(void *arg)
{
	struct stm32f4_timer_softc *sc;
	uint32_t reg;

	sc = arg;

	reg = RD4(sc, TIM_CNT);

	return (reg);
}

int
stm32f4_timer_init(struct stm32f4_timer_softc *sc,
    uint32_t base, uint32_t freq)
{

	sc->base = base;
	sc->freq = freq;

	/*
	 * Since the timer is 16-bit only, set some reasonable prescaler.
	 * Let's say 10000 ticks is 1 second.
	 */
	WR4(sc, TIM_CR1, 0);
	WR4(sc, TIM_PSC, (freq / 10000) - 1);

	sc->mt.start = stm32f4_timer_start;
	sc->mt.stop = stm32f4_timer_stop;
	sc->mt.count = stm32f4_timer_count;
	sc->mt.maxcnt = 0x0000ffff;
	sc->mt.frequency = 10000;
	sc->mt.usec_to_ticks = mdx_time_usec_to_ticks;
	sc->mt.arg = sc;
	mdx_callout_register(&sc->mt);

	return (0);
}
