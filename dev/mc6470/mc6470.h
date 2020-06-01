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

#ifndef	_DEV_MC6470_MC6470_H_
#define	_DEV_MC6470_MC6470_H_

/* i2c addresses for accelerometer and magnetometer. */
#define	MC6470_ACC		0x4c
#define	MC6470_ACC_ALT		0x6c
#define	MC6470_MAG		0x0c

#define	MC6470_SR		0x03
#define	MC6470_OPSTAT		0x04
#define	MC6470_INTEN		0x06
#define	 INTEN_TIXPEN		(1 << 0) /* Positive X-axis TAP interrupt en */
#define	 INTEN_TIXNEN		(1 << 1) /* Negative X-axis TAP interrupt en */
#define	 INTEN_TIYPEN		(1 << 2) /* Positive Y-axis TAP interrupt en */
#define	 INTEN_TIYNEN		(1 << 3) /* Negative Y-axis TAP interrupt en */
#define	 INTEN_TIZPEN		(1 << 4) /* Positive Z-axis TAP interrupt en */
#define	 INTEN_TIZNEN		(1 << 5) /* Negative Z-axis TAP interrupt en */
#define	MC6470_MODE		0x07
#define	 MODE_OPCON_S		0
#define	 MODE_OPCON_M		(0x3 << MODE_OPCON_S)
#define	 MODE_OPCON_STANDBY	(0x0 << MODE_OPCON_S)
#define	 MODE_OPCON_WAKE	(0x1 << MODE_OPCON_S)
#define	MC6470_SRTFR		0x08
#define	 SRTFR_RATE_S		0
#define	 SRTFR_RATE_M		(0xf << SRTFR_RATE_S)
#define	 SRTFR_RATE_32HZ	(0x0 << SRTFR_RATE_S)
#define	 SRTFR_RATE_16HZ	(0x1 << SRTFR_RATE_S)
#define	 SRTFR_RATE_8HZ		(0x2 << SRTFR_RATE_S)
#define	 SRTFR_RATE_4HZ		(0x3 << SRTFR_RATE_S)
#define	 SRTFR_RATE_2HZ		(0x4 << SRTFR_RATE_S)
#define	 SRTFR_RATE_1HZ		(0x5 << SRTFR_RATE_S)
#define	 SRTFR_RATE_05HZ	(0x6 << SRTFR_RATE_S)
#define	 SRTFR_RATE_025HZ	(0x7 << SRTFR_RATE_S)
#define	 SRTFR_RATE_64HZ	(0x8 << SRTFR_RATE_S)
#define	 SRTFR_RATE_128HZ	(0x9 << SRTFR_RATE_S)
#define	 SRTFR_RATE_256HZ	(0xa << SRTFR_RATE_S)
#define	MC6470_TAPEN		0x09
#define	 TAPEN_TAPXPEN		(1 << 0)
#define	 TAPEN_TAPXNEN		(1 << 1)
#define	 TAPEN_TAPYPEN		(1 << 2)
#define	 TAPEN_TAPYNEN		(1 << 3)
#define	 TAPEN_TAPZPEN		(1 << 4)
#define	 TAPEN_TAPZNEN		(1 << 5)
#define	 TAPEN_THRDUR		(1 << 6)
#define	 TAPEN_TAP_EN		(1 << 7)
#define	MC6470_TTTRX		0x0A
#define	MC6470_TTTRY		0x0B
#define	MC6470_TTTRZ		0x0C
#define	MC6470_XOUT_EX_L	0x0D
#define	MC6470_XOUT_EX_H	0x0E
#define	MC6470_YOUT_EX_L	0x0F
#define	MC6470_YOUT_EX_H	0x10
#define	MC6470_ZOUT_EX_L	0x11
#define	MC6470_ZOUT_EX_H	0x12
#define	MC6470_OUTCFG		0x20
#define	 OUTCFG_RANGE_S		4
#define	 OUTCFG_RANGE_M		(0xf << OUTCFG_RANGE_S)
#define	 OUTCFG_RANGE_2G	(0x0 << OUTCFG_RANGE_S)
#define	 OUTCFG_RANGE_4G	(0x1 << OUTCFG_RANGE_S)
#define	 OUTCFG_RANGE_8G	(0x2 << OUTCFG_RANGE_S)
#define	 OUTCFG_RANGE_16G	(0x3 << OUTCFG_RANGE_S)
#define	MC6470_XOFFL		0x21
#define	MC6470_XOFFH		0x22
#define	MC6470_YOFFL		0x23
#define	MC6470_YOFFH		0x24
#define	MC6470_ZOFFL		0x25
#define	MC6470_ZOFFH		0x26
#define	MC6470_XGAIN		0x27
#define	MC6470_YGAIN		0x28
#define	MC6470_ZGAIN		0x29
#define	MC6470_PCODE		0x3B

