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
#include <sys/of.h>

#include <dev/intc/intc.h>

#include <arm/arm/nvic.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static struct nvic_intr_entry intr_map[MDX_ARM_NVIC_NINTRS];

void
arm_nvic_intr(uint32_t irq)
{

	if (intr_map[irq].handler != NULL)
		intr_map[irq].handler(intr_map[irq].arg, irq);
	else
		printf("%s: spurious intr %d\n", __func__, irq);
}

static int
arm_nvic_setup_intr(mdx_device_t dev, int irq,
    void (*handler) (void *arg, int irq),
    void *arg)
{

	if (irq >= MDX_ARM_NVIC_NINTRS)
		return (-1);

	intr_map[irq].handler = handler;
	intr_map[irq].arg = arg;

	return (0);
}

static void
arm_nvic_enable_intr(mdx_device_t dev, int n)
{
	struct arm_nvic_softc *sc;

	sc = mdx_device_get_softc(dev);

	WR4(sc, NVIC_ISER(n / 32), (1 << (n % 32)));
}

static void
arm_nvic_disable_intr(mdx_device_t dev, int n)
{
	struct arm_nvic_softc *sc;

	sc = mdx_device_get_softc(dev);

	WR4(sc, NVIC_ICER(n / 32), (1 << (n % 32)));
}

static void
arm_nvic_set_pending(mdx_device_t dev, int n)
{
	struct arm_nvic_softc *sc;

	sc = mdx_device_get_softc(dev);

	WR4(sc, NVIC_ISPR(n / 32), (1 << (n % 32)));
}

static void
arm_nvic_clear_pending(mdx_device_t dev, int n)
{
	struct arm_nvic_softc *sc;

	sc = mdx_device_get_softc(dev);

	WR4(sc, NVIC_ICPR(n / 32), (1 << (n % 32)));
}

static void
arm_nvic_set_prio(mdx_device_t dev, int n, int prio)
{
	struct arm_nvic_softc *sc;
	int reg;

	sc = mdx_device_get_softc(dev);

	reg = RD4(sc, NVIC_IPR(n / 4));
	reg &= ~(0xff << ((n % 4) * 8));
	reg |= (prio << ((n % 4) * 8));
	WR4(sc, NVIC_IPR(n / 4), reg);
}

#ifdef MDX_OF
static int
arm_nvic_map(mdx_device_t dev, const void *regp, int ncells, int *irq)
{

	*irq = mdx_of_ld32(regp);

	return (0);
}
#endif

void
arm_nvic_target_ns(mdx_device_t dev, uint32_t n, int secure)
{
	struct arm_nvic_softc *sc;
	int reg;

	sc = mdx_device_get_softc(dev);

	reg = RD4(sc, NVIC_ITNS(n / 32));

	if (secure)
		reg &= ~(1 << (n % 32));
	else
		reg |= (1 << (n % 32));

	WR4(sc, NVIC_ITNS(n / 32), reg);
}

static struct mdx_intc_ops arm_nvic_intc_ops = {
	.setup = arm_nvic_setup_intr,
	.enable = arm_nvic_enable_intr,
	.disable = arm_nvic_disable_intr,
	.set = arm_nvic_set_pending,
	.clear = arm_nvic_clear_pending,
	.set_prio = arm_nvic_set_prio,
#ifdef MDX_OF
	.map = arm_nvic_map,
#endif
};

int
arm_nvic_init(mdx_device_t dev, uint32_t base)
{
	struct arm_nvic_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(*sc));
	sc->base = base;

	dev->ops = &arm_nvic_intc_ops;

	return (0);
}

#ifdef MDX_OF
static int
arm_nvic_probe(mdx_device_t dev)
{

	if (!mdx_of_is_compatible(dev, "arm,v8m-nvic"))
		return (MDX_ERROR);

	return (MDX_OK);
}

static int
arm_nvic_attach(mdx_device_t dev)
{
	struct arm_nvic_softc *sc;
	int error;

	sc = mdx_device_get_softc(dev);

	error = mdx_of_get_reg(dev, 0, &sc->base, NULL);
	if (error)
		return (error);

	dev->ops = &arm_nvic_intc_ops;

	return (0);
}

static struct mdx_device_ops arm_nvic_ops = {
	.probe = arm_nvic_probe,
	.attach = arm_nvic_attach,
};

static mdx_driver_t nvic_driver = {
	"nvic",
	&arm_nvic_ops,
	sizeof(struct arm_nvic_softc),
};

DRIVER_MODULE(nvic, nvic_driver);
#endif
