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
#include <sys/mutex.h>

#include <machine/pcpu.h>

#include "fpu_test.h"
#include "ftoa.h"

static struct thread test_thr;
static struct thread test_thr1;
static uint8_t test_thr_stack[4096];
static uint8_t test_thr1_stack[4096];
static mdx_mutex_t mtx;

static void
test(void *arg)
{
	char buf[100];
	float a;
	float b;
	int i;

	while (1) {
		b = 0;
		a = 0.1;
		for (i = 0; i < 1000000; i++)
			b += a;

		ftoa(b, buf, 10);

		mdx_mutex_lock(&mtx);
		printf("cpu%d: result0 %s\n", PCPU_GET(cpuid), buf);
		mdx_mutex_unlock(&mtx);
	}
}

static void
test1(void *arg)
{

	while (1);
}

void
fpu_test(void)
{
	char buf[128];
	struct thread *td;
	int error;
	float a;
	float b;
	int i;

	mdx_mutex_init(&mtx);
	td = &test_thr;
	td->td_stack = test_thr_stack;
	td->td_stack_size = 8192;
	error = mdx_thread_setup(td, "test", 1, 10000, test, NULL);
	if (error)
		panic("Can't setup thread.\n");
	mdx_sched_add(&test_thr);

	/*
	 * Add a 3rd thread to avoid the situation when in a dual-core system
	 * first two threads take a core each.
	 */
	td = &test_thr1;
	td->td_stack = test_thr1_stack;
	td->td_stack_size = 8192;
	error = mdx_thread_setup(td, "test1", 1, 10000, test1, NULL);
	if (error)
		panic("Can't setup thread.\n");
	mdx_sched_add(&test_thr1);

	while (1) {
		b = 0;
		a = 0.5;
		for (i = 0; i < 1000000; i++)
			b += a;

		ftoa(b, buf, 10);

		mdx_mutex_lock(&mtx);
		printf("cpu%d: result1 %s\n", PCPU_GET(cpuid), buf);
		mdx_mutex_unlock(&mtx);
	}
}
