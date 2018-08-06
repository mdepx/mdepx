/*-
 * Copyright (c) 2018 Ruslan Bukin <br@bsdpad.com>
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

#ifndef	_DEV_LIS3DH_LIS3DH_H_
#define	_DEV_LIS3DH_LIS3DH_H_

#define	LIS3DH_STATUS_REG_AUX	0x07	/* (r) */
#define	LIS3DH_OUT_ADC1_L	0x08	/* (r) */
#define	LIS3DH_OUT_ADC1_H	0x09	/* (r) */
#define	LIS3DH_OUT_ADC2_L	0x0A	/* (r) */
#define	LIS3DH_OUT_ADC2_H	0x0B	/* (r) */
#define	LIS3DH_OUT_ADC3_L	0x0C	/* (r) */
#define	LIS3DH_OUT_ADC3_H	0x0D	/* (r) */
#define	LIS3DH_WHO_AM_I		0x0F	/* (r) */
#define	LIS3DH_CTRL_REG0	0x1E	/* (rw) */
#define	LIS3DH_TEMP_CFG_REG	0x1F	/* (rw) */
#define	LIS3DH_CTRL_REG1	0x20	/* (rw) */
#define	LIS3DH_CTRL_REG2	0x21	/* (rw) */
#define	LIS3DH_CTRL_REG3	0x22	/* (rw) */
#define	LIS3DH_CTRL_REG4	0x23	/* (rw) */
#define	LIS3DH_CTRL_REG5	0x24	/* (rw) */
#define	LIS3DH_CTRL_REG6	0x25	/* (rw) */
#define	LIS3DH_REFERENCE	0x26	/* (rw) */
#define	LIS3DH_STATUS_REG	0x27	/* (r) */
#define	LIS3DH_OUT_X_L		0x28	/* (r) */
#define	LIS3DH_OUT_X_H		0x29	/* (r) */
#define	LIS3DH_OUT_Y_L		0x2A	/* (r) */
#define	LIS3DH_OUT_Y_H		0x2B	/* (r) */
#define	LIS3DH_OUT_Z_L		0x2C	/* (r) */
#define	LIS3DH_OUT_Z_H		0x2D	/* (r) */
#define	LIS3DH_FIFO_CTRL_REG	0x2E	/* (rw) */
#define	LIS3DH_FIFO_SRC_REG	0x2F	/* (r) */
#define	LIS3DH_INT1_CFG		0x30	/* (rw) */
#define	LIS3DH_INT1_SRC		0x31	/* (r) */
#define	LIS3DH_INT1_THS		0x32	/* (rw) */
#define	LIS3DH_INT1_DURATION	0x33	/* (rw) */
#define	LIS3DH_INT2_CFG		0x34	/* (rw) */
#define	LIS3DH_INT2_SRC		0x35	/* (r) */
#define	LIS3DH_INT2_THS		0x36	/* (rw) */
#define	LIS3DH_INT2_DURATION	0x37	/* (rw) */
#define	LIS3DH_CLICK_CFG	0x38	/* (rw) */
#define	LIS3DH_CLICK_SRC	0x39	/* (r) */
#define	LIS3DH_CLICK_THS	0x3A	/* (rw) */
#define	LIS3DH_TIME_LIMIT	0x3B	/* (rw) */
#define	LIS3DH_TIME_LATENCY	0x3C	/* (rw) */
#define	LIS3DH_TIME_WINDOW	0x3D	/* (rw) */
#define	LIS3DH_ACT_THS		0x3E	/* (rw) */
#define	LIS3DH_ACT_DUR		0x3F	/* (rw) */

#endif /* !_DEV_LIS3DH_LIS3DH_H_ */
