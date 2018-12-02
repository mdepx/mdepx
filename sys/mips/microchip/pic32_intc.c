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
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#include <mips/microchip/pic32_intc.h>

#define	INTC_DEBUG
#undef	INTC_DEBUG

#ifdef	INTC_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	RD4(_sc, _reg)		*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
pic32_intc_intr(void *arg, struct trapframe *frame, int irq)
{
	struct pic32_intc_softc *sc;
	uint32_t ifs;
	uint32_t ipend;
	int i, b;

	sc = (struct pic32_intc_softc *)arg;

	dprintf("hard intr %d\n", irq);

	for (i = 0; i < 6; i++) {
		ifs = RD4(sc, INTC_IFS(i));
		for (b = 0; b < 31; b++) {
			if (ifs & (1 << b)) {
				ipend = (i * 32 + b);
				if (sc->map[ipend].handler != NULL) {
					sc->map[ipend].handler(sc->map[ipend].arg,
					    frame, irq, ipend);
					dprintf("intr %d\n", ipend);
					pic32_intc_clear_pending(sc, ipend);
				}
			}
		}
	}
}

void
pic32_intc_init(struct pic32_intc_softc *sc, uint32_t base)
{

	sc->base = base;

	WR4(sc, INTC_INTCON, 0);
}

void
pic32_intc_install_intr_map(struct pic32_intc_softc *sc,
    const struct intc_intr_entry *map)
{

	sc->map = map;
}

void
pic32_intc_enable(struct pic32_intc_softc *sc, int irq, uint8_t prio)
{
	uint32_t reg;
	uint8_t prio_shift;

	prio_shift = 2 + 8 * (irq % 4);

	reg = RD4(sc, INTC_IEC(irq / 32));
	reg |= (1 << (irq % 32));
	WR4(sc, INTC_IEC(irq / 32), reg);

	reg = RD4(sc, INTC_IPC(irq / 4));
	reg |= (prio << prio_shift);
	WR4(sc, INTC_IPC(irq / 4), reg);
}

void
pic32_intc_clear_pending(struct pic32_intc_softc *sc, int irq)
{
	uint32_t reg;

	reg = RD4(sc, INTC_IFS(irq / 32));
	reg &= ~(1 << (irq % 32));
	WR4(sc, INTC_IFS(irq / 32), reg);
}
