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
#include <sys/endian.h>
#include <sys/thread.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/mutex.h>

#include <machine/frame.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#include <app/callout_test/callout_test.h>

#include "board.h"

#ifdef MDX_SCHED
static mdx_mutex_t m0;

static void __unused
test_thr(void *arg)
{

	while (1) {
		if (!mdx_mutex_timedlock(&m0, 1000))
			continue;
		printf("cpu%d: hello from thread%04d cn %d\n",
		    PCPU_GET(cpuid), (size_t)arg, curthread->td_critnest);
		mdx_mutex_unlock(&m0);

		mdx_tsleep(1000);
	}
}
#endif

int
main(void)
{

	callout_test();

	/* NOT REACHED */

#ifdef MDX_SCHED
	struct thread *td;
	size_t i;

	mdx_mutex_init(&m0);

	for (i = 1; i < 5000; i++) {
		printf("Creating thread %d\n", i);
		td = mdx_thread_create("test", 1, 1000 * i,
		    4096, test_thr, (void *)i);
		if (td == NULL)
			break;
		td->td_index = i;
		mdx_sched_add(td);
	}
#endif

	while (1) {
		printf("Hello world\n");
		mdx_usleep(100000);
	}

	return (0);
}
