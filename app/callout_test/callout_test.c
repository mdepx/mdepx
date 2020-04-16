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
#include <sys/console.h>
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/spinlock.h>
#include <sys/malloc.h>
#include <sys/mutex.h>
#include <sys/sem.h>
#include <sys/list.h>
#include <sys/smp.h>

#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#include "callout_test.h"

#ifdef MDX_SCHED
static mdx_mutex_t m0 __unused;
static mdx_mutex_t m1 __unused;
#endif

static struct mdx_callout c1[1000] __unused;

#ifdef MDX_SCHED
static void __unused
test_thr(void *arg)
{

	while (1) {
		if (!mdx_mutex_timedlock(&m0, 1000))
			continue;
		printf("cpu%d: hello from thread%04d cn %d mstatus %x\n",
		    PCPU_GET(cpuid), (size_t)arg, curthread->td_critnest,
		    csr_read(mstatus));
		mdx_mutex_unlock(&m0);

		mdx_tsleep(1000);
	}
}

static void __unused
test_thr1(void *arg)
{

	while (1) {
		printf("cpu%d: hello from thread%d\n",
		    PCPU_GET(cpuid), (size_t)arg);
		mdx_tsleep(10000000);
	}
}

static void __unused
test_thr2(void *arg)
{

	while (1) {
		printf("cpu%d: hello from thread%d\n",
		    PCPU_GET(cpuid), (size_t)arg);
		mdx_tsleep(5000000);
	}
}

static void __unused
test_m0(void *arg)
{

	while (1) {
		if (mdx_mutex_timedlock(&m1, 50000) == 0) {
			printf("again %ld\n", (uint64_t)arg);
			continue;
		}
		printf("test_m0 acuired the mutex\n");
		mdx_mutex_unlock(&m1);
	}
}

static void __unused
test_m1(void *arg)
{

	while (1) {
		if (mdx_mutex_timedlock(&m1, 50000) == 0) {
			printf("again %ld\n", (uint64_t)arg);
			continue;
		}
		printf("test_m1 acuired the mutex\n");
		mdx_mutex_unlock(&m1);
	}
}

static void __unused
test_m2(void *arg)
{

	while (1) {
		if (mdx_mutex_timedlock(&m1, 50000) == 0) {
			printf("again %ld\n", (uint64_t)arg);
			continue;
		}
		printf("test_m2 acuired the mutex\n");
		mdx_mutex_unlock(&m1);
	}
}
#endif

static void __unused
cb1(void *arg)
{
	mdx_sem_t *sem;

	sem = arg;

	mdx_sem_post(sem);
}

static void __unused
cb(void *arg)
{
	struct mdx_callout *c;
	uint32_t ticks;

	c = arg;

	ticks = c->ticks_orig;
	ticks /= 2;
	printf("ticks %d\n", ticks);
	mdx_callout_set_ticks_locked(c, ticks, cb, (void *)c);
}

static void __unused
test(void *arg)
{

	printf("%d", PCPU_GET(cpuid));
}

void
callout_test(void)
{

	printf("cpu%d: pcpu size %d\n", PCPU_GET(cpuid), sizeof(struct pcpu));

#ifdef MDX_SCHED
	mdx_mutex_init(&m0);
#endif

	/* Some testing routines. */
#if 0
	struct thread *td;
	size_t i;
	for (i = 0; i < 20; i++) {
		td = mdx_thread_create("test", 1, 1000,
		    4096, test_thr, (void *)i);
		if (td == NULL)
			break;
		printf("td %p created\n", td);
		mdx_sched_add(td);
	}
#endif

#ifdef MDX_SCHED
	struct thread *td;
	size_t i;
	for (i = 1; i < 500; i++) {
		td = mdx_thread_create("test", 1, 1000 * i,
		    4096, test_thr, (void *)i);
		if (td == NULL)
			break;
		td->td_index = i;
		mdx_sched_add(td);
	}
#endif

#if 0
	struct thread *td;
	td = mdx_thread_create("test", 1, 1000,
	    4096, test_thr1, (void *)0);
	mdx_sched_add(td);

	td = mdx_thread_create("test", 1, 2000,
	    4096, test_thr2, (void *)1);
	mdx_sched_add(td);

	td = mdx_thread_create("test", 1, 2000,
	    4096, test_thr2, (void *)2);
	mdx_sched_add(td);
#endif

#if 0
	struct thread *td;
	td = mdx_thread_create("test1", 1, 500,
	    4096, test_m0, (void *)0);
	td->td_index = 0;
	mdx_sched_add(td);

	td = mdx_thread_create("test2", 1, 500,
	    4096, test_m1, (void *)1);
	td->td_index = 1;
	mdx_sched_add(td);

	td = mdx_thread_create("test3", 1, 500,
	    4096, test_m2, (void *)2);
	td->td_index = 2;
	mdx_sched_add(td);
#endif

#if 1
	int j;
	for (j = 0; j < 100; j++)
		mdx_callout_set(&c1[j], 100000 * j, cb, (void *)&c1[j]);
#endif

#ifndef MDX_SCHED
	mdx_sem_t sem;
	int ret;

	mdx_sem_init(&sem, 0);

	while (1) {
		mdx_callout_set(&c1[0], 60000, cb1, (void *)&sem);
		ret = mdx_sem_timedwait(&sem, 40000);

		critical_enter();
		if (ret == 0)
			mdx_callout_cancel(&c1[0]);
		critical_exit();

		printf("Hello world, %d\n", ret);
	}
#endif

	while (1) {
		printf("Hello world\n");
		mdx_usleep(1000000);
	}
}
