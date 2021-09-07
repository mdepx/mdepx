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

#ifndef _SYS_KERNEL_H_
#define _SYS_KERNEL_H_

enum mdx_subsystem_id {
	SI_SUB_BOARD,
	SI_SUB_DRIVERS,
	SI_SUB_CLOCKS,
};

enum mdx_order_id {
	SI_ORDER_FIRST,
	SI_ORDER_SECOND,
	SI_ORDER_THIRD,
	SI_ORDER_FOURTH,
	SI_ORDER_FIFTH,
	SI_ORDER_MIDDLE,
	SI_ORDER_ANY,
};

struct mdx_sysinit {
	enum mdx_subsystem_id	subsystem;
	enum mdx_order_id	order;
	void			(*func)(void *);
	void			*arg;
};

#define SYSINIT(name, subsystem, order, func, arg)	\
	struct mdx_sysinit name##_sysinit		\
	    __attribute__((__section__(".sysinit"),	\
	    __used__, __aligned__(4))) = {		\
		subsystem,				\
		order,					\
		func,					\
		arg,					\
	};

#endif /* !_SYS_KERNEL_H_ */
