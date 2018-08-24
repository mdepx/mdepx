/*-
 * Copyright (c) 2017 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _SYS_TYPES_H_
#define _SYS_TYPES_H_

#include <machine/types.h>
#include <machine/endian.h>

/*
 * Unusual type definitions.
 */
#ifdef __GNUCLIKE_BUILTIN_VARARGS
typedef	__builtin_va_list	__va_list;	/* internally known to gcc */
#else
typedef	char *			__va_list;
#endif /* __GNUCLIKE_BUILTIN_VARARGS */
#if defined(__GNUCLIKE_BUILTIN_VAALIST) && !defined(__GNUC_VA_LIST) \
    && !defined(__NO_GNUC_VA_LIST)
#define	__GNUC_VA_LIST
typedef	__va_list		__gnuc_va_list;	/* compatibility w/GNU headers*/
#endif

typedef	__int8_t	int8_t;
typedef	__int16_t	int16_t;
typedef	__int32_t	int32_t;
typedef	__int64_t	int64_t;

typedef	__uint8_t	uint8_t;
typedef	__uint16_t	uint16_t;
typedef	__uint32_t	uint32_t;
typedef	__uint64_t	uint64_t;

typedef	__uintptr_t	uintptr_t;
typedef	__size_t	size_t;
typedef	__ssize_t	ssize_t;
typedef	__uintmax_t	uintmax_t;
typedef	__intmax_t	intmax_t;
typedef	__vm_paddr_t	vm_paddr_t;
typedef	__vm_size_t	vm_size_t;

typedef	__uint64_t	u_quad_t;
typedef	__int64_t	quad_t;
typedef	quad_t *	qaddr_t;

typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;

typedef	__int64_t	off_t;
typedef	__register_t	register_t;

typedef	__uint8_t	__sa_family_t;
typedef	__uint32_t	__socklen_t;

typedef	__uint16_t	in_port_t;
typedef	__uint32_t	in_addr_t;

typedef char *		caddr_t; /* core address */

#define	INT_MAX		__INT_MAX
#define	NULL		((void *)0)

typedef	_Bool		bool;

#define	true		1
#define	false		0

#endif /* !_SYS_TYPES_H_ */
