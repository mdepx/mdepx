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

#ifndef _DEV_GPIO_GPIO_H_
#define	_DEV_GPIO_GPIO_H_

#define	MDX_GPIO_INPUT		(1 << 0)
#define	MDX_GPIO_OUTPUT		(1 << 1)

struct mdx_gpio_ops {
	int (*pin_set)(void *arg, int pin, int value);
	int (*pin_get)(void *arg, int pin);
	int (*pin_configure)(void *arg, int pin, int flags);
};

struct mdx_gpio_bank {
	struct mdx_gpio_ops *ops;
	void *arg;
};

int mdx_gpio_bank_register(int bank_id, struct mdx_gpio_ops *ops, void *arg);
int mdx_gpio_configure(int bank_id, int pin, int flags);
int mdx_gpio_set(int bank_id, int pin, int value);
int mdx_gpio_get(int bank_id, int pin);

#endif /* !_DEV_GPIO_GPIO_H_ */
