/*-
 * Copyright (c) 2023 Ruslan Bukin <br@bsdpad.com>
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

#include <arm/raspberrypi/rp2040_dma.h>
#include <arm/raspberrypi/rp2040_dma_regs.h>

#define	RP2040_PLL_DEBUG
#undef	RP2040_PLL_DEBUG

#ifdef	RP2040_PLL_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

int
rp2040_dma_init(mdx_device_t dev, uint32_t base)
{
	struct rp2040_dma_softc *sc;

	sc = mdx_device_get_softc(dev);
	sc->base = base;

	return (0);
}

void
rp2040_dma_channel_abort(mdx_device_t dev, int channel)
{
	struct rp2040_dma_softc *sc;
	uint32_t reg;
	int bit;

	sc = mdx_device_get_softc(dev);

	bit = 1 << channel;

	WR4(sc, RP2040_DMA_CHAN_ABORT_OFFSET, bit);

	do
		reg = RD4(sc, RP2040_DMA_CHAN_ABORT_OFFSET);
	while (reg & bit);
}

void
rp2040_dma_configure(mdx_device_t dev, int channel,
    struct rp2040_dma_channel_config *config)
{
	struct rp2040_dma_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	WR4(sc, RP2040_DMA_READ_ADDR_OFFSET, config->src_addr);
	WR4(sc, RP2040_DMA_WRITE_ADDR_OFFSET, config->dst_addr);
	WR4(sc, RP2040_DMA_TRANS_COUNT_OFFSET, config->count);
	reg = RP2040_DMA_CTRL_TRIG_READ_ERROR |
		RP2040_DMA_CTRL_TRIG_WRITE_ERROR |
		(channel << RP2040_DMA_CTRL_TRIG_CHAIN_TO_SHIFT) |
		(config->size << RP2040_DMA_CTRL_TRIG_DATA_SIZE_SHIFT);
	reg |= RP2040_DMA_CTRL_TRIG_INCR_READ;
	reg |= RP2040_DMA_CTRL_TRIG_INCR_WRITE;
	reg |= RP2040_DMA_CTRL_TRIG_EN;

	WR4(sc, RP2040_DMA_CTRL_TRIG_OFFSET + (channel * 0x40), reg);
}

int
rp2040_dma_channel_is_busy(mdx_device_t dev, int channel)
{
	struct rp2040_dma_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	reg = RD4(sc, RP2040_DMA_AL1_CTRL_OFFSET + (channel * 0x40));
	printf("%s: reg %x\n", __func__, reg);
	if (reg & RP2040_DMA_CTRL_TRIG_BUSY)
		return (1);

	return (0);
}
