/*-
 * Copyright (c) 2023 Ruslan Bukin <br@bsdpad.com>
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

#include <arm/stm/stm32f4_tim.h>
#include <arm/stm/stm32f4_pwm.h>

#define	PWM_DEBUG
#undef	PWM_DEBUG

#ifdef	PWM_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
stm32f4_pwm_intr(void *arg, int irq)
{
	struct stm32f4_pwm_softc *sc;

	sc = arg;

	WR4(sc, TIM_SR, 0);
}

void
stm32f4_pwm_init(struct stm32f4_pwm_softc *sc,
    uint32_t base, uint32_t freq)
{

	sc->base = base;
	sc->freq = freq;
}

int
stm32f4_pwm_step(struct stm32f4_pwm_softc *sc, int chanset, uint32_t freq)
{
	uint32_t reg;
	uint32_t psc;
	int ch;

	WR4(sc, TIM_CR1, 0);

	/* Calculate prescaler value. */
	psc = (sc->freq / freq) - 1;

	if (chanset == 0)
		return (-1);

	for (ch = 0; ch < 4; ch++) {
		if ((chanset & (1 << ch)) == 0)
			continue;

		switch (ch) {
		case 0:
			reg = CCMR1_OC1M_PWM2 | CCMR1_OC1PE;
			WR4(sc, TIM_CCMR1, reg);
			WR4(sc, TIM_CCER, CCER_CC1E);
			break;
		case 1:
			reg = CCMR1_OC2M_PWM2 | CCMR1_OC2PE;
			WR4(sc, TIM_CCMR1, reg);
			WR4(sc, TIM_CCER, CCER_CC2E);
			break;
		case 2:
			reg = CCMR2_OC3M_PWM2 | CCMR2_OC3PE;
			WR4(sc, TIM_CCMR2, reg);
			WR4(sc, TIM_CCER, CCER_CC3E);
			break;
		case 3:
			reg = CCMR2_OC4M_PWM2 | CCMR2_OC4PE;
			WR4(sc, TIM_CCMR2, reg);
			WR4(sc, TIM_CCER, CCER_CC4E);
			break;
		default:
			panic("unknown channel");
		};
	}

	/*
	 * Formula:
	 * Timer freq = (Input clock / (TIM_PSC + 1)))
	 * TIM_ARR / freq = period
	 * TIM_CCR1 / freq = duty cycle
	 * period > duty
	 * sc->freq = input clock
	 */

	WR4(sc, TIM_PSC, psc);
	WR4(sc, TIM_CCR1, 300); /* duty cycle */
	WR4(sc, TIM_ARR, 400); /* period */

	WR4(sc, TIM_SR, 0);
	WR4(sc, TIM_DIER, DIER_UIE);
	WR4(sc, TIM_CR1, (CR1_OPM | CR1_CEN));

	return (0);
}
