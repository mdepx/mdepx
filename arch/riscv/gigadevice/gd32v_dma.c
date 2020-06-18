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
 * ARE DISCLAIMED.  IN NO DMA SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#include <dev/dma/dma.h>

#include "gd32v_dma.h"

#define	DMA_DEBUG
#undef	DMA_DEBUG

#ifdef	DMA_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
gd32v_dma_intr(void *arg, int irq)
{
	struct gd32v_dma_softc *sc;
	mdx_device_t dev;
	uint32_t pending;

	dev = arg;
	sc = mdx_device_get_softc(dev);

	pending = RD4(sc, DMA_INTF);

	dprintf("%s: %x\n", __func__, pending);

	WR4(sc, DMA_INTC, pending);

	mdx_sem_post(&sc->sem);
}

void
gd32v_dma_setup(mdx_device_t dev, int chan, struct dma_desc *desc)
{
	struct gd32v_dma_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	WR4(sc, DMA_CHCTL(chan), 0);

	mdx_sem_init(&sc->sem, 0);

	dprintf("dma: chan %d copying %d times %x -> %x\n",
	    chan, desc->count, desc->src_addr, desc->dst_addr);

	WR4(sc, DMA_CHMADDR(chan), desc->src_addr);
	WR4(sc, DMA_CHPADDR(chan), desc->dst_addr);

	reg = 0;

	switch (desc->src_width) {
	case 32:
		reg |= CHCTL_MWIDTH_32BIT;
		break;
	case 16:
		reg |= CHCTL_MWIDTH_16BIT;
		break;
	case 8:
		reg |= CHCTL_MWIDTH_8BIT;
		break;
	};

	switch (desc->dst_width) {
	case 32:
		reg |= CHCTL_PWIDTH_32BIT;
		break;
	case 16:
		reg |= CHCTL_PWIDTH_16BIT;
		break;
	case 8:
		reg |= CHCTL_PWIDTH_8BIT;
		break;
	};

	if (desc->src_inc)
		reg |= CHCTL_MNAGA;
	if (desc->dst_inc)
		reg |= CHCTL_PNAGA;

	switch (desc->direction) {
	case DMA_MEM_TO_MEM:
		reg |= CHCTL_M2M;
		break;
	case DMA_MEM_TO_DEV:
		reg |= CHCTL_DIR;
		break;
	case DMA_DEV_TO_MEM:
	case DMA_DEV_TO_DEV:
		break;
	};

	reg |= CHCTL_FTFIE | CHCTL_ERRIE;
	WR4(sc, DMA_CHCTL(chan), reg);
	WR4(sc, DMA_CHCNT(chan), desc->count);

	/* Enable the channel. */
	reg |= CHCTL_CHEN;
	WR4(sc, DMA_CHCTL(chan), reg);

	mdx_sem_wait(&sc->sem);
}

void
gd32v_dma_init(mdx_device_t dev, uint32_t base)
{
	struct gd32v_dma_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(struct gd32v_dma_softc));
	sc->base = base;
}
