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

#include <sys/cdefs.h>
#include <machine/frame.h>
#include <machine/cpuregs.h>
#include <machine/cpufunc.h>
#include <mips/mips/trap.h>

#define	TRAP_DEBUG
#undef	TRAP_DEBUG

#ifdef	TRAP_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

static struct mips_intr mips_intr_table[7];

void
mips_assign_intr(uint8_t ip, void (*handler)(struct trapframe *, int irq))
{
	int i;

	/*
	 * TODO: add KASSERT
	 * KASSERT(ip < MIPS_N_INTR, (""));
	 */

	/* TODO: clear BSS */
	for (i = 0; i < MIPS_N_INTR; i++)
		mips_intr_table[i].active = 0;

	mips_intr_table[ip].active = 1;
	mips_intr_table[ip].handler = handler;
}

void
mips_exception(struct trapframe *frame)
{
	uint32_t exc_code;
	uint32_t cause;
	int i;

	cause = mips_rd_cause();
	exc_code = (cause & MIPS_CR_EXC_CODE_M) >> \
	    MIPS_CR_EXC_CODE_S;
	switch (exc_code) {
	case MIPS_CR_EXC_CODE_INTERRUPT:
		for (i = 0; i < MIPS_N_INTR; i++)
			if ((cause & MIPS_CR_IP(i)) && \
			    (mips_intr_table[i].active))
				mips_intr_table[i].handler(frame, i);
		break;
	default:
		printf("Add handler\n");
	}

	dprintf("Exception cause: %x, code %d\n", cause, exc_code);
}
