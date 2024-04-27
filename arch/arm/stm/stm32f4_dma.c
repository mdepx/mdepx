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

#include "stm32f4_dma.h"

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
stm32f4_dma_intr(void *arg, struct stm32f4_dma_conf *conf)
{
	struct stm32f4_dma_softc *sc;
	int pending;
	int sid;

	sc = arg;

	sid = conf->sid;

	pending = RD4(sc, DMA_ISR(sid));

	dprintf("%s: %x, sid %d, TCIF(sid) %x, DMA_SM0AR(sid) %x\n",
	    pending, sid, TCIF(sid), RD4(sc, DMA_SM0AR(sid)));

	if (pending & HTIF(sid)) {
		if (conf->user_half_done)
			conf->user_half_done(conf->arg);
		WR4(sc, DMA_IFCR(sid), HTIF(sid));
	}

	if (pending & TCIF(sid)) {
		if (conf->user_done)
			conf->user_done(conf->arg);
		WR4(sc, DMA_IFCR(sid), TCIF(sid));
	}

	if (pending & FEIF(sid)) {
		printf("FIFO error (sid %d)\n", sid);
		WR4(sc, DMA_IFCR(sid), FEIF(sid));
	}
}

int
stm32f4_dma_curtarget(struct stm32f4_dma_softc *sc, int sid)
{
	uint32_t reg;

	reg = RD4(sc, DMA_SCR(sid));
	if (reg & SCR_CT)
		return (1);

	return (0);
}

void
stm32f4_dma_control(struct stm32f4_dma_softc *sc, uint32_t sid, bool enable)
{
	uint32_t reg;

	reg = RD4(sc, DMA_SCR(sid));
	if (enable)
		reg |= SCR_EN;
	else
		reg &= ~SCR_EN;
	WR4(sc, DMA_SCR(sid), reg);
}

void
stm32f4_dma_set_mem(struct stm32f4_dma_softc *sc, uint32_t sid, uint32_t target,
    uint32_t ptr)
{
	uint32_t reg;

	reg = target == 0 ? DMA_SM0AR(sid) : DMA_SM1AR(sid);

	WR4(sc, reg, ptr);
}

void
stm32f4_dma_setup(struct stm32f4_dma_softc *sc, struct stm32f4_dma_conf *conf)
{
	uint32_t reg;
	int sid;

	sid = conf->sid;

	WR4(sc, DMA_SCR(sid), 0);
	WR4(sc, DMA_IFCR(sid), 0xffffffff);
	WR4(sc, DMA_SM0AR(sid), conf->mem0);
	if (conf->mem1 != 0)
		WR4(sc, DMA_SM1AR(sid), conf->mem1);

	reg = 0;
	if (conf->fifo_mode) {
		reg |= SFCR_DMDIS;
		reg |= (SFCR_FTH_FULL << SFCR_FTH_S);
	}
	WR4(sc, DMA_SFCR(sid), reg);
	WR4(sc, DMA_SPAR(sid), conf->periph_addr);

	reg = (conf->channel << SCR_CHSEL_S);
	if (conf->dir == 1)
		reg |= (SCR_DIR_M2P << SCR_DIR_S);
	else
		reg |= (SCR_DIR_P2M << SCR_DIR_S);

	/* Interrupts. */
	reg |= SCR_TEIE | SCR_DMEIE;
	if (conf->user_half_done)
		reg |= SCR_HTIE;
	if (conf->user_done)
		reg |= SCR_TCIE;

	if (conf->periph_control)
		reg |= (SCR_PFCTRL);
	reg |= (SCR_MINC);
	if (conf->circ)
		reg |= (SCR_CIRC);
	if (conf->mem1 != 0)
		reg |= (SCR_DBM);
	switch(conf->psize) {
	case 32:
		reg |= (MSIZE_32 << SCR_MSIZE_S);
		reg |= (PSIZE_32 << SCR_PSIZE_S);
		break;
	case 16:
		reg |= (MSIZE_16 << SCR_MSIZE_S);
		reg |= (PSIZE_16 << SCR_PSIZE_S);
		break;
	case 8:
		reg |= (MSIZE_8 << SCR_MSIZE_S);
		reg |= (PSIZE_8 << SCR_PSIZE_S);
		break;
	default:
		break;
	}
	if (conf->burst) {
		reg |= (MBURST_INCR4 << SCR_MBURST_S);
		reg |= (PBURST_INCR4 << SCR_PBURST_S);
	}
	WR4(sc, DMA_SCR(sid), reg);

	if (!conf->periph_control) {
		/* Number of transfers */
		switch (conf->psize) {
		case 32:
			WR4(sc, DMA_SNDTR(sid), (conf->nbytes / 4));
			break;
		case 16:
			WR4(sc, DMA_SNDTR(sid), (conf->nbytes / 2));
			break;
		case 8:
			WR4(sc, DMA_SNDTR(sid), (conf->nbytes));
			break;
		default:
			break;
		}
	}
}

uint32_t
stm32f4_dma_getcnt(struct stm32f4_dma_softc *sc, int sid)
{
	uint32_t cnt;

	cnt = RD4(sc, DMA_SNDTR(sid));

	return (cnt);
}

void
stm32f4_dma_init(struct stm32f4_dma_softc *sc, uint32_t base)
{

	sc->base = base;
}
