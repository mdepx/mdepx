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

#ifndef _MIPS_MICROCHIP_PIC32_UART_H_
#define	_MIPS_MICROCHIP_PIC32_UART_H_

#include <sys/types.h>

#define	UART_MODE	0x00
#define	 MODE_ON	(1 << 15)
#define	UART_STA	0x10		/* Status and control */
#define	 STA_TRMT	(1 << 8)	/* Transmit Shift Reg is Empty */
#define	 STA_UTXBF	(1 << 9)	/* Transmit Buffer Full Status bit (read-only) */
#define	 STA_UTXEN	(1 << 10)	/* Transmit Enable */
#define	 STA_UTXBRK	(1 << 11)	/* Transmit Break bit */
#define	 STA_URXEN	(1 << 12)	/* Receiver Enable */
#define	 STA_URXDA	(1 << 0)	/* Receive Buffer Data Available bit (read-only) */
#define	UART_TXREG	0x20
#define	UART_RXREG	0x30
#define	UART_BRG	0x40

struct pic32_uart_softc {
	uint32_t base;
};

void pic32_uart_init(struct pic32_uart_softc *sc, uint32_t base,
    uint32_t speed, uint32_t cpu_freq, uint32_t div);
void pic32_putc(struct pic32_uart_softc *sc, char c);
uint8_t pic32_getc(struct pic32_uart_softc *sc);

#endif /* !_MIPS_MICROCHIP_PIC32_UART_H_ */
