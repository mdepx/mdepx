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

#include <sys/cdefs.h>
#include <sys/callout.h>
#include <sys/errno.h>
#include <sys/pcpu.h>
#include <sys/spinlock.h>
#include <sys/systm.h>
#include <sys/thread.h>

/*
 * A hardware timer must be configured by a driver as:
 * (1) upcouning;
 * (2) free-running with automatic value reloading;
 * (3) in the SMP case it must be per-cpu timer.
 *
 * One-shot timers are not supported.
 *
 * Example drivers see at:
 *     grep mdx_callout_register arch/ -R
 */

static struct mi_timer *mi_tmr;
static struct entry callouts_list[MDX_CPU_MAX];

/*
 * Lock is only required in the SMP case when we need to cancel a callout
 * that belongs to another CPU, otherwise callout code is lock-free.
 */

#ifdef MDX_SCHED_SMP
static struct spinlock l[MDX_CPU_MAX];
#define	callout_lock(cpuid)	sl_lock(&(l)[cpuid]);
#define	callout_unlock(cpuid)	sl_unlock(&(l)[cpuid]);
#else
#define	callout_lock(cpuid)	(void)cpuid;
#define	callout_unlock(cpuid)	(void)cpuid;
#endif

static struct mdx_callout *
first(int cpuid)
{
	struct mdx_callout *c;

	if (list_empty(&callouts_list[cpuid]))
		return (NULL);

	c = CONTAINER_OF(callouts_list[cpuid].next, struct mdx_callout, node);

	return (c);
}

static struct mdx_callout *
next(int cpuid, struct mdx_callout *c0)
{
	struct mdx_callout *c;

	if (c0->node.next == &callouts_list[cpuid])
		return (NULL);

	c = CONTAINER_OF(c0->node.next, struct mdx_callout, node);

	return (c);
}

static void
mi_tmr_start(uint32_t ticks)
{

	if (ticks > mi_tmr->maxcnt)
		mi_tmr->start(mi_tmr->arg, mi_tmr->maxcnt);
	else
		mi_tmr->start(mi_tmr->arg, ticks);
}

void
mdx_callout_init(struct mdx_callout *c)
{

	c->state = CALLOUT_STATE_READY;
	c->flags = 0;
}

/*
 * Calculates amount of ticks elapsed on the timer of a current CPU
 * since it was last configured.
 */
static uint32_t
get_elapsed(uint32_t *count_saved)
{
	uint32_t elapsed;
	uint32_t count;
	uint32_t saved;
	int cpuid;

	KASSERT(curthread != NULL, ("curthread is NULL"));
	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section.", __func__));

	cpuid = PCPU_GET(cpuid);

	count = mi_tmr->count(mi_tmr->arg);
	saved = mi_tmr->count_saved[cpuid];

	if (count > saved)
		elapsed = (count - saved);
	else
		elapsed = (mi_tmr->maxcnt - saved + count);

	if (count_saved != NULL)
		*count_saved = count;

	return (elapsed);
}

#ifndef MDX_SCHED_SMP
/*
 * Calculates amount of ticks left to run for the given callout c0.
 * Could only be run on the same CPU where callout c0 was configured.
 */
uint32_t
mdx_callout_ticks(struct mdx_callout *c0)
{
	uint32_t ticks_elapsed;
	mdx_callout_t *c;
	int cpuid;
	int val;

	critical_enter();

	if ((c0->flags & CALLOUT_FLAG_ACTIVE) == 0) {
		critical_exit();
		return (0);
	}

	ticks_elapsed = get_elapsed(NULL);

	val = 0;

	cpuid = PCPU_GET(cpuid);

	KASSERT(cpuid == c0->cpuid, ("Called from a wrong CPU."));

	for (c = first(cpuid); c != NULL; c = next(cpuid, c)) {
		if (ticks_elapsed >= c->ticks) {
			if (c == c0)
				break;
			else {
				ticks_elapsed -= c->ticks;
				continue;
			}
		} else {
			val += c->ticks - ticks_elapsed;
			ticks_elapsed = 0;

			if (c == c0)
				break;
		}
	}
	critical_exit();

	return (val);
}

uint32_t
mdx_callout_usec(struct mdx_callout *c0)
{
	uint32_t ticks;
	uint32_t usec;

	ticks = mdx_callout_ticks(c0);
	usec = mdx_callout_ticks_to_usec(ticks);

	return (usec);
}
#endif

