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

#ifndef	_DEV_LIS3DSH_LIS3DSH_H_
#define	_DEV_LIS3DSH_LIS3DSH_H_

#define	LIS3DSH_OUT_T		0x0C	/* Temperature output */
#define	LIS3DSH_INFO1		0x0D	/* Information register 1 */
#define	LIS3DSH_INFO2		0x0E	/* Information register 2 */
#define	LIS3DSH_WHO_AM_I	0x0F	/* Who I am ID */
#define	LIS3DSH_OFF_X		0x10	/* X-axis offset correction */
#define	LIS3DSH_OFF_Y		0x11	/* Y-axis offset correction */
#define	LIS3DSH_OFF_Z		0x12	/* Z-axis offset correction */
#define	LIS3DSH_CS_X		0x13	/* Constant shift X */
#define	LIS3DSH_CS_Y		0x14	/* Constant shift Y */
#define	LIS3DSH_CS_Z		0x15	/* Constant shift Z */
#define	LIS3DSH_LC_L		0x16	/* Long counter registers */
#define	LIS3DSH_LC_H		0x17	/* Long counter registers */
#define	LIS3DSH_STAT		0x18	/* Interrupt synchronization */
#define	LIS3DSH_PEAK1		0x19	/* Peak value */
#define	LIS3DSH_PEAK2		0x1A	/* Peak value */
#define	LIS3DSH_VFC_1		0x1B	/* Vector filter coefficient 1 */
#define	LIS3DSH_VFC_2		0x1C	/* Vector filter coefficient 2 */
#define	LIS3DSH_VFC_3		0x1D	/* Vector filter coefficient 3 */
#define	LIS3DSH_VFC_4		0x1E	/* Vector filter coefficient 4 */
#define	LIS3DSH_THRS3		0x1F	/* Threshold value 3 */
#define	LIS3DSH_CTRL_REG4	0x20	/* Control register */
#define	LIS3DSH_CTRL_REG1	0x21	/* SM1 control register */
#define	LIS3DSH_CTRL_REG2	0x22	/* SM2 control register */
#define	LIS3DSH_CTRL_REG3	0x23	/* Control registers */
#define	LIS3DSH_CTRL_REG5	0x24	/* Control registers */
#define	LIS3DSH_CTRL_REG6	0x25	/* Control registers */
#define	LIS3DSH_STATUS		0x27	/* Status data register */
#define	LIS3DSH_OUT_X_L		0x28	/* Output registers */
#define	LIS3DSH_OUT_X_H		0x29	/* Output registers */
#define	LIS3DSH_OUT_Y_L		0x2A	/* Output registers */
#define	LIS3DSH_OUT_Y_H		0x2B	/* Output registers */
#define	LIS3DSH_OUT_Z_L		0x2C	/* Output registers */
#define	LIS3DSH_OUT_Z_H		0x2D	/* Output registers */
#define	LIS3DSH_FIFO_CTRL	0x2E	/* FIFO registers */
#define	LIS3DSH_FIFO_SRC	0x2F	/* FIFO registers */
#define	LIS3DSH_ST1_X(n)	(0x40 + (n)) /* SM1 code register (X =1-16) */
#define	LIS3DSH_TIM4_1		0x50	/* SM1 general timer */
#define	LIS3DSH_TIM3_1		0x51	/* SM1 general timer */
#define	LIS3DSH_TIM2_1_0	0x52	/* SM1 general timer */
#define	LIS3DSH_TIM2_1_1	0x53	/* SM1 general timer */
#define	LIS3DSH_TIM1_1_0	0x54	/* SM1 general timer */
#define	LIS3DSH_TIM1_1_1	0x55	/* SM1 general timer */
#define	LIS3DSH_THRS2_1		0x56	/* SM1 threshold value 1 */
#define	LIS3DSH_THRS1_1		0x57	/* SM1 threshold value 2 */
#define	LIS3DSH_MASK1_B		0x59	/* SM1 axis and sign mask */
#define	LIS3DSH_MASK1_A		0x5A	/* SM1 axis and sign mask */
#define	LIS3DSH_SETT1		0x5B	/* SM1 detection settings */
#define	LIS3DSH_PR1		0x5C	/* Program-reset pointer */
#define	LIS3DSH_TC1_0		0x5D	/* Timer counter */
#define	LIS3DSH_TC1_1		0x5E	/* Timer counter */
#define	LIS3DSH_OUTS1		0x5F	/* Main set flag */
#define	LIS3DSH_ST2_X(n)	(0x60 + (n)) /* SM2 code register (X =1-16) */
#define	LIS3DSH_TIM4_2		0x70	/* SM2 general timer */
#define	LIS3DSH_TIM3_2		0x71	/* SM2 general timer */
#define	LIS3DSH_TIM2_2_0	0x72	/* SM2 general timer */
#define	LIS3DSH_TIM2_2_1	0x73	/* SM2 general timer */
#define	LIS3DSH_TIM1_2_0	0x74	/* SM2 general timer */
#define	LIS3DSH_TIM1_2_1	0x75	/* SM2 general timer */
#define	LIS3DSH_THRS2_2		0x76	/* SM2 threshold value 1 */
#define	LIS3DSH_THRS1_2		0x77	/* SM2 threshold value 2 */
#define	LIS3DSH_DES2		0x78	/* Decimation factor */
#define	LIS3DSH_MASK2_B		0x79	/* SM2 axis and sign mask */
#define	LIS3DSH_MASK2_A		0x7A	/* SM2 axis and sign mask */
#define	LIS3DSH_SETT2		0x7B	/* SM2 detection settings */
#define	LIS3DSH_PR2		0x7C	/* Program-reset pointer */
#define	LIS3DSH_TC2_0		0x7D	/* Timer counter */
#define	LIS3DSH_TC2_1		0x7E	/* Timer counter */
#define	LIS3DSH_OUTS2		0x7F	/* Main set flag */

#endif /* !_DEV_LIS3DSH_LIS3DSH_H_ */
