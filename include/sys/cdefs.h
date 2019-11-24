/*-
 * Copyright (c) 2018-2019 Ruslan Bukin <br@bsdpad.com>
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

#ifndef	_SYS_CDEFS_H_
#define	_SYS_CDEFS_H_

#include <sys/types.h>
#include <sys/errno.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#define	__predict_true(e)	__builtin_expect((e), 1)
#define	__predict_false(e)	__builtin_expect((e), 0)
#define	__unused		__attribute__((__unused__))
#define	__used			__attribute__((__used__))
#define	__packed		__attribute__((__packed__))
#define	__aligned(x)		__attribute__((__aligned__(x)))
#define	__section(x)		__attribute__((__section__(x)))
#define	__weak_symbol		__attribute__((__weak__))

/* Check if compiler supports GNU extensions */
#if defined(__GNUC__)
#define	__GNUCLIKE___SECTION 1
#endif

#define	__CONCAT1(x,y)	x ## y
#define	__CONCAT(x,y)	__CONCAT1(x,y)

#define	__GLOBL1(sym)	__asm__(".globl " #sym)
#define	__GLOBL(sym)	__GLOBL1(sym)

#define	offsetof(st, m) __builtin_offsetof(st, m)

#define	CONTAINER_OF(ptr, type, field)	\
	((type *)((char *)ptr - offsetof(type, field)))

#if defined(__GNUC__)
#define	__always_inline		__attribute__((__always_inline__))
#define	__result_use_check	__attribute__((__warn_unused_result__))
#else
#define	__always_inline
#define	__result_use_check
#endif

#endif	/* !_SYS_CDEFS_H_ */
