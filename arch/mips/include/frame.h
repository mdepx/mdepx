/*-
 * Copyright (c) 2018 Ruslan Bukin <br@bsdpad.com>
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
 * ("CTSRD"), as part of the DARPA CRASH research programme.
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

#ifndef	_MACHINE_FRAME_H_
#define	_MACHINE_FRAME_H_

#include <machine/abi.h>

#ifndef __ASSEMBLER__
struct trapframe {
	register_t	tf_at;
	register_t	tf_v[2];
#if defined(__mips_n32) || defined(__mips_n64)
	register_t	tf_a[8];
	register_t	tf_t[6];
#else
	register_t	tf_a[4];
	register_t	tf_t[10];
#endif
	register_t	tf_s[8];
	register_t	tf_gp;
	register_t	tf_sp;
	register_t	tf_s8;
	register_t	tf_ra;
	register_t	tf_mullo;
	register_t	tf_mulhi;
	register_t	tf_status;
	register_t	tf_cause;
	register_t	tf_badvaddr;
	register_t	tf_pc;
#ifdef CPU_CHERI
	register_t	reserved[1]; /* alignment for capabilities */
	capability	tf_pcc;	/* PCC */
#endif
};
#endif

#ifdef CPU_CHERI
#define	TF_SIZE		(38 * REG_SIZE)
#else
#define	TF_SIZE		(35 * REG_SIZE)
#endif

#endif /* !_MACHINE_FRAME_H_ */
