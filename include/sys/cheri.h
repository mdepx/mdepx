/*-
 * Copyright (c) 2020-2021 Ruslan Bukin <br@bsdpad.com>
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

#ifndef	_SYS_CHERI_H_
#define	_SYS_CHERI_H_

#if __has_feature(capabilities)
#include <sys/cheric.h>
#endif

static inline void *
mdx_incoffset(void *a, size_t len)
{
	void *result;

#ifdef __CHERI_PURE_CAPABILITY__
	result = cheri_incoffset(a, len);
#else
	result = (void *)((uint8_t *)a + len);
#endif

	return (result);
}

static inline void *
mdx_decoffset(void *a, size_t len)
{
	void *result;

#ifdef __CHERI_PURE_CAPABILITY__
	result = cheri_incoffset(a, -len);
#else
	result = (void *)((uint8_t *)a - len);
#endif

	return (result);
}

static inline void *
mdx_setoffset(void *a, size_t len)
{
	void *result;

#ifdef __CHERI_PURE_CAPABILITY__
	result = cheri_setoffset(a, len);
#else
	result = (void *)(len);
#endif

	return (result);
}

static inline void *
mdx_setbounds(void *a, size_t len)
{
	void *result;

#ifdef __CHERI_PURE_CAPABILITY__
	result = cheri_setbounds(a, len);
#else
	result = a;
#endif

	return (result);
}

static inline size_t
mdx_getaddress(void *a)
{
	size_t result;

#ifdef __CHERI_PURE_CAPABILITY__
	result = cheri_getaddress(a);
#else
	result = (size_t)a;
#endif

	return (result);
}

static inline void *
mdx_getdefault(void)
{

#ifdef __CHERI_PURE_CAPABILITY__
	return (cheri_getdefault());
#else
	return (NULL);
#endif
}

#endif /* !_SYS_CHERI_H_ */
