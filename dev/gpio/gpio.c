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

#include <sys/cdefs.h>
#include <sys/console.h>

#include <dev/gpio/gpio.h>

static struct mdx_gpio_bank gpio_bank[MDX_GPIO_NBANKS];

int
mdx_gpio_bank_register(int bank_id, struct mdx_gpio_ops *ops, void *arg)
{

	if (bank_id >= MDX_GPIO_NBANKS)
		return (-1);

	gpio_bank[bank_id].ops = ops;
	gpio_bank[bank_id].arg = arg;

	return (0);
}

int
mdx_gpio_set(int bank_id, int pin, int value)
{
	struct mdx_gpio_bank *bank;
	int error;

	bank = &gpio_bank[bank_id];

	error = bank->ops->pin_set(bank->arg, pin, value);

	return (error);
}

int
mdx_gpio_get(int bank_id, int pin)
{
	struct mdx_gpio_bank *bank;
	int value;

	bank = &gpio_bank[bank_id];

	value = bank->ops->pin_get(bank->arg, pin);

	return (value);
}

int
mdx_gpio_configure(int bank_id, int pin, int flags)
{
	struct mdx_gpio_bank *bank;
	int error;

	bank = &gpio_bank[bank_id];

	error = bank->ops->pin_configure(bank->arg, pin, flags);

	return (error);
}
