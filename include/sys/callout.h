/*-
 * Copyright (c) 2019-2020 Ruslan Bukin <br@bsdpad.com>
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory (Department of Computer Science and
 * Technology) under DARPA contract HR0011-18-C-0016 ("ECATS"), as part of the
 * DARPA SSITH research programme.
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

#ifndef	_SYS_CALLOUT_H_
#define	_SYS_CALLOUT_H_

#include <sys/list.h>

struct mdx_callout {
	int state;
#define	CALLOUT_STATE_READY	0
#define	CALLOUT_STATE_FIRED	1
	uint32_t ticks;
	uint32_t ticks_orig;
	void (*func)(void *arg);
	void *arg;
	int flags;
#define	CALLOUT_FLAG_ACTIVE	(1 << 0)
	struct entry node;
	int cpuid;
};

struct mi_timer {
	void (*start)(void *arg, uint32_t ticks);
	void (*stop)(void *arg);
	ticks_t (*count)(void *arg);
	void *arg;
	uint32_t count_saved[MDX_CPU_MAX];
	uint32_t maxcnt;
	uint32_t frequency;
	uint32_t (*usec_to_ticks)(uint32_t freq, uint32_t usec);
	uint32_t (*ticks_to_usec)(uint32_t freq, uint32_t ticks);
	int state[MDX_CPU_MAX];
#define	MI_TIMER_STATE_READY		0
#define	MI_TIMER_STATE_RUNNING		1
#define	MI_TIMER_STATE_EXCEPTION	2
};

typedef struct mdx_callout mdx_callout_t;

void mdx_callout_init(struct mdx_callout *c);

int mdx_callout_set_ticks_locked(struct mdx_callout *c, uint32_t ticks,
    void (*func)(void *arg), void *arg);
int mdx_callout_set_locked(struct mdx_callout *c, uint32_t usec,
    void (*func)(void *arg), void *arg);

int mdx_callout_set_ticks(struct mdx_callout *c, uint32_t ticks,
    void (*func)(void *arg), void *arg);
int mdx_callout_set(struct mdx_callout *c, uint32_t usec,
    void (*func)(void *arg), void *arg);

int mdx_callout_callback(struct mi_timer *);
int mdx_callout_register(struct mi_timer *);
int mdx_callout_cancel(struct mdx_callout *c);

uint32_t mdx_cputime_usec_to_ticks(uint32_t freq, uint32_t usec);
uint32_t mdx_cputime_usec_to_ticks_1mhz(uint32_t freq, uint32_t usec);
uint32_t mdx_cputime_ticks_to_usec_1mhz(uint32_t freq, uint32_t ticks);

uint32_t mdx_callout_usec_to_ticks(uint32_t usec);
uint32_t mdx_callout_ticks_to_usec(uint32_t usec);
uint32_t mdx_callout_ticks(struct mdx_callout *c);
uint32_t mdx_callout_usec(struct mdx_callout *c);

#endif /* _SYS_CALLOUT_H_ */
