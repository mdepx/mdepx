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
#include <sys/thread.h>

#include <machine/frame.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#include <riscv/riscv/trap.h>
#include <riscv/include/intr.h>
#include <riscv/include/clic.h>

#include <dev/intc/intc.h>

#define	RD1(_sc, _reg)		\
	*(volatile uint8_t *)((_sc)->base + _reg)
#define	WR1(_sc, _reg, _val)	\
	*(volatile uint8_t *)((_sc)->base + _reg) = _val

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

struct clic_softc {
	size_t base;
};

static struct clic_softc *clic_sc;
static struct clic_intr_entry intr_map[MDX_RISCV_CLIC_NINTR];

void
clic_intr(int irq)
{

	KASSERT(irq < MDX_RISCV_CLIC_NINTR, ("can't handle irq"));

	if (intr_map[irq].handler != NULL)
		intr_map[irq].handler(intr_map[irq].arg, irq);
	else
		printf("stray interrupt %d\n", irq);
}

static int
clic_setup_intr(mdx_device_t dev, int irq,
    void (*handler) (void *arg, int irq),
    void *arg)
{

	if (irq >= MDX_RISCV_CLIC_NINTR)
		return (-1);

	intr_map[irq].handler = handler;
	intr_map[irq].arg = arg;

	return (0);
}

static void
clic_enable_intr(mdx_device_t dev, int n)
{
	struct clic_softc *sc;

	sc = mdx_device_get_softc(dev);

	WR1(sc, CLIC_INT_IE(n), 1);
}

static void
clic_disable_intr(mdx_device_t dev, int n)
{
	struct clic_softc *sc;

	sc = mdx_device_get_softc(dev);

	WR1(sc, CLIC_INT_IE(n), 0);
}

static void
clic_set_pending(mdx_device_t dev, int n)
{

	/* TODO */
}

static void
clic_clear_pending(mdx_device_t dev, int n)
{

	/* TODO */
}

static void
clic_set_prio(mdx_device_t dev, int n, int prio)
{

	/* TODO */
}

#ifdef MDX_OF
static int
clic_map(mdx_device_t dev, const void *regp, int ncells, int *irq)
{

	/* TODO */

	return (0);
}
#endif

static struct mdx_intc_ops clic_intc_ops = {
	.setup = clic_setup_intr,
	.enable = clic_enable_intr,
	.disable = clic_disable_intr,
	.set = clic_set_pending,
	.clear = clic_clear_pending,
	.set_prio = clic_set_prio,
#ifdef MDX_OF
	.map = clic_map,
#endif
};

void
clic_init(mdx_device_t dev, size_t base)
{
	struct clic_softc *sc;
	int i;

	sc = mdx_device_alloc_softc(dev, sizeof(struct clic_softc));
	sc->base = base;

	clic_sc = sc;
	dev->ops = &clic_intc_ops;

	WR4(sc, CLIC_CFG, 0);

	for (i = 0; i < CLIC_MAX_INTERRUPTS; i++)
		WR4(sc, CLIC_INT_IP(i), 0);
}
