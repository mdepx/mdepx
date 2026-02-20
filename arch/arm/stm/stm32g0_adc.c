/*-
 * Copyright (c) 2026 Ruslan Bukin <br@bsdpad.com>
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
#include <arm/stm/stm32g0_adc.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

int
stm32g0_adc_enable(struct stm32g0_adc_softc *sc)
{
	uint32_t reg;
	int timeout;

	/* Enable ADC. */
	WR4(sc, ADC_CR, CR_ADEN);

	/* Ensure we are ready. */
	timeout = 100;
	do {
		reg = RD4(sc, ADC_ISR);
		if (reg & ISR_ADRDY)
			break;
	} while (timeout--);

	if (timeout <= 0)
		return (-1);

	return (0);
}

int
stm32g0_adc_conv(struct stm32g0_adc_softc *sc, int chan, int *data)
{
	uint32_t reg;
	int timeout;

	/* Select channel. */
	WR4(sc, ADC_CHSELR, (1 << chan));

	/* Start conversion. */
	WR4(sc, ADC_CR, CR_ADSTART);

	/* Wait for conversion to complete. Typically 2 loops. */
	timeout = 100;
	do {
		reg = RD4(sc, ADC_CR);
		if ((reg & CR_ADSTART) == 0)
			break;
	} while (timeout--);

	if (timeout <= 0)
		return (-1);

	*data = RD4(sc, ADC_DR);

	return (0);
}

void
stm32g0_adc_init(struct stm32g0_adc_softc *sc, uint32_t base)
{

	sc->base = base;
}
