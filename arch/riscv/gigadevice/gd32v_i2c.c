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

#include "gd32v_i2c.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static int
gd32v_i2c_wait_bsy(struct gd32v_i2c_softc *sc)
{
	int timeout;

	timeout = 100;

	do
		if ((RD4(sc, I2C_STAT1) & STAT1_I2CBSY) == 0)
			break;
	while (timeout--);

	if (timeout <= 0) {
		printf("i2c bus is busy\n");
		return (0);
	}

	return (1);
}

static int
gd32v_i2c_wait_flag(struct gd32v_i2c_softc *sc, int flag)
{
	int timeout;

	timeout = 100;

	do
		if (RD4(sc, I2C_STAT0) & flag)
			break;
	while (timeout--);

	if (timeout <= 0) {
		printf("timeout\n");
		return (0);
	}

	return (1);
}

static int
gd32v_i2c_xfer(mdx_device_t dev, struct i2c_msg *msgs, int len)
{
	struct gd32v_i2c_softc *sc;
	struct i2c_msg *msg;
	uint32_t reg;
	int i;
	int j;

	sc = mdx_device_get_softc(dev);

	for (i = 0; i < len; i++) {
		msg = &msgs[i];

		/* Ensure i2c bus is not busy */
		if (gd32v_i2c_wait_bsy(sc) == 0)
			return (-1);

		/* Send start condition. */
		reg = RD4(sc, I2C_CTL0);
		reg |= CTL0_START;
		WR4(sc, I2C_CTL0, reg);

		/* Wait for the start condition to happen. */
		if (gd32v_i2c_wait_flag(sc, STAT0_SBSEND) == 0)
			return (-1);

		/* Send i2c address. */
		if (msg->flags & IIC_M_RD)
			WR4(sc, I2C_DATA, (msg->slave << 1 | 1));
		else
			WR4(sc, I2C_DATA, (msg->slave << 1 | 0));

		/* Wait until address sent. */
		if (gd32v_i2c_wait_flag(sc, STAT0_ADDSEND) == 0)
			return (-1);

		/* Clear ADDSEND by reading STAT1. */
		reg = RD4(sc, I2C_STAT1);

		if (msg->flags & IIC_M_RD) {
			for (j = 0; j < msg->len; j++) {
				if (j == (msg->len - 1)) {
					reg = RD4(sc, I2C_CTL0);
					reg &= ~CTL0_ACKEN;
					WR4(sc, I2C_CTL0, reg);

					reg = RD4(sc, I2C_CTL0);
					reg |= CTL0_STOP;
					WR4(sc, I2C_CTL0, reg);
				}
				/* Wait for the data. */
				while ((RD4(sc, I2C_STAT0) & STAT0_RBNE) == 0);

				msg->buf[j] = RD4(sc, I2C_DATA);
			}

			while (RD4(sc, I2C_STAT0) & STAT0_LOSTARB);

			/* Restore ACK. */
			reg = RD4(sc, I2C_CTL0);
			reg |= CTL0_ACKEN;
			WR4(sc, I2C_CTL0, reg);
		} else {
			for (j = 0; j < msg->len; j++) {
				while ((RD4(sc, I2C_STAT0) & STAT0_TBE) == 0);
				WR4(sc, I2C_DATA, msg->buf[j]);
				while ((RD4(sc, I2C_STAT0) & STAT0_BTC) == 0);
			}
			if ((msg->flags & IIC_M_NOSTOP) == 0) {
				reg = RD4(sc, I2C_CTL0);
				reg |= CTL0_STOP;
				WR4(sc, I2C_CTL0, reg);
				while (RD4(sc, I2C_STAT0) & STAT0_LOSTARB);
			}
		}
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
	WR4(sc, I2C_CTL1, reg);
	WR4(sc, I2C_CTL0, CTL0_I2CEN | CTL0_ACKEN);
#endif

	return (0);
}
