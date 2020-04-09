/*-
 * Copyright (c) 2019-2020 Ruslan Bukin <br@bsdpad.com>
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

#ifndef	_SYS_THREAD_H_
#define	_SYS_THREAD_H_

#include <sys/callout.h>
#include <sys/pcpu.h>

#include <machine/thread.h>
#include <machine/pcb.h>

struct thread {
	const char *		td_name;	/* Name of the thread. */
	struct mdthread		td_md;		/* Machine dependent. */
	volatile u_int		td_critnest;	/* Critical section nesting. */
	struct trapframe *	td_tf;		/* Trapframe on stack. */
	uint8_t *		td_stack;	/* Pointer to stack. */
	uint8_t *		td_stack_top;	/* Top of the stack. */
	uint32_t		td_stack_size;	/* Size of the stack. */
	struct entry		td_node;	/* Entry in runq or mutex q */
	struct mdx_callout	td_c;		/* Thread deadline callout. */
	uint32_t		td_quantum;	/* Time slice in raw ticks. */
	uint8_t			td_idle;	/* This is an idle thread. */
	uint32_t		td_index;	/* For debugging only. */
	int			td_prio;	/* Priority. 0 for idle. */
	int			td_flags;	/* Thread's flags. */
#define	TD_FLAGS_DYN_ALLOC_SP	(1 << 0)	/* Stack allocated dynamic. */
#define	TD_FLAGS_DYN_ALLOC_TD	(1 << 1)	/* Thread allocated dynamic. */
	int			td_state;	/* Current state. */
#define	TD_STATE_READY		0
#define	TD_STATE_RUNNING	1
#define	TD_STATE_SLEEPING	2
#define	TD_STATE_WAKEUP		3
#define	TD_STATE_TERMINATING	4
#define	TD_STATE_ACK		5
#define	TD_STATE_SEM_WAIT	6
#define	TD_STATE_SEM_UNLOCK	7
#define	TD_STATE_SEM_UNLOCK_ACK	8
#define	TD_STATE_YIELDING	9
	struct pcb		td_pcb;
};

void mdx_thread_init(int cpuid);
void mdx_thread_main(void);

struct thread *mdx_thread_create(const char *name, int prio,
    uint32_t quantum_usec, uint32_t stack_size,
    void *entry, void *arg);
struct thread * mdx_thread_alloc(uint32_t stack_size);
void mdx_thread_free(struct thread *td);
int mdx_thread_setup(struct thread *td, const char *name,
    int prio, uint32_t quantum_usec, void *entry, void *arg);
void mdx_thread_terminate_cleanup(struct thread *td);
void mdx_thread_yield(void);

void cpu_idle(void);

/* Scheduler */
struct thread * mdx_sched_next(void);
void mdx_sched_add(struct thread *td0);
void mdx_sched_enter(void);
void mdx_sched_init(void);
void mdx_sched_cpu_add(struct pcpu *pcpup);
void mdx_sched_cpu_avail(struct pcpu *pcpup, bool available);
bool mdx_sched_release(struct thread *td);

/* Thread MD part */
void md_init(int cpuid);
void md_thread_yield(void);
void md_setup_frame(struct trapframe *tf, void *entry,
    void *arg, void *terminate);
void md_thread_terminate(struct thread *td);
void md_init_secondary(int cpuid);

int main(void);
void board_init(void);

#endif /* !_SYS_THREAD_H_ */
