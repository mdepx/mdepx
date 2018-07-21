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

#ifndef _ARM_MAXIM_MAX32625_UART_H_
#define _ARM_MAXIM_MAX32625_UART_H_

#define	UART_CTRL		0x00	/* Control Register */
#define	 CTRL_EN		(1 << 0)	/* UART Enable */
#define	 CTRL_RX_FIFO_EN	(1 << 1)	/* RX FIFO Enable */
#define	 CTRL_TX_FIFO_EN	(1 << 2)	/* TX FIFO Enable */
#define	 CTRL_DATA_SIZE_S	4
#define	 CTRL_DATA_SIZE_M	(0x3 << CTRL_DATA_SIZE_S)
#define	 CTRL_DATA_SIZE_5BITS	(0x0 << CTRL_DATA_SIZE_S)
#define	 CTRL_DATA_SIZE_6BITS	(0x1 << CTRL_DATA_SIZE_S)
#define	 CTRL_DATA_SIZE_7BITS	(0x2 << CTRL_DATA_SIZE_S)
#define	 CTRL_DATA_SIZE_8BITS	(0x3 << CTRL_DATA_SIZE_S)
#define	 CTRL_EXTRA_STOP	(1 << 8) /* Extra Stop Enable */
#define	 CTRL_PARITY_S		12
#define	 CTRL_PARITY_M		(0x3 << CTRL_PARITY_S)
#define	 CTRL_PARITY_DISABLED	(0x0 << CTRL_PARITY_S)
#define	 CTRL_PARITY_ODD	(0x1 << CTRL_PARITY_S)
#define	 CTRL_PARITY_EVEN	(0x2 << CTRL_PARITY_S)
#define	 CTRL_PARITY_MULTIDROP	(0x3 << CTRL_PARITY_S)
#define	 CTRL_CTS_EN		(1 << 16) /* CTS Enable */
#define	 CTRL_CTS_POLARITY	(1 << 17)
#define	 CTRL_RTS_EN		(1 << 18)
#define	 CTRL_RTS_POLARITY	(1 << 19)
#define	 CTRL_RTS_LEVEL_S	20
#define	 CTRL_RTS_LEVEL_M	(0x3f << CTRL_RTS_LEVEL_S)
#define	UART_BAUD		0x04	/* Baud Control Register */
#define	UART_TX_FIFO_CTRL	0x08	/* TX FIFO Control Register */
#define	UART_RX_FIFO_CTRL	0x0C	/* RX FIFO Control Register */
#define	UART_MD_CTRL		0x10	/* Multidrop Control Register */
#define	UART_INTFL		0x14	/* Interrupt Flags */
#define	UART_INTEN		0x18	/* Interrupt Enable/Disable Controls */

struct max32625_uart_softc {
	uint32_t base;
	uint32_t tx;
	uint32_t rx;
};

void max32625_uart_init(struct max32625_uart_softc *sc, uint32_t base, uint32_t tx, uint32_t rx);
void max32625_uart_putc(struct max32625_uart_softc *sc, char ch);
void max32625_uart_intr(void *arg, uint32_t irqno);

#endif /* !_ARM_MAXIM_MAX32625_UART_H_ */
