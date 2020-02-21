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
#include <sys/console.h>
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/spinlock.h>
#include <sys/malloc.h>
#include <sys/mutex.h>
#include <sys/sem.h>
#include <sys/list.h>
#include <sys/smp.h>

#include <machine/pcpu.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#include "board.h"

static struct thread test_thr;
uint8_t test_thr_stack[8192];
static mdx_mutex_t mtx;

static void
test(void *arg)
{

	while (1) {
		mdx_mutex_lock(&mtx);
		printf("ok\n");
		mdx_mutex_unlock(&mtx);
	}
}

int
main(void)
{
	struct thread *td;
	int error;

	mdx_mutex_init(&mtx);
	td = &test_thr;
	td->td_stack = test_thr_stack;
	td->td_stack_size = 8192;

	error = mdx_thread_setup(td, "test", 1, 10000, test, NULL);
	if (error)
		panic("Can't setup thread.\n");

	mdx_sched_add(&test_thr);

	while (1) {
		mdx_mutex_lock(&mtx);
		printf("Hello World!\n");
		mdx_mutex_unlock(&mtx);
	}

	return (0);
}
