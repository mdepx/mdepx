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

#include <dev/i2c/i2c.h>

#include <machine/frame.h>

#include "nrf_twim.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

#define	TWIM_TIMEOUT	10000	/* usec */

void
nrf_twim_intr(void *arg, struct trapframe *tf, int irq)
{
	struct nrf_twim_softc *sc;

	sc = arg;

	if (RD4(sc, TWIM_EVENTS_LASTTX)) {
		WR4(sc, TWIM_EVENTS_LASTTX, 0);
		mdx_sem_post(&sc->sem_tx);
	}

	if (RD4(sc, TWIM_EVENTS_LASTRX)) {
		WR4(sc, TWIM_EVENTS_LASTRX, 0);
		mdx_sem_post(&sc->sem_rx);
	}

	if (RD4(sc, TWIM_EVENTS_STOPPED)) {
		WR4(sc, TWIM_EVENTS_STOPPED, 0);
		mdx_sem_post(&sc->sem_stop);
	}
}

int
nrf_twim_xfer(void *arg, struct i2c_msg *msgs, int len)
{
	struct nrf_twim_softc *sc;
	struct i2c_msg *msg;
	int err;
	int i;

	sc = arg;

	/* TODO: handle errors. */

	for (i = 0; i < len; i++) {
		msg = &msgs[i];

		WR4(sc, TWIM_ADDRESS, msg->slave);

		if (msg->flags & IIC_M_RD) {
			WR4(sc, TWIM_RXD_MAXCNT, msg->len);
			WR4(sc, TWIM_RXD_PTR, (uint32_t)msg->buf);

			mdx_sem_init(&sc->sem_rx, 0);
			WR4(sc, TWIM_TASKS_STARTRX, 1);
			err = mdx_sem_timedwait(&sc->sem_rx, TWIM_TIMEOUT);
			if (err == 0)
				return (-1);

		} else {
			WR4(sc, TWIM_TXD_MAXCNT, msg->len);
			WR4(sc, TWIM_TXD_PTR, (uint32_t)msg->buf);

			mdx_sem_init(&sc->sem_tx, 0);
			WR4(sc, TWIM_TASKS_STARTTX, 1);
			err = mdx_sem_timedwait(&sc->sem_tx, TWIM_TIMEOUT);
			if (err == 0)
				return (-1);
		}

		if ((msg->flags & IIC_M_NOSTOP) == 0) {
			mdx_sem_init(&sc->sem_stop, 0);
			WR4(sc, TWIM_TASKS_STOP, 1);
			err = mdx_sem_timedwait(&sc->sem_stop, TWIM_TIMEOUT);
			if (err == 0)
				return (-1);
		}
	}

	return (0);
}

void
nrf_twim_setup(struct nrf_twim_softc *sc, struct nrf_twim_conf *conf)
{

	WR4(sc, TWIM_FREQUENCY, conf->freq);
	WR4(sc, TWIM_PSEL_SCL, conf->pin_scl);
	WR4(sc, TWIM_PSEL_SDA, conf->pin_sda);
	WR4(sc, TWIM_INTEN, INTEN_LASTTX | INTEN_LASTRX | INTEN_STOPPED);
	WR4(sc, TWIM_ENABLE, TWIM_ENABLE_EN);
}

void
nrf_twim_init(struct nrf_twim_softc *sc, uint32_t base)
{

	sc->base = base;
}
