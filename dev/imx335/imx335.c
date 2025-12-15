/*-
 * Copyright (c) 2025 Ruslan Bukin <br@bsdpad.com>
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
#include <dev/imx335/imx335.h>

struct regval {
	uint16_t reg;
	uint8_t val;
};

#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))

static const struct regval res_2592_1944_regs[] = {
	{0x3000, 0x01},
	{0x3002, 0x00},
	{0x300c, 0x3b},
	{0x300d, 0x2a},
	{0x3018, 0x04},
	{0x302c, 0x3c},
	{0x302e, 0x20},
	{0x3056, 0x98},
	{0x3074, 0xc8},
	{0x3076, 0x30},
	{0x304c, 0x00},
	{0x314c, 0xc6},
	{0x315a, 0x02},
	{0x3168, 0xa0},
	{0x316a, 0x7e},
	{0x31a1, 0x00},
	{0x3288, 0x21},
	{0x328a, 0x02},
	{0x3414, 0x05},
	{0x3416, 0x18},
	{0x3648, 0x01},
	{0x364a, 0x04},
	{0x364c, 0x04},
	{0x3678, 0x01},
	{0x367c, 0x31},
	{0x367e, 0x31},
	{0x3706, 0x10},
	{0x3708, 0x03},
	{0x3714, 0x02},
	{0x3715, 0x02},
	{0x3716, 0x01},
	{0x3717, 0x03},
	{0x371c, 0x3d},
	{0x371d, 0x3f},
	{0x372c, 0x00},
	{0x372d, 0x00},
	{0x372e, 0x46},
	{0x372f, 0x00},
	{0x3730, 0x89},
	{0x3731, 0x00},
	{0x3732, 0x08},
	{0x3733, 0x01},
	{0x3734, 0xfe},
	{0x3735, 0x05},
	{0x3740, 0x02},
	{0x375d, 0x00},
	{0x375e, 0x00},
	{0x375f, 0x11},
	{0x3760, 0x01},
	{0x3768, 0x1b},
	{0x3769, 0x1b},
	{0x376a, 0x1b},
	{0x376b, 0x1b},
	{0x376c, 0x1a},
	{0x376d, 0x17},
	{0x376e, 0x0f},
	{0x3776, 0x00},
	{0x3777, 0x00},
	{0x3778, 0x46},
	{0x3779, 0x00},
	{0x377a, 0x89},
	{0x377b, 0x00},
	{0x377c, 0x08},
	{0x377d, 0x01},
	{0x377e, 0x23},
	{0x377f, 0x02},
	{0x3780, 0xd9},
	{0x3781, 0x03},
	{0x3782, 0xf5},
	{0x3783, 0x06},
	{0x3784, 0xa5},
	{0x3788, 0x0f},
	{0x378a, 0xd9},
	{0x378b, 0x03},
	{0x378c, 0xeb},
	{0x378d, 0x05},
	{0x378e, 0x87},
	{0x378f, 0x06},
	{0x3790, 0xf5},
	{0x3792, 0x43},
	{0x3794, 0x7a},
	{0x3796, 0xa1},
	{0x37b0, 0x36},
	{0x3a00, 0x01},
};

static const struct regval mode_2l_10b_regs[] = {
	{0x3050, 0x00},
	{0x319D, 0x00},
	{0x341c, 0xff},
	{0x341d, 0x01},
	{0x3a01, 0x01},
};

static const struct regval test_pattern_enable_regs[] = {
	{0x3148, 0x10},
	{0x3280, 0x00},
	{0x329c, 0x01},
	{0x32a0, 0x11},
	{0x3302, 0x00},
	{0x3303, 0x00},
	{0x336c, 0x00},
};

static const struct regval test_pattern_disable_regs[] __unused = {
	{0x3148, 0x00},
	{0x3280, 0x01},
	{0x329c, 0x00},
	{0x32a0, 0x10},
	{0x3302, 0x32},
	{0x3303, 0x00},
	{0x336c, 0x01},
};

static const struct regval inck_24Mhz_regs[] __unused = {
	{0x300c, 0x3B},
	{0x300d, 0x2A},
	{0x314c, 0xC6},
	{0x314d, 0x00},
	{0x315a, 0x02},
	{0x3168, 0xA0},
	{0x316a, 0x7E},
};

static const struct regval inck_74Mhz_regs[] __unused = {
	{0x300c, 0xB6},
	{0x300d, 0x7F},
	{0x314c, 0x80},
	{0x314d, 0x00},
	{0x315a, 0x03},
	{0x3168, 0x68},
	{0x316a, 0x7F},
};

static const struct regval framerate_30fps_regs[] __unused = {
	{0x3030, 0x94},
	{0x3031, 0x11},
};

int
imx335_read_data(mdx_device_t dev, uint8_t i2c_addr, uint16_t reg, int n,
    uint8_t *val)
{
	struct i2c_msg msgs[2];
	uint8_t data[2];
	int err;

	data[0] = (reg >> 8) & 0xff;
	data[1] = reg & 0xff;

	/* Write register */
	msgs[0].slave = i2c_addr;
	msgs[0].buf = data;
	msgs[0].len = 2;
	msgs[0].flags = IIC_M_NOSTOP;

	/* Read data */
	msgs[1].slave = i2c_addr;
	msgs[1].buf = val;
	msgs[1].len = n;
	msgs[1].flags = IIC_M_RD;

	err = mdx_i2c_transfer(dev, msgs, 2);

	return (err);
}

