/*-
 * Copyright (c) 2020 Ruslan Bukin <br@bsdpad.com>
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

#ifndef	_MDX_CHERI_H_
#define	_MDX_CHERI_H_

#if __has_feature(capabilities)
#include <machine/cheric.h>
#endif

static inline void *
mdx_incoffset(void *a, int len)
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
mdx_decoffset(void *a, int len)
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
mdx_setbounds(void *a, int len)
{
	void *result;

#ifdef __CHERI_PURE_CAPABILITY__
	result = cheri_csetbounds(a, len);
#else
	result = a;
#endif

	return (result);
}

#if __has_feature(capabilities)
static inline uint8_t
mips_cap_ioread_uint8(capability cap, size_t offset)
{
	uint8_t v;

	__asm__ __volatile__ ("clb %[v], %[offset],  0(%[cap])"
		: [v] "=r" (v)
		: [cap] "C" (cap), [offset] "r" (offset));

	return (v);
}

static inline uint16_t
mips_cap_ioread_uint16(capability cap, size_t offset)
{
	uint16_t v;

	__asm__ __volatile__ ("clh %[v], %[offset],  0(%[cap])"
		: [v] "=r" (v)
		: [cap] "C" (cap), [offset] "r" (offset));

	return (v);
}

static inline uint32_t
mips_cap_ioread_uint32(capability cap, size_t offset)
{
	uint32_t v;

	__asm__ __volatile__ ("clw %[v], %[offset],  0(%[cap])"
		: [v] "=r" (v)
		: [cap] "C" (cap), [offset] "r" (offset));

	return (v);
}

static inline uint64_t
mips_cap_ioread_uint64(capability cap, size_t offset)
{
	uint64_t v;

	__asm__ __volatile__ ("cld %[v], %[offset],  0(%[cap])"
		: [v] "=r" (v)
		: [cap] "C" (cap), [offset] "r" (offset));

	return (v);
}

static inline void
mips_cap_iowrite_uint8(capability cap, size_t offset, uint8_t v)
{

	__asm__ __volatile__ ("csb %[v], %[offset],  0(%[cap])"
		:: [cap] "C" (cap), [offset] "r" (offset), [v] "r" (v));
}

static inline void
mips_cap_iowrite_uint16(capability cap, size_t offset, uint16_t v)
{

	__asm__ __volatile__ ("csh %[v], %[offset],  0(%[cap])"
		:: [cap] "C" (cap), [offset] "r" (offset), [v] "r" (v));
}

static inline void
mips_cap_iowrite_uint32(capability cap, size_t offset, uint32_t v)
{

	__asm__ __volatile__ ("csw %[v], %[offset],  0(%[cap])"
		:: [cap] "C" (cap), [offset] "r" (offset), [v] "r" (v));
}

static inline void
mips_cap_iowrite_uint64(capability cap, size_t offset, uint64_t v)
{

	__asm__ __volatile__ ("csd %[v], %[offset],  0(%[cap])"
		:: [cap] "C" (cap), [offset] "r" (offset), [v] "r" (v));
}
#endif

#endif /* !_MDX_CHERI_H_ */
