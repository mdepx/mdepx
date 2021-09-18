/*-
 * Copyright (c) 2021 Ruslan Bukin <br@bsdpad.com>
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

#ifndef	_MACHINE_VMPARAM_H_
#define	_MACHINE_VMPARAM_H_

#define	VM_MIN_ADDRESS		(0x0000000000000000UL)
#define	VM_MAX_ADDRESS		(0xffffffffffffffffUL)

#define	VM_MIN_KERNEL_ADDRESS	(0xffffffc000000000UL)
#define	VM_MAX_KERNEL_ADDRESS	(0xffffffc800000000UL)

#define	DMAP_MIN_ADDRESS	(0xffffffd000000000UL)
#define	DMAP_MAX_ADDRESS	(0xfffffff000000000UL)

#define	VM_MIN_USER_ADDRESS	(0x0000000000000000UL)
#define	VM_MAX_USER_ADDRESS	(0x0000004000000000UL)

#ifdef MDX_RISCV_SUPERVISOR_MODE
#define	DMAPBASE		(DMAP_MIN_ADDRESS)
#define	KERNBASE		(DMAP_MIN_ADDRESS + MDX_RISCV_PHYSMEM_START + \
				    MDX_RISCV_BBL_SIZE)
#else
#define	DMAPBASE		(0x0UL)
#define	KERNBASE		(MDX_RISCV_PHYSMEM_START)
#endif

#define	PHYS_TO_DMAP(pa)		\
({					\
	((pa) + DMAPBASE);		\
})

#define	DMAP_TO_PHYS(pa)		\
({					\
	((pa) - DMAPBASE);		\
})

#endif /* !_MACHINE_VMPARAM_H_ */
