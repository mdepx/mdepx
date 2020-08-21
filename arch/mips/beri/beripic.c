/*-
 * Copyright (c) 2018-2020 Ruslan Bukin <br@bsdpad.com>
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
 * ("CTSRD"), as part of the DARPA CRASH research programme.
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
#include <sys/endian.h>
#include <sys/io.h>

#include <machine/frame.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#include <mips/beri/beripic.h>

#define	BERIPIC_DEBUG
#undef	BERIPIC_DEBUG

#ifdef	BERIPIC_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	WR_CFG(_sc, _reg, _val)		\
    mdx_iowrite_uint64((_sc)->res->cfg, _reg, _val)
#define	WR_IP_SET(_sc, _reg, _val)	\
    mdx_iowrite_uint64((_sc)->res->ip_set, _reg, _val)
#define	WR_IP_CLEAR(_sc, _reg, _val)	\
    mdx_iowrite_uint64((_sc)->res->ip_clear, _reg, _val)
#define	RD_CFG(_sc, _reg)		\
    mdx_ioread_uint64((_sc)->res->cfg, _reg)
#define	RD_IP_READ(_sc, _reg)		\
    mdx_ioread_uint64((_sc)->res->ip_read, _reg)

void
beripic_intr(void *arg, struct trapframe *frame, int irq)
{
	struct beripic_softc *sc;
	uint32_t hard_irq;
	mdx_device_t dev;
	uint64_t reg;
	uint64_t intr;
	int i;

	dev = arg;

	sc = mdx_device_get_softc(dev);

	hard_irq = (irq - 2);

	intr = RD_IP_READ(sc, 0);

	dprintf("%s: intr %lx (hard_irq %d)\n", __func__, intr, hard_irq);

	for (i = 0; i < 32; i++) {
		if (intr & (1 << i)) {
			dprintf("%s: intr %d\n", __func__, i);

			reg = RD_CFG(sc, (i * 8));
			if ((reg & BP_CFG_IRQ_M) != hard_irq)
				continue;
			if ((reg & BP_CFG_ENABLE) == 0)
				continue;

			if (sc->map[i].handler != NULL)
				sc->map[i].handler(sc->map[i].arg);

			WR_IP_CLEAR(sc, 0, (1 << i));
		}
	}
}

void
beripic_install_intr_map(mdx_device_t dev,
    const struct beripic_intr_entry *map)
{
	struct beripic_softc *sc;

	sc = mdx_device_get_softc(dev);

	sc->map = map;
}

void
beripic_disable(mdx_device_t dev, uint32_t beripic_irq)
{
	struct beripic_softc *sc;
	int tid;

	sc = mdx_device_get_softc(dev);

	tid = 0; /* thread ID */

	WR_CFG(sc, (beripic_irq + tid) * 8, 0);

	WR_IP_CLEAR(sc, 0, (1 << beripic_irq));
}

void
beripic_enable(mdx_device_t dev, uint32_t beripic_irq, uint32_t hard_irq)
{
	struct beripic_softc *sc;
	uint64_t reg;
	int tid;

	sc = mdx_device_get_softc(dev);

	tid = 0; /* thread ID */

	printf("%s: enabling IRQ %d\n", __func__, beripic_irq);

	WR_IP_CLEAR(sc, 0, (1 << beripic_irq));

	reg = BP_CFG_ENABLE;
	reg |= (hard_irq << BP_CFG_IRQ_S);
	reg |= (tid << BP_CFG_TID_S);
	WR_CFG(sc, (beripic_irq + tid) * 8, reg);
}

void
beripic_ip_set(mdx_device_t dev, uint32_t irq)
{
	struct beripic_softc *sc;

	sc = mdx_device_get_softc(dev);

	WR_IP_SET(sc, 0, (1 << irq));
}

void
beripic_ip_clear(mdx_device_t dev, uint32_t irq)
{
	struct beripic_softc *sc;

	sc = mdx_device_get_softc(dev);

	WR_IP_CLEAR(sc, 0, (1 << irq));
}

int
beripic_init(mdx_device_t dev, struct beripic_resource *res)
{
	struct beripic_softc *sc;
	int i;

	sc = mdx_device_get_softc(dev);
	sc->res = res;

	for (i = 0; i < BERIPIC_NIRQS; i++)
		WR_CFG(sc, i * 8, 0);

	return (0);
}
