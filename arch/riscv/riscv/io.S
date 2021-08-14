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

#if !__has_feature(capabilities)

ASM_ENTRY(mdx_ioread_uint8)
	add	a2, a0, a1
	lbu	a0, 0(a2)
	ret
END(mdx_ioread_uint8)

ASM_ENTRY(mdx_ioread_uint16)
	add	a2, a0, a1
	lhu	a0, 0(a2)
	ret
END(mdx_ioread_uint16)

ASM_ENTRY(mdx_ioread_uint32)
	add	a2, a0, a1
	lw	a0, 0(a2)
	ret
END(mdx_ioread_uint32)

#if __riscv_xlen == 64
ASM_ENTRY(mdx_ioread_uint64)
	add	a2, a0, a1
	ld	a0, 0(a2)
	ret
END(mdx_ioread_uint64)
#endif

ASM_ENTRY(mdx_iowrite_uint8)
	add	a3, a0, a1
	sb	a2, 0(a3)
	ret
END(mdx_iowrite_uint8)

ASM_ENTRY(mdx_iowrite_uint16)
	add	a3, a0, a1
	sh	a2, 0(a3)
	ret
END(mdx_iowrite_uint16)

ASM_ENTRY(mdx_iowrite_uint32)
	add	a3, a0, a1
	sw	a2, 0(a3)
	ret
END(mdx_iowrite_uint32)

#if __riscv_xlen == 64
ASM_ENTRY(mdx_iowrite_uint64)
	add	a3, a0, a1
	sd	a2, 0(a3)
	ret
END(mdx_iowrite_uint64)
#endif

#else

ASM_ENTRY(mdx_ioread_uint8)
	cincoffset ca2, ca0, a1
	clbu	a0, 0(ca2)
	cret
END(mdx_ioread_uint8)

ASM_ENTRY(mdx_ioread_uint16)
	cincoffset ca2, ca0, a1
	clhu	a0, 0(ca2)
	cret
END(mdx_ioread_uint16)

ASM_ENTRY(mdx_ioread_uint32)
	cincoffset ca2, ca0, a1
	clw	a0, 0(ca2)
	cret
END(mdx_ioread_uint32)

#if __riscv_xlen == 64
ASM_ENTRY(mdx_ioread_uint64)
	cincoffset ca2, ca0, a1
	cld	a0, 0(ca2)
	cret
END(mdx_ioread_uint64)
#endif

ASM_ENTRY(mdx_iowrite_uint8)
	cincoffset ca3, ca0, a1
	csb	a2, 0(ca3)
	cret
END(mdx_iowrite_uint8)

ASM_ENTRY(mdx_iowrite_uint16)
	cincoffset ca3, ca0, a1
	csh	a2, 0(ca3)
	cret
END(mdx_iowrite_uint16)

ASM_ENTRY(mdx_iowrite_uint32)
	cincoffset ca3, ca0, a1
	csw	a2, 0(ca3)
	cret
END(mdx_iowrite_uint32)

#if __riscv_xlen == 64
ASM_ENTRY(mdx_iowrite_uint64)
	cincoffset ca3, ca0, a1
	csd	a2, 0(ca3)
	cret
END(mdx_iowrite_uint64)
#endif

#endif
