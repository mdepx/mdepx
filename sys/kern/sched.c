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
#include <sys/spinlock.h>
#include <sys/pcpu.h>
#include <sys/smp.h>

#include <machine/smp.h>
#include <machine/frame.h>

#define	SCHED_DEBUG
#undef	SCHED_DEBUG

#ifdef	SCHED_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

static struct thread *runq;
static struct thread *runq_tail;
static struct entry pcpu_list = LIST_INIT(&pcpu_list);
struct entry pcpu_all = LIST_INIT(&pcpu_all);

#ifdef SMP
static struct spinlock	l;
#define	sched_lock()	sl_lock(&l)
#define	sched_unlock()	sl_unlock(&l)
#else
#define	sched_lock()
#define	sched_unlock()
#endif

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
static void
sched_remove(struct thread *td)
{

	dprintf("%s\n", __func__);

	if (td->td_next != NULL)
		td->td_next->td_prev = td->td_prev;
	else
		runq_tail = td->td_prev;

	if (td->td_prev != NULL)
		td->td_prev->td_next = td->td_next;
	else
		runq = td->td_next;
}

static void
sched_cb(void *arg)
{
	struct thread *td;

	td = arg;

	sched_lock();
	if (td->td_state == TD_STATE_RUNNING)
		td->td_state = TD_STATE_READY;
	sched_unlock();
}

#ifdef SMP
static void
sched_cpu_notify(void)
{
	struct pcpu *p;

	/*
	 * Check if some hart is available to pick up new thread.
	 */
	if (!list_empty(&pcpu_list)) {
		p = CONTAINER_OF(pcpu_list.next, struct pcpu, pc_avail);
		KASSERT(curpcpu != p,
		    ("Found myself in the list of available CPUs."));
		send_ipi((1 << p->pc_cpuid), IPI_IPI);
	}
}
#endif

/*
 * Add td to the tail of run queue.
 */
void
sched_add_tail(struct thread *td)
{

	dprintf("%s\n", __func__);

	td->td_prev = runq_tail;
	td->td_next = NULL;

	if (runq == NULL)
		runq = td;
	else
		runq_tail->td_next = td;

	runq_tail = td;
}

/*
 * Add td to the head of run queue.
 */
void
sched_add_head(struct thread *td)
{

	dprintf("%s\n", __func__);

	td->td_prev = NULL;
	td->td_next = runq;

	if (runq == NULL)
		runq_tail = td;
	else
		runq->td_prev = td;

	runq = td;
}

/*
 * Add td to the run queue.
 */
void
sched_add(struct thread *td0)
{
	struct thread *td;

	critical_enter();
	sched_lock();

	for (td = runq;
	    (td != NULL && td->td_prio >= td0->td_prio);
	    (td = td->td_next));

	if (td == NULL)
		sched_add_tail(td0);
	else {
		/* Insert td0 before td. */
		td0->td_next = td;
		td0->td_prev = td->td_prev;
		td->td_prev = td0;
		if (td0->td_prev == NULL)
			runq = td0;
		else
			td0->td_prev->td_next = td0;
	}

#ifdef SMP
	sched_cpu_notify();
#endif

	sched_unlock();
	critical_exit();
}

int
sched_ack(struct thread *td, struct trapframe *tf)
{

	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section.", __func__));

	/* Save the frame address */
	td->td_tf = tf;

	switch (td->td_state) {
	case TD_STATE_TERMINATING:
	case TD_STATE_SEM_WAIT:
	case TD_STATE_SLEEPING:
		td->td_state = TD_STATE_ACK;
		return (1);
	default:
		break;
	}

	return (0);
}

int
sched_park(struct thread *td)
{

	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section.", __func__));

	switch (td->td_state) {
	case TD_STATE_WAKEUP:
		panic("could we get here ?");
	case TD_STATE_RUNNING:
		/*
		 * Current thread is still running and has quantum.
		 * Do not switch.
		 */
		return (0);
	default:
		sched_add(td);

		return (1);
	}
}

struct thread *
sched_next(void)
{
	struct thread *td;
	struct pcpu *p;

	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section.", __func__));

	p = curpcpu;

	sched_lock();
	td = runq;
	sched_remove(td);
	if (td->td_idle)
		list_append(&pcpu_list, &p->pc_avail);
	sched_unlock();

	if (!td->td_idle) {
		td->td_state = TD_STATE_RUNNING;
		callout_init(&td->td_c);
		callout_set(&td->td_c, td->td_quantum, sched_cb, td);
	}

	dprintf("%s%d: curthread %p, tf %p, name %s, idx %x\n",
	    __func__, PCPU_GET(cpuid), td, td->td_tf,
		td->td_name, (uint32_t)td->td_index);

	return (td);
}

void
sched_enter(void)
{
	struct thread *td;

	td = curthread;

	KASSERT(td->td_idle == 1,
	    ("sched_enter() called from non-idle thread"));

	md_thread_yield();

	while (1)
		cpu_idle();
}

void
sched_cpu_avail(struct pcpu *pcpup, bool available)
{

	sched_lock();
	if (available)
		list_append(&pcpu_list, &pcpup->pc_avail);
	else
		list_remove(&pcpup->pc_avail);
	sched_unlock();
}

void
sched_cpu_add(struct pcpu *pcpup)
{

	sched_lock();
	list_append(&pcpu_all, &pcpup->pc_all);
	sched_unlock();
}

void
sched_init(void)
{

#ifdef SMP
	sl_init(&l);
#endif
}
