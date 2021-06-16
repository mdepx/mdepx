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

#include <sys/cdefs.h>
#include <sys/callout.h>
#include <sys/pcpu.h>
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/tick.h>

#include <machine/atomic.h>

static void
mdx_timer_tick(void *arg)
{
	struct mdx_timer *timer;

	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section.", __func__));

	timer = arg;
	timer->count_us += timer->period_us;

	mdx_callout_set_locked(&timer->c, timer->period_us,
	    mdx_timer_tick, timer);
}

struct mdx_timer *
mdx_timer_create(void)
{
	struct mdx_timer *timer;

	timer = malloc(sizeof(struct mdx_timer));
	if (timer == NULL)
		return (NULL);

	mdx_callout_init(&timer->c);

	return (timer);
}

int
mdx_timer_start(struct mdx_timer *timer, int period_us)
{
	int res;

	res = atomic_cmpset_int(&timer->started, 0, 1);
	if (!res)
		return (MDX_ERROR);

	timer->period_us = period_us;

	mdx_callout_set(&timer->c, timer->period_us, mdx_timer_tick, timer);

	return (0);
}

/*
 * Returns time in microseconds passed since timer started.
 */
uint64_t
mdx_timer_count(struct mdx_timer *timer)
{

	return (timer->count_us);
}
