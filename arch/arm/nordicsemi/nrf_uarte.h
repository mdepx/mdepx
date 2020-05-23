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

#ifndef _ARM_NORDICSEMI_NRF9160_UARTE_H_
#define _ARM_NORDICSEMI_NRF9160_UARTE_H_

#define	UARTE_TASKS_STARTRX	0x000	/* Start UART receiver */
#define	UARTE_TASKS_STOPRX	0x004	/* Stop UART receiver */
#define	UARTE_TASKS_STARTTX	0x008	/* Start UART transmitter */
#define	UARTE_TASKS_STOPTX	0x00C	/* Stop UART transmitter */
#define	UARTE_TASKS_FLUSHRX	0x02C	/* Flush RX FIFO into RX buffer */
#define	UARTE_SUBSCRIBE_STARTRX	0x080	/* Subscribe configuration for task STARTRX */
#define	UARTE_SUBSCRIBE_STOPRX	0x084	/* Subscribe configuration for task STOPRX */
#define	UARTE_SUBSCRIBE_STARTTX	0x088	/* Subscribe configuration for task STARTTX */
#define	UARTE_SUBSCRIBE_STOPTX	0x08C	/* Subscribe configuration for task STOPTX */
#define	UARTE_SUBSCRIBE_FLUSHRX	0x0AC	/* Subscribe configuration for task FLUSHRX */
#define	UARTE_EVENTS_CTS	0x100	/* CTS is activated (set low). Clear To Send. */
#define	UARTE_EVENTS_NCTS	0x104	/* CTS is deactivated (set high). Not Clear To Send. */
#define	UARTE_EVENTS_RXDRDY	0x108	/* Data received in RXD (but potentially not yet transferred to Data RAM) */
#define	UARTE_EVENTS_ENDRX	0x110	/* Receive buffer is filled up */
#define	UARTE_EVENTS_TXDRDY	0x11C	/* Data sent from TXD */
#define	UARTE_EVENTS_ENDTX	0x120	/* Last TX byte transmitted */
#define	UARTE_EVENTS_ERROR	0x124	/* Error detected */
#define	UARTE_EVENTS_RXTO	0x144	/* Receiver timeout */
#define	UARTE_EVENTS_RXSTARTED	0x14C	/* UART receiver has started */
#define	UARTE_EVENTS_TXSTARTED	0x150	/* UART transmitter has started */
#define	UARTE_EVENTS_TXSTOPPED	0x158	/* Transmitter stopped */
#define	UARTE_PUBLISH_CTS	0x180	/* Publish configuration for event CTS */
#define	UARTE_PUBLISH_NCTS	0x184	/* Publish configuration for event NCTS */
#define	UARTE_PUBLISH_RXDRDY	0x188	/* Publish configuration for event RXDRDY */
#define	UARTE_PUBLISH_ENDRX	0x190	/* Publish configuration for event ENDRX */
#define	UARTE_PUBLISH_TXDRDY	0x19C	/* Publish configuration for event TXDRDY */
#define	UARTE_PUBLISH_ENDTX	0x1A0	/* Publish configuration for event ENDTX */
#define	UARTE_PUBLISH_ERROR	0x1A4	/* Publish configuration for event ERROR */
#define	UARTE_PUBLISH_RXTO	0x1C4	/* Publish configuration for event RXTO */
#define	UARTE_PUBLISH_RXSTARTED	0x1CC	/* Publish configuration for event RXSTARTED */
#define	UARTE_PUBLISH_TXSTARTED	0x1D0	/* Publish configuration for event TXSTARTED */
#define	UARTE_PUBLISH_TXSTOPPED	0x1D8	/* Publish configuration for event TXSTOPPED */
#define	UARTE_SHORTS	0x200	/* Shortcuts between local events and tasks */
#define	UARTE_INTEN	0x300	/* Enable or disable interrupt */
#define	UARTE_INTENSET	0x304	/* Enable interrupt */
#define	 INTENSET_RXDRDY	(1 << 2)
#define	 INTENSET_TXDRDY	(1 << 7)
#define	 INTENSET_ENDTX		(1 << 8)
#define	 INTENSET_ENDRX		(1 << 4)
#define	UARTE_INTENCLR	0x308	/* Disable interrupt */
#define	UARTE_ERRORSRC	0x480	/* Error source (read/write one to clear). */
#define	UARTE_ENABLE	0x500	/* Enable UART */
#define	 ENABLE_EN		8
#define	UARTE_PSEL_RTS	0x508	/* Pin select for RTS signal */
#define	UARTE_PSEL_TXD	0x50C	/* Pin select for TXD signal */
#define	UARTE_PSEL_CTS	0x510	/* Pin select for CTS signal */
#define	UARTE_PSEL_RXD	0x514	/* Pin select for RXD signal */
#define	UARTE_BAUDRATE	0x524	/* Baud rate. Accuracy depends on the HFCLK source selected. */
#define	 BAUD_115200	0x01D7E000
#define	UARTE_RXD_PTR	0x534	/* Data pointer */
#define	UARTE_RXD_MAXCNT 0x538	/* Maximum number of bytes in receive buffer */
#define	UARTE_RXD_AMOUNT 0x53C	/* Number of bytes transferred in the last transaction */
#define	UARTE_TXD_PTR	0x544	/* Data pointer */
#define	UARTE_TXD_MAXCNT 0x548	/* Maximum number of bytes in transmit buffer */
#define	UARTE_TXD_AMOUNT 0x54C	/* Number of bytes transferred in the last transaction */
#define	UARTE_CONFIG	0x56C	/* Configuration of parity and hardware flow control */

struct nrf_uarte_softc {
	uint32_t base;
	uint8_t rx_data[16];
	int pin_tx;
	int pin_rx;
	uint32_t baudrate;
	void (*cb)(int c, void *arg);
	void *cb_arg;
};

void nrf_uarte_init(mdx_device_t dev,
    uint32_t base, uint8_t pin_tx, uint8_t pin_rx);
void nrf_uarte_intr(void *arg, int irq);
void nrf_uarte_register_callback(mdx_device_t dev,
    void (*func)(int c, void *arg), void *arg);

#endif /* !_ARM_NORDICSEMI_NRF9160_UARTE_H_ */
