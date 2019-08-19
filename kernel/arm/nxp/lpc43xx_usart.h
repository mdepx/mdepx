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
#define	USART_IER		0x004	/* Interrupt Enable Register. */
#define	USART_IIR		0x008	/* Interrupt ID Register. */
#define	USART_FCR		0x008	/* FIFO Control Register. */
#define	 FCR_FIFOEN		(1 << 0) /* FIFO Enable. */
#define	USART_LCR		0x00C	/* Line Control Register. */
#define	 LCR_WLS_S		0	/* Word Length Select. */
#define	 LCR_WLS_M		(0x3 << LCR_WLS_S)
#define	 LCR_WLS_5		(0x0 << LCR_WLS_S)
#define	 LCR_WLS_6		(0x1 << LCR_WLS_S)
#define	 LCR_WLS_7		(0x2 << LCR_WLS_S)
#define	 LCR_WLS_8		(0x3 << LCR_WLS_S)
#define	 LCR_DLAB		(1 << 7) /* Divisor Latch Access Bit. */
#define	USART_LSR		0x014	/* Line Status Register. */
#define	 LSR_TEMT		(1 << 6) /* Transmitter Empty. */
#define	USART_SCR		0x01C	/* Scratch Pad Register. */
#define	USART_ACR		0x020	/* Auto-baud Control Register. */
#define	USART_ICR		0x024	/* IrDA control reg (USART3 only) */
#define	USART_FDR		0x028	/* Fractional Divider Register. */
#define	 FDR_DIVADDVAL_S	0 /* Baud rate generation pre-scaler divisor.*/
#define	 FDR_DIVADDVAL_M	(0xf << FDR_DIVADDVAL_S)
#define	 FDR_MULVAL_S		4 /* Baud rate pre-scaler multiplier value. */
#define	 FDR_MULVAL_M		(0xf << FDR_MULVAL_S)
#define	USART_OSR		0x02C	/* Oversampling Register. */
#define	USART_HDEN		0x040	/* Half-duplex enable Register */
#define	USART_SCICTRL		0x048	/* Smart card interface control */
#define	USART_RS485CTRL		0x04C	/* RS-485/EIA-485 Control. */
#define	USART_RS485ADRMATCH	0x050	/* RS-485/EIA-485 address match. */
#define	USART_RS485DLY		0x054	/* RS-485/EIA-485 direction control */
#define	USART_SYNCCTRL		0x058	/* Synchronous mode control register */
#define	USART_TER		0x05C	/* Transmit Enable Register. */

struct usart_baud_entry {
	uint8_t dlm;
	uint8_t dll;
	uint8_t divaddval;
	uint8_t mulval;
};

struct usart_softc {
	size_t base;
};

void lpc43xx_usart_putc(struct usart_softc *sc, char ch);
void lpc43xx_usart_init(struct usart_softc *sc, uint32_t base,
    const struct usart_baud_entry *baud);

#endif /* !_ARM_NXP_LPC43XX_USART_H_ */
