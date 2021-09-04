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
#include <sys/io.h>
#include <sys/of.h>

#include <machine/intr.h>
#include <machine/plic.h>
#include <dev/intc/intc.h>

#define	RD4(_sc, _reg)		\
	mdx_ioread_uint32((_sc)->base, _reg)
#define	RD8(_sc, _reg)		\
	mdx_ioread_uint64((_sc)->base, _reg)

#define	WR4(_sc, _reg, _val)	\
	mdx_iowrite_uint32((_sc)->base, _reg, _val)
#define	WR8(_sc, _reg, _val)	\
	mdx_iowrite_uint64((_sc)->base, _reg, _val)

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
		WR4(sc, reg, irq);
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
 * Low-level PLIC initialisation (on platforms without FDT support).
 */
void
plic_init(mdx_device_t dev, capability base, int cpu, int context)
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

static int
plic_setup_interrupts(struct plic_softc *sc)
{
	mdx_device_t dev;
	const char *prop;
	uint32_t hart;
	uint32_t reg;
	int cpu_phandle;
	int clic_node;
	int cpu_node;
	int context;
	int error;
	int nintr;
	int mode;
	int len;
	int i;

	dev = sc->dev;

	prop = mdx_of_dev_get_prop(dev, "interrupts-extended", &len);
	if (len <= 0)
		return (0);

	nintr = len / sizeof(int);

	for (i = 0, context = 0; i < nintr; i += 2, context++) {
		cpu_phandle = mdx_of_ld32(prop);
		prop += sizeof(int);

		mode = mdx_of_ld32(prop);
		prop += sizeof(int);

#ifdef MDX_RISCV_SUPERVISOR_MODE
		if (mode != IRQ_EXTERNAL_SUPERVISOR)
			continue;
#else
		if (mode != IRQ_EXTERNAL_MACHINE)
			continue;
#endif

		clic_node = mdx_of_node_offset_by_phandle(cpu_phandle);
		if (clic_node < 0)
			continue;

		cpu_node = mdx_of_parent_offset(clic_node);
		error = mdx_of_get_prop32(cpu_node, "reg", &hart, NULL);
		if (error != 0)
			continue;

		KASSERT(hart < MDX_CPU_MAX, ("CPU id is out of range"));

		sc->contexts[hart].enable_offset = PLIC_ENABLE_BASE +
		    PLIC_ENABLE_STRIDE * context;
		sc->contexts[hart].context_offset = PLIC_CONTEXT_BASE +
		    PLIC_CONTEXT_STRIDE * context;

		/* Accept all priorities. */
		reg = sc->contexts[hart].context_offset +
		    PLIC_CONTEXT_THRESHOLD;
		WR4(sc, reg, 0);
	}

	return (0);
}

#ifdef MDX_OF
static int
plic_probe(mdx_device_t dev)
{

	if (!mdx_of_is_compatible(dev, "riscv,plic0"))
		return (MDX_ERROR);

	return (MDX_OK);
}

static int
plic_attach(mdx_device_t dev)
{
	struct plic_softc *sc;
	size_t base;
	int error;

	sc = mdx_device_get_softc(dev);
	sc->dev = dev;

	plic_sc = sc;

	error = mdx_of_get_reg(dev, 0, &base, NULL);
	if (error)
		return (error);
	sc->base = (void *)base;

	plic_setup_interrupts(sc);

	dev->ops = &plic_intc_ops;

	return (0);
}

static struct mdx_device_ops plic_ops = {
	.probe = plic_probe,
	.attach = plic_attach,
};

static mdx_driver_t plic_driver = {
	"plic",
	&plic_ops,
	sizeof(struct plic_softc),
};

DRIVER_MODULE(plic, plic_driver);
#endif
