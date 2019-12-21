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

#include <bluetooth/log.h>

#include "fiber.h"
#include "mdx_support.h"

void
fiber_start(char *stack, unsigned stack_size, nano_fiber_entry_t entry,
    int arg1, int arg2, unsigned prio, unsigned options)
{
	struct thread *td;
	int quantum;

	BT_DBG("starting new thread\n");

	KASSERT(arg2 == 0, ("Not supported"));

	stack_size = BLUETOOTH_STACK_SIZE;

	td = zalloc(sizeof(struct thread));
	if (td == NULL) {
		printf("Could not allocate thread.");
		return;
	}

	td->td_stack_size = stack_size;
	td->td_stack_bottom = zalloc(stack_size);
	if (td->td_stack_bottom == NULL) {
		printf("Could not allocate stack.");
		free(td);
		return;
	}

	td->td_stack = (void *)((uintptr_t)td->td_stack_bottom + stack_size);

	quantum = 100000;

	mdx_thread_setup(td, "fiber", prio, quantum, entry, (void *)arg1);
	mdx_sched_add(td);
}
