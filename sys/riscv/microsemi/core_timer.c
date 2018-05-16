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

#include <sys/types.h>

#include <riscv/microsemi/core_timer.h>

#define	RD4(_sc, _reg)		*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
core_timer_udelay(struct core_timer_softc *sc,
    uint32_t usec, uint32_t osc_freq)
{
	uint32_t first, last;
	uint32_t delta;
	uint32_t counts;

	counts = 0;
	first = core_timer_value(sc);
	while (counts < usec) {
		last = core_timer_value(sc);
		if (last < first)
			delta = first - last;
		else
			delta = first + (sc->load_val - last) + 1;

		counts += (delta * (1000000 / osc_freq));

		first = last;
	}
}

uint32_t
core_timer_value(struct core_timer_softc *sc)
{
	uint32_t reg;

	reg = RD4(sc, TIMER_VALUE);

	return (reg);
}

int
core_timer_init(struct core_timer_softc *sc, uint32_t base,
    uint32_t load_val, uint8_t prescaler)
{

	sc->base = base;
	sc->load_val = load_val;

	WR4(sc, TIMER_CONTROL, 0);
	WR4(sc, TIMER_LOAD, sc->load_val);
	WR4(sc, TIMER_PRESCALER, prescaler);
	WR4(sc, TIMER_CONTROL, (CONTROL_ENABLE));

	return (0);
}
