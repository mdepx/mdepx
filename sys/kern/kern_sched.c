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

#include <machine/frame.h>

#define	SCHED_DEBUG
#undef	SCHED_DEBUG

#ifdef	SCHED_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

static struct thread thread0;
static struct thread *runq;

void
thread0_init(void)
{

	thread0.td_name = "idle";
	thread0.td_idle = 1;
	curthread = &thread0;

	runq = NULL;
}

static void __unused
dump_runq(void)
{
	struct thread *td;

	for (td = runq; td != NULL; td = td->td_next)
		printf("%s: td %p name %s\n", __func__, td, td->td_name);
}

/*
 * Remove curthread from run queue.
 */
static void
thread_terminate(void)
{
	struct thread *td;

	critical_enter();

	td = curthread;

	dprintf("%s: %s\n", __func__, td->td_name);

	td->td_running = 0;

	if (td->td_next != NULL && td->td_prev != NULL) {
		td->td_prev->td_next = td->td_next;
		td->td_next->td_prev = td->td_prev;
	} else if (td->td_next != NULL) {
		runq = td->td_next;
		runq->td_prev = NULL;
	} else if (td->td_prev != NULL)
		td->td_prev->td_next = NULL;
	else
		runq = NULL;

	critical_exit();

	md_thread_terminate();

	/* NOT REACHED */

	panic("md_thread_terminate() returned\n");
}

struct thread *
thread_create(const char *name, uint32_t quantum,
    void *entry, void *arg)
{
	struct thread *td;

	critical_enter();

	td = malloc(sizeof(struct thread));
	if (td == NULL) {
		critical_exit();
		return (NULL);
	}
	memset(td, 0, sizeof(struct thread));
	td->td_name = name;
	td->td_quantum = quantum;
	td->td_idle = 0;
	td->td_mem_size = 1024;
	td->td_mem = malloc(td->td_mem_size);
	if (td->td_mem == NULL) {
		free(td);
		critical_exit();
		return (NULL);
	}
	memset(td->td_mem, 0, td->td_mem_size);

	td->td_tf = (struct trapframe *)((uint8_t *)td->td_mem
	    + td->td_mem_size - sizeof(struct trapframe));
	md_setup_frame(td->td_tf, entry, arg, thread_terminate);

	td->td_running = 0;

	if (runq == NULL) {
		runq = td;
		td->td_next = NULL;
		td->td_prev = NULL;
	} else {
		td->td_next = runq;
		td->td_prev = NULL;
		runq->td_prev = td;
		runq = td;
	}

	critical_exit();

	return (td);
}

static void
sched_cb(void *arg)
{
	struct thread *td;

	td = arg;
	td->td_running = 0;
}

struct trapframe *
sched_next(struct trapframe *tf)
{
	struct thread *td;

	KASSERT(curthread->td_critnest > 0,
	    ("Not in critical section."));

	/* Save old */   
	curthread->td_tf = tf;

	if (runq == NULL) {
		curthread = &thread0;
		return (curthread->td_tf);
	}

	if (curthread != &thread0 &&
	    curthread->td_running == 1)
		return (curthread->td_tf);
	else if (curthread == &thread0)
		td = runq;
	else {
		td = curthread->td_next;
		if (td == NULL)
			td = runq;
	}

	td->td_running = 1;
	callout_init(&td->td_c);
	callout_reset(&td->td_c, td->td_quantum, sched_cb, td);

	curthread = td;

	dprintf("%s: curthread %p, tf %p, name %s\n",
	    __func__, td, td->td_tf, td->td_name);

	return (curthread->td_tf);
}
