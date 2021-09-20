/*-
 * Copyright (c) 2018 Ruslan Bukin <br@bsdpad.com>
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

#ifndef	_SYS_LIMITS_H_
#define	_SYS_LIMITS_H_

#include <machine/limits.h>

#define	ULONG_MAX	__ULONG_MAX
#define	UINT_MAX	__UINT_MAX
#define	USHRT_MAX	__USHRT_MAX
#define	UCHAR_MAX	__UCHAR_MAX
#define	SIZE_T_MAX	__SIZE_T_MAX
#define	LONG_BIT	__LONG_BIT
#define	CHAR_BIT	__CHAR_BIT
#define	LONG_MAX	__LONG_MAX
#define	LONG_MIN	__LONG_MIN
#define	OFF_MAX		__OFF_MAX	/* off_t max val */
#define	OFF_MIN		__OFF_MIN	/* off_t min val */
#define	INT_MIN		__INT_MIN
#define	INT_MAX		__INT_MAX
#define	UQUAD_MAX	__UQUAD_MAX
#define	QUAD_MIN	__QUAD_MIN
#define	QUAD_MAX	__QUAD_MAX

#endif /* !_SYS_LIMITS_H_ */
