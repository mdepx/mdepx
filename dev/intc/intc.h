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

#ifndef _DEV_INTC_INTC_H_
#define	_DEV_INTC_INTC_H_

struct mdx_intc_ops {
	int (*setup)(mdx_device_t dev, int irq,
	    void (*handler)(void *arg, int irq),
	    void *arg);
	void (*enable)(mdx_device_t dev, int irq);
	void (*disable)(mdx_device_t dev, int irq);
	void (*set)(mdx_device_t dev, int irq);
	void (*clear)(mdx_device_t dev, int irq);
	void (*set_prio)(mdx_device_t dev, int irq, int prio);
};

int mdx_intc_setup(mdx_device_t dev, int irq,
    void (*handler)(void *arg, int irq),
    void *arg);
void mdx_intc_enable(mdx_device_t dev, int irq);
void mdx_intc_disable(mdx_device_t dev, int irq);
void mdx_intc_set(mdx_device_t dev, int irq);
void mdx_intc_clear(mdx_device_t dev, int irq);
void mdx_intc_set_prio(mdx_device_t dev, int irq, int prio);

#endif /* !_DEV_INTC_INTC_H_ */
