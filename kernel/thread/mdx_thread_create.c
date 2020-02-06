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
#include <sys/malloc.h>

#include <machine/frame.h>

#define	THREAD_DEBUG
#undef	THREAD_DEBUG

#ifdef	THREAD_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

struct thread *
mdx_thread_alloc(uint32_t stack_size)
{
	struct thread *td;

	td = zalloc(sizeof(struct thread));
	if (td == NULL)
		return (NULL);

	td->td_flags = TD_FLAGS_DYN_ALLOC_TD;

	if (stack_size == 0)
		return (td);

	td->td_stack = zalloc(stack_size);
	if (td->td_stack == NULL) {
		free(td);
		return (NULL);
	}
	td->td_flags = TD_FLAGS_DYN_ALLOC_SP;
	td->td_stack_size = stack_size;

	return (td);
}

void
mdx_thread_free(struct thread *td)
{

	if (td->td_flags & TD_FLAGS_DYN_ALLOC_SP)
		free(td->td_stack);

	if (td->td_flags & TD_FLAGS_DYN_ALLOC_TD)
		free(td);
}

struct thread *
mdx_thread_create(const char *name, int prio, uint32_t quantum,
    uint32_t stack_size, void *entry, void *arg)
{
	struct thread *td;
	int ret;

	td = mdx_thread_alloc(stack_size);
	if (td == NULL)
		return (NULL);

	ret = mdx_thread_setup(td, name, prio, quantum, entry, arg);
	if (ret != 0) {
		free(td->td_stack);
		free(td);
		td = NULL;
	}

	return (td);
}
