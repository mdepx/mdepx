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

#include <riscv/kendryte/k210.h>

#include <dev/i2c/i2c.h>
#include <dev/i2c/bitbang/i2c_bitbang.h>
#include <dev/bme680/bme680.h>
#include <dev/bme680/bme680_driver.h>
#include <dev/uart/uart_16550.h>
#include <dev/mh_z19b/mh_z19b.h>
#include <dev/gpio/gpio.h>

static struct mdx_bme680_dev dev;
static struct i2c_bus i2cb;
static struct i2c_bitbang_softc i2c_bitbang_sc;

extern struct k210_i2c_softc i2c_sc;
extern struct uart_16550_softc uart_sc;

static void
i2c_scl(struct i2c_bitbang_softc *sc, bool enable)
{

	if (enable) {
		mdx_gpio_configure(0, 28, MDX_GPIO_INPUT);
	} else {
		mdx_gpio_configure(0, 28, MDX_GPIO_OUTPUT);
	}
}

static void
i2c_sda(struct i2c_bitbang_softc *sc, bool enable)
{

	if (enable) {
		mdx_gpio_set(0, 29, 0);
	} else {
		mdx_gpio_set(0, 29, 1);
	}
}

static int
i2c_sda_val(struct i2c_bitbang_softc *sc)
{
	uint8_t reg;

	reg = mdx_gpio_get(0, 29);

	return (reg & 0x1);
}

static int
bme680_sensor_init(void)
{
	int ret;

	i2c_bitbang_sc.i2c_scl = i2c_scl;
	i2c_bitbang_sc.i2c_sda = i2c_sda;
	i2c_bitbang_sc.i2c_sda_val = i2c_sda_val;

	/* For bitbang */
	i2cb.xfer = i2c_bitbang_xfer;
	i2cb.arg = &i2c_bitbang_sc;

	/* For k210 i2c controller */
	i2cb.xfer = k210_i2c_xfer;
	i2cb.arg = &i2c_sc;

	mdx_gpio_set(0, 28, 0);
	mdx_gpio_set(0, 29, 0);

	dev.i2cb = &i2cb;
	ret = bme680_initialize(&dev);

	return (ret);
}

static void
mh_z19_cycle(uint8_t *req, uint8_t *reply)
{
	int i;

	for (i = 0; i < 9; i++)
		uart_16550_putc(&uart_sc, req[i]);
	for (i = 0; i < 9; i++)
		reply[i] = uart_16550_getc(&uart_sc);
}

int
main(void)
{
	struct bme680_field_data data;
	uint8_t reply[9];
	uint8_t req[9];
	uint32_t co2;
	int error;

	printf("Hello world!\n");

	mdx_gpio_set(0, 20, 1);
	mdx_gpio_set(0, 21, 1);
	mdx_gpio_set(0, 22, 1);

	/* bme680 SDO CS */
	mdx_gpio_configure(0, 26, MDX_GPIO_OUTPUT);
	mdx_gpio_configure(0, 27, MDX_GPIO_OUTPUT);
	mdx_gpio_set(0, 26, 1);
	mdx_gpio_set(0, 27, 1);

	mdx_gpio_set(0, 20, 0);
	mdx_gpio_set(0, 22, 1);

	error = bme680_sensor_init();
	if (error)
		printf("could not initialize bme680\n");

	mh_z19b_set_range_req(req, 2000);
	mh_z19_cycle(req, reply);

	mh_z19b_read_co2_req(req);

	while (1) {
		mh_z19_cycle(req, reply);

		error = mh_z19b_read_co2_reply(reply, &co2);
		if (error) {
			printf("Failed to read CO2 data, error %d\n",
			    error);
			continue;
		}

		error = bme680_read_data(&dev, &data);
		if (error == 0) {
			printf("CO2 %d ppm, temp %d\n", co2, data.temperature);
			mdx_usleep(2000000);
			bme680_trigger(&dev);
			mdx_usleep(2000000);
		} else
			printf("Failed to read bme680 data, error %d\n",
			    error);
	}

	return (0);
}
