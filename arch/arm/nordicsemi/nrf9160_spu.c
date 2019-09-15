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

#include "nrf9160.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
spu_periph_set_attr(struct spu_softc *sc, int periph_id,
    int secure_attr, int secure_dma)
{
	int reg;

	reg = RD4(sc, SPU_PERIPHID_PERM(periph_id));
	if ((reg & PERIPHID_PRESENT) == 0)
		return;

	if ((reg & PERIPHID_SECMAP_M) != PERIPHID_SECMAP_USER_SELECT &&
	    (reg & PERIPHID_SECMAP_M) != PERIPHID_SECMAP_SPLIT)
		return;

	if (secure_attr)
		reg |= PERIPHID_SECATTR;
	else
		reg &= ~PERIPHID_SECATTR;
	if (secure_dma)
		reg |= PERIPHID_DMASEC;
	else
		reg &= ~PERIPHID_DMASEC;
	WR4(sc, SPU_PERIPHID_PERM(periph_id), reg);
}

void
spu_flash_set_perm(struct spu_softc *sc, int region_id,
    int secure)
{
	int reg;

	reg = PERM_EXECUTE | PERM_WRITE | PERM_READ;

	if (secure)
		reg |= PERM_SECATTR;

	reg |= PERM_LOCK;

	WR4(sc, SPU_FLASHREGION_PERM(region_id), reg);
}

void
spu_sram_set_perm(struct spu_softc *sc, int region_id,
    int secure)
{
	int reg;

	reg = PERM_EXECUTE | PERM_WRITE | PERM_READ;

	if (secure)
		reg |= PERM_SECATTR;

	reg |= PERM_LOCK;

	WR4(sc, SPU_RAMREGION_PERM(region_id), reg);
}

void
spu_gpio_set_perm(struct spu_softc *sc, int region_id,
    int perm)
{

	WR4(sc, SPU_GPIOPORT_PERM(region_id), perm);
}

void
spu_init(struct spu_softc *sc, uint32_t base)
{

	sc->base = base;
}
