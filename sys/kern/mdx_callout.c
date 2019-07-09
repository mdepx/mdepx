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
#include <sys/spinlock.h>
#include <sys/pcpu.h>

static struct mi_timer *mi_tmr;
static struct entry callouts_list[MDX_SMP_MAXCPU];

/*
 * Lock is only required in the SMP case when we need to cancel a callout
 * that belongs to another CPU, otherwise callout code is lock-free.
 */

#ifdef MDX_SMP
static struct spinlock l[MDX_SMP_MAXCPU];
#define	callout_lock(cpuid)	sl_lock(&(l)[cpuid]);
#define	callout_unlock(cpuid)	sl_unlock(&(l)[cpuid]);
#else
#define	callout_lock(cpuid)	(void)cpuid;
#define	callout_unlock(cpuid)	(void)cpuid;
#endif

static struct callout *
first(int cpuid)
{
	struct callout *c;

	if (list_empty(&callouts_list[cpuid]))
		return (NULL);

	c = CONTAINER_OF(callouts_list[cpuid].next, struct callout, node);

	return (c);
}

static struct callout *
next(int cpuid, struct callout *c0)
{
	struct callout *c;

	if (c0->node.next == &callouts_list[cpuid])
		return (NULL);

	c = CONTAINER_OF(c0->node.next, struct callout, node);

	return (c);
}

void
callout_init(struct callout *c)
{

	c->state = CALLOUT_STATE_READY;
	c->flags = 0;
}

static uint32_t
get_elapsed(uint32_t *count_saved)
{
	uint32_t elapsed;
	uint32_t count;
	int cpuid;

	cpuid = PCPU_GET(cpuid);

	count = mi_tmr->count(mi_tmr->arg);
	if (count > mi_tmr->count_saved[cpuid])
		elapsed = (count - mi_tmr->count_saved[cpuid]);
	else
		elapsed = (mi_tmr->width - mi_tmr->count_saved[cpuid] + count);
	if (count_saved != NULL)
		*count_saved = count;

	return (elapsed);
}

static void
callout_set_one(struct callout *c0)
{
	struct callout *c;
	uint32_t elapsed;
	int cpuid;

	cpuid = PCPU_GET(cpuid);

	KASSERT(curthread != NULL, ("curthread is NULL"));
	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section.", __func__));

	elapsed = 0;
	if (mi_tmr->state[cpuid] == MI_TIMER_RUNNING) {
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
	case MI_TIMER_RUNNING:
		if (c0 == first(cpuid))
			mi_tmr->start(mi_tmr->arg, c0->ticks - elapsed);
		break;
	case MI_TIMER_EXCP:
		/* We are in the exception handler */
		break;
	case MI_TIMER_READY:
		/* We are free to run. */
		mi_tmr->state[cpuid] = MI_TIMER_RUNNING;
		mi_tmr->count_saved[cpuid] = mi_tmr->count(mi_tmr->arg);
		mi_tmr->start(mi_tmr->arg, first(cpuid)->ticks);
	}
}

int
callout_set_locked(struct callout *c, uint32_t ticks,
    void (*func)(void *arg), void *arg)
{

	KASSERT(mi_tmr != NULL, ("mi timer is NULL"));
	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section.", __func__));

	if (c->flags & CALLOUT_FLAG_ACTIVE)
		return (-1);

	if (ticks == 0) {
		c->state = CALLOUT_STATE_FIRED;
		return (0);
	}

	c->ticks_orig = ticks;
	c->ticks = ticks;
	c->func = func;
	c->arg = arg;
	c->cpuid = PCPU_GET(cpuid);

	callout_set_one(c);
	c->flags |= CALLOUT_FLAG_ACTIVE;

	return (0);
}

int
callout_set(struct callout *c, uint32_t ticks,
    void (*func)(void *arg), void *arg)
{
	int cpuid;
	int error;

	critical_enter();

	cpuid = PCPU_GET(cpuid);

	callout_lock(cpuid);
	error = callout_set_locked(c, ticks, func, arg);
	callout_unlock(cpuid);

	critical_exit();

	return (error);
}

static int
callout_cancel_locked(struct callout *c)
{
	struct callout *n;

	if ((c->flags & CALLOUT_FLAG_ACTIVE) == 0) {
		/* Callout c is not in the callouts queue. */
		return (1);
	}

	n = next(c->cpuid, c);
	if (n != NULL)
		n->ticks += c->ticks;

	list_remove(&c->node);

	c->flags &= ~CALLOUT_FLAG_ACTIVE;

	return (0);
}

int
callout_cancel(struct callout *c)
{
	int error;

	KASSERT(mi_tmr != NULL, ("mi timer is NULL"));
	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section.", __func__));

	callout_lock(c->cpuid);
	error = callout_cancel_locked(c);
	callout_unlock(c->cpuid);

	return (error);
}

int
callout_callback(struct mi_timer *mt)
{
	struct callout *c, *tmp;
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
		return (0);

	ticks_elapsed = get_elapsed(&mi_tmr->count_saved[cpuid]);

	mi_tmr->state[cpuid] = MI_TIMER_EXCP;

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
		if (mi_tmr->state[cpuid] == MI_TIMER_EXCP) {
			mi_tmr->state[cpuid] = MI_TIMER_RUNNING;
			mi_tmr->start(mi_tmr->arg, c->ticks);
		}
	} else {
		mi_tmr->state[cpuid] = MI_TIMER_READY;
		mi_tmr->stop(mi_tmr->arg);
	}
	callout_unlock(cpuid);

	return (0);
}

int
callout_register(struct mi_timer *mt)
{
	int i;

	if (mi_tmr != NULL)
		return (EEXIST);

	if (mt->width == 0 ||
	    mt->start == NULL ||
	    mt->stop == NULL ||
	    mt->count == NULL) {
		printf("%s: can't register timer\n", __func__);
		return (ENXIO);
	}

	mi_tmr = mt;

	for (i = 0; i < MDX_SMP_MAXCPU; i++) {
#ifdef MDX_SMP
		sl_init(&l[i]);
#endif
		list_init(&callouts_list[i]);
		mi_tmr->state[i] = MI_TIMER_READY;
		mi_tmr->count_saved[i] = 0;
	}

	return (0);
}
