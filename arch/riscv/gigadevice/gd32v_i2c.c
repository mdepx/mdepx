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
#include <sys/mutex.h>
#include <sys/sem.h>

#include <dev/i2c/i2c.h>

#include "gd32v_i2c.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
gd32v_i2c_event_intr(void *arg, int irq)
{
	struct gd32v_i2c_softc *sc;
	struct i2c_msg *msg;
	mdx_device_t dev;
	int pending0;
	int pending1;
	uint32_t reg;

	dev = arg;
	sc = mdx_device_get_softc(dev);

	pending0 = RD4(sc, I2C_STAT0);
	pending1 = RD4(sc, I2C_STAT1);

	msg = sc->curmsg;

	if (pending1 == 0) {
		if (pending0 == 0) {
			/* Not sure why we are here. */
			return;
		}
		/*
		 * I2C bus is not active, but fifo has entries.
		 * Drain it.
		 */
		while (RD4(sc, I2C_STAT0) & STAT0_RBNE)
			RD4(sc, I2C_DATA);
		mdx_sem_post(&sc->sem);
		return;
	}

	if (pending0 & STAT0_SBSEND) {
		/* Send i2c address. */
		if (msg->flags & IIC_M_RD)
			WR4(sc, I2C_DATA, (msg->slave << 1 | 1));
		else
			WR4(sc, I2C_DATA, (msg->slave << 1 | 0));
		return;
	}

	if (msg->len == 0)
		return;

	if (msg->flags & IIC_M_RD) {
		if (pending0 & STAT0_RBNE) {
			while (RD4(sc, I2C_STAT0) & STAT0_RBNE) {
				if (msg->len == 1) {
					reg = RD4(sc, I2C_CTL0);
					reg &= ~CTL0_ACKEN;
					WR4(sc, I2C_CTL0, reg);

					reg = RD4(sc, I2C_CTL0);
					reg |= CTL0_STOP;
					WR4(sc, I2C_CTL0, reg);
				}

				msg->buf[sc->bufpos++] = RD4(sc, I2C_DATA);
				msg->len -= 1;
				if (msg->len == 0)
					break;
			}
		}
	} else {
		if (pending0 & STAT0_TBE) {
			WR4(sc, I2C_DATA, msg->buf[sc->bufpos++]);
			msg->len -= 1;
			if (msg->len == 0) {
				reg = RD4(sc, I2C_CTL0);
				reg |= CTL0_STOP;
				WR4(sc, I2C_CTL0, reg);
				mdx_sem_post(&sc->sem);
			}
		}
	}
}

void
gd32v_i2c_error_intr(void *arg, int irq)
{

	printf("%s\n", __func__);
}

static int
gd32v_i2c_xfer(mdx_device_t dev, struct i2c_msg *msgs, int len)
{
	struct gd32v_i2c_softc *sc;
	struct i2c_msg *msg;
	uint32_t reg;
	int error;
	int i;

	sc = mdx_device_get_softc(dev);

	for (i = 0; i < len; i++) {
		msg = &msgs[i];

		sc->curmsg = msg;

		mdx_sem_init(&sc->sem, 0);
		sc->bufpos = 0;

		/* Send start condition. */
		reg = RD4(sc, I2C_CTL0);
		reg |= CTL0_ACKEN;
		reg |= CTL0_START;
		WR4(sc, I2C_CTL0, reg);

		error = mdx_sem_timedwait(&sc->sem, 1000000);
		if (error == 0)
			return (-1);
	}

	return (0);
}

static struct mdx_i2c_ops gd32v_i2c_ops = {
	.xfer = gd32v_i2c_xfer,
};

int
gd32v_i2c_init(mdx_device_t dev, uint32_t base)
{
	struct gd32v_i2c_softc *sc;
	uint32_t reg;

	sc = mdx_device_alloc_softc(dev, sizeof(struct gd32v_i2c_softc));
	sc->base = base;

	dev->ops = (void *)&gd32v_i2c_ops;

#if 1
	/* TODO: remove this */
	WR4(sc, I2C_RT, 4);
	WR4(sc, I2C_CKCFG, 30);

	reg = 25 << CTL1_I2CCLK_S;
	reg |= CTL1_BUFIE | CTL1_EVIE | CTL1_ERRIE;
	WR4(sc, I2C_CTL1, reg);
	WR4(sc, I2C_CTL0, CTL0_I2CEN | CTL0_ACKEN);
#endif

	return (0);
}
