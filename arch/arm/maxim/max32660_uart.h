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

#ifndef _ARM_MAXIM_MAX32660_UART_H_
#define _ARM_MAXIM_MAX32660_UART_H_

#define	UART_CTRL0		0x00	/* R/W UARTn Control 0 Register */
#define	 CTRL0_SIZE_S		8 /* Character Size */
#define	 CTRL0_SIZE_M		(0x3 << CTRL0_SIZE_S)
#define	 CTRL0_SIZE_5BITS	(0x0 << CTRL0_SIZE_S)
#define	 CTRL0_SIZE_6BITS	(0x1 << CTRL0_SIZE_S)
#define	 CTRL0_SIZE_7BITS	(0x2 << CTRL0_SIZE_S)
#define	 CTRL0_SIZE_8BITS	(0x3 << CTRL0_SIZE_S)
#define	 CTRL0_EN		(1 << 0) /* UART Enable */
#define	UART_CTRL1		0x04	/* R/W UARTn Control 1 Register */
#define	UART_STAT		0x08	/* RO UARTn Status Register */
#define	 STAT_TX_FULL		(1 << 7) /* TX FIFO Full Status Flag */
#define	UART_INT_EN		0x0C	/* R/W UARTn Interrupt Enable Register */
#define	UART_INT_FL		0x10	/* R/1 UARTn Interrupt Flag Register */
#define	UART_BAUD0		0x14	/* R/W UARTn Baud Rate Integer Register */
#define	 BAUD0_CLKDIV_S		16 /* Bit Rate Clock Divisor */
#define	 BAUD0_CLKDIV_M		(0x7 << BAUD0_CLKDIV_S)
#define	 BAUD0_CLKDIV_128	(0x0 << BAUD0_CLKDIV_S)
#define	 BAUD0_CLKDIV_64	(0x1 << BAUD0_CLKDIV_S)
#define	 BAUD0_CLKDIV_32	(0x2 << BAUD0_CLKDIV_S)
#define	 BAUD0_CLKDIV_16	(0x3 << BAUD0_CLKDIV_S)
#define	 BAUD0_CLKDIV_8		(0x4 << BAUD0_CLKDIV_S)
#define	 BAUD0_IBAUD_S		0 /* Integer Portion of Baud Rate Divisor */
#define	 BAUD0_IBAUD_M		(0xfff << BAUD0_IBAUD_S)
#define	UART_BAUD1		0x18	/* R/W UARTn Baud Rate Decimal Register */
#define	 BAUD1_DBAUD_S		0 /* Decimal Portion of Baud Rate Divisor */
#define	 BAUD1_DBAUD_M		(0xfff << BAUD0_DBAUD_S)
#define	UART_FIFO		0x1C	/* R/W UARTn FIFO Read/Write Register */
#define	UART_DMA		0x20	/* R/W UARTn DMA Configuration Register */
#define	UART_TXFIFO		0x24	/*  RO UARTn TX FIFO Register */

struct max32660_uart_softc {
	uint32_t base;
	uint32_t tx;
	uint32_t rx;
};

void max32660_uart_init(struct max32660_uart_softc *sc, uint32_t base,
    uint32_t uart_freq, uint32_t baud_rate);
void max32660_uart_putc(struct max32660_uart_softc *sc, char ch);
void max32660_uart_intr(void *arg, uint32_t irqno);

#endif /* !_ARM_MAXIM_MAX32660_UART_H_ */
