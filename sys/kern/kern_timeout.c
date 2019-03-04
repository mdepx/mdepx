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

#include <machine/cpufunc.h>

#define	CALLOUT_DEBUG
#undef CALLOUT_DEBUG

struct callout *callouts;
struct mi_timer *mi_tmr;

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
		printf("callout%d: usec %d\n", i, c->usec);
		i += 1;
		c = c->next;
	} while (c);
}
#endif

void
callout_init(struct callout *c)
{

	c->state = 0;
	c->next = NULL;
	c->prev = NULL;
}

static void
callout_add(struct callout *c0)
{
	struct callout *c, *c1;
	uint32_t usec_elapsed;

	if (callouts == NULL) {
		callouts = c0;
		mi_tmr->start(mi_tmr->arg, c0->usec);
		return;
	}

	usec_elapsed = mi_tmr->count(mi_tmr->arg);
	mi_tmr->stop(mi_tmr->arg);

	c1 = NULL;

	for (c = callouts; c != NULL; c = c->next) {
		/* Decrease timeouts */
		c->usec -= usec_elapsed;

		/*
		 * Check if this is a suitable
		 * place for the new callout.
		 */
		if (c1 == NULL && c0->usec <= c->usec)
			c1 = c;

		if (c->next == NULL)
			break;
	}

	if (c1 != NULL) {
		c0->next = c1;
		c0->prev = c1->prev;
		if (c0->prev == NULL)
			callouts = c0;
		else
			c0->prev->next = c0;
		c1->prev = c0;
	} else {
		c0->next = NULL;
		c0->prev = c;
		c->next = c0;
	}

	mi_tmr->start(mi_tmr->arg, callouts->usec);

#ifdef CALLOUT_DEBUG
	callout_dump();
#endif
}

void
callout_reset(struct callout *c, uint32_t usec,
    void (*func)(void *arg), void *arg)
{
	uint32_t intr;

	if (usec == 0)
		return;

	c->usec = usec;
	c->func = func;
	c->arg = arg;

	intr = intr_disable();
	callout_add(c);
	intr_restore(intr);
}

int
callout_callback(struct mi_timer *mt, uint32_t usec_elapsed)
{
	struct callout *c;

	/*
	 * Mi timer is halted at this step.
	 * Interrupts are disabled.
	 */

	KASSERT(mi_tmr == mt,
	    ("%s: callback for the wrong device", __func__));
	KASSERT(callouts != NULL,
	    ("%s: callouts is NULL", __func__));

	/*
	 * Decrease all the timeouts first,
	 * then call user callback function.
	 */
	for (c = callouts; c != NULL; c = c->next) {
		if (usec_elapsed >= c->usec)
			c->usec = 0;
		else
			c->usec -= usec_elapsed;
	}

	for (c = callouts; c != NULL; c = c->next) {
		if (c->usec != 0)
			break;

		c->state = 1;

		/* Cut the link. */
		if (c->next) {
			c->next->prev = NULL;
			callouts = c->next;
		} else
			callouts = NULL;

		if (c->func)
			c->func(c->arg);
	}

	if (callouts != NULL)
		mi_tmr->start(mi_tmr->arg, callouts->usec);

	return (0);
}

int
callout_register(struct mi_timer *mt)
{

	if (mi_tmr != NULL)
		return (EEXIST);

	mi_tmr = mt;

	return (0);
}