#define	MC6470_MAG_SELFTEST	0x0C	/* Self test response */
#define	MC6470_MAG_MOREINFO_VER	0x0D	/* More info version */
#define	MC6470_MAG_MOREINFO	0x0E
#define	MC6470_MAG_WHOAMI	0x0F
#define	MC6470_MAG_XOUTL	0x10
#define	MC6470_MAG_XOUTH	0x11
#define	MC6470_MAG_YOUTL	0x12
#define	MC6470_MAG_YOUTH	0x13
#define	MC6470_MAG_ZOUTL	0x14
#define	MC6470_MAG_ZOUTH	0x15
#define	MC6470_MAG_STATUS	0x18
#define	MC6470_MAG_CTRL1	0x1B
#define	 MAG_CTRL1_PC		(1 << 7)
#define	 MAG_CTRL1_ODR_S	3
#define	 MAG_CTRL1_ODR_M	(0x3 << MAG_CTRL1_ODR_S)
#define	 MAG_CTRL1_FS		(1 << 1)
#define	MC6470_MAG_CTRL2	0x1C
#define	 MAG_CTRL2_AVG		(1 << 7)
#define	 MAG_CTRL2_FCO		(1 << 6)
#define	 MAG_CTRL2_AOR		(1 << 5)
#define	 MAG_CTRL2_FF		(1 << 4)
#define	 MAG_CTRL2_DEN		(1 << 3)
#define	 MAG_CTRL2_DRP		(1 << 2)
#define	 MAG_CTRL2_DTS		(1 << 1)
#define	 MAG_CTRL2_DOS		(1 << 0)
#define	MC6470_MAG_CTRL3	0x1D
#define	 MAG_CTRL3_SRST		(1 << 7)
#define	 MAG_CTRL3_FORCE	(1 << 6)
#define	 MAG_CTRL3_STC		(1 << 4)
#define	 MAG_CTRL3_TCS		(1 << 1)
#define	 MAG_CTRL3_OCL		(1 << 0)
#define	MC6470_MAG_CTRL4	0x1E
#define	 MAG_CTRL4_MMD_S	6
#define	 MAG_CTRL4_MMD_M	(0x3 << MAG_CTRL4_MMD_S)
#define	 MAG_CTRL4_RS		(1 << 4)
#define	 MAG_CTRL4_AS		(1 << 3)
#define	MC6470_MAG_XOFFL	0x20
#define	MC6470_MAG_XOFFH	0x21
#define	MC6470_MAG_YOFFL	0x22
#define	MC6470_MAG_YOFFH	0x23
#define	MC6470_MAG_ZOFFL	0x24
#define	MC6470_MAG_ZOFFH	0x25
#define	MC6470_MAG_ITHRL	0x26
#define	MC6470_MAG_ITHRH	0x27
#define	MC6470_MAG_TEMP		0x31

int mc6470_read_reg(mdx_device_t dev, uint8_t i2c_addr,
    uint8_t reg, uint8_t *val);
int mc6470_write_reg(mdx_device_t dev, uint8_t i2c_addr,
    uint8_t reg, uint8_t val);
int mc6470_set_freq(mdx_device_t dev, uint8_t val);

#endif /* !_DEV_MC6470_MC6470_H_ */
