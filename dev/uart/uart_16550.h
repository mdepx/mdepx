/*-
 * Copyright (c) 2019 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _DEV_UART_UART_16550_H_
#define _DEV_UART_UART_16550_H_

#include <dev/uart/uart.h>

#define	REG_DATA		0x00 /* data register (rw) */
#define	REG_IER			0x01 /* interrupt enable register (wo) */
#define	 IER_ERXRDY		(1 << 0)
#define	REG_IIR			0x02 /* interrupt identification register(ro) */
#define	REG_FCR			0x02 /* fifo control register (w) */
#define	REG_LCR			0x03 /* line control register (rw) */
#define	 LCR_BITS_S		0 /* Data width */
#define	 LCR_BITS_M		(0x3 << LCR_BITS_S)
#define	 LCR_BITS_5		(0x0 << LCR_BITS_S)
#define	 LCR_BITS_6		(0x1 << LCR_BITS_S)
#define	 LCR_BITS_7		(0x2 << LCR_BITS_S)
#define	 LCR_BITS_8		(0x3 << LCR_BITS_S)
#define	 LCR_STOPB		(1 << 2)
#define	 LCR_DLAB		(1 << 7)
#define	 LCR_PARITY_S		3
#define	 LCR_PARITY_NONE	(0 << LCR_PARITY_S)
#define	 LCR_PARITY_ODD		(1 << LCR_PARITY_S)
#define	 LCR_PARITY_EVEN	(3 << LCR_PARITY_S)
#define	REG_MCR			0x04 /* modem control register (rw) */
#define	 MCR_RTS		(1 << 1)
#define	REG_LSR			0x05 /* line status register (rw) */
#define	 LSR_RXRDY		(1 << 0)
#define	 LSR_THRE		(1 << 5)
#define	REG_MSR			0x06 /* modem status register (rw) */
#define	REG_SCR			0x07 /* scratch register (rw) */

#define	REG_DLL			0x00 /* divisor latch low (rw) */
#define	REG_DLM			0x01 /* divisor latch high (rw) */

struct uart_16550_softc {
	capability base;
	uint8_t reg_shift;
	uint32_t bus_freq;
};

void uart_16550_init(mdx_device_t dev, capability base,
    uint8_t reg_shift, uint32_t bus_freq);

#endif /* !_DEV_UART_UART_16550_H_ */
