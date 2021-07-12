/*-
 * Copyright (c) 2021 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/spinlock.h>

#include <arm/raspberrypi/rp2040.h>

#ifdef MDX_SCHED_SMP
static int sl_cnt = 0;
#endif

#define	SL_BASE		(RP2040_SIO_BASE + 0x100)
#define	SL_LOCK(n)	(SL_BASE + 0x4 * (n))

void
sl_init(struct spinlock *l)
{

#ifdef MDX_SCHED_SMP
	l->n = sl_cnt++;

	if (sl_cnt > 31)
		panic("No spinlocks available.");

	*(volatile uint32_t *)(SL_LOCK(l->n)) = 1;
#else
	l->v = 0;
#endif
}

void
sl_lock(struct spinlock *l)
{
	uint32_t reg;

#ifdef MDX_SCHED_SMP
	do
		reg = *(volatile uint32_t *)(SL_LOCK(l->n));
	while (reg == 0);
#else
	KASSERT(l->v == 0,
	    ("%s: lock is already held", __func__));
	l->v = 1;
#endif
}

void
sl_unlock(struct spinlock *l)
{

#ifdef MDX_SCHED_SMP
	*(volatile uint32_t *)(SL_LOCK(l->n)) = 1;
#else
	KASSERT(l->v == 1,
	    ("%s: lock is not taken", __func__));
	l->v = 0;
#endif
};
