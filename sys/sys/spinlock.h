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

#ifndef	_SYS_SPINLOCK_H_
#define	_SYS_SPINLOCK_H_

#include <machine/atomic.h>

struct spinlock {
	uintptr_t v;
};

static inline void
sl_init(struct spinlock *l)
{

	l->v = 0;
}

static inline void
sl_lock(struct spinlock *l)
{

	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section", __func__));

#ifdef MDX_SMP
	while (atomic_cmpset_acq_ptr(&l->v, 0, 1) == 0);
#else
	KASSERT(l->v == 0,
	    ("%s: lock is already held", __func__));

	l->v = 1;
#endif
}

static inline int
sl_trylock(struct spinlock *l)
{

	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section", __func__));

#ifdef MDX_SMP
	return (atomic_cmpset_acq_ptr(&l->v, 0, 1));
#else
	KASSERT(l->v == 0,
	    ("%s: lock is already held", __func__));

	l->v = 1;

	return (1);
#endif
}

static inline void
sl_unlock(struct spinlock *l)
{

	KASSERT(curthread->td_critnest > 0,
	    ("%s: Not in critical section", __func__));

#ifdef MDX_SMP
	if (!atomic_cmpset_rel_ptr(&l->v, 1, 0))
		panic("lock is not taken");
#else
	KASSERT(l->v == 1,
	    ("%s: lock is not taken", __func__));

	l->v = 0;
#endif
}

#endif /* _SYS_SPINLOCK_H_ */
