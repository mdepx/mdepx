/*-
 * Copyright (c) 2019 Ruslan Bukin <br@bsdpad.com>
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

#include <sys/pcpu.h>
#include <sys/list.h>

struct callout {
	int state;
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
	uint32_t (*count)(void *arg);
	void *arg;
	uint32_t count_saved[MAXCPU];
	uint32_t width;
	int state[MAXCPU];
#define	MI_TIMER_READY		0
#define	MI_TIMER_RUNNING	1
#define	MI_TIMER_EXCP		2
};

void callout_init(struct callout *c);
int callout_set_locked(struct callout *c, uint32_t ticks,
    void (*func)(void *arg), void *arg);
int callout_set(struct callout *c, uint32_t ticks,
    void (*func)(void *arg), void *arg);
int callout_callback(struct mi_timer *);
int callout_register(struct mi_timer *);
int callout_cancel(struct callout *c);
void callout_lock(int cpuid);
void callout_unlock(int cpuid);

#endif /* _SYS_CALLOUT_H_ */
