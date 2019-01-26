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

#ifndef _ARM_NXP_LPC43XX_USART_H_
#define _ARM_NXP_LPC43XX_USART_H_

#define	USART_RBR		0x000	/* Receiver Buffer Register. */
#define	USART_THR		0x000	/* Transmit Holding Register. */
#define	USART_DLL		0x000	/* Divisor Latch LSB. */
#define	USART_DLM		0x004	/* Divisor Latch MSB. */
#define	USART_IER		0x004	/* Interrupt Enable Register. /*
#define	USART_IIR		0x008	/* Interrupt ID Register. */
#define	USART_FCR		0x008	/* FIFO Control Register. */
#define	USART_LCR		0x00C	/* Line Control Register. */
#define	USART_LSR		0x014	/* Line Status Register. */
#define	USART_SCR		0x01C	/* Scratch Pad Register. */
#define	USART_ACR		0x020	/* Auto-baud Control Register. */
#define	USART_ICR		0x024	/* IrDA control reg (USART3 only) */
#define	USART_FDR		0x028	/* Fractional Divider Register. */
#define	USART_OSR		0x02C	/* Oversampling Register. */
#define	USART_HDEN		0x040	/* Half-duplex enable Register */
#define	USART_SCICTRL		0x048	/* Smart card interface control */
#define	USART_RS485CTRL		0x04C	/* RS-485/EIA-485 Control. */
#define	USART_RS485ADRMATCH	0x050	/* RS-485/EIA-485 address match. */
#define	USART_RS485DLY		0x054	/* RS-485/EIA-485 direction control */
#define	USART_SYNCCTRL		0x058	/* Synchronous mode control register */
#define	USART_TER		0x05C	/* Transmit Enable Register. */

struct usart_softc {
	size_t base;
};

#endif /* !_ARM_NXP_LPC43XX_USART_H_ */
