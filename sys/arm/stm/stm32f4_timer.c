/*-
 * Copyright (c) 2018 Ruslan Bukin <br@bsdpad.com>
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
#include <machine/frame.h>
#include <arm/stm/stm32f4_timer.h>

#define	RD4(_sc, _reg)		*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
stm32f4_timer_intr(void *arg, struct trapframe *tf, int irq)
{
	struct stm32f4_timer_softc *sc;
	uint32_t reg;

	sc = arg;

	reg = RD4(sc, TIM_SR);
	reg &= ~SR_UIF;
	WR4(sc, TIM_SR, reg);

	reg = RD4(sc, TIM_DIER);
	reg &= ~DIER_UIE;
	WR4(sc, TIM_DIER, reg);
}

void
stm32f4_timer_setup(struct stm32f4_timer_softc *sc, uint32_t usec)
{
	uint32_t ticks;
	uint32_t reg;

	ticks = usec * (sc->freq / 1000000);

	WR4(sc, TIM_CNT, ticks);

	reg = RD4(sc, TIM_CR1);
	reg |= CR1_CEN;
	reg |= CR1_DIR;
	WR4(sc, TIM_CR1, reg);

	reg = RD4(sc, TIM_DIER);
	reg |= DIER_UIE;
	WR4(sc, TIM_DIER, reg);
}

void
stm32f4_timer_udelay(struct stm32f4_timer_softc *sc, uint32_t usec)
{

}

void
stm32f4_timer_usleep(struct stm32f4_timer_softc *sc, uint32_t usec)
{
	uint32_t primask;

	__asm __volatile( "mrs     %0, primask\n"
	    : "=&r" (primask) :: "memory");

	__asm __volatile("cpsid i");

	stm32f4_timer_setup(sc, usec);

	__asm __volatile("wfi");

	__asm __volatile( "msr     primask, %0\n"
	    :: "r" (primask) : "memory");
}

int
stm32f4_timer_init(struct stm32f4_timer_softc *sc,
    uint32_t base, uint32_t freq)
{

	sc->base = base;
	sc->freq = freq;

	return (0);
}
