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

#ifndef	_DEV_IMX335_IMX335_H_
#define	_DEV_IMX335_IMX335_H_

#define	IMX335_STANDBY		0x3000
#define	 STANDBY_STANDBY	(1 << 0)
#define	IMX335_HOLD		0x3001
#define	IMX335_GAIN		0x30e8
#define	IMX335_TPG		0x329e

int imx335_read_reg(mdx_device_t dev, uint8_t i2c_addr, uint16_t reg,
    uint8_t *val);
int imx335_write_reg(mdx_device_t dev, uint8_t i2c_addr, uint16_t reg,
    uint8_t val);
int imx335_read_data(mdx_device_t dev, uint8_t i2c_addr, uint16_t reg, int n,
    uint8_t *val);
int imx335_init(mdx_device_t dev, uint8_t i2c_addr);
int imx335_test_pattern_enable(mdx_device_t dev, uint8_t i2c_addr,
    uint8_t mode);
int imx335_set_gain(mdx_device_t dev, uint8_t i2c_addr, uint8_t gain);

#endif /* !_DEV_IMX335_IMX335_H_ */
