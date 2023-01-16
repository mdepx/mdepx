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

#include <dev/gpio/gpio.h>

int
mdx_gpio_set(mdx_device_t dev, int pin, int value)
{
	struct mdx_gpio_ops *ops;
	int error;

	ops = dev->ops;

	error = ops->pin_set(dev, pin, value);

	return (error);
}

int
mdx_gpio_get(mdx_device_t dev, int pin)
{
	struct mdx_gpio_ops *ops;
	int value;

	ops = dev->ops;

	value = ops->pin_get(dev, pin);

	return (value);
}

int
mdx_gpio_configure(mdx_device_t dev, int pin, int flags)
{
	struct mdx_gpio_ops *ops;
	int error;

	ops = dev->ops;

	error = ops->pin_configure(dev, pin, flags);

	return (error);
}

int
mdx_gpio_set_function(mdx_device_t dev, int pin, int function)
{
	struct mdx_gpio_ops *ops;
	int error;

	ops = dev->ops;

	error = ops->pin_set_function(dev, pin, function);

	return (error);
}

int
mdx_gpio_set_dir(mdx_device_t dev, int pin, int direction)
{
	struct mdx_gpio_ops *ops;
	int error;

	ops = dev->ops;

	error = ops->pin_set_dir(dev, pin, direction);

	return (error);
}
