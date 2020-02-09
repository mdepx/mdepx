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
#include <sys/sem.h>
#include <sys/spinlock.h>

#include <machine/pcpu.h>
#include <machine/atomic.h>

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
	list_init(&sem->td_list);
	sl_init(&sem->l);
}

static void
mdx_sem_cb(void *arg)
{
	struct thread *td;
	struct token *t;
	mdx_sem_t *sem;

	t = arg;
	sem = t->sem;
	td = t->td;

	KASSERT(curthread->td_critnest > 0,
	    ("Not in critical section"));

	sl_lock(&sem->l);

	if (td->td_state == TD_STATE_SEM_UNLOCK) {
		/* too late */
		td->td_state = TD_STATE_SEM_UNLOCK_ACK;
		sl_unlock(&sem->l);
		return;
	}
	t->timeout = true;

	KASSERT(td->td_state == TD_STATE_ACK,
	    ("%s: wrong state %d\n", __func__, td->td_state));

	list_remove(&td->td_node);

	td->td_state = TD_STATE_WAKEUP;

	mdx_sched_add(td);

	sl_unlock(&sem->l);
}

int
mdx_sem_timedwait(mdx_sem_t *sem, int usec)
{
	struct token t;
	struct thread *td;

	td = curthread;

	KASSERT(td->td_idle == 0,
	    ("Can't lock mutex/sem from the idle thread"));

	for (;;) {
		critical_enter();
		sl_lock(&sem->l);
		if (sem->sem_count > 0) {
			sem->sem_count--;
			/* Lock acquired. */
			sl_unlock(&sem->l);
			critical_exit();
			break;
		}

		/* Lock is owned by another thread, sleep. */
		mdx_callout_cancel(&td->td_c);
		if (usec) {
			t.td = td;
			t.sem = sem;
			t.timeout = false;
			mdx_callout_init(&td->td_c);
			mdx_callout_set(&td->td_c, usec, mdx_sem_cb, &t);
		}

		td->td_state = TD_STATE_SEM_WAIT;

		list_append(&sem->td_list, &td->td_node);

		sl_unlock(&sem->l);
		critical_exit();

		md_thread_yield();

		/*
		 * We are here by one of the reasons:
		 * 1. sem_post added us to the sched run queue
		 * 2. sem_cb added us to the sched run queue
		 *
		 * td_c (mdx_sem_cb) is cancelled here;
		 * td_c (mdx_sched_cb) could be re-configured
		 * by the scheduler here.
		 */

		if (usec && t.timeout)
			return (MDX_OK);
	}

	return (MDX_ERROR);
}

void
mdx_sem_wait(mdx_sem_t *m)
{

	mdx_sem_timedwait(m, 0);
}

int
mdx_sem_trywait(mdx_sem_t *sem)
{
	int ret;

	critical_enter();
	sl_lock(&sem->l);

	if (sem->sem_count > 0) {
		sem->sem_count--;
		ret = 1;
	} else
		ret = 0;

	sl_unlock(&sem->l);
	critical_exit();

	return (ret);
}

int
mdx_sem_post(mdx_sem_t *sem)
{
	struct thread *td;
	int error;

	critical_enter();
	sl_lock(&sem->l);

	sem->sem_count += 1;

	if (!list_empty(&sem->td_list)) {
		/* Someone is waiting for the semaphore. */
		td = CONTAINER_OF(sem->td_list.next, struct thread, td_node);
		list_remove(&td->td_node);

		/* Ensure td left CPU. */
		while (td->td_state != TD_STATE_ACK);

		/*
		 * Ensure sem_cb will not pick up this thread just
		 * after sl_unlock() and before callout_cancel().
		 */
		td->td_state = TD_STATE_SEM_UNLOCK;
		sl_unlock(&sem->l);

		/* mdx_sem_cb could be called here by another CPU. */

		error = mdx_callout_cancel(&td->td_c);
		if (error) {
			/*
			 * We are here by one of the reasons:
			 * 1. It could be that callout was not configured
			 *    (timeout == 0), in that case the td state
			 *    must not change (TD_STATE_SEM_UNLOCK).
			 * 2. sem_cb is already called by another CPU.
			 *    In that case td state must be changed to
			 *    TD_STATE_SEM_UNLOCK_ACK by sem_cb().
			 * 
			 * There is nothing to do here in either case.
			 */
		}

		KASSERT(td->td_state == TD_STATE_SEM_UNLOCK ||
			td->td_state == TD_STATE_SEM_UNLOCK_ACK,
		    ("%s: wrong state %d\n", __func__, td->td_state));
		KASSERT(td != curthread, ("td is curthread"));

		td->td_state = TD_STATE_WAKEUP;

		mdx_sched_add(td);
	} else
		sl_unlock(&sem->l);

	critical_exit();

	return (1);
}
