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

#include <mips/microchip/pic32_port.h>

#define	RD4(_sc, _reg)		*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
pic32_port_ansel(struct pic32_port_softc *sc,
    uint32_t port, uint32_t pin, int digital)
{
	uint32_t reg;

	reg = RD4(sc, PORT_ANSEL(port));
	if (digital)
		reg &= ~(1 << pin);
	else /* analog */
		reg |= (1 << pin);
	WR4(sc, PORT_ANSEL(port), reg);
}

void
pic32_port_tris(struct pic32_port_softc *sc,
    uint32_t port, uint32_t pin, enum port_state st)
{
	uint32_t reg;

	reg = RD4(sc, PORT_TRIS(port));

	switch (st) {
	case PORT_INPUT:
		reg |= (1 << pin);
		break;
	case PORT_OUTPUT:
		reg &= ~(1 << pin);
		break;
	default:
		break;
	};

	WR4(sc, PORT_TRIS(port), reg);
}

void
pic32_port_lat(struct pic32_port_softc *sc,
    uint32_t port, uint32_t pin, uint8_t enable)
{
	uint32_t reg;

	reg = RD4(sc, PORT_LAT(port));
	if (enable)
		reg |= (1 << pin);
	else
		reg &= ~(1 << pin);
	WR4(sc, PORT_LAT(port), reg);
}

void
pic32_port_init(struct pic32_port_softc *sc,
    uint32_t base)
{

	sc->base = base;
}
