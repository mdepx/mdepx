/*-
 * Copyright (c) 2019 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/systm.h>
#include <sys/thread.h>

static void
raw_sleep_cb(void *arg)
{
	struct callout *c;
	struct thread *td;

	KASSERT(curthread->td_critnest > 0,
	    ("Not in critical section."));

	c = arg;
	td = c->td;

	if (td->td_idle == 0)
		sched_add(c->td);
}

void
raw_sleep(uint32_t ticks)
{
	struct thread *td;
	struct callout c;

	td = curthread;

	callout_init(&c);
	c.td = td;

	if (td->td_idle) {
		callout_reset(&c, ticks, raw_sleep_cb, &c);
		while (c.state == 0)
			cpu_idle();
	} else {
		critical_enter();
		callout_reset(&c, ticks, raw_sleep_cb, &c);
		sched_remove(td);
		critical_exit();

		md_thread_leave();
	}
}