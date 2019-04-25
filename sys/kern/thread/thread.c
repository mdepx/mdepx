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

#include <sys/cdefs.h>
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

/*
 * Used by thread to terminate itself.
 */
void
thread_terminate(void)
{
	struct thread *td;

	dprintf("%s: %s\n", __func__, td->td_name);

	td = curthread;

	critical_enter();
	td->td_state = TD_STATE_TERMINATING;
	sched_remove(td);
	callout_cancel(&td->td_c);
	md_thread_terminate(td);
	critical_exit();

	md_thread_yield();

	/* NOT REACHED */

	panic("md_thread_terminate() returned\n");
}

int
thread_setup(struct thread *td, const char *name,
    int prio, uint32_t quantum, void *entry, void *arg)
{

	if (td == NULL || td->td_stack == NULL)
		return (-1);

	if (quantum == 0)
		return (-1);

	if (entry == NULL)
		return (-1);

	td->td_name = name;
	td->td_quantum = quantum;
	td->td_state = TD_STATE_READY;
	td->td_tf = (struct trapframe *)((uint8_t *)td->td_stack
	    + td->td_stack_size - sizeof(struct trapframe));
	td->td_prio = prio;
	md_setup_frame(td->td_tf, entry, arg, thread_terminate);

	critical_enter();
	sched_add(td);
	critical_exit();

	return (0);
}
