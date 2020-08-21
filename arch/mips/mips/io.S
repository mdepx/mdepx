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

#include <machine/asm.h>
#include <machine/regdef.h>

#if !__has_feature(capabilities)

ENTRY(mdx_ioread_uint8)
	PTR_ADDU	a3, a0, a1
	lbu		v0, 0(a3)
	jr		ra
	nop
END(mdx_ioread_uint8)

ENTRY(mdx_ioread_uint16)
	PTR_ADDU	a3, a0, a1
	lhu		v0, 0(a3)
	jr		ra
	nop
END(mdx_ioread_uint16)

ENTRY(mdx_ioread_uint32)
	PTR_ADDU	a3, a0, a1
	lwu		v0, 0(a3)
	jr		ra
	nop
END(mdx_ioread_uint32)

ENTRY(mdx_ioread_uint64)
	PTR_ADDU	a3, a0, a1
	ld		v0, 0(a3)
	jr		ra
	nop
END(mdx_ioread_uint64)

ENTRY(mdx_iowrite_uint8)
	PTR_ADDU	a3, a0, a1
	sb		a2, 0(a3)
	jr		ra
	nop
END(mdx_iowrite_uint8)

ENTRY(mdx_iowrite_uint16)
	PTR_ADDU	a3, a0, a1
	sh		a2, 0(a3)
	jr		ra
	nop
END(mdx_iowrite_uint16)

ENTRY(mdx_iowrite_uint32)
	PTR_ADDU	a3, a0, a1
	sw		a2, 0(a3)
	jr		ra
	nop
END(mdx_iowrite_uint32)

ENTRY(mdx_iowrite_uint64)
	PTR_ADDU	a3, a0, a1
	sd		a2, 0(a3)
	jr		ra
	nop
END(mdx_iowrite_uint64)

#else

ENTRY(mdx_ioread_uint8)
	clbu	v0, a0, 0($c3)
	RET
	nop
END(mdx_ioread_uint8)

ENTRY(mdx_ioread_uint16)
	clhu	v0, a0, 0($c3)
	RET
	nop
END(mdx_ioread_uint16)

ENTRY(mdx_ioread_uint32)
	clwu	v0, a0, 0($c3)
	RET
	nop
END(mdx_ioread_uint32)

ENTRY(mdx_ioread_uint64)
	cld	v0, a0, 0($c3)
	RET
	nop
END(mdx_ioread_uint64)

ENTRY(mdx_iowrite_uint8)
	csb	a1, a0, 0($c3)
	RET
	nop
END(mdx_iowrite_uint8)

ENTRY(mdx_iowrite_uint16)
	csh	a1, a0, 0($c3)
	RET
	nop
END(mdx_iowrite_uint16)

ENTRY(mdx_iowrite_uint32)
	csw	a1, a0, 0($c3)
	RET
	nop
END(mdx_iowrite_uint32)

ENTRY(mdx_iowrite_uint64)
	csd	a1, a0, 0($c3)
	RET
	nop
END(mdx_iowrite_uint64)

#endif
