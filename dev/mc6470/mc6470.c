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
#include <sys/console.h>
#include <sys/callout.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/thread.h>

#include <dev/i2c/i2c.h>
#include <dev/mc6470/mc6470.h>

#define	MC6470_DEVID	0x4c

int
mc6470_read_reg(struct mc6470_dev *dev, uint8_t reg, uint8_t *val)
{
	struct i2c_msg msgs[2];
	struct i2c_bus *i2cb;
	uint8_t dev_id;
	int err;

	i2cb = dev->i2cb;

	dev_id = MC6470_DEVID;

	/* Write register */
	msgs[0].slave = dev_id;
	msgs[0].buf = &reg;
	msgs[0].len = 1;
	msgs[0].flags = 0;

        /* Read data */
	msgs[1].slave = dev_id;
	msgs[1].buf = val;
	msgs[1].len = 1;
	msgs[1].flags = IIC_M_RD;

	err = i2cb->xfer(i2cb->arg, msgs, 2);

	return (err);
}

int
mc6470_write_reg(struct mc6470_dev *dev, uint8_t reg, uint8_t val)
{
	struct i2c_msg msgs[1];
	struct i2c_bus *i2cb;
	uint8_t data[2];
	uint8_t dev_id;
	int err;

	i2cb = dev->i2cb;

	dev_id = MC6470_DEVID;

	data[0] = reg;
	data[1] = val;

	/* Write register and data. */
	msgs[0].slave = dev_id;
	msgs[0].buf = data;
	msgs[0].len = 2;
	msgs[0].flags = 0;

	err = i2cb->xfer(i2cb->arg, msgs, 1);

	return (err);
}

int
mc6470_set_freq(struct mc6470_dev *dev, uint8_t val)
{
	int err;

	err = mc6470_write_reg(dev, MC6470_SRTFR, val);

	return (err);
}
