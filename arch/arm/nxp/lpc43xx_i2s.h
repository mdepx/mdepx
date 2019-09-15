/*-
 * Copyright (c) 2019 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _ARM_NXP_LPC43XX_I2S_H_
#define _ARM_NXP_LPC43XX_I2S_H_

#define	I2S_DAO			0x00 /* I2S Digital Audio Output Register */
#define	 DAO_WORDWIDTH_S	0 /* Selects the number of bytes in data */
#define	 DAO_WORDWIDTH_M	(0x3 << DAO_WORDWIDTH_S)
#define	 DAO_WORDWIDTH8		(0x0 << DAO_WORDWIDTH_S)
#define	 DAO_WORDWIDTH16	(0x1 << DAO_WORDWIDTH_S)
#define	 DAO_WORDWIDTH32	(0x3 << DAO_WORDWIDTH_S)
#define	 DAO_MONO		(1 << 2) /* Data is of monaural format */
#define	 DAO_STOP		(1 << 3) /* Disables accesses on FIFOs */
#define	 DAO_RESET		(1 << 4) /* Reset transmit channel and FIFO */
#define	 DAO_WS_SEL		(1 << 5) /* The interface is in slave mode */
#define	 DAO_WS_HALFPERIOD_S	6 /* Word select half period minus 1 */
#define	 DAO_WS_HALFPERIOD_M	(0x1ff << DAO_WS_HALFPERIOD_S)
#define	 DAO_MUTE		(1 << 15)
#define	I2S_DAI			0x04 /* I2S Digital Audio Input Register */
#define	I2S_TXFIFO		0x08 /* I2S Transmit FIFO */
#define	I2S_RXFIFO		0x0C /* I2S Receive FIFO */
#define	I2S_STATE		0x10 /* I2S Status Feedback Register */
#define	I2S_DMA1		0x14 /* I2S DMA Configuration Register 1 */
#define	I2S_DMA2		0x18 /* I2S DMA Configuration Register 2 */
#define	I2S_IRQ			0x1C /* I2S Interrupt Request Control */
#define	 IRQ_TX_DEPTH_IRQ_S	16 /* FIFO level for an irq request */
#define	 IRQ_TX_DEPTH_IRQ_M	(0xf << IRQ_TX_DEPTH_IRQ_S)
#define	 IRQ_TX_IRQ_ENABLE	(1 << 1) /* Enables I2S transmit interrupt */
#define	 IRQ_RX_IRQ_ENABLE	(1 << 0) /* Enables I2S receive interrupt */
#define	I2S_TXRATE		0x20 /* I2S Transmit MCLK divider */
#define	 TXRATE_Y_DIVIDER_S	0 /* I2S transmit MCLK rate denominator */
#define	 TXRATE_Y_DIVIDER_M	(0xff << TXRATE_Y_DIVIDER_S)
#define	 TXRATE_X_DIVIDER_S	8 /* I2S transmit MCLK rate numerator */
#define	 TXRATE_X_DIVIDER_M	(0xff << TXRATE_X_DIVIDER_S)
#define	I2S_RXRATE		0x24 /* I2S Receive MCLK divider */
#define	I2S_TXBITRATE		0x28 /* I2S Transmit bit rate divider */
#define	I2S_RXBITRATE		0x2C /* I2S Receive bit rate divider */
#define	I2S_TXMODE		0x30 /* I2S Transmit mode control */
#define	I2S_RXMODE		0x34 /* I2S Receive mode control */

#endif /* !_ARM_NXP_LPC43XX_I2S_H_ */
