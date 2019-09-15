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

#ifndef _SYS_RISCV_MICROSEMI_CORE_UART_H_
#define _SYS_RISCV_MICROSEMI_CORE_UART_H_

#define	UART_TXDATA		0x00
#define	UART_CTRL1		0x08
#define	UART_STATUS		0x10
#define	 STATUS_TXEMPTY		(1 << 0)
#define	UART_CTRL2		0x0C
#define	 CTRL2_DATA_7_BITS	(0)
#define	 CTRL2_DATA_8_BITS	(1 << 0)
#define	 CTRL2_NO_PARITY	(0)
#define	 CTRL2_EVEN_PARITY	(1 << 1)
#define	 CTRL2_ODD_PARITY	(3 << 1)

struct core_uart_softc {
	uint32_t base;
};

int core_uart_init(struct core_uart_softc *sc, uint32_t base,
    uint32_t cpu_freq, uint32_t baud_rate, uint32_t ctrl2);
void core_uart_putc(struct core_uart_softc *sc, char c);

#endif /* !_SYS_RISCV_MICROSEMI_CORE_UART_H_ */
