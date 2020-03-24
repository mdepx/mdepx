/*-
 * Copyright (c) 2002 David E. O'Brien.  All rights reserved.
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)stdarg.h	8.1 (Berkeley) 6/10/93
 * $FreeBSD: head/sys/riscv/include/stdarg.h 292407 2015-12-17 18:44:30Z br $
 */

#ifndef _SYS_STDARG_H_
#define	_SYS_STDARG_H_

#include <sys/types.h>

#ifndef _VA_LIST_DECLARED
#define	_VA_LIST_DECLARED
typedef	__builtin_va_list	va_list;
#endif

#define	va_start(ap, last) \
	__builtin_va_start((ap), (last))

#define	va_arg(ap, type) \
	__builtin_va_arg((ap), type)

#define	va_copy(dest, src) \
	__builtin_va_copy((dest), (src))

#define	va_end(ap) \
	__builtin_va_end(ap)

int vscanf(const char *format, va_list ap);
int vsscanf(const char *str, const char *format, va_list ap);

#endif /* !_SYS_STDARG_H_ */
