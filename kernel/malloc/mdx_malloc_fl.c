/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2018-2021 Ruslan Bukin <br@bsdpad.com>
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory (Department of Computer Science and
 * Technology) under DARPA contract HR0011-18-C-0016 ("ECATS"), as part of the
 * DARPA SSITH research programme.
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
#include <sys/malloc.h>

void *
zalloc(size_t size)
{
	void *ret;

	ret = malloc(size);
	if (ret == NULL)
		return (ret);

	bzero(ret, size);

	return (ret);
}

void *
malloc(size_t size)
{
	void *ret;

	critical_enter();
	ret = mdx_fl_malloc(&mdx_fl_defaultzone, size);
	critical_exit();

#ifdef MDX_MALLOC_DEBUG_ENOMEM
	if (ret == NULL)
		printf("%s: no memory (request/avail %d/%d)\n",
		    __func__, size, mdx_fl_count(&mdx_fl_defaultzone));
#endif

	return (ret);
}

void
free(void *ptr)
{

	critical_enter();
	mdx_fl_free(&mdx_fl_defaultzone, ptr);
	critical_exit();
}

void *
calloc(size_t number, size_t size)
{
	void *ret;

	critical_enter();
	ret = mdx_fl_calloc(&mdx_fl_defaultzone, number, size);
	critical_exit();

#ifdef MDX_MALLOC_DEBUG_ENOMEM
	if (ret == NULL)
		printf("%s: no memory (request/avail %d/%d)\n",
		    __func__, size, mdx_fl_count(&mdx_fl_defaultzone));
#endif

	return (ret);
}

void *
realloc(void *ptr, size_t size)
{
	void *ret;

	critical_enter();
	ret = mdx_fl_realloc(&mdx_fl_defaultzone, ptr, size);
	critical_exit();

#ifdef MDX_MALLOC_DEBUG_ENOMEM
	if (ret == NULL)
		printf("%s: no memory (request/avail %d/%d)\n",
		    __func__, size, mdx_fl_count(&mdx_fl_defaultzone));
#endif

	return (ret);
}

void            
malloc_add_region(void *base, int size)
{

	critical_enter();
	mdx_fl_add_region(&mdx_fl_defaultzone, base, size);
	critical_exit();
}

void
malloc_init(void)
{

	critical_enter();
	mdx_fl_init(&mdx_fl_defaultzone);
	critical_exit();
}

#ifdef __CHERI_PURE_CAPABILITY__
void
malloc_init_purecap(void *cap)
{
	struct mdx_fl_zone *fl;

	fl = &mdx_fl_defaultzone;
	fl->datacap = cap;

	malloc_init();
}
#endif
