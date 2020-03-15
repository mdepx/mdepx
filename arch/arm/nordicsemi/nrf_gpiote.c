/*-
 * Copyright (c) 2020 Ruslan Bukin <br@bsdpad.com>
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

#include <machine/frame.h>

#include "nrf_gpiote.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
nrf_gpiote_intr(void *arg, struct trapframe *tf, int irq)
{
	struct nrf_gpiote_softc *sc;
	uint32_t reg;
	int i;

	sc = arg;

	for (i = 0; i < NRF_GPIOTE_NCONFIGS; i++) {
		reg = RD4(sc, GPIOTE_EVENTS_IN(i));
		if (reg) {
			if (sc->intr[i].handler != NULL)
				sc->intr[i].handler(sc->intr[i].arg, tf, irq);
			WR4(sc, GPIOTE_EVENTS_IN(i), 0);
		};
	};
}

int
nrf_gpiote_setup_intr(struct nrf_gpiote_softc *sc, int irq,
    void (*handler) (void *arg, struct trapframe *frame, int irq),
    void *arg)
{

	if (irq < 0)
		return (-1);

	if (irq >= NRF_GPIOTE_NCONFIGS)
		return (-1);

	sc->intr[irq].handler = handler;
	sc->intr[irq].arg = arg;

	return (0);
}

void
nrf_gpiote_intctl(struct nrf_gpiote_softc *sc, uint8_t config_id,
    bool enable)
{

	if (enable)
		WR4(sc, GPIOTE_INTENSET, (1 << config_id));
	else
		WR4(sc, GPIOTE_INTENCLR, (1 << config_id));
}

void
nrf_gpiote_config(struct nrf_gpiote_softc *sc, uint8_t config_id,
    struct nrf_gpiote_conf *conf)
{
	uint32_t reg;

	reg = CONFIG_PSEL(conf->pin);

	switch (conf->pol) {
	case GPIOTE_POLARITY_LOTOHI:
		reg |= CONFIG_POLARITY_LOTOHI;
		break;
	case GPIOTE_POLARITY_HITOLO:
		reg |= CONFIG_POLARITY_HITOLO;
		break;
	case GPIOTE_POLARITY_TOGGLE:
		reg |= CONFIG_POLARITY_TOGGLE;
		break;
	}

	switch (conf->mode) {
	case GPIOTE_MODE_TASK:
		reg |= CONFIG_MODE_TASK;
		break;
	case GPIOTE_MODE_EVENT:
		reg |= CONFIG_MODE_EVENT;
		break;
	}

	WR4(sc, GPIOTE_CONFIG(config_id), reg);
}

void
nrf_gpiote_init(struct nrf_gpiote_softc *sc, uint32_t base)
{

	sc->base = base;
}
