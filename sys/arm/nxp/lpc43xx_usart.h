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

#define	USART_RBR		0x000	/* Receiver Buffer Register. Contains the next received character to be read (DLAB = 0). */
#define	USART_THR		0x000	/* Transmit Holding Register. The next character to be transmitted is written here (DLAB = 0). */
#define	USART_DLL		0x000	/* Divisor Latch LSB. Least significant byte of the baud rate divisor value. The full divisor is used to generate a baud rate from the fractional rate divider (DLAB = 1). */
#define	USART_DLM		0x004	/* Divisor Latch MSB. Most significant byte of the baud rate divisor value. The full divisor is used to generate a baud rate from the fractional rate divider (DLAB = 1). */
#define	USART_IER		0x004	/* Interrupt Enable Register. Contains individual interrupt enable bits for the 7 potential USART interrupts (DLAB = 0). */
#define	USART_IIR		0x008	/* Interrupt ID Register. Identifies which interrupt(s) are pending. */
#define	USART_FCR		0x008	/* FIFO Control Register. Controls USART FIFO usage and modes. */
#define	USART_LCR		0x00C	/* Line Control Register. Contains controls for frame formatting and break generation. */
#define	USART_LSR		0x014	/* Line Status Register. Contains flags for transmit and receive status, including line errors. */
#define	USART_SCR		0x01C	/* Scratch Pad Register. Eight-bit temporary storage for software. */
#define	USART_ACR		0x020	/* Auto-baud Control Register. Contains controls for the auto-baud feature. */
#define	USART_ICR		0x024	/* IrDA control register (USART3 only) */
#define	USART_FDR		0x028	/* Fractional Divider Register. Generates a clock input for the baud rate divider. */
#define	USART_OSR		0x02C	/* Oversampling Register. Controls the degree of oversampling during each bit time. */
#define	USART_HDEN		0x040	/* Half-duplex enable Register */
#define	USART_SCICTRL		0x048	/* Smart card interface control register */
#define	USART_RS485CTRL		0x04C	/* RS-485/EIA-485 Control. Contains controls to configure various aspects of RS-485/EIA-485 modes. */
#define	USART_RS485ADRMATCH	0x050	/* RS-485/EIA-485 address match. Contains the address match value for RS-485/EIA-485 mode. */
#define	USART_RS485DLY		0x054	/* RS-485/EIA-485 direction control delay. */
#define	USART_SYNCCTRL		0x058	/* Synchronous mode control register. */
#define	USART_TER		0x05C	/* Transmit Enable Register. Turns off USART transmitter for use with software flow control. */

struct usart_softc {
	size_t base;
};

#endif /* !_ARM_NXP_LPC43XX_USART_H_ */
