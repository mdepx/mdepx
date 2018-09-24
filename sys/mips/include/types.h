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

#ifndef _MACHINE_TYPES_H_
#define _MACHINE_TYPES_H_

typedef	char			__int8_t;
typedef	short			__int16_t;
typedef	int			__int32_t;

typedef	unsigned char		__uint8_t;
typedef	unsigned short		__uint16_t;
typedef	unsigned int		__uint32_t;

#ifdef __mips_n64
typedef	long			__int64_t;
typedef	unsigned long		__uint64_t;
#else
typedef	long long		__int64_t;
typedef	unsigned long long	__uint64_t;
#endif

#ifdef __mips_n64
typedef	__int64_t		__ptrdiff_t;
typedef	__int64_t		__segsz_t;
typedef	__uint64_t		__size_t;
typedef	__int64_t		__ssize_t;
typedef	__uint64_t		__uintfptr_t;
typedef	__uint64_t		__uintptr_t;
typedef	__int64_t		__intptr_t;
typedef	__uint64_t		__register_t;
typedef	__uint64_t		__vm_paddr_t;
typedef	__uint64_t		__vm_offset_t;
typedef	__uint64_t		__vm_size_t;
#else
typedef	__int32_t		__ptrdiff_t;
typedef	__int32_t		__segsz_t;
typedef	__uint32_t		__size_t;
typedef	__int32_t		__ssize_t;
typedef	__uint32_t		__uintfptr_t;
typedef	__uint32_t		__uintptr_t;
typedef	__int32_t		__intptr_t;
typedef	__uint32_t		__register_t;
typedef	__uint32_t		__vm_paddr_t;
typedef	__uint32_t		__vm_offset_t;
typedef	__uint32_t		__vm_size_t;
#endif

typedef	__uint64_t		__uintmax_t;
typedef	__int64_t		__intmax_t;
typedef	__uint8_t		__u_char_t;

#define	NBBY	8

#endif /* !_MACHINE_TYPES_H_ */