static void
mdx_callout_set_one(struct mdx_callout *c0)
{
	struct mdx_callout *c;
	uint32_t elapsed;
	int cpuid;

	cpuid = PCPU_GET(cpuid);

	KASSERT(curthread != NULL, ("curthread is NULL"));
	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section.", __func__));

	elapsed = 0;
	if (mi_tmr->state[cpuid] == MI_TIMER_STATE_RUNNING) {
		elapsed = get_elapsed(NULL);
		c0->ticks += elapsed;
	}

	for (c = first(cpuid); c != NULL; c = next(cpuid, c)) {
		if (c->ticks > c0->ticks) {
			c->ticks -= c0->ticks;
			list_insert(&c->node, &c0->node);
			break;
		}
		c0->ticks -= c->ticks;
	}

	if (c == NULL)
		list_append(&callouts_list[cpuid], &c0->node);

	switch (mi_tmr->state[cpuid]) {
	case MI_TIMER_STATE_RUNNING:
		if (c0 == first(cpuid))
			mi_tmr_start(c0->ticks - elapsed);
		break;
	case MI_TIMER_STATE_EXCEPTION:
		/* We are in the exception handler */
		break;
	case MI_TIMER_STATE_READY:
		/* Start the timer. */
		mi_tmr->state[cpuid] = MI_TIMER_STATE_RUNNING;
		mi_tmr->count_saved[cpuid] = mi_tmr->count(mi_tmr->arg);
		mi_tmr_start(first(cpuid)->ticks);
	}
}

int
mdx_callout_set_ticks_locked(struct mdx_callout *c, uint32_t ticks,
    void (*func)(void *arg), void *arg)
{

	KASSERT(mi_tmr != NULL, ("mi timer is NULL"));
	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section.", __func__));

	if (c->flags & CALLOUT_FLAG_ACTIVE)
		return (MDX_ERROR);

	if (ticks == 0) {
		c->state = CALLOUT_STATE_FIRED;
		return (MDX_OK);
	}

	c->ticks_orig = ticks;
	c->ticks = ticks;
	c->func = func;
	c->arg = arg;
	c->cpuid = PCPU_GET(cpuid);

	mdx_callout_set_one(c);
	c->flags |= CALLOUT_FLAG_ACTIVE;

	return (MDX_OK);
}

int
mdx_callout_set_locked(struct mdx_callout *c, uint32_t usec,
    void (*func)(void *arg), void *arg)
{
	uint32_t ticks;
	int error;

	ticks = mdx_callout_usec_to_ticks(usec);
	error = mdx_callout_set_ticks_locked(c, ticks, func, arg);

	return (error);
}

int
mdx_callout_set_ticks(struct mdx_callout *c, uint32_t ticks,
    void (*func)(void *arg), void *arg)
{
	int cpuid;
	int error;

	critical_enter();

	cpuid = PCPU_GET(cpuid);

	callout_lock(cpuid);
	error = mdx_callout_set_ticks_locked(c, ticks, func, arg);
	callout_unlock(cpuid);

	critical_exit();

	return (error);
}

int
mdx_callout_set(struct mdx_callout *c, uint32_t usec,
    void (*func)(void *arg), void *arg)
{
	uint32_t ticks;
	int error;

	ticks = mdx_callout_usec_to_ticks(usec);
	error = mdx_callout_set_ticks(c, ticks, func, arg);

	return (error);
}

static int
mdx_callout_cancel_locked(struct mdx_callout *c)
{
	struct mdx_callout *n;

	if ((c->flags & CALLOUT_FLAG_ACTIVE) == 0) {
		/* Callout c is not in the callouts queue. */
		return (1);
	}

	n = next(c->cpuid, c);
	if (n != NULL)
		n->ticks += c->ticks;

	list_remove(&c->node);

	c->flags &= ~CALLOUT_FLAG_ACTIVE;

	return (MDX_OK);
}

int
mdx_callout_cancel(struct mdx_callout *c)
{
	int error;

	KASSERT(mi_tmr != NULL, ("mi timer is NULL"));
	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section.", __func__));

	callout_lock(c->cpuid);
	error = mdx_callout_cancel_locked(c);
	callout_unlock(c->cpuid);

	return (error);
}

