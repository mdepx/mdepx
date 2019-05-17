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
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/mutex.h>
#include <sys/spinlock.h>
#include <sys/pcpu.h>

#include <machine/atomic.h>

#define	MUTEX_DEBUG
#undef	MUTEX_DEBUG

#ifdef	MUTEX_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

void
mtx_lock(struct mtx *m)
{
	struct thread *td;
	uintptr_t tid;
	int ret;

	td = curthread;

	KASSERT(td->td_idle == 0,
	    ("Can't lock mutex from idle thread"));

	tid = (uintptr_t)td;

	for (;;) {
		critical_enter();
		sl_lock(&m->l);
		ret = atomic_cmpset_acq_ptr(&(m)->mtx_lock, 0, (tid));
		if (ret) {
			/* Lock acquired. */
			sl_unlock(&m->l);
			critical_exit();
			break;
		}

		/* Lock is owned by another thread, sleep. */
		callout_cancel(&td->td_c);
		td->td_state = TD_STATE_MUTEX_WAIT;

		if (m->td_first == NULL) {
			td->td_next = NULL;
			td->td_prev = NULL;
			m->td_first = td;
			m->td_last = td;
		} else {
			td->td_prev = m->td_last;
			td->td_next = NULL;
			m->td_last->td_next = td;
			m->td_last = td;
		}
		sl_unlock(&m->l);

		critical_exit();

		md_thread_yield();
	}
}

int
mtx_trylock(struct mtx *m)
{
	uintptr_t tid;
	int ret;

	tid = (uintptr_t)curthread;

	ret = atomic_cmpset_acq_ptr(&(m)->mtx_lock, 0, (tid));
	if (ret) {
		/* Lock acquired. */
		return (1);
	}

	return (0);
}

int
mtx_unlock(struct mtx *m)
{
	struct thread *td;
	uintptr_t tid;

	tid = (uintptr_t)curthread;

	critical_enter();
	sl_lock(&m->l);

	if (!atomic_cmpset_rel_ptr(&(m)->mtx_lock, (tid), 0))
		panic("Can't unlock mutex.\n");

	td = m->td_first;
	if (td) {
		/* Someone is waiting for the mutex. */
		m->td_first = td->td_next;
		if (td->td_next == NULL)
			m->td_last = NULL;

		/* Ensure td left CPU. */
		while (td->td_state != TD_STATE_ACK);
		KASSERT(td->td_state == TD_STATE_ACK,
		    ("wrong state %d\n", td->td_state));

		sched_lock();
		td->td_state = TD_STATE_WAKEUP;
		KASSERT(td != curthread, ("td is curthread"));
		sched_add(td);
		sched_unlock();
	}
	sl_unlock(&m->l);
	critical_exit();

	return (1);
}
