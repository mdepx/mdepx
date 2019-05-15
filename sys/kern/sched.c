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

#include <machine/frame.h>

#include <riscv/sifive/e300g_clint.h>

#define	SCHED_DEBUG
#undef	SCHED_DEBUG

#ifdef	SCHED_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

static struct thread *runq;
static struct thread *runq_tail;
static struct spinlock l;
static struct entry pcpu_list = LIST_INIT(&pcpu_list);

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

	if (td->td_next != NULL)
		td->td_next->td_prev = td->td_prev;
	else
		runq_tail = td->td_prev;

	if (td->td_prev != NULL)
		td->td_prev->td_next = td->td_next;
	else
		runq = td->td_next;
}

static struct thread *
sched_picknext(void)
{
	struct thread *td;

	td = runq;

	sched_remove(td);

	return (td);
}

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

#if MAXCPU > 1
	struct pcpu *p;
	p = curpcpu;
	list_remove(&p->pc_node);

	/*
	 * Check if some hart is available to pick up this thread.
	 */
	if (!list_empty(&pcpu_list)) {
		p = CONTAINER_OF(pcpu_list.next, struct pcpu, pc_node);
		KASSERT(curpcpu != p,
		    ("Adding new threads from idle thread."));
		send_ipi(p->pc_cpuid);
	}
#endif

	critical_exit();
}

static void
sched_cb(void *arg)
{
	struct thread *td;

	td = arg;

	if (td->td_state == TD_STATE_RUNNING)
		td->td_state = TD_STATE_READY;
}

void
sched_park(struct trapframe *tf)
{
	struct thread *td;

	td = curthread;

	/* Save old */
	td->td_tf = tf;

	switch (td->td_state) {
	case TD_STATE_TERMINATING:
	case TD_STATE_MUTEX_WAIT:
	case TD_STATE_SEM_WAIT:
	case TD_STATE_SLEEPING:
		td->td_state = TD_STATE_ACK;
	case TD_STATE_WAKEUP:
	default:
		break;
	}
}

struct trapframe *
sched_next(struct trapframe *tf)
{
	struct thread *td;

	dprintf("%s%d\n", __func__, PCPU_GET(cpuid));

	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section. td name %s critnest %d",
	    __func__, curthread->td_name, curthread->td_critnest));

	td = curthread;

	/* Save old */   
	td->td_tf = tf;

	switch (td->td_state) {
	case TD_STATE_TERMINATING:
	case TD_STATE_MUTEX_WAIT:
	case TD_STATE_SEM_WAIT:
	case TD_STATE_SLEEPING:
		td->td_state = TD_STATE_ACK;
	case TD_STATE_WAKEUP:
	case TD_STATE_ACK:
		td->td_critnest--;
		break;
	case TD_STATE_RUNNING:
		/*
		 * Current thread is still running and has quantum.
		 * Do not switch.
		 */
		return (td->td_tf);
	default:
		td->td_critnest--;

		KASSERT(td->td_critnest == 0,
		    ("adding wrong critnest %d, td_name %s, td_state %d",
		    td->td_critnest, td->td_name, td->td_state));

		sched_lock();
		sched_add(td);
		sched_unlock();
	}

	sched_lock();
	td = sched_picknext();
	struct pcpu *p;
	p = curpcpu;
	if (td->td_idle)
		list_append(&pcpu_list, &p->pc_node);
	sched_unlock();

	PCPU_SET(curthread, td);

	td->td_critnest++;

	if (!td->td_idle) {
		td->td_state = TD_STATE_RUNNING;
		callout_init(&td->td_c);
		callout_set(&td->td_c, td->td_quantum, sched_cb, td);
	}

	dprintf("%s%d: curthread %p, tf %p, name %s, idx %x\n",
	    __func__, PCPU_GET(cpuid), td, td->td_tf,
		td->td_name, (uint32_t)td->td_index);

	return (curthread->td_tf);
}

void
sched_lock(void)
{

	sl_lock(&l);
}

void
sched_unlock(void)
{

	sl_unlock(&l);
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
sched_add_cpu(struct pcpu *pcpup)
{

	sched_lock();
	list_append(&pcpu_list, &pcpup->pc_node);
	sched_unlock();
}

void
sched_init(void)
{

	sl_init(&l);
}
