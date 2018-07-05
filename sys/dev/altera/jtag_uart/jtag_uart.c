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
#include <sys/endian.h>

#include <dev/altera/jtag_uart/jtag_uart.h>

#include <machine/cpuregs.h>

#define	MIPS_XKPHYS_UNCACHED_BASE	0x9000000000000000ULL

#define	RD4(_sc, _reg)		*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	*(volatile uint32_t *)((_sc)->base + _reg) = _val
#define	RD2(_sc, _reg)		*(volatile uint16_t *)((_sc)->base + _reg)
#define	WR2(_sc, _reg, _val)	*(volatile uint16_t *)((_sc)->base + _reg) = _val
#define	RD1(_sc, _reg)		*(volatile uint8_t *)((_sc)->base + _reg)
#define	WR1(_sc, _reg, _val)	*(volatile uint8_t *)((_sc)->base + _reg) = _val

void
aju_putc(struct aju_softc *sc, int c)
{
	uint32_t reg;

	do {
		reg = RD4(sc, AJU_CONTROL);
	} while ((le32toh(reg) & AJU_CONTROL_WSPACE_M) == 0);

	WR4(sc, AJU_DATA, htole32(c));
}

int
aju_init(struct aju_softc *sc, uint32_t base)
{

	sc->base = base | MIPS_XKPHYS_UNCACHED_BASE;

	return (0);
}
