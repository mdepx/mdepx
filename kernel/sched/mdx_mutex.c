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
#include <sys/mutex.h>
#include <sys/sem.h>

void
mdx_mutex_init(mdx_mutex_t *m)
{

	m->td = NULL;
	mdx_sem_init(&m->sem, 1);
}

int
mdx_mutex_timedlock(mdx_mutex_t *m, int usec)
{
	int ret;

	ret = mdx_sem_timedwait(&m->sem, usec);
	if (ret)
		m->td = curthread;

	return (ret);
}

void
mdx_mutex_lock(mdx_mutex_t *m)
{

	KASSERT(curthread->td_critnest == 0,
	    ("Can't lock mutex in the critical section."));

	mdx_sem_wait(&m->sem);

	/*
	 * Ensure that mdx_mutex_unlock() will be called
	 * by the same thread only.
	 */
	m->td = curthread;
}

int
mdx_mutex_trylock(mdx_mutex_t *m)
{
	int ret;

	ret = mdx_sem_trywait(&m->sem);
	if (ret)
		m->td = curthread;

	return (ret);
}

int
mdx_mutex_unlock(mdx_mutex_t *m)
{
	int ret;

	if (m->td != curthread)
		panic("can't unlock mutex, it is owned by a different thread");

	ret = mdx_sem_post(&m->sem);

	return (ret);
}
