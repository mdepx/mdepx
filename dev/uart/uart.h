/*-
 * Copyright (c) 2020 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _DEV_UART_UART_H_
#define _DEV_UART_UART_H_

#include <sys/device.h>

typedef enum uart_databits {
	UART_DATABITS_5 = 5,
	UART_DATABITS_6,
	UART_DATABITS_7,
	UART_DATABITS_8,
} uart_databits_t;

typedef enum uart_parity {
	UART_PARITY_NONE,
	UART_PARITY_ODD,
	UART_PARITY_EVEN,
} uart_parity_t;

typedef enum uart_stopbits {
	UART_STOPBITS_1,
	UART_STOPBITS_1_5,
	UART_STOPBITS_2
} uart_stopbits_t;

struct mdx_uart_ops {
	void (*putc)(mdx_device_t dev, int c);
	int (*rxready)(mdx_device_t dev);
	int (*getc)(mdx_device_t dev);
	void (*setup)(mdx_device_t dev, int baudrate,
	    enum uart_databits databits,
	    enum uart_stopbits stopbits,
	    enum uart_parity parity);
};

void mdx_uart_putc(mdx_device_t dev, int c);
void mdx_uart_setup(mdx_device_t dev, int baudrate,
    enum uart_databits databits,
    enum uart_stopbits stopbits,
    enum uart_parity parity);

#endif /* !_DEV_UART_UART_H_ */
