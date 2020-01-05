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
#include <sys/systm.h>
#include <sys/thread.h>

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

#ifdef MDX_SCHED
static struct thread intr_thread[MDX_CPU_MAX];
#endif

void MipsTLBMissException(void);

static void
dump_frame(struct trapframe *tf)
{
	int i;

	printf("tf->tf_at == %zx\n", tf->tf_at);

	for (i = 0; i < 2; i++)
		printf("tf->tf_v%d == %zx\n", i, tf->tf_v[i]);

#if defined(__mips_n32) || defined(__mips_n64)
	for (i = 0; i < 8; i++)
		printf("tf->tf_a%d == %zx\n", i, tf->tf_a[i]);
	for (i = 0; i < 6; i++)
		printf("tf->tf_t%d == %zx\n", i, tf->tf_t[i]);
#else
	for (i = 0; i < 4; i++)
		printf("tf->tf_a%d == %zx\n", i, tf->tf_a[i]);
	for (i = 0; i < 10; i++)
		printf("tf->tf_t%d == %zx\n", i, tf->tf_t[i]);
#endif

	for (i = 0; i < 8; i++)
		printf("tf->tf_s%d == %zx\n", i, tf->tf_s[i]);

	printf("tf->tf_gp == %zx\n", tf->tf_gp);
	printf("tf->tf_sp == %zx\n", tf->tf_sp);
	printf("tf->tf_s8 == %zx\n", tf->tf_s8);
	printf("tf->tf_ra == %zx\n", tf->tf_ra);

	printf("tf->tf_mullo == %zx\n", tf->tf_mullo);
	printf("tf->tf_mulhi == %zx\n", tf->tf_mulhi);

	printf("tf->tf_status == %zx\n", tf->tf_status);
	printf("tf->tf_badvaddr == %zx\n", tf->tf_badvaddr);
	printf("tf->tf_cause == %zx\n", tf->tf_cause);
	printf("tf->tf_pc == %zx\n", tf->tf_pc);
}


static void
default_handler(void *arg, struct trapframe *frame, int i)
{

	printf("Interrupt handler is missing for int %d\n", i);
}

static const struct mips_intr_entry default_intr_map[MIPS_N_INTR] = {
	[0] = { default_handler, NULL },
	[1] = { default_handler, NULL },
	[2] = { default_handler, NULL },
	[3] = { default_handler, NULL },
	[4] = { default_handler, NULL },
	[5] = { default_handler, NULL },
	[6] = { default_handler, NULL },
	[7] = { default_handler, NULL },
};

static const struct mips_intr_entry *mips_intr_map = default_intr_map;

void
mips_install_intr_map(const struct mips_intr_entry *m)
{

	mips_intr_map = m;
}

static void
mips_handle_intr(int cause)
{
	int i;

	for (i = 0; i < MIPS_N_INTR; i++)
		if (cause & MIPS_CR_IP(i)) {
			if (mips_intr_map[i].handler != NULL)
				mips_intr_map[i].handler(mips_intr_map[i].arg,
				    NULL, i);
			else
				printf("%s: spurious intr %d\n",
				    __func__, i);
		}
}

static void
handle_exception(struct trapframe *tf, int exc_code)
{

	switch (exc_code) {
	case MIPS_CR_EXC_CODE_TLBL:
	case MIPS_CR_EXC_CODE_TLBS:
#ifdef MDX_MIPS_TLB
		MipsTLBMissException();
#else
		panic("%s: unhandled TLB exception at pc %zx, badvaddr %zx\n",
		    __func__, tf->tf_pc, tf->tf_badvaddr);
#endif
		break;
	case MIPS_CR_EXC_CODE_ADES:
		dump_frame(tf);
		panic("%s: Address error (store) at pc %zx, badvaddr %zx\n",
		    __func__, tf->tf_pc, tf->tf_badvaddr);
	case MIPS_CR_EXC_CODE_SYS:
		tf->tf_pc += 4;
		break;
	case MIPS_CR_EXC_CODE_RI:
		dump_frame(tf);
		panic("%s: reserved instruction at pc %zx, badvaddr %zx\n",
		    __func__, tf->tf_pc, tf->tf_badvaddr);
	case MIPS_CR_EXC_CODE_ADEL:
		dump_frame(tf);
		panic("%s: Address error (load) at pc %zx, badvaddr %zx\n",
		    __func__, tf->tf_pc, tf->tf_badvaddr);
	default:
		dump_frame(tf);
		panic("%s: no handler: exc_code %d, pc %zx, badvaddr %zx\n",
		    __func__, exc_code, tf->tf_pc, tf->tf_badvaddr);
	}
}

#ifdef MDX_SCHED

struct trapframe *
mips_exception(struct trapframe *tf)
{
	struct thread *td;
	uint32_t exc_code;
	uint32_t cause;
	bool released;
	bool intr;

	released = false;
	intr = false;

	td = curthread;

	/* Switch to the interrupt thread. Stack is the same. */
	PCPU_SET(curthread, &intr_thread[PCPU_GET(cpuid)]);
	curthread->td_critnest++;

	cause = mips_rd_cause();
	exc_code = (cause & MIPS_CR_EXC_CODE_M) >> \
	    MIPS_CR_EXC_CODE_S;

	dprintf("%s: cause %x, exc_code %d\n",
	    __func__, cause, exc_code);

	if (exc_code == MIPS_CR_EXC_CODE_INT)
		intr = true;
	else
		handle_exception(tf, exc_code);

	released = mdx_sched_ack(td, tf);

	if (intr)
		mips_handle_intr(cause);

	if (td->td_state == TD_STATE_TERMINATING) {
		mdx_thread_terminate_cleanup(td);
		released = true;
	}

	if (!released) 
		released = mdx_sched_park(td);

	if (released)
		td = mdx_sched_next();

	curthread->td_critnest--;
	PCPU_SET(curthread, td);

	return (td->td_tf);
}

#else

struct trapframe *
mips_exception(struct trapframe *tf)
{
	struct thread *td;
	uint32_t exc_code;
	uint32_t cause;
	bool intr;

	intr = false;

	td = curthread;

	curthread->td_critnest++;

	cause = mips_rd_cause();
	exc_code = (cause & MIPS_CR_EXC_CODE_M) >> \
	    MIPS_CR_EXC_CODE_S;

	dprintf("%s: cause %x, exc_code %d\n",
	    __func__, cause, exc_code);

	if (exc_code == MIPS_CR_EXC_CODE_INT)
		intr = true;
	else
		handle_exception(tf, exc_code);

	td->td_tf = tf;
	if (intr)
		mips_handle_intr(cause);

	curthread->td_critnest--;

	return (td->td_tf);
}

#endif
