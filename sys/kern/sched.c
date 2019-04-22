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

/* Round-Robin scheduler */

#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/mutex.h>

#include <machine/frame.h>

#define	SCHED_DEBUG
#undef	SCHED_DEBUG

#ifdef	SCHED_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

extern struct thread thread0;
static struct thread *runq;
static struct thread *runq_tail;

static void __unused
dump_runq(void)
{
	struct thread *td;

	for (td = runq; td != NULL; td = td->td_next)
		printf("%s: td %p name %s\n", __func__, td, td->td_name);
}

/*
 * Remove td from the run queue.
 */
void
sched_remove(struct thread *td)
{

	dprintf("%s\n", __func__);

	if (td->td_next != NULL && td->td_prev != NULL) {
		td->td_prev->td_next = td->td_next;
		td->td_next->td_prev = td->td_prev;
	} else if (td->td_next != NULL) {
		runq = td->td_next;
		runq->td_prev = NULL;
	} else if (td->td_prev != NULL) {
		td->td_prev->td_next = NULL;
		runq_tail = td->td_prev;
	} else {
		runq = NULL;
		runq_tail = NULL;
	}
}

/*
 * Add td to the tail of run queue.
 */
void
sched_add_tail(struct thread *td)
{

	dprintf("%s\n", __func__);

	if (runq == NULL) {
		td->td_prev = NULL;
		td->td_next = NULL;
		runq = td;
		runq_tail = td;
	} else {
		td->td_prev = runq_tail;
		td->td_next = NULL;
		runq_tail->td_next = td;
		runq_tail = td;
	}
}

/*
 * Add td to the head of run queue.
 */
void
sched_add_head(struct thread *td)
{

	dprintf("%s\n", __func__);

	if (runq == NULL) {
		td->td_prev = NULL;
		td->td_next = NULL;
		runq = td;
		runq_tail = td;
	} else {
		td->td_prev = NULL;
		td->td_next = runq;
		runq->td_prev = td;
		runq = td;
	}
}

static void
sched_cb(void *arg)
{
	struct thread *td;

	td = arg;
	td->td_state = TD_STATE_READY;
}

struct trapframe *
sched_next(struct trapframe *tf)
{
	struct thread *td;

	dprintf("%s\n", __func__);

	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section. td name %s critnest %d",
	    __func__, curthread->td_name, curthread->td_critnest));

	/* Save old */   
	curthread->td_tf = tf;

	if (curthread != &thread0) {
		switch (curthread->td_state) {
		case TD_STATE_TERMINATING:
		case TD_STATE_MUTEX_WAIT:
		case TD_STATE_SEM_WAIT:
		case TD_STATE_SLEEPING:
			break;
		case TD_STATE_RUNNING:
			/*
			 * Current thread is still running and has quantum.
			 * Do not switch.
			 */
			return (curthread->td_tf);
		default:
			sched_add_tail(curthread);
		}
	}

	if (runq == NULL) {
		curthread = &thread0;
		return (curthread->td_tf);
	}

	td = runq;

	sched_remove(td);

	td->td_state = TD_STATE_RUNNING;
	callout_init(&td->td_c);
	callout_set(&td->td_c, td->td_quantum, sched_cb, td);

	curthread = td;

	dprintf("%s: curthread %p, tf %p, name %s\n",
	    __func__, td, td->td_tf, td->td_name);

	return (curthread->td_tf);
}

void
sched_enter(void)
{
	struct thread *td;

	td = curthread;

	KASSERT(td == &thread0,
	    ("sched_enter() called from non-idle thread"));

	md_thread_yield();

	while (1)
		cpu_idle();
}
