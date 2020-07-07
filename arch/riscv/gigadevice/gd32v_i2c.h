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

#ifndef _RISCV_GIGADEVICE_GD32V_I2C_H_
#define _RISCV_GIGADEVICE_GD32V_I2C_H_

#include <sys/sem.h>

#define	I2C_CTL0	0x00	/* Control register 0 */
#define	 CTL0_ACKEN	(1 << 10) /* Whether or not to send an ACK */
#define	 CTL0_STOP	(1 << 9) /* Generate a STOP condition on I2C bus */
#define	 CTL0_START	(1 << 8) /* Generate a START condition on I2C bus */
#define	 CTL0_SS	(1 << 7) /* Whether to stretch SCL low when data is not ready in slave mode. */
#define	 CTL0_GCEN	(1 << 6) /* Whether or not to response to a General Call (0x00) */
#define	 CTL0_I2CEN	(1 << 0) /* I2C peripheral enable */
#define	I2C_CTL1	0x04	/* Control register 1 */
#define	 CTL1_BUFIE	(1 << 10) /* BUFIE  Buffer interrupt enable */
#define	 CTL1_EVIE	(1 << 9) /* Event interrupt enable */
#define	 CTL1_ERRIE	(1 << 8) /* Error interrupt enable */
#define	 CTL1_I2CCLK_S	0	/*I2C Peripheral clock frequency */
#define	 CTL1_I2CCLK_M	(0x3f << CTL1_I2CCLK_S)
#define	I2C_SADDR0	0x08	/* Slave address register 0 */
#define	I2C_SADDR1	0x0C	/* Slave address register 1 */
#define	I2C_DATA	0x10	/* Transfer buffer register */
#define	I2C_STAT0	0x14	/* Transfer status register 0 */
#define	 STAT0_LOSTARB	(1 << 9) /*  Arbitration Lost in master mode */
#define	 STAT0_BERR	(1 << 8) /* A bus error occurs */
#define	 STAT0_TBE	(1 << 7) /* I2C_DATA is Empty during transmitting */
#define	 STAT0_RBNE	(1 << 6) /* I2C_DATA is not Empty during receiving */
#define	 STAT0_STPDET	(1 << 4) /* STOP condition detected in slave mode */
#define	 STAT0_BTC	(1 << 2) /* Byte transmission completed. */
#define	 STAT0_ADDSEND	(1 << 1) /* Address is sent in master mode or received and matches in slave mode. */
#define	 STAT0_SBSEND	(1 << 0) /* START condition sent out in master mode */
#define	I2C_STAT1	0x18	/* Transfer status register 1 */
#define	 STAT1_I2CBSY	(1 << 1) /* Busy flag */
#define	 STAT1_MASTER	(1 << 0) /* I2C block is in master mode */
#define	I2C_CKCFG	0x1C	/* Clock configure register */
#define	I2C_RT		0x20	/* Rise time register */
#define	I2C_FMPCFG	0x90	/* Fast mode plus configure register */

struct gd32v_i2c_softc {
	uint32_t base;
	struct i2c_msg *curmsg;
	int bufpos;
	mdx_sem_t sem;
	int error;
};

int gd32v_i2c_init(mdx_device_t dev, uint32_t base);
void gd32v_i2c_event_intr(void *arg, int irq);
void gd32v_i2c_error_intr(void *arg, int irq);

#endif /* !_RISCV_GIGADEVICE_GD32V_I2C_H_ */
