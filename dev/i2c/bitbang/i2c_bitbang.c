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

#include "i2c_bitbang.h"

static void
i2c_start_condition(struct i2c_bitbang_softc *sc)
{
	struct i2c_bitbang_ops *ops;

	ops = sc->ops;

	ops->i2c_scl(sc, 1);
	ops->i2c_sda(sc, 1);
	udelay(5);
	ops->i2c_sda(sc, 0);
	udelay(5);
	ops->i2c_scl(sc, 0);
	udelay(5);
}

static void
i2c_stop_condition(struct i2c_bitbang_softc *sc)
{
	struct i2c_bitbang_ops *ops;

	ops = sc->ops;

	ops->i2c_sda(sc, 0);
	udelay(5);
	ops->i2c_scl(sc, 1);
	udelay(5);
	ops->i2c_sda(sc, 1);
	udelay(5);
}

static void
i2c_write_bit(struct i2c_bitbang_softc *sc, uint8_t b)
{
	struct i2c_bitbang_ops *ops;

	ops = sc->ops;

	if (b > 0)
		ops->i2c_sda(sc, 1);
	else
		ops->i2c_sda(sc, 0);

	udelay(5);
	ops->i2c_scl(sc, 1);
	udelay(5);
	ops->i2c_scl(sc, 0);
}

static uint8_t
i2c_read_bit(struct i2c_bitbang_softc *sc)
{
	struct i2c_bitbang_ops *ops;
	uint8_t b;

	ops = sc->ops;

	ops->i2c_sda(sc, 1);
	udelay(5);
	ops->i2c_scl(sc, 1);
	udelay(5);

	b = ops->i2c_sda_val(sc);

	ops->i2c_scl(sc, 0);
 
	return (b);
}

static int
i2c_write_byte(struct i2c_bitbang_softc *sc,
    uint8_t b, int start)
{
	uint8_t i;
	bool ack;

	ack = 0;

	if (start)
		i2c_start_condition(sc);

	for (i = 0; i < 8; i++) {
		/* Write the most-significant bit. */
		if (b & 0x80)
			i2c_write_bit(sc, 1);
		else
			i2c_write_bit(sc, 0);
		b <<= 1;
	}
 
	ack = i2c_read_bit(sc);

	return (!ack);
}

static uint8_t
i2c_read_byte(struct i2c_bitbang_softc *sc, bool ack)
{
	uint8_t b;
	uint8_t i;

	b = 0;

	for (i = 0; i < 8; i++) {
		b <<= 1;
		b |= i2c_read_bit(sc);
	}

	if (ack)
		i2c_write_bit(sc, 0);
	else
		i2c_write_bit(sc, 1);

	return (b);
}

static int
i2c_bitbang_xfer(mdx_device_t dev, struct i2c_msg *msgs, int len)
{
	struct i2c_bitbang_softc *sc;
	struct i2c_msg *msg;
	int i;
	int j;
	bool ack;
	uint8_t addr;
	int error;

	sc = mdx_device_get_softc(dev);

	error = 0;

	critical_enter();

	for (i = 0; i < len; i++) {
		msg = &msgs[i];
		addr = (msg->slave << 1);

		if (msg->flags & IIC_M_RD) {
			ack = i2c_write_byte(sc, addr | I2C_RD, true);
			if (!ack) {
				printf("ack not received\n");
				error = -1;
				goto done;
			}

			for (j = 0; j < msg->len; j++) {
				if (j == msg->len - 1)
					ack = false;
				else
					ack = true;
				msg->buf[j] = i2c_read_byte(sc, ack);
			}
		} else {
			ack = i2c_write_byte(sc, addr | I2C_WR, true);
			if (!ack) {
				printf("ack not received\n");
				error = -1;
				goto done;
			}

			for (j = 0; j < msg->len; j++) {
				ack = i2c_write_byte(sc, msg->buf[j], false);
				if (!ack) {
					printf("ack not received\n");
					error = -1;
					goto done;
				}
			}
		}

		if ((msg->flags & IIC_M_NOSTOP) == 0)
			i2c_stop_condition(sc);
	}

done:
	critical_exit();

	return (error);
}

static struct mdx_i2c_ops i2c_bitbang_ops = {
	.xfer = i2c_bitbang_xfer,
};

void
i2c_bitbang_init(mdx_device_t dev, struct i2c_bitbang_ops *ops)
{
	struct i2c_bitbang_softc *sc;

	sc = mdx_device_get_softc(dev);
	sc->ops = ops;

	dev->ops = &i2c_bitbang_ops;
}
