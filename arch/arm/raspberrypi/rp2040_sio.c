/*-
 * Copyright (c) 2021 Ruslan Bukin <br@bsdpad.com>
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

#include <arm/raspberrypi/rp2040_sio.h>

#define	RP2040_SIO_DEBUG
#undef	RP2040_SIO_DEBUG

#ifdef	RP2040_SIO_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static int
rp2040_sio_ipi_rcvd(struct rp2040_sio_softc *sc)
{
	uint32_t data;
	int stat;

	stat = RD4(sc, RP2040_SIO_FIFO_ST);
	if (stat & (SIO_FIFO_ST_ROE | SIO_FIFO_ST_WOF))
		WR4(sc, RP2040_SIO_FIFO_ST, 0);

	if (RD4(sc, RP2040_SIO_FIFO_ST) & SIO_FIFO_ST_VLD) {
		data = RD4(sc, RP2040_SIO_FIFO_RD);
		return (data);
	}

	return (-1);
}

void
rp2040_sio_ipi(struct rp2040_sio_softc *sc, uint32_t msg)
{

	while (!(RD4(sc, RP2040_SIO_FIFO_ST) & SIO_FIFO_ST_RDY))
		cpu_nullop();
	WR4(sc, RP2040_SIO_FIFO_WR, msg);
}

void
rp2040_sio_intr(void *arg, int irq)
{
	struct rp2040_sio_softc *sc;

	sc = arg;

	rp2040_sio_ipi_rcvd(sc);
}

/*
 * Needed to boot secondary core on rp2040 only.
 */
void
rp2040_sio_fifo_drain(struct rp2040_sio_softc *sc)
{

	WR4(sc, RP2040_SIO_FIFO_ST, 0);

	while (RD4(sc, RP2040_SIO_FIFO_ST) & SIO_FIFO_ST_VLD)
		RD4(sc, RP2040_SIO_FIFO_RD);

	__asm __volatile("sev");
}

/*
 * Needed to boot secondary core on rp2040 only.
 */
int
rp2040_sio_fifo_comm(struct rp2040_sio_softc *sc, uint32_t msg)
{
	uint32_t rcv;

	while (!(RD4(sc, RP2040_SIO_FIFO_ST) & SIO_FIFO_ST_RDY))
		;
	WR4(sc, RP2040_SIO_FIFO_WR, msg);

	__asm __volatile("sev");

	while (!(RD4(sc, RP2040_SIO_FIFO_ST) & SIO_FIFO_ST_VLD))
		__asm __volatile("wfe");

	rcv = RD4(sc, RP2040_SIO_FIFO_RD);

	return (msg == rcv);
}

void
rp2040_sio_init(struct rp2040_sio_softc *sc, uint32_t base)
{

	sc->base = base;
}
