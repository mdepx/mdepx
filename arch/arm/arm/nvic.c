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

#include <arm/arm/nvic.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static struct nvic_intr_entry intr_map[MDX_ARM_NVIC_NINTRS];

int
arm_nvic_setup_intr(struct arm_nvic_softc *sc, int irq,
    void (*handler) (void *arg, struct trapframe *frame, int irq),
    void *arg)
{

	if (irq >= MDX_ARM_NVIC_NINTRS)
		return (-1);

	intr_map[irq].handler = handler;
	intr_map[irq].arg = arg;

	return (0);
}

void
arm_nvic_intr(uint32_t irq, struct trapframe *frame)
{

	if (intr_map[irq].handler != NULL)
		intr_map[irq].handler(intr_map[irq].arg, frame, irq);
	else
		printf("%s: spurious intr %d\n", __func__, irq);
}

void
arm_nvic_set_prio(struct arm_nvic_softc *sc, uint32_t n,
    int prio)
{
	int reg;

	reg = RD4(sc, NVIC_IPR(n / 4));
	reg &= ~(0xff << ((n % 4) * 8));
	reg |= (prio << ((n % 4) * 8));
	WR4(sc, NVIC_IPR(n / 4), reg);
}

void
arm_nvic_enable_intr(struct arm_nvic_softc *sc, uint32_t n)
{

	WR4(sc, NVIC_ISER(n / 32), (1 << (n % 32)));
}

void
arm_nvic_disable_intr(struct arm_nvic_softc *sc, uint32_t n)
{

	WR4(sc, NVIC_ICER(n / 32), (1 << (n % 32)));
}

void
arm_nvic_set_pending(struct arm_nvic_softc *sc, uint32_t n)
{

	WR4(sc, NVIC_ISPR(n / 32), (1 << (n % 32)));
}

void
arm_nvic_clear_pending(struct arm_nvic_softc *sc, uint32_t n)
{

	WR4(sc, NVIC_ICPR(n / 32), (1 << (n % 32)));
}

void
arm_nvic_target_ns(struct arm_nvic_softc *sc, uint32_t n,
    int secure)
{
	int reg;

	reg = RD4(sc, NVIC_ITNS(n / 32));

	if (secure)
		reg &= ~(1 << (n % 32));
	else
		reg |= (1 << (n % 32));

	WR4(sc, NVIC_ITNS(n / 32), reg);
}

int
arm_nvic_init(struct arm_nvic_softc *sc, uint32_t base)
{

	sc->base = base;

	return (0);
}
