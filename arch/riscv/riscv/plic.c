/*-
 * Copyright (c) 2021 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/pcpu.h>

#include <machine/plic.h>
#include <dev/intc/intc.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static struct plic_softc *plic_sc;
static struct plic_intr_entry intr_map[MDX_RISCV_PLIC_NINTR];

void
plic_intr(void)
{
	struct plic_softc *sc;
	uint32_t reg;
	int irq;
	int hart;

	sc = plic_sc;

	hart = PCPU_GET(cpuid);

	reg = sc->contexts[hart].context_offset + PLIC_CONTEXT_CLAIM;

	irq = RD4(sc, reg);
	if (irq) {
		KASSERT(irq < MDX_RISCV_PLIC_NINTR, ("can't handle irq"));
		if (intr_map[irq].handler != NULL)
			intr_map[irq].handler(intr_map[irq].arg, irq);
		else
			printf("%s: stray interrupt %d\n", __func__, irq);
	}
}

static int
plic_setup_intr(mdx_device_t dev, int irq,
    void (*handler) (void *arg, int irq),
    void *arg)
{

	if (irq >= MDX_RISCV_PLIC_NINTR)
		return (-1);

	intr_map[irq].handler = handler;
	intr_map[irq].arg = arg;

	return (0);
}

static void
plic_enable_intr(mdx_device_t dev, int irq)
{
	struct plic_softc *sc;
	uint32_t reg;
	uint32_t val;
	int hart;

	sc = mdx_device_get_softc(dev);

	hart = PCPU_GET(cpuid);

	reg = sc->contexts[hart].enable_offset + (irq / 32) * sizeof(uint32_t);
	val = RD4(sc, reg);
	val |= (1 << (irq % 32));
	WR4(sc, reg, val);

	WR4(sc, PLIC_PRIORITY(irq), 1);
}

static void
plic_disable_intr(mdx_device_t dev, int irq)
{
	struct plic_softc *sc;

	sc = mdx_device_get_softc(dev);

	WR4(sc, PLIC_PRIORITY(irq), 0);
}

static void
plic_set_pending(mdx_device_t dev, int irq)
{

	/* TODO */
}

static void
plic_clear_pending(mdx_device_t dev, int irq)
{

	/* TODO */
}

static void
plic_set_prio(mdx_device_t dev, int irq, int prio)
{

	/* TODO */
}

#ifdef MDX_OF
static int
plic_map(mdx_device_t dev, const void *regp, int ncells, int *irq)
{

	/* TODO */

	return (0);
}
#endif

static struct mdx_intc_ops plic_intc_ops = {
	.setup = plic_setup_intr,
	.enable = plic_enable_intr,
	.disable = plic_disable_intr,
	.set = plic_set_pending,
	.clear = plic_clear_pending,
	.set_prio = plic_set_prio,
#ifdef MDX_OF
	.map = plic_map,
#endif
};

/*
 * TODO: need to map context to hart id (cpu) based on DTS file.
 */
void
plic_init(mdx_device_t dev, size_t base, int cpu, int context)
{
	struct plic_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);
	sc->base = base;

	dev->ops = &plic_intc_ops;
	plic_sc = sc;

	KASSERT(cpu < MDX_CPU_MAX, ("CPU id is out of range"));

	sc->contexts[cpu].enable_offset = PLIC_ENABLE_BASE +
	    PLIC_ENABLE_STRIDE * context;
	sc->contexts[cpu].context_offset = PLIC_CONTEXT_BASE +
	    PLIC_CONTEXT_STRIDE * context;

	/* Accept all priorities. */
	reg = sc->contexts[cpu].context_offset + PLIC_CONTEXT_THRESHOLD;
	WR4(sc, reg, 0);
}
