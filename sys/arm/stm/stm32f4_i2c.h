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

#ifndef	_ARM_STM_STM32F4_I2C_H_
#define	_ARM_STM_STM32F4_I2C_H_

#define	I2C_CR1		0x00 /* Control register 1 */
#define	 CR1_ERRIE	(1 << 7) /* Error interrupts enable */
#define	 CR1_TCIE	(1 << 6) /* Transfer Complete interrupt enable */
#define	 CR1_STOPIE	(1 << 5) /* STOP detection Interrupt enable */
#define	 CR1_NACKIE	(1 << 4) /* Not acknowledge received Interrupt enable */
#define	 CR1_ADDRIE	(1 << 3) /* Address match Interrupt enable (slave only) */
#define	 CR1_RXIE	(1 << 2) /* RX Interrupt enable */
#define	 CR1_TXIE	(1 << 1) /* TX Interrupt enable */
#define	 CR1_PE		(1 << 0) /* Peripheral enable */
#define	I2C_CR2		0x04 /* Control register 2 */
#define	 CR2_AUTOEND	25 /* Automatic end mode (master mode) */
#define	 CR2_RELOAD	24 /* NBYTES reload mode */
#define	 CR2_NBYTES_S	16
#define	 CR2_NBYTES_M	0xff
#define	 CR2_NACK	(1 << 15) /* NACK generation (slave mode) */
#define	 CR2_STOP	(1 << 14) /* Stop generation (master mode) */
#define	 CR2_START	(1 << 13) /* Start generation */
#define	 CR2_RD_WRN	(1 << 10) /* Master Read transfer */
#define	 CR2_SADD_S	0
#define	 CR2_SADD_M	0x3ff
#define	I2C_OAR1	0x08 /* Own address 1 register */
#define	I2C_OAR2	0x0C /* Own address 2 register */
#define	I2C_TIMINGR	0x10 /* Timing register */
#define	 TIMINGR_SCLL_S	0
#define	 TIMINGR_SCLH_S	8
#define	I2C_TIMEOUTR	0x14 /* Timeout register */
#define	I2C_ISR		0x18 /* Interrupt and status register */
#define	 ISR_BUSY	(1 << 15) /* Bus busy */
#define	 ISR_ARLO	(1 << 9) /* Arbitration lost */
#define	 ISR_TC		(1 << 6) /* Transfer Complete (master mode) */
#define	 ISR_STOPF	(1 << 5) /* Stop detection flag */
#define	 ISR_NACKF	(1 << 4) /* Not Acknowledge received flag */
#define	 ISR_RXNE	(1 << 2) /* Receive data register not empty (receivers) */
#define	 ISR_TXIS	(1 << 1) /* Transmit interrupt status (transmitters) */
#define	 ISR_TXE	(1 << 0) /* Transmit data register empty (transmitters) */
#define	I2C_ICR		0x1C /* Interrupt clear register */
#define	I2C_PECR	0x20 /* Packet error checking register */
#define	I2C_RXDR	0x24 /* Receive data register */
#define	I2C_TXDR	0x28 /* Transmit data register */

struct stm32f4_i2c_softc {
	uint32_t base;
};

#endif	/* !_ARM_STM_STM32F4_I2C_H_ */
