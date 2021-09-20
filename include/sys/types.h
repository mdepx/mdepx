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

#ifndef	_INT8_T_DECLARED
typedef	__int8_t	int8_t;
#define	_INT8_T_DECLARED
#endif

#ifndef	_INT16_T_DECLARED
typedef	__int16_t	int16_t;
#define	_INT16_T_DECLARED
#endif

#ifndef	_INT32_T_DECLARED
typedef	__int32_t	int32_t;
#define	_INT32_T_DECLARED
#endif

#ifndef	_INT64_T_DECLARED
typedef	__int64_t	int64_t;
#define	_INT64_T_DECLARED
#endif

#ifndef	_UINT8_T_DECLARED
typedef	__uint8_t	uint8_t;
#define	_UINT8_T_DECLARED
#endif

#ifndef	_UINT16_T_DECLARED
typedef	__uint16_t	uint16_t;
#define	_UINT16_T_DECLARED
#endif

#ifndef	_UINT32_T_DECLARED
typedef	__uint32_t	uint32_t;
#define	_UINT32_T_DECLARED
#endif

#ifndef	_UINT64_T_DECLARED
typedef	__uint64_t	uint64_t;
#define	_UINT64_T_DECLARED
#endif

#ifndef	_INTPTR_T_DECLARED
typedef	__intptr_t	intptr_t;
#define	_INTPTR_T_DECLARED
#endif

#ifndef	_UINTPTR_T_DECLARED
typedef	__uintptr_t	uintptr_t;
#define	_UINTPTR_T_DECLARED
#endif

#ifndef	_INTMAX_T_DECLARED
typedef	__intmax_t	intmax_t;
#define	_INTMAX_T_DECLARED
#endif

#ifndef	_UINTMAX_T_DECLARED
typedef	__uintmax_t	uintmax_t;
#define	_UINTMAX_T_DECLARED
#endif

#ifndef	_SIZE_T_DECLARED
typedef	__size_t	size_t;
#define	_SIZE_T_DECLARED
#endif

#ifndef	_SSIZE_T_DECLARED
typedef	__ssize_t	ssize_t;
#define	_SSIZE_T_DECLARED
#endif

typedef	__vm_paddr_t	vm_paddr_t;
typedef	__vm_offset_t	vm_offset_t;
typedef	__vm_size_t	vm_size_t;

typedef	__uint64_t	u_quad_t;
typedef	__int64_t	quad_t;
typedef	quad_t *	qaddr_t;

typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;

typedef	__int64_t	__off_t;
typedef	__off_t		off_t;
typedef	__register_t	register_t;
typedef	__time_t	time_t;

typedef	__uint8_t	__sa_family_t;
typedef	__uint32_t	__socklen_t;

typedef	__uint16_t	in_port_t;
typedef	__uint32_t	in_addr_t;

typedef char *		caddr_t; /* core address */
typedef	uint8_t		clockid_t;

#define	INT_MAX		__INT_MAX
#define	NULL		((void *)0)

typedef	_Bool		bool;

#define	true		1
#define	false		0

#ifndef __has_feature
#define	__has_feature(x)	0
#endif

/* CHERI Capabilities */
#if __has_feature(capabilities)
typedef	void * __capability capability;
typedef	void * __capability otype_t;
#else
typedef	void * capability;
#endif

#if !__has_feature(capabilities)
typedef	__intptr_t	__intcap_t;
typedef	__uintptr_t	__uintcap_t;
#endif

typedef	__uintptr_t	vaddr_t;

#ifdef __PTRADDR_TYPE__
typedef	__PTRADDR_TYPE__	__ptraddr_t;
#else
typedef	__uintptr_t		__ptraddr_t;
#endif

/* Floating point */
typedef	__uint8_t	uint_fast8_t;
typedef	__uint16_t	uint_fast16_t;
typedef	__uint32_t	uint_fast32_t;
typedef	__uint64_t	uint_fast64_t;

typedef	__int8_t	int_fast8_t;
typedef	__int16_t	int_fast16_t;
typedef	__int32_t	int_fast32_t;
typedef	__int64_t	int_fast64_t;

typedef	__uint8_t	uint_least8_t;

typedef	__double_t	double_t;
typedef	__float_t	float_t;

/* stub */
typedef	int		locale_t;

typedef	__uint32_t	ticks_t;

#endif /* !_SYS_TYPES_H_ */
