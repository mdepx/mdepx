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

static void
default_handler(void *arg, struct trapframe *frame, int i)
{

	printf("Install interrupt handler!\n");
}

static const struct mips_intr_entry mips_intr_map[MIPS_N_INTR] = {
	[0] = { default_handler, NULL },
	[1] = { default_handler, NULL },
	[2] = { default_handler, NULL },
	[3] = { default_handler, NULL },
	[4] = { default_handler, NULL },
	[5] = { default_handler, NULL },
	[6] = { default_handler, NULL },
	[7] = { default_handler, NULL },
};

static const struct mips_intr_entry *map = mips_intr_map;

void
mips_install_intr_map(const struct mips_intr_entry *m)
{

	map = m;
}

void
mips_exception(struct trapframe *frame)
{
	uint32_t exc_code;
	uint32_t cause;
	int i;

	cause = mips_rd_cause();
	dprintf("%s: cause %x\n", __func__, cause);
	exc_code = (cause & MIPS_CR_EXC_CODE_M) >> \
	    MIPS_CR_EXC_CODE_S;
	switch (exc_code) {
	case MIPS_CR_EXC_CODE_INT:
		for (i = 0; i < MIPS_N_INTR; i++)
			if (cause & MIPS_CR_IP(i)) {
				if (map[i].handler != NULL)
					map[i].handler(map[i].arg, frame, i);
				else
					printf("%s: spurious intr %d\n", __func__, i);
			}
		break;
	default:
		printf("%s: missing handler: exc_code %d, badvaddr %x\n",
		    __func__, exc_code, frame->tf_badvaddr);
	}

	dprintf("Exception cause: %x, code %d\n", cause, exc_code);
}
