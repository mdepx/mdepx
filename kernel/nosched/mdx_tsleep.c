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
#include <sys/callout.h>
#include <sys/systm.h>
#include <sys/thread.h>

#include <machine/pcpu.h>

static void
mdx_tsleep_cb(void *arg)
{
	struct thread *td;

	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section.", __func__));

	td = arg;
	td->td_state = TD_STATE_READY;
}

/*
 * Sleep given amount of ticks in single-threaded environment.
 */
void
mdx_tsleep(uint32_t ticks)
{
	struct thread *td;
	struct mdx_callout c;

	td = curthread;

	KASSERT(td->td_idle == 0,
	    ("%s: called from idle thread.", __func__));
	KASSERT(td->td_critnest == 0,
	    ("%s: sleeping in critical section is not allowed", __func__));

	mdx_callout_init(&c);
	mdx_callout_set_ticks(&c, ticks, mdx_tsleep_cb, td);

#ifdef MDX_MIPS_QEMU
	/*
	 * Interrupts have to be left enabled before entering wait
	 * state in qemu/mips, otherwise CPU will never wakeup.
	 */
	while (c.state != CALLOUT_STATE_FIRED)
		cpu_idle();
#else
	while (1) {
		critical_enter();
		if (c.state == CALLOUT_STATE_FIRED) {
			critical_exit();
			break;
		}
		cpu_idle();
		critical_exit();
	}
#endif
}

void
mdx_usleep(uint32_t usec)
{
	uint32_t ticks;

	ticks = mdx_callout_usec_to_ticks(usec);

	mdx_tsleep(ticks);
}
