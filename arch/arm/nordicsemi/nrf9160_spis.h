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

#ifndef _ARM_NORDICSEMI_NRF9160_SPIS_H_
#define _ARM_NORDICSEMI_NRF9160_SPIS_H_

#define	SPIS_TASKS_ACQUIRE	0x024	/* Acquire SPI semaphore */
#define	SPIS_TASKS_RELEASE	0x028	/* Release SPI semaphore, enabling the SPI slave to acquire it */
#define	SPIS_SUBSCRIBE_ACQUIRE	0x0A4	/* Subscribe configuration for task ACQUIRE */
#define	SPIS_SUBSCRIBE_RELEASE	0x0A8	/* Subscribe configuration for task RELEASE */
#define	SPIS_EVENTS_END		0x104	/* Granted transaction completed */
#define	SPIS_EVENTS_ENDRX	0x110	/* End of RXD buffer reached */
#define	SPIS_EVENTS_ACQUIRED	0x128	/* Semaphore acquired */
#define	SPIS_PUBLISH_END	0x184	/* Publish configuration for event END */
#define	SPIS_PUBLISH_ENDRX	0x190	/* Publish configuration for event ENDRX */
#define	SPIS_PUBLISH_ACQUIRED	0x1A8	/* Publish configuration for event ACQUIRED */
#define	SPIS_SHORTS		0x200	/* Shortcuts between local events and tasks */
#define	SPIS_INTENSET		0x304	/* Enable interrupt */
#define	SPIS_INTENCLR		0x308	/* Disable interrupt */
#define	SPIS_SEMSTAT		0x400	/* Semaphore status register */
#define	SPIS_STATUS		0x440	/* Status from last transaction */
#define	SPIS_ENABLE		0x500	/* Enable SPI slave */
#define	SPIS_PSEL_SCK		0x508	/* Pin select for SCK. Deprecated */
#define	SPIS_PSEL_MISO		0x50C	/* Pin select for MISO signal. Deprecated */
#define	SPIS_PSEL_MOSI		0x510	/* Pin select for MOSI signal. Deprecated */
#define	SPIS_PSEL_CSN		0x514	/* Pin select for CSN signal. Deprecated */
#define	SPIS_PSELSCK		0x508	/* Pin select for SCK */
#define	SPIS_PSELMISO		0x50C	/* Pin select for MISO */
#define	SPIS_PSELMOSI		0x510	/* Pin select for MOSI */
#define	SPIS_PSELCSN		0x514	/* Pin select for CSN */
#define	SPIS_RXD_PTR		0x534	/* RXD data pointer */
#define	SPIS_RXD_MAXCNT		0x538	/* Maximum number of bytes in receive buffer */
#define	SPIS_RXD_AMOUNT		0x53C	/* Number of bytes received in last granted transaction */
#define	SPIS_RXDPTR		0x534	/* RXD data pointer. Deprecated */
#define	SPIS_MAXRX		0x538	/* Maximum number of bytes in receive buffer. Deprecated */
#define	SPIS_AMOUNTRX		0x53C	/* Number of bytes received in last granted transaction. Deprecated */
#define	SPIS_TXD_PTR		0x544	/* TXD data pointer */
#define	SPIS_TXD_MAXCNT		0x548	/* Maximum number of bytes in transmit buffer */
#define	SPIS_TXD_AMOUNT		0x54C	/* Number of bytes transmitted in last granted transaction */
#define	SPIS_TXDPTR		0x544	/* TXD data pointer. Deprecated */
#define	SPIS_MAXTX		0x548	/* Maximum number of bytes in transmit buffer.Deprecated */
#define	SPIS_AMOUNTTX		0x54C	/* Number of bytes transmitted in last granted transaction. Deprecated */
#define	SPIS_CONFIG		0x554	/* Configuration register */
#define	SPIS_DEF		0x55C	/* Default character. Character clocked out in case of an ignored transaction. */
#define	SPIS_ORC		0x5C0	/* Over-read character */

struct nrf_spis_softc {
	size_t base;
};

#endif /* !_ARM_NORDICSEMI_NRF9160_SPIS_H_ */
