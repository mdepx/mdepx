/*-
 * Copyright (c) 2021 Ruslan Bukin <br@bsdpad.com>
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

#ifndef	_RP2040_UART_H_
#define	_RP2040_UART_H_

#define	RP2040_UART_UARTDR		0x000
#define	RP2040_UART_UARTRSR		0x004
#define	RP2040_UART_UARTFR		0x018
#define	 UART_UARTFR_TXFF		(1 << 5) /* Transmit FIFO full */
#define	RP2040_UART_UARTILPR		0x020
#define	RP2040_UART_UARTIBRD		0x024
#define	RP2040_UART_UARTFBRD		0x028
#define	RP2040_UART_UARTLCR_H		0x02c
#define	 UART_UARTLCR_H_WLEN_S		5
#define	 UART_UARTLCR_H_WLEN_M		(0x3 << UART_UARTLCR_H_WLEN_S)
#define	 UART_UARTLCR_H_WLEN_8BIT	(0x3 << UART_UARTLCR_H_WLEN_S)
#define	 UART_UARTLCR_H_FEN		(1 << 4) /* RX/TX fifos are enabled */
#define	RP2040_UART_UARTCR		0x030
#define	 UART_UARTCR_RXE		(1 << 9) /* Receive enable */
#define	 UART_UARTCR_TXE		(1 << 8) /* Transmit enable */
#define	 UART_UARTCR_UARTEN		(1 << 0)
#define	RP2040_UART_UARTIFLS		0x034
#define	RP2040_UART_UARTIMSC		0x038
#define	RP2040_UART_UARTRIS		0x03c
#define	RP2040_UART_UARTMIS		0x040
#define	RP2040_UART_UARTICR		0x044
#define	RP2040_UART_UARTDMACR		0x048
#define	RP2040_UART_UARTPERIPHID0	0xfe0
#define	RP2040_UART_UARTPERIPHID1	0xfe4
#define	RP2040_UART_UARTPERIPHID2	0xfe8
#define	RP2040_UART_UARTPERIPHID3	0xfec
#define	RP2040_UART_UARTPCELLID0	0xff0
#define	RP2040_UART_UARTPCELLID1	0xff4
#define	RP2040_UART_UARTPCELLID2	0xff8
#define	RP2040_UART_UARTPCELLID3	0xffc

struct rp2040_uart_softc {
	uint32_t base;
	uint32_t baudrate;
};

void rp2040_uart_intr(void *arg, int irq);
void rp2040_uart_init(mdx_device_t dev, uint32_t base);

#endif /* !_RP2040_UART_H_ */
