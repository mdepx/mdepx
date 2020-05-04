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

#include <dev/bme680/bme680.h>
#include <dev/bme680/bme680_driver.h>

#define	BME680_XFER_MAX_LEN	32
#define	BME680_CHIP_ID_VAL	0x61

static int
bme680_chip_id(mdx_device_t dev, int dev_id, uint8_t *result)
{
	struct i2c_msg msgs[2];
	uint8_t chip_reg;
	uint8_t chip_id;
	int err;

	chip_reg = BME680_CHIP_ID_ADDR;

	/* Write register */
	msgs[0].slave = dev_id;
	msgs[0].buf = &chip_reg;
	msgs[0].len = 1;
	msgs[0].flags = IIC_M_NOSTOP;

	/* Read data */
	msgs[1].slave = dev_id;
	msgs[1].buf = &chip_id;
	msgs[1].len = 1;
	msgs[1].flags = IIC_M_RD;
	err = mdx_i2c_transfer(dev, msgs, 2);
	if (err)
		return (err);

	*result = chip_id;

	return (0);
}

static int8_t
user_i2c_read(void *arg, uint8_t dev_id, uint8_t reg_addr,
    uint8_t *reg_data, uint16_t len)
{
	uint8_t tx_data[BME680_XFER_MAX_LEN];
	uint8_t rx_data[BME680_XFER_MAX_LEN];
	mdx_device_t dev;
	struct i2c_msg msgs[2];
	int err;
	int i;

	if (len > BME680_XFER_MAX_LEN) {
		printf("%s: failed to read data: len %d\n", __func__, len);
		return (-1);
	}

	dev = arg;

	bzero(&msgs[0], sizeof(struct i2c_msg));

	/* Read Data */
	tx_data[0] = reg_addr;
	msgs[0].slave = dev_id;
	msgs[0].buf = tx_data;
	msgs[0].len = 1;
	msgs[0].flags = IIC_M_NOSTOP;

	msgs[1].slave = dev_id;
	msgs[1].buf = rx_data;
	msgs[1].len = len;
	msgs[1].flags = IIC_M_RD;
	err = mdx_i2c_transfer(dev, msgs, 2);
	if (err)
		return (err);

	for (i = 0; i < len; i++)
		reg_data[i] = rx_data[i];

	return (0);
}

static int8_t
user_i2c_write(void *arg, uint8_t dev_id, uint8_t reg_addr,
    uint8_t *reg_data, uint16_t len)
{
	uint8_t tx_data[BME680_XFER_MAX_LEN];
	mdx_device_t dev;
	struct i2c_msg msgs[1];
	int err;
	int i;

	dev = arg;

	if (len > (BME680_XFER_MAX_LEN - 1)) {
		printf("%s: failed to write data: len %d\n", len);
		return (-1);
	}

	bzero(&msgs[0], sizeof(struct i2c_msg));

	/* Write Data */
	tx_data[0] = reg_addr;
	for (i = 0; i < len; i++)
		tx_data[1 + i] = reg_data[i];
	msgs[0].slave = dev_id;
	msgs[0].buf = tx_data;
	msgs[0].len = 1 + len;
	err = mdx_i2c_transfer(dev, msgs, 1);
	if (err)
		return (err);

	return (0);
}

static void
user_delay_ms(uint32_t period)
{

	udelay(period * 1000);
}

static int
bme680_configure(struct bme680_dev *gas_sensor)
{
	uint8_t set_required_settings;
	int8_t rslt;

	/* Set the temperature, pressure and humidity settings */
	gas_sensor->tph_sett.os_hum = BME680_OS_2X;
	gas_sensor->tph_sett.os_pres = BME680_OS_4X;
	gas_sensor->tph_sett.os_temp = BME680_OS_8X;
	gas_sensor->tph_sett.filter = BME680_FILTER_SIZE_3;

	/* Set the remaining gas sensor settings and link the heating profile */
	gas_sensor->gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
	/* Create a ramp heat waveform in 3 steps */
	gas_sensor->gas_sett.heatr_temp = 320; /* degree Celsius */
	gas_sensor->gas_sett.heatr_dur = 150; /* milliseconds */

	/* Select the power mode. */
	/* Must be set before writing the sensor configuration. */
	gas_sensor->power_mode = BME680_FORCED_MODE; 

	/* Set the required sensor settings needed. */
	set_required_settings = BME680_OST_SEL | BME680_OSP_SEL |
	    BME680_OSH_SEL | BME680_FILTER_SEL | BME680_GAS_SENSOR_SEL;

	/* Set the desired sensor configuration. */
	rslt = bme680_set_sensor_settings(set_required_settings, gas_sensor);

	if (rslt != 0)
		return (rslt);

	/* Set the power mode. */
	rslt = bme680_set_sensor_mode(gas_sensor);

	return (rslt);
}

int
bme680_initialize(mdx_device_t dev, struct bme680_dev *gas_sensor)
{
	uint8_t chip_id;
	int8_t rslt;

	gas_sensor->dev_id = BME680_I2C_ADDR_SECONDARY;
	gas_sensor->intf = BME680_I2C_INTF;
	gas_sensor->read = user_i2c_read;
	gas_sensor->write = user_i2c_write;
	gas_sensor->arg = dev;
	gas_sensor->delay_ms = user_delay_ms;
	gas_sensor->amb_temp = 25;

	rslt = bme680_chip_id(dev, gas_sensor->dev_id, &chip_id);
	if (rslt != BME680_OK)
		return (rslt);

	if (chip_id != BME680_CHIP_ID_VAL) {
		printf("%s: failed to initialize bme680 (wrong chip_id: %x)\n",
		    __func__, chip_id);
		return (-1);
	}

	rslt = bme680_init(gas_sensor);
	if (rslt != BME680_OK)
		return (rslt);

	rslt = bme680_configure(gas_sensor);

	return (rslt);
}

/*
 * Trigger the next measurement.
 */
int
bme680_trigger(struct bme680_dev *gas_sensor)
{
	int8_t rslt;

	rslt = BME680_OK;

	if (gas_sensor->power_mode == BME680_FORCED_MODE)
		rslt = bme680_set_sensor_mode(gas_sensor);

	return (rslt);
}

/*
 * Read the measurements.
 * Note that a delay is required after triggering and before reading the data.
 */
int
bme680_read_data(struct bme680_dev *gas_sensor, struct bme680_field_data *data)
{
	int8_t rslt;

	rslt = bme680_get_sensor_data(data, gas_sensor);

	return (rslt);
}
