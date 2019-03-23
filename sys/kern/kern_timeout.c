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

static void __unused
callout_dump(void)
{
	struct callout *c;

	for (c = callouts; c != NULL; c = c->next)
		printf("callout: %u\n", c->ticks);
}

void
callout_init(struct callout *c)
{

	c->state = 0;
	c->flags = 0;
}

static uint32_t
get_elapsed(void)
{
	uint32_t elapsed;
	uint32_t count;

	count = mi_tmr->count(mi_tmr->arg);
	if (count > mi_tmr->count_saved)
		elapsed = (count - mi_tmr->count_saved);
	else
		elapsed = (0xffffffff - mi_tmr->count_saved + count);
	mi_tmr->count_saved = count;

	dprintf("%s: elapsed %u\n", __func__, elapsed);

	return (elapsed);
}

static void
fix_timeouts(void)
{
	struct callout *c;
	uint32_t ticks_elapsed;

	ticks_elapsed = get_elapsed();

	for (c = callouts; c != NULL; c = c->next)
		if (ticks_elapsed >= c->ticks)
			c->ticks = 0;
		else
			c->ticks -= ticks_elapsed;
}

static void
callout_add(struct callout *c0)
{
	struct callout *c;

	KASSERT(curthread != NULL, ("curthread is NULL"));
	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section.", __func__));

	dprintf("%s: ticks %d\n", __func__, c0->ticks);

	if (callouts == NULL) {
		KASSERT(mi_tmr->running == 0,
		    ("mi timer is running, but callouts == NULL"));
		callouts = c0;
		callouts_tail = c0;
		c0->next = NULL;
		c0->prev = NULL;
		mi_tmr->count_saved = mi_tmr->count(mi_tmr->arg);
		mi_tmr->running = 1;
		mi_tmr->start(mi_tmr->arg, callouts->ticks);

#ifdef CALLOUT_DEBUG
		callout_dump();
#endif
		return;
	}

	if (mi_tmr->running)
		fix_timeouts();

	for (c = callouts;
	    (c != NULL && c->ticks < c0->ticks);
	    (c = c->next));

	if (c != NULL) {
		c0->next = c;
		c0->prev = c->prev;
		c->prev = c0;
		if (c0->prev == NULL) {
			callouts = c0;
			if (mi_tmr->running)
				mi_tmr->start(mi_tmr->arg,
				    callouts->ticks);
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
callout_cancel(struct callout *c)
{

	KASSERT(mi_tmr != NULL, ("mi timer is NULL"));
	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section.", __func__));

	if ((c->flags & CALLOUT_FLAG_ACTIVE) == 0) {
		/* Callout c is not in the callouts queue. */
		return (-1);
	}

	if (c == callouts) {
		/*
		 * c is the currently running callout or the next
		 * one to run (which is possible if this function
		 * is called from another callout handler).
		 */
		if (mi_tmr->running)
			fix_timeouts();
		if (c->next) {
			callouts = c->next;
			callouts->prev = NULL;
			if (mi_tmr->running)
				mi_tmr->start(mi_tmr->arg,
				    callouts->ticks);
		} else {
			callouts = NULL;
			callouts_tail = NULL;
			mi_tmr->running = 0;
			mi_tmr->stop(mi_tmr->arg);
		}
	} else {
		/*
		 * Active but not running callout.
		 * Just remove it from the list.
		 */
		if (c->next) {
			c->prev->next = c->next;
			c->next->prev = c->prev;
		} else {
			c->prev->next = NULL;
			callouts_tail = c->prev;
		}
	}

	c->flags &= ~CALLOUT_FLAG_ACTIVE;

	return (0);
}

int
callout_reset(struct callout *c, uint32_t ticks,
    void (*func)(void *arg), void *arg)
{

	KASSERT(mi_tmr != NULL, ("mi timer is NULL"));

	critical_enter();

	if (c->flags & CALLOUT_FLAG_ACTIVE) {
		critical_exit();
		return (-1);
	}

	dprintf("%s: adding callout ticks %d\n", __func__, ticks);

	if (ticks == 0) {
		c->state = 1;
		critical_exit();
		return (0);
	}

	c->ticks_orig = ticks;
	c->ticks = ticks;
	c->func = func;
	c->arg = arg;

	callout_add(c);
	c->flags |= CALLOUT_FLAG_ACTIVE;
	critical_exit();

	return (0);
}

int
callout_callback(struct mi_timer *mt)
{
	struct callout *c, *old, *tmp;

	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section.", __func__));

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
		if (c->ticks != 0)
			break;

	if (c != NULL && c->prev != NULL) {
		c->prev->next = NULL;
		c->prev = NULL;
	}

	callouts = c;
	if (callouts == NULL)
		callouts_tail = NULL;

	mi_tmr->running = 0;

	if (callouts != old) {
		c = old;
		while (c != NULL) {
			tmp = c->next;
			c->state = 1;
			c->flags &= ~CALLOUT_FLAG_ACTIVE;
			if (c->func)
				c->func(c->arg);
			c = tmp;
		}
	}

	if (callouts != NULL) {
		if (mi_tmr->running == 0) {
			mi_tmr->running = 1;
			mi_tmr->start(mi_tmr->arg, callouts->ticks);
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
