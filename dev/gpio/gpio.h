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

#include <sys/device.h>

#ifndef _DEV_GPIO_GPIO_H_
#define	_DEV_GPIO_GPIO_H_

#define	MDX_GPIO_INPUT		(1 << 0)
#define	MDX_GPIO_OUTPUT		(1 << 1)
#define	MDX_GPIO_OPEN_DRAIN	(1 << 2)
#define	MDX_GPIO_PUSH_PULL	(1 << 3)
#define	MDX_GPIO_FLOATING	(1 << 4)
#define	MDX_GPIO_PULL_UP	(1 << 5)
#define	MDX_GPIO_PULL_DOWN	(1 << 6)
#define	MDX_GPIO_SPEED_LOW	(1 << 7)
#define	MDX_GPIO_SPEED_MEDIUM	(1 << 8)
#define	MDX_GPIO_SPEED_HIGH	(1 << 9)
#define	MDX_GPIO_ALT_FUNC	(1 << 10)	/* Alternate function */
#define	MDX_GPIO_ANALOG		(1 << 11)	/* Pin is in analog mode */

struct mdx_gpio_ops {
	int (*pin_set)(mdx_device_t dev, int bank, int pin, int value);
	int (*pin_get)(mdx_device_t dev, int bank, int pin);
	int (*pin_configure)(mdx_device_t dev, int bank, int pin, int flags);
};

int mdx_gpio_set(mdx_device_t dev, int bank, int pin, int value);
int mdx_gpio_get(mdx_device_t dev, int bank, int pin);
int mdx_gpio_configure(mdx_device_t dev, int bank, int pin, int flags);

#endif /* !_DEV_GPIO_GPIO_H_ */
