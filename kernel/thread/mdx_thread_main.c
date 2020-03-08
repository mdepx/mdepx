/*-
 * Copyright (c) 2020 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/systm.h>
#include <sys/thread.h>

#if defined(MDX_SCHED) && !defined(MDX_THREAD_DYNAMIC_ALLOC)
static struct thread main_thread;
static uint8_t main_thread_stack[MDX_THREAD_STACK_SIZE]
    __aligned(MDX_THREAD_STACK_ALIGN);
#endif

void
mdx_thread_main(void)
{
#ifdef MDX_SCHED
	struct thread *td;

#ifdef MDX_THREAD_DYNAMIC_ALLOC
	td = mdx_thread_create("main", MDX_THREAD_PRIO,
	    MDX_THREAD_QUANTUM, MDX_THREAD_STACK_SIZE, main, NULL);
	if (td == NULL)
		panic("can't create the main thread\n");
#else
	td = &main_thread;
	td->td_stack = (uint8_t *)main_thread_stack;
	td->td_stack_size = MDX_THREAD_STACK_SIZE;
	mdx_thread_setup(td, "main", MDX_THREAD_PRIO,
	    MDX_THREAD_QUANTUM, main, NULL);
#endif
	mdx_sched_add(td);
	mdx_sched_enter();
#else
	main();
#endif
}
