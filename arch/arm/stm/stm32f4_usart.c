/*-
 * Copyright (c) 2018-2023 Ruslan Bukin <br@bsdpad.com>
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
#include <arm/stm/stm32f4_usart.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
stm32f4_usart_intr(void *arg, int irq)
{
	struct stm32f4_usart_softc *sc;
	uint32_t pending;
	uint8_t ch;

	sc = arg;

	pending = RD4(sc, USART_SR);
	if (pending & USART_SR_RXNE) {
		ch = RD4(sc, USART_DR);
		if (sc->cb_configured)
			sc->cb(ch);
	}
}

void
stm32f4_usart_putc(struct stm32f4_usart_softc *sc, char c)
{

	while ((RD4(sc, USART_SR) & USART_SR_TXE) == 0)
		;

	WR4(sc, USART_DR, c);
}

int
stm32f4_usart_init(struct stm32f4_usart_softc *sc, uint32_t base,
    uint32_t cpu_freq, uint32_t baud_rate)
{
	uint32_t reg;

	sc->base = base;

	reg = (cpu_freq / baud_rate);
	WR4(sc, USART_BRR, reg);

	reg = RD4(sc, USART_CR1);
	reg |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);
	WR4(sc, USART_CR1, reg);

	return (0);
}

void
stm32f4_usart_setup_receiver(struct stm32f4_usart_softc *sc, int dma,
    void (*cb)(uint8_t ch))
{
	uint32_t reg;

	if (dma) {
		reg = CR3_DMAR;
		WR4(sc, USART_CR3, reg);
	} else {
		sc->cb = cb;
		sc->cb_configured = 1;

		/* Enable receive interrupts. */
		reg = RD4(sc, USART_CR1);
		reg |= (USART_CR1_RXNEIE);
		WR4(sc, USART_CR1, reg);
	}
}
