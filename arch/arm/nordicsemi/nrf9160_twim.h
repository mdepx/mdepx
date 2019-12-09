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

#ifndef _ARM_NORDICSEMI_NRF9160_TWIM_H_
#define _ARM_NORDICSEMI_NRF9160_TWIM_H_

#define	TWIM_TASKS_STARTRX	0x000	/* Start TWI receive sequence */
#define	TWIM_TASKS_STARTTX	0x008	/* Start TWI transmit sequence */
#define	TWIM_TASKS_STOP		0x014	/* Stop TWI transaction. Must be issued while the TWI master is not suspended. */
#define	TWIM_TASKS_SUSPEND	0x01C	/* Suspend TWI transaction */
#define	TWIM_TASKS_RESUME	0x020	/* Resume TWI transaction */
#define	TWIM_SUBSCRIBE_STARTRX	0x080	/* Subscribe configuration for task STARTRX */
#define	TWIM_SUBSCRIBE_STARTTX	0x088	/* Subscribe configuration for task STARTTX */
#define	TWIM_SUBSCRIBE_STOP	0x094	/* Subscribe configuration for task STOP */
#define	TWIM_SUBSCRIBE_SUSPEND	0x09C	/* Subscribe configuration for task SUSPEND */
#define	TWIM_SUBSCRIBE_RESUME	0x0A0	/* Subscribe configuration for task RESUME */
#define	TWIM_EVENTS_STOPPED	0x104	/* TWI stopped */
#define	TWIM_EVENTS_ERROR	0x124	/* TWI error */
#define	TWIM_EVENTS_SUSPENDED	0x148	/* Last byte has been sent out after the SUSPEND task has been issued, TWI traffic is now suspended. */
#define	TWIM_EVENTS_RXSTARTED	0x14C	/* Receive sequence started */
#define	TWIM_EVENTS_TXSTARTED	0x150	/* Transmit sequence started */
#define	TWIM_EVENTS_LASTRX	0x15C	/* Byte boundary, starting to receive the last byte */
#define	TWIM_EVENTS_LASTTX	0x160	/* Byte boundary, starting to transmit the last byte */
#define	TWIM_PUBLISH_STOPPED	0x184	/* Publish configuration for event STOPPED */
#define	TWIM_PUBLISH_ERROR	0x1A4	/* Publish configuration for event ERROR */
#define	TWIM_PUBLISH_SUSPENDED	0x1C8	/* Publish configuration for event SUSPENDED */
#define	TWIM_PUBLISH_RXSTARTED	0x1CC	/* Publish configuration for event RXSTARTED */
#define	TWIM_PUBLISH_TXSTARTED	0x1D0	/* Publish configuration for event TXSTARTED */
#define	TWIM_PUBLISH_LASTRX	0x1DC	/* Publish configuration for event LASTRX */
#define	TWIM_PUBLISH_LASTTX	0x1E0	/* Publish configuration for event LASTTX */
#define	TWIM_SHORTS		0x200	/* Shortcuts between local events and tasks */
#define	TWIM_INTEN		0x300	/* Enable or disable interrupt */
#define	TWIM_INTENSET		0x304	/* Enable interrupt */
#define	TWIM_INTENCLR		0x308	/* Disable interrupt */
#define	TWIM_ERRORSRC		0x4C4	/* Error source */
#define	TWIM_ENABLE		0x500	/* Enable TWIM */
#define	TWIM_PSEL_SCL		0x508	/* Pin select for SCL signal */
#define	TWIM_PSEL_SDA		0x50C	/* Pin select for SDA signal */
#define	TWIM_FREQUENCY		0x524	/* TWI frequency. Accuracy depends on the HFCLK source selected. */
#define	TWIM_RXD_PTR		0x534	/* Data pointer */
#define	TWIM_RXD_MAXCNT		0x538	/* Maximum number of bytes in receive buffer */
#define	TWIM_RXD_AMOUNT		0x53C	/* Number of bytes transferred in the last transaction */
#define	TWIM_RXD_LIST		0x540	/* EasyDMA list type */
#define	TWIM_TXD_PTR		0x544	/* Data pointer */
#define	TWIM_TXD_MAXCNT		0x548	/* Maximum number of bytes in transmit buffer */
#define	TWIM_TXD_AMOUNT		0x54C	/* Number of bytes transferred in the last transaction */
#define	TWIM_TXD_LIST		0x550	/* EasyDMA list type */
#define	TWIM_ADDRESS		0x588	/* Address used in the TWI transfer */

struct nrf_twim_softc {
	size_t base;
};

#endif /* !_ARM_NORDICSEMI_NRF9160_TWIM_H_ */
