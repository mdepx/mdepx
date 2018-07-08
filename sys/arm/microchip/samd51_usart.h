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

#ifndef _ARM_MICROCHIP_SAMD51_USART_H_
#define _ARM_MICROCHIP_SAMD51_USART_H_

#define	USART_CTRLA	0x00 /* Control A */
#define	USART_CTRLB	0x04 /* Control B */
#define	USART_CTRLC	0x08 /* Control C */
#define	USART_BAUD	0x0C /* Baud */
#define	USART_RXPL	0x0E /* Receive Pulse Length Register */
#define	USART_INTENCLR	0x14 /* Interrupt Enable Clear */
#define	USART_INTENSET	0x16 /* Interrupt Enable Set */
#define	USART_INTFLAG	0x18 /* Interrupt Flag Status and Clear */
#define	USART_STATUS	0x1A /* Status */
#define	USART_SYNCBUSY	0x1C /* Synchronization Busy */
#define	USART_RXERRCNT	0x20 /* Receive Error Count */
#define	USART_LENGTH	0x22 /* Length */
#define	USART_DATA	0x28 /* Data */
#define	USART_DBGCTRL	0x30 /* Debug Control */

struct samd51_usart_softc {
	uint32_t base;
};

#endif /* !_ARM_MICROCHIP_SAMD51_USART_H_ */
