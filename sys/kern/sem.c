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

#define	SEM_DEBUG
#undef	SEM_DEBUG

#ifdef	SEM_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

void
sem_init(sem_t *sem, int count)
{

	sem->sem_count = count;
	sem->sem_count_initial = count;
	sem->td_first = NULL;
	sem->td_last = NULL;
	sl_init(&sem->l);
}

void
sem_take(sem_t *sem)
{
	struct thread *td;

	td = curthread;

	KASSERT(td->td_idle == 0,
	    ("Can't take semaphore from idle thread"));

	for (;;) {
		critical_enter();
		sl_lock(&sem->l);
		if (sem->sem_count > 0) {
			sem->sem_count--;
			sl_unlock(&sem->l);
			critical_exit();
			break;
		}

		/* Semaphore count is zero, sleep. */
		td->td_state = TD_STATE_SEM_WAIT;

		if (sem->td_first == NULL) {
			td->td_next = NULL;
			td->td_prev = NULL;
			sem->td_first = td;
			sem->td_last = td;
		} else {
			td->td_prev = sem->td_last;
			td->td_next = NULL;
			sem->td_last->td_next = td;
			sem->td_last = td;
		}

		callout_cancel(&td->td_c);

		sl_unlock(&sem->l);
		critical_exit();

		md_thread_yield();
	}
}

int
sem_post(sem_t *sem)
{
	struct thread *td;

	critical_enter();

	sl_lock(&sem->l);
	sem->sem_count += 1;

	td = sem->td_first;
	if (td) {
		/* Someone is waiting for the semaphore. */
		sem->td_first = td->td_next;
		if (td->td_next == NULL)
			sem->td_last = NULL;

		while (td->td_state != TD_STATE_ACK);
		td->td_state = TD_STATE_WAKEUP;
		sched_lock();
		sched_add(td);
		sched_unlock();
	}
	sl_unlock(&sem->l);
	critical_exit();

	return (1);
}
