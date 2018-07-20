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

#ifndef _ARM_NORDICSEMI_NRF52_UARTE_H_
#define _ARM_NORDICSEMI_NRF52_UARTE_H_

#define	TASKS_STARTRX		0x000 /* Start UART receiver */
#define	TASKS_STOPRX		0x004 /* Stop UART receiver */
#define	TASKS_STARTTX		0x008 /* Start UART transmitter */
#define	TASKS_STOPTX		0x00C /* Stop UART transmitter */
#define	TASKS_FLUSHRX		0x02C /* Flush RX FIFO into RX buffer */
#define	EVENTS_CTS		0x100 /* CTS is activated (set low). Clear To Send. */
#define	EVENTS_NCTS		0x104 /* CTS is deactivated (set high). Not Clear To Send. */
#define	EVENTS_RXDRDY		0x108 /* Data received in RXD (but potentially not yet transferred to Data RAM) */
#define	EVENTS_ENDRX		0x110 /* Receive buffer is filled up */
#define	EVENTS_TXDRDY		0x11C /* Data sent from TXD */
#define	EVENTS_ENDTX		0x120 /* Last TX byte transmitted */
#define	EVENTS_ERROR		0x124 /* Error detected */
#define	EVENTS_RXTO		0x144 /* Receiver timeout */
#define	EVENTS_RXSTARTED	0x14C /* UART receiver has started */
#define	EVENTS_TXSTARTED	0x150 /* UART transmitter has started */
#define	EVENTS_TXSTOPPED	0x158 /* Transmitter stopped */
#define	SHORTS			0x200 /* Shortcut register */
#define	INTEN			0x300 /* Enable or disable interrupt */
#define	INTENSET		0x304 /* Enable interrupt */
#define	 RXDRDY			(1 << 2)
#define	 TXDRDY			(1 << 7)
#define	 ENDTX			(1 << 8)
#define	 ENDRX			(1 << 4)
#define	INTENCLR		0x308 /* Disable interrupt */
#define	ERRORSRC		0x480 /* Error source */
#define	ENABLE			0x500 /* Enable UART */
#define	 ENABLE_EN		8
#define	PSEL_RTS		0x508 /* Pin select for RTS signal */
#define	PSEL_TXD		0x50C /* Pin select for TXD signal */
#define	PSEL_CTS		0x510 /* Pin select for CTS signal */
#define	PSEL_RXD		0x514 /* Pin select for RXD signal */
#define	BAUDRATE		0x524 /* Baud rate. Accuracy depends on the HFCLK source selected. */
#define	 BAUD_115200		0x01D60000
#define	RXD_PTR			0x534 /* Data pointer */
#define	RXD_MAXCNT		0x538 /* Maximum number of bytes in receive buffer */
#define	RXD_AMOUNT		0x53C /* Number of bytes transferred in the last transaction */
#define	TXD_PTR			0x544 /* Data pointer */
#define	TXD_MAXCNT		0x548 /* Maximum number of bytes in transmit buffer */
#define	TXD_AMOUNT		0x54C /* Number of bytes transferred in the last transaction */
#define	CONFIG			0x56C /* Configuration of parity and hardware flow control */

struct uarte_softc {
	uint32_t base;
	uint8_t rx_data[16];
};

void uarte_init(struct uarte_softc *sc, uint32_t base);
void uarte_putc(struct uarte_softc *sc, char ch);
void uarte_intr(void *arg, uint32_t irqno);

#endif /* !_ARM_NORDICSEMI_NRF52_UARTE_H_ */
