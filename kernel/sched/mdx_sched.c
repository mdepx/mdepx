/*-
 * Copyright (c) 2019-2020 Ruslan Bukin <br@bsdpad.com>
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

/* A Round-Robin scheduler. */

#include <sys/cdefs.h>
#include <sys/mutex.h>
#include <sys/pcpu.h>
#include <sys/smp.h>
#include <sys/spinlock.h>
#include <sys/systm.h>
#include <sys/thread.h>

#include <machine/frame.h>

#define	MDX_SCHED_DEBUG
#undef	MDX_SCHED_DEBUG

#ifdef	MDX_SCHED_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

CTASSERT(MDX_SCHED_NPRIO > 1);

static struct entry runq[MDX_SCHED_NPRIO];

#ifdef MDX_SCHED_SMP
static struct entry pcpu_list = LIST_INIT_STATIC(&pcpu_list);
struct entry pcpu_all = LIST_INIT_STATIC(&pcpu_all);
static struct spinlock l;
#define	mdx_sched_lock()	sl_lock(&l)
#define	mdx_sched_unlock()	sl_unlock(&l)
#else
#define	mdx_sched_lock()
#define	mdx_sched_unlock()
#endif

/*
 * Take the next thread from the run queue.
 */
static struct thread *
mdx_sched_pick(void)
{
	struct thread *td;
	int i;

	for (i = (MDX_SCHED_NPRIO - 1); i >= 0; i--) {
		if (list_empty(&runq[i]))
			continue;
		td = CONTAINER_OF(runq[i].next, struct thread, td_node);
		list_remove(&td->td_node);

		return (td);
	}

	panic("%s: thread not found\n", __func__);

	return (NULL);
}

/*
 * A thread's callout callback. Called when thread's quantum is exhausted.
 */
static void
mdx_sched_cb(void *arg)
{
	struct thread *td;

	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section.", __func__));

	td = arg;

	mdx_sched_lock();
	KASSERT(td->td_state == TD_STATE_RUNNING,
	    ("sched_cb: thread is not running"));
	td->td_state = TD_STATE_READY;
	mdx_sched_unlock();
}

#ifdef MDX_SCHED_SMP
static void
mdx_sched_cpu_notify(void)
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
 * Add td to the run queue.
 */
void
mdx_sched_add(struct thread *td)
{

	critical_enter();
	mdx_sched_lock();

	KASSERT(td->td_prio < MDX_SCHED_NPRIO,
	    ("td_prio(%d) >= nprio (%d)\n", td->td_prio, MDX_SCHED_NPRIO));

	list_append(&runq[td->td_prio], &td->td_node);

#ifdef MDX_SCHED_SMP
	mdx_sched_cpu_notify();
#endif

	mdx_sched_unlock();
	critical_exit();
}

/*
 * Called by the exception handler only to release a thread from CPU.
 */
bool
mdx_sched_release(struct thread *td)
{
	bool released;

	released = false;

	switch (td->td_state) {
	case TD_STATE_RUNNING:
		/* Current thread is still running. Do not switch. */
		break;
	case TD_STATE_TERMINATING:
		/* This thread has finished work. */
		mdx_thread_terminate_cleanup(td);
		released = true;
		break;
	case TD_STATE_SEM_WAIT:
	case TD_STATE_SLEEPING:
		td->td_state = TD_STATE_ACK;
		/* Note that this thread can now be used by another CPU. */
		released = true;
		break;
	case TD_STATE_YIELDING:
	case TD_STATE_READY:
		mdx_sched_add(td);
		released = true;
		break;
	case TD_STATE_WAKEUP:
	default:
		panic("unknown state %d, thread name %s",
		    td->td_state, td->td_name);
		break;
	}

	return (released);
}

/*
 * Called by the exception handler only to pick the next thread to run.
 */
struct thread *
mdx_sched_next(void)
{
	struct thread *td;

	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section.", __func__));

	mdx_sched_lock();
	td = mdx_sched_pick();

#ifdef MDX_SCHED_SMP
	if (td->td_idle)
		list_append(&pcpu_list, &curpcpu->pc_avail);
#endif

	mdx_sched_unlock();

	if (!td->td_idle)
		td->td_state = TD_STATE_RUNNING;

	if (td->td_quantum) {
		KASSERT(td->td_idle == 0, ("idle thread has quantum"));

		mdx_callout_init(&td->td_c);
		mdx_callout_set_ticks(&td->td_c, td->td_quantum,
		    mdx_sched_cb, td);
	}

	dprintf("%s\n", td->td_name);
	dprintf("%s%d: curthread %p, tf %p, name %s, idx %x\n",
	    __func__, PCPU_GET(cpuid), td, td->td_tf,
		td->td_name, (uint32_t)td->td_index);

	return (td);
}

void
mdx_sched_enter(void)
{

	KASSERT(curthread->td_idle == 1,
	    ("mdx_sched_enter() called from non-idle thread"));

	md_thread_yield();

	while (1)
		cpu_idle();
}

#ifdef MDX_SCHED_SMP
void
mdx_sched_cpu_avail(struct pcpu *pcpup, bool available)
{

	critical_enter();
	mdx_sched_lock();
	if (available)
		list_append(&pcpu_list, &pcpup->pc_avail);
	else
		list_remove(&pcpup->pc_avail);
	mdx_sched_unlock();
	critical_exit();
}

void
mdx_sched_cpu_add(struct pcpu *pcpup)
{

	critical_enter();
	mdx_sched_lock();
	list_append(&pcpu_all, &pcpup->pc_all);
	mdx_sched_unlock();
	critical_exit();
}
#endif

void
mdx_sched_init(void)
{
	int i;

	for (i = 0; i < MDX_SCHED_NPRIO; i++)
		list_init(&runq[i]);

#ifdef MDX_SCHED_SMP
	sl_init(&l);
#endif
}
