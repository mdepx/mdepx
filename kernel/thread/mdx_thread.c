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

#include <sys/cdefs.h>
#include <sys/cheri.h>
#include <sys/pcpu.h>
#include <sys/systm.h>
#include <sys/thread.h>

#include <machine/frame.h>

#define	THREAD_DEBUG
#undef	THREAD_DEBUG

#ifdef	THREAD_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#ifdef MDX_SCHED

/*
 * Called at the exception time to release thread's resources
 * in case of thread termination.
 */
void
mdx_thread_terminate_cleanup(struct thread *td)
{

	dprintf("%s: %s\n", __func__, td->td_name);

	KASSERT(td->td_state == TD_STATE_TERMINATING,
	    ("thread is not in terminating state"));

#ifdef MDX_THREAD_DYNAMIC_ALLOC
	mdx_thread_free(td);
#endif
}

/*
 * Used by a thread to terminate itself.
 */
static void
mdx_thread_terminate(void)
{
	struct thread *td;

	td = curthread;

	dprintf("%s: %s\n", __func__, td->td_name);

	critical_enter();
	td->td_state = TD_STATE_TERMINATING;
	mdx_callout_cancel(&td->td_c);
	md_thread_terminate(td);
	critical_exit();

	md_thread_yield();

	/* NOT REACHED */

	panic("md_thread_terminate() returned\n");
}

/*
 * Used by a thread to leave the CPU.
 * The thread will be added to the scheduler run queue.
 */
void
mdx_thread_yield(void)
{
	struct thread *td;

	td = curthread;

	critical_enter();
	td->td_state = TD_STATE_YIELDING;
	critical_exit();

	/*
	 * Note that an interrupt could fire right here.
	 * The thread will be added back to the run queue.
	 * On return to this place the thread will have RUNNING
	 * state and the following yield will trigger an exception.
	 * The exception will have no effect for this thread.
	 */

	md_thread_yield();
}

int
mdx_thread_setup(struct thread *td, const char *name,
    int prio, uint32_t quantum_usec, void *entry, void *arg)
{

	if (td == NULL || td->td_stack == NULL)
		return (MDX_ERROR);

	if (td->td_stack_size == 0)
		return (MDX_ERROR);

	if (entry == NULL)
		return (MDX_ERROR);

	td->td_stack_top = mdx_incoffset(td->td_stack, td->td_stack_size);

	td->td_name = name;
	td->td_quantum = quantum_usec ?
		mdx_callout_usec_to_ticks(quantum_usec) : 0;
	td->td_state = TD_STATE_READY;
	td->td_tf = mdx_decoffset(td->td_stack_top, sizeof(struct trapframe));
	td->td_prio = prio;
	md_setup_frame(td->td_tf, entry, arg, mdx_thread_terminate);

	return (MDX_OK);
}

#endif /* !MDX_SCHED */
