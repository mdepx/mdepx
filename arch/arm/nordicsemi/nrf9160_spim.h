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

#ifndef _ARM_NORDICSEMI_NRF9160_SPIM_H_
#define _ARM_NORDICSEMI_NRF9160_SPIM_H_

#define	SPIM_TASKS_START	0x010	/* Start SPI transaction */
#define	SPIM_TASKS_STOP		0x014	/* Stop SPI transaction */
#define	SPIM_TASKS_SUSPEND	0x01C	/* Suspend SPI transaction */
#define	SPIM_TASKS_RESUME	0x020	/* Resume SPI transaction */
#define	SPIM_SUBSCRIBE_START	0x090	/* Subscribe configuration for task START */
#define	SPIM_SUBSCRIBE_STOP	0x094	/* Subscribe configuration for task STOP */
#define	SPIM_SUBSCRIBE_SUSPEND	0x09C	/* Subscribe configuration for task SUSPEND */
#define	SPIM_SUBSCRIBE_RESUME	0x0A0	/* Subscribe configuration for task RESUME */
#define	SPIM_EVENTS_STOPPED	0x104	/* SPI transaction has stopped */
#define	SPIM_EVENTS_ENDRX	0x110	/* End of RXD buffer reached */
#define	SPIM_EVENTS_END		0x118	/* End of RXD buffer and TXD buffer reached */
#define	SPIM_EVENTS_ENDTX	0x120	/* End of TXD buffer reached */
#define	SPIM_EVENTS_STARTED	0x14C	/* Transaction started */
#define	SPIM_PUBLISH_STOPPED	0x184	/* Publish configuration for event STOPPED */
#define	SPIM_PUBLISH_ENDRX	0x190	/* Publish configuration for event ENDRX */
#define	SPIM_PUBLISH_END	0x198	/* Publish configuration for event END */
#define	SPIM_PUBLISH_ENDTX	0x1A0	/* Publish configuration for event ENDTX */
#define	SPIM_PUBLISH_STARTED	0x1CC	/* Publish configuration for event STARTED */
#define	SPIM_SHORTS		0x200	/* Shortcuts between local events and tasks */
#define	SPIM_INTENSET		0x304	/* Enable interrupt */
#define	SPIM_INTENCLR		0x308	/* Disable interrupt */
#define	SPIM_ENABLE		0x500	/* Enable SPIM */
#define	SPIM_PSEL_SCK		0x508	/* Pin select for SCK */
#define	SPIM_PSEL_MOSI		0x50C	/* Pin select for MOSI signal */
#define	SPIM_PSEL_MISO		0x510	/* Pin select for MISO signal */
#define	SPIM_FREQUENCY		0x524	/* SPI frequency. Accuracy depends on the HFCLK source selected. */
#define	SPIM_RXD_PTR		0x534	/* Data pointer */
#define	SPIM_RXD_MAXCNT		0x538	/* Maximum number of bytes in receive buffer */
#define	SPIM_RXD_AMOUNT		0x53C	/* Number of bytes transferred in the last transaction */
#define	SPIM_RXD_LIST		0x540	/* EasyDMA list type */
#define	SPIM_TXD_PTR		0x544	/* Data pointer */
#define	SPIM_TXD_MAXCNT		0x548	/* Maximum number of bytes in transmit buffer */
#define	SPIM_TXD_AMOUNT		0x54C	/* Number of bytes transferred in the last transaction */
#define	SPIM_TXD_LIST		0x550	/* EasyDMA list type */
#define	SPIM_CONFIG		0x554	/* Configuration register */
#define	SPIM_ORC		0x5C0	/* Over-read character. Character clocked out in case and over-read of the TXD buffer. */

struct spim_softc {
	size_t base;
};

#endif /* !_ARM_NORDICSEMI_NRF9160_SPIM_H_ */