int
mdx_callout_callback(struct mi_timer *mt)
{
	struct mdx_callout *c, *tmp;
	uint32_t ticks_elapsed;
	int cpuid;

	cpuid = PCPU_GET(cpuid);

	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section.", __func__));
	KASSERT(mi_tmr != NULL,
	    ("%s: mi timer is not registered", __func__));
	KASSERT(mi_tmr == mt,
	    ("%s: callback for the wrong device", __func__));

	if (list_empty(&callouts_list[cpuid]))
		return (MDX_OK);

	ticks_elapsed = get_elapsed(&mi_tmr->count_saved[cpuid]);

	mi_tmr->state[cpuid] = MI_TIMER_STATE_EXCEPTION;

	callout_lock(cpuid);
	for (c = first(cpuid); c != NULL; ) {
		tmp = next(cpuid, c);
		if (ticks_elapsed >= c->ticks) {
			ticks_elapsed -= c->ticks;
			list_remove(&c->node);
			c->ticks = 0;
			c->state = CALLOUT_STATE_FIRED;
			c->flags &= ~CALLOUT_FLAG_ACTIVE;
			if (c->func)
				c->func(c->arg);
		} else {
			c->ticks -= ticks_elapsed;
			break;
		}
		c = tmp;
	}

	if ((c = first(cpuid)) != NULL) {
		if (mi_tmr->state[cpuid] == MI_TIMER_STATE_EXCEPTION) {
			mi_tmr->state[cpuid] = MI_TIMER_STATE_RUNNING;
			mi_tmr_start(c->ticks);
		}
	} else {
		mi_tmr->state[cpuid] = MI_TIMER_STATE_READY;
		mi_tmr->stop(mi_tmr->arg);
	}
	callout_unlock(cpuid);

	return (MDX_OK);
}

uint32_t
mdx_callout_usec_to_ticks(uint32_t usec)
{
	uint32_t ticks;

	KASSERT(mi_tmr != NULL, ("mi timer is not registered."));
	KASSERT(mi_tmr->usec_to_ticks != NULL,
	    ("usec_to_ticks() is not provided"));

	ticks = mi_tmr->usec_to_ticks(mi_tmr->frequency, usec);

	return (ticks);
}

uint32_t
mdx_callout_ticks_to_usec(uint32_t ticks)
{
	uint32_t usec;

	KASSERT(mi_tmr != NULL, ("mi timer is not registered."));
	KASSERT(mi_tmr->ticks_to_usec != NULL,
	    ("ticks_to_usec() is not provided"));

	usec = mi_tmr->ticks_to_usec(mi_tmr->frequency, ticks);

	return (usec);
}

static int
mdx_callout_setup(struct mi_timer *mt)
{

	if (mt->usec_to_ticks) {
		/* Already provided by a driver. */
		return (MDX_OK);
	}

#ifdef MDX_CALLOUT_USEC_TO_TICKS_1MHZ
	if (mt->frequency == 1000000) {
		mt->usec_to_ticks = mdx_time_usec_to_ticks_1mhz;
		mt->ticks_to_usec = mdx_time_ticks_to_usec_1mhz;
		return (MDX_OK);
	}
#endif

#ifdef MDX_CALLOUT_USEC_TO_TICKS
	if (mt->frequency != 1000000) {
		mt->usec_to_ticks = mdx_time_usec_to_ticks;
		mt->ticks_to_usec = NULL; /* TODO */
		return (MDX_OK);
	}
#endif

	return (MDX_ERROR);
}

int
mdx_callout_register(struct mi_timer *mt)
{
	int i;

	if (mi_tmr != NULL)
		return (MDX_EEXIST);

	if (mt->maxcnt == 0 ||
	    mt->start == NULL ||
	    mt->stop == NULL ||
	    mt->frequency == 0 ||
	    mt->count == NULL) {
		printf("%s: Error: can't register timer: ",
		    "invalid configuration.\n", __func__);
		return (MDX_ERROR);
	}

	if (mdx_callout_setup(mt) != MDX_OK) {
		printf("%s: Error: could not register the timer: "
		    "usec_to_ticks() is not set.\n", __func__);
		return (MDX_ERROR);
	}

	mi_tmr = mt;

	for (i = 0; i < MDX_CPU_MAX; i++) {
#ifdef MDX_SCHED_SMP
		sl_init(&l[i]);
#endif
		list_init(&callouts_list[i]);
		mi_tmr->state[i] = MI_TIMER_STATE_READY;
		mi_tmr->count_saved[i] = 0;
	}

	return (MDX_OK);
}
