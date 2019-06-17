/*-
 * Copyright (c) 2019 Ruslan Bukin <br@bsdpad.com>
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

#ifndef	_MACHINE_PTE_H_
#define	_MACHINE_PTE_H_

#define	L0_SHIFT	39
#define	L1_SHIFT	30
#define	L2_SHIFT	21
#define	L3_SHIFT	9

#define	L_ENTRIES	(1 << 9)
#define	L_ENTRY_M	(L_ENTRIES - 1)

#define	pmap_l0_index(va)	(((va) >> L0_SHIFT) & L_ENTRY_M)
#define	pmap_l1_index(va)	(((va) >> L1_SHIFT) & L_ENTRY_M)
#define	pmap_l2_index(va)	(((va) >> L2_SHIFT) & L_ENTRY_M)
#define	pmap_l3_index(va)	(((va) >> L3_SHIFT) & L_ENTRY_M)

#define	PTE_V		(1 << 0) /* Valid */
#define	PTE_R		(1 << 1) /* Read */
#define	PTE_W		(1 << 2) /* Write*/
#define	PTE_X		(1 << 3) /* eXecute */
#define	PTE_U		(1 << 4) /* User */
#define	PTE_G		(1 << 5) /* Global */
#define	PTE_A		(1 << 6) /* Accessed */
#define	PTE_D		(1 << 7) /* Dirty */
#define	PTE_RSW0	(1 << 8) /* Reserved for software */
#define	PTE_RSW1	(1 << 9) /* Reserved for software */
#define	PTE_PPN0_S	10 /* physical page number */
#define	PTE_PPN1_S	19
#define	PTE_PPN2_S	28
#define	PTE_PPN3_S	37

#endif /* !_MACHINE_PTE_H_ */
