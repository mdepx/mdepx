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

#ifndef _ARM_NORDICSEMI_NRF_TWIS_H_
#define _ARM_NORDICSEMI_NRF_TWIS_H_

#define	TWIS_TASKS_STOP		0x014	/* Stop TWI transaction */
#define	TWIS_TASKS_SUSPEND	0x01C	/* Suspend TWI transaction */
#define	TWIS_TASKS_RESUME	0x020	/* Resume TWI transaction */
#define	TWIS_TASKS_PREPARERX	0x030	/* Prepare the TWI slave to respond to a write command */
#define	TWIS_TASKS_PREPARETX	0x034	/* Prepare the TWI slave to respond to a read command */
#define	TWIS_SUBSCRIBE_STOP	0x094	/* Subscribe configuration for task STOP */
#define	TWIS_SUBSCRIBE_SUSPEND	0x09C	/* Subscribe configuration for task SUSPEND */
#define	TWIS_SUBSCRIBE_RESUME	0x0A0	/* Subscribe configuration for task RESUME */
#define	TWIS_SUBSCRIBE_PREPARERX 0x0B0	/* Subscribe configuration for task PREPARERX */
#define	TWIS_SUBSCRIBE_PREPARETX 0x0B4	/* Subscribe configuration for task PREPARETX */
#define	TWIS_EVENTS_STOPPED	0x104	/* TWI stopped */
#define	TWIS_EVENTS_ERROR	0x124	/* TWI error */
#define	TWIS_EVENTS_RXSTARTED	0x14C	/* Receive sequence started */
#define	TWIS_EVENTS_TXSTARTED	0x150	/* Transmit sequence started */
#define	TWIS_EVENTS_WRITE	0x164	/* Write command received */
#define	TWIS_EVENTS_READ	0x168	/* Read command received */
#define	TWIS_PUBLISH_STOPPED	0x184	/* Publish configuration for event STOPPED */
#define	TWIS_PUBLISH_ERROR	0x1A4	/* Publish configuration for event ERROR */
#define	TWIS_PUBLISH_RXSTARTED	0x1CC	/* Publish configuration for event RXSTARTED */
#define	TWIS_PUBLISH_TXSTARTED	0x1D0	/* Publish configuration for event TXSTARTED */
#define	TWIS_PUBLISH_WRITE	0x1E4	/* Publish configuration for event WRITE */
#define	TWIS_PUBLISH_READ	0x1E8	/* Publish configuration for event READ */
#define	TWIS_SHORTS		0x200	/* Shortcuts between local events and tasks */
#define	TWIS_INTEN		0x300	/* Enable or disable interrupt */
#define	TWIS_INTENSET		0x304	/* Enable interrupt */
#define	TWIS_INTENCLR		0x308	/* Disable interrupt */
#define	TWIS_ERRORSRC		0x4D0	/* Error source */
#define	TWIS_MATCH		0x4D4	/* Status register indicating which address had a match */
#define	TWIS_ENABLE		0x500	/* Enable TWIS */
#define	TWIS_PSEL_SCL		0x508	/* Pin select for SCL signal */
#define	TWIS_PSEL_SDA		0x50C	/* Pin select for SDA signal */
#define	TWIS_RXD_PTR		0x534	/* RXD Data pointer */
#define	TWIS_RXD_MAXCNT		0x538	/* Maximum number of bytes in RXD buffer */
#define	TWIS_RXD_AMOUNT		0x53C	/* Number of bytes transferred in the last RXD transaction */
#define	TWIS_TXD_PTR		0x544	/* TXD Data pointer */
#define	TWIS_TXD_MAXCNT		0x548	/* Maximum number of bytes in TXD buffer */
#define	TWIS_TXD_AMOUNT		0x54C	/* Number of bytes transferred in the last TXD transaction */
#define	TWIS_ADDRESS(n)		(0x588 + (n) * 0x4)	/* TWI slave address (0..1) */
#define	TWIS_CONFIG		0x594	/* Configuration register for the address match mechanism */
#define	TWIS_ORC		0x5C0	/* Over-read character. Character sent out in case of an over-read of the transmit buffer. */

struct nrf_twis_softc {
	size_t base;
};

#endif /* !_ARM_NORDICSEMI_NRF_TWIS_H_ */
