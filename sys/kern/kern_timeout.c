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

#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/errno.h>
#include <sys/callout.h>
#include <sys/thread.h>

#include <machine/cpufunc.h>

#define	CALLOUT_DEBUG
#undef	CALLOUT_DEBUG

#ifdef	CALLOUT_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

static struct callout *callouts;
static struct callout *callouts_tail;
static struct mi_timer *mi_tmr;

#ifdef CALLOUT_DEBUG
static void
callout_dump(void)
{
	struct callout *c;
	int i;

	c = callouts;
	if (c == NULL)
		return;

	i = 0;
	do {
		dprintf("callout%d: usec %u\n", i, c->usec);
		i += 1;
		c = c->next;
	} while (c);
}
#endif

void
callout_init(struct callout *c)
{

	c->state = 0;
}

static uint32_t
get_elapsed(void)
{
	uint32_t ticks_elapsed;
	uint32_t usec_elapsed;
	uint32_t count;

	count = mi_tmr->count(mi_tmr->arg);
	if (count > mi_tmr->count_saved)
		ticks_elapsed = (count - mi_tmr->count_saved);
	else
		ticks_elapsed = (0xffffffff - mi_tmr->count_saved + count);
	mi_tmr->count_saved = count;
	usec_elapsed = (ticks_elapsed / mi_tmr->ticks_per_usec);

	dprintf("%s: usec_elapsed %u\n", __func__, usec_elapsed);

	return (usec_elapsed);
}

static void
fix_timeouts(void)
{
	struct callout *c;
	uint32_t usec_elapsed;

	usec_elapsed = get_elapsed();

	for (c = callouts; c != NULL; c = c->next)
		if (usec_elapsed >= c->usec)
			c->usec = 0;
		else
			c->usec -= usec_elapsed;
}

static void
callout_add(struct callout *c0)
{
	struct callout *c;

	KASSERT(curthread != NULL, ("curthread is NULL"));
	KASSERT(curthread->td_critnest > 0, ("Not in critical section."));

	dprintf("%s\n", __func__);

	if (callouts == NULL) {
		KASSERT(mi_tmr->running == 0,
		    ("mi timer is running, but callouts == NULL"));
		callouts = c0;
		callouts_tail = c0;
		c0->next = NULL;
		c0->prev = NULL;
		mi_tmr->count_saved = mi_tmr->count(mi_tmr->arg);
		mi_tmr->running = 1;
		mi_tmr->start(mi_tmr->arg, callouts->usec);

#ifdef CALLOUT_DEBUG
		callout_dump();
#endif
		return;
	}

	if (mi_tmr->running)
		fix_timeouts();

	for (c = callouts;
	    (c != NULL && c->usec < c0->usec);
	    (c = c->next));

	if (c != NULL) {
		c0->next = c;
		c0->prev = c->prev;
		c->prev = c0;
		if (c0->prev == NULL) {
			callouts = c0;
			if (mi_tmr->running)
				mi_tmr->start(mi_tmr->arg,
				    callouts->usec);
		} else
			c0->prev->next = c0;
	} else {
		callouts_tail->next = c0;
		c0->prev = callouts_tail;
		callouts_tail = c0;
		callouts_tail->next = NULL;
	}

#ifdef CALLOUT_DEBUG
	callout_dump();
#endif
}

int
callout_reset(struct callout *c, uint32_t usec,
    void (*func)(void *arg), void *arg)
{

	critical_enter();

	if (c->flags & CALLOUT_FLAG_RUNNING) {
		critical_exit();
		return (-1);
	}

	dprintf("%s: adding callout usec %d\n", __func__, usec);

	if (usec == 0) {
		c->state = 1;
		critical_exit();
		return (0);
	}

	c->usec_orig = usec;
	c->usec = usec;
	c->func = func;
	c->arg = arg;

	callout_add(c);
	c->flags |= CALLOUT_FLAG_RUNNING;
	critical_exit();

	return (0);
}

int
callout_callback(struct mi_timer *mt)
{
	struct callout *c, *old, *tmp;

	KASSERT(curthread->td_critnest > 0, ("Not in critical section."));

	dprintf("%s\n", __func__);

	KASSERT(mi_tmr != NULL,
	    ("%s: mi timer is not registered", __func__));
	KASSERT(mi_tmr == mt,
	    ("%s: callback for the wrong device", __func__));

	if (callouts == NULL)
		return (0);

	KASSERT(callouts != NULL,
	    ("%s: callouts is NULL", __func__));

	fix_timeouts();

	old = callouts;

	for (c = callouts; c != NULL; c = c->next)
		if (c->usec != 0)
			break;

	if (c != NULL && c->prev != NULL) {
		c->prev->next = NULL;
		c->prev = NULL;
	}

	callouts = c;
	if (callouts == NULL)
		callouts_tail = NULL;

	KASSERT(callouts != old, ("wrong"));

	mi_tmr->running = 0;

	c = old;
	while (c != NULL) {
		tmp = c->next;	
		c->state = 1;
		c->flags &= ~CALLOUT_FLAG_RUNNING;
		if (c->func)
			c->func(c->arg);
		c = tmp;
	}

	if (callouts != NULL) {
		if (mi_tmr->running == 0) {
			mi_tmr->running = 1;
			mi_tmr->start(mi_tmr->arg, callouts->usec);
		}
	} else {
		mi_tmr->running = 0;
		mi_tmr->stop(mi_tmr->arg);
	}

	return (0);
}

int
callout_register(struct mi_timer *mt)
{

	if (mi_tmr != NULL)
		return (EEXIST);

	mi_tmr = mt;
	mi_tmr->running = 0;

	return (0);
}
