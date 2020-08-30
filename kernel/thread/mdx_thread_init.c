/*-
 * Copyright (c) 2019-2020 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/pcpu.h>
#include <sys/thread.h>

/*
 * Note that each CPU is booting on it's idle thread.
 * In a single-threaded environment the idle thread becomes a main thread.
 */

/*
 * Used early by assembly code to setup a stack before calling to md_init().
 */ 
uint8_t idle_thread_stack[MDX_CPU_MAX][MDX_THREAD_STACK_SIZE]
    __aligned(MDX_THREAD_STACK_ALIGN);
static struct thread idle_thread[MDX_CPU_MAX];

void
mdx_thread_init(int cpuid)
{
	struct thread *td;

	td = &idle_thread[cpuid];
	bzero(td, sizeof(struct thread));
	td->td_name = "idle";
	td->td_quantum = 0;
	td->td_prio = 0;

	/*
	 * In a single-threaded environment the booting thread is
	 * the main thread.
	 * In a multi-threaded case this is an idle thread.
	 */
#ifdef MDX_SCHED
	td->td_idle = 1;
#else
	td->td_idle = 0;
#endif
	td->td_state = TD_STATE_READY;

#ifdef MDX_CPU
	PCPU_SET(curthread, td);
#endif
}
