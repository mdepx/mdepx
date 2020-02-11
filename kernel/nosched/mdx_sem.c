/*-
 * Copyright (c) 2020 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/sem.h>
#include <sys/spinlock.h>

#include <machine/pcpu.h>
#include <machine/atomic.h>

#ifdef MDX_SCHED
#error Invalid configuration
#endif

/*
 * A single-threaded (scheduler disabled) implementation of MDX semaphores.
 * Required for a peripheral device drivers compatibility.
 *
 * For a complete (scheduler enabled) implementation see kernel/sched/.
 */

struct token {
	struct thread *td;
	mdx_sem_t *sem;
	bool timeout;
};

void
mdx_sem_init(mdx_sem_t *sem, int count)
{

	sem->sem_count = count;
	sem->sem_count_initial = count;
}

static void
mdx_sem_cb(void *arg)
{
	struct token *t;

	t = arg;

	KASSERT(curthread->td_critnest > 0,
	    ("Not in critical section"));

	t->timeout = true;
}

int
mdx_sem_timedwait(mdx_sem_t *sem, int usec)
{
	struct token t;
	struct thread *td;
	int callout_configured;

	td = curthread;

	KASSERT(td->td_idle == 0,
	    ("Can't lock mutex/sem from the idle thread."));

	callout_configured = false;

	while (1) {
		critical_enter();
		if (sem->sem_count > 0) {
			sem->sem_count--;
			/* The callout is no longer required. */
			if (callout_configured)
				mdx_callout_cancel(&td->td_c);
			critical_exit();
			/* Lock acquired. */
			break;
		}

		if (usec) {
			if (!callout_configured) {
				t.td = td;
				t.sem = sem;
				t.timeout = false;
				mdx_callout_init(&td->td_c);
				mdx_callout_set(&td->td_c, usec,
				    mdx_sem_cb, &t);
				callout_configured = true;
			} else if (t.timeout) {
				critical_exit();
				return (0);
			}
		}
		cpu_idle();
		critical_exit();

		/* We will be interrupted right here. */
	}
	
	return (1);
}

void
mdx_sem_wait(mdx_sem_t *sem)
{

	mdx_sem_timedwait(sem, 0);
}

int
mdx_sem_trywait(mdx_sem_t *sem)
{
	int ret;

	critical_enter();

	if (sem->sem_count > 0) {
		sem->sem_count--;
		ret = 1;
	} else
		ret = 0;

	critical_exit();

	return (ret);
}

int
mdx_sem_post(mdx_sem_t *sem)
{

	critical_enter();
	sem->sem_count += 1;
	critical_exit();

	return (1);
}