int
imx335_read_reg(mdx_device_t dev, uint8_t i2c_addr, uint16_t reg, uint8_t *val)
{
	int err;

	err = imx335_read_data(dev, i2c_addr, reg, 1, val);

	return (err);
}

static int
imx335_write_data(mdx_device_t dev, uint8_t i2c_addr, uint16_t reg,
    uint8_t *val, int len)
{
	struct i2c_msg msgs[1];
	uint8_t data[2 + 16];
	int err;

	if (len > 16)
		return (-1);

	data[0] = (reg >> 8) & 0xff;
	data[1] = reg & 0xff;
	memcpy(&data[2], val, len);

	/* Write register and data. */
	msgs[0].slave = i2c_addr;
	msgs[0].buf = data;
	msgs[0].len = 2 + len;
	msgs[0].flags = 0;

	err = mdx_i2c_transfer(dev, msgs, 1);

	return (err);
}

int
imx335_write_reg(mdx_device_t dev, uint8_t i2c_addr, uint16_t reg, uint8_t val)
{
	int err;

	err = imx335_write_data(dev, i2c_addr, reg, &val, 1);

	return (err);
}

static int
imx335_init_table(mdx_device_t dev, uint8_t i2c_addr, const struct regval *rv,
    int len)
{
	uint16_t reg;
	uint8_t val;
	int error;
	int i;

	for (i = 0; i < len; i++) {
		reg = rv[i].reg;
		val = rv[i].val;
		error = imx335_write_reg(dev, 0x34, reg, val);
		if (error)
			return (error);
	}

	return (0);
}

int
imx335_init(mdx_device_t dev, uint8_t i2c_addr)
{
	int error;

	error = imx335_init_table(dev, i2c_addr, res_2592_1944_regs,
	    ARRAY_LEN(res_2592_1944_regs));
	if (error) {
		printf("%s: cant init resolution\n", __func__);
		return (error);
	}

	error = imx335_init_table(dev, i2c_addr, mode_2l_10b_regs,
	    ARRAY_LEN(mode_2l_10b_regs));
	if (error) {
		printf("%s: cant init mode\n", __func__);
		return (error);
	}

	/* Set frequency. */
	error = imx335_init_table(dev, i2c_addr, inck_24Mhz_regs,
	    ARRAY_LEN(inck_24Mhz_regs));
	if (error) {
		printf("%s: cant set freq\n", __func__);
		return (error);
	}

	/* Set frame rate. */
	error = imx335_init_table(dev, i2c_addr, framerate_30fps_regs,
	    ARRAY_LEN(framerate_30fps_regs));
	if (error) {
		printf("%s: cant set frame rate\n", __func__);
		return (error);
	}

	/* Unstandby. */
	error = imx335_write_reg(dev, i2c_addr, IMX335_STANDBY, 0);
	if (error) {
		printf("%s: cant start streaming\n", __func__);
		return (error);
	}

	printf("%s: init done\n", __func__);

	return (0);
}

int
imx335_test_pattern_enable(mdx_device_t dev, uint8_t i2c_addr, uint8_t mode)
{
	int error;

	error = imx335_write_reg(dev, i2c_addr, IMX335_TPG, mode);
	if (error) {
		printf("%s: cant start test pattern\n", __func__);
		return (error);
	}

	error = imx335_init_table(dev, i2c_addr, test_pattern_enable_regs,
	    ARRAY_LEN(test_pattern_enable_regs));
	if (error) {
		printf("%s: cant init test pattern mode\n", __func__);
		return (error);
	}

	return (0);
}

int
imx335_set_gain(mdx_device_t dev, uint8_t i2c_addr, uint16_t gain)
{
	int error;

	error = imx335_write_reg(dev, i2c_addr, IMX335_HOLD, 1);
	if (error) {
		printf("%s: cant hold\n", __func__);
		return (error);
	}

	error = imx335_write_data(dev, i2c_addr, IMX335_GAIN, (uint8_t *)&gain,
	    2);
	if (error) {
		printf("%s: cant set gain\n", __func__);
		return (error);
	}

	error = imx335_write_reg(dev, i2c_addr, IMX335_HOLD, 0);
	if (error) {
		printf("%s: cant hold\n", __func__);
		return (error);
	}

#if 0
	uint8_t test;
	imx335_read_reg(dev, i2c_addr, IMX335_GAIN, &test);
	printf("%s: test %x\n", __func__, test);
#endif

	return (0);
}

int
imx335_set_exposure(mdx_device_t dev, uint8_t i2c_addr, uint32_t exposure)
{
	int error;

	error = imx335_write_reg(dev, i2c_addr, IMX335_HOLD, 1);
	if (error) {
		printf("%s: cant hold\n", __func__);
		return (error);
	}

	error = imx335_write_data(dev, i2c_addr, IMX335_SHUTTER,
	    (uint8_t *)&exposure, 3);
	if (error) {
		printf("%s: cant set exposure\n", __func__);
		return (error);
	}

	error = imx335_write_reg(dev, i2c_addr, IMX335_HOLD, 0);
	if (error) {
		printf("%s: cant hold\n", __func__);
		return (error);
	}

	return (0);
}
