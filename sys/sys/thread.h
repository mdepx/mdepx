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

#ifndef	_SYS_THREAD_H_
#define	_SYS_THREAD_H_

#include <sys/callout.h>

#include <machine/thread.h>

struct thread {
	const char *		td_name;
	struct mdthread		td_md;
	volatile u_int		td_critnest;
	struct trapframe *	td_tf;
	uint8_t *		td_stack;
	uint32_t		td_stack_size;
	struct callout		td_c;
	struct thread *		td_next;
	struct thread *		td_prev;
	uint32_t		td_quantum;
	uint8_t			td_idle;
	uint32_t		td_index;
	int			td_prio;
	int			td_state;
#define	TD_STATE_READY		0
#define	TD_STATE_RUNNING	1
#define	TD_STATE_SLEEPING	2
#define	TD_STATE_WAKEUP		3
#define	TD_STATE_MUTEX_WAIT	4
#define	TD_STATE_SEM_WAIT	5
#define	TD_STATE_TERMINATING	6
#define	TD_STATE_ACK		7
};

void thread_init(int cpuid);
struct thread *thread_create(const char *name, int prio,
    uint32_t quantum, uint32_t stack_size,
    void *entry, void *arg);
struct thread * thread_alloc(uint32_t stack_size);
int thread_setup(struct thread *td, const char *name,
    int prio, uint32_t quantum, void *entry, void *arg);
void cpu_idle(void);
void thread_terminate(void);

/* Scheduler */
struct thread *sched_next(void);
void sched_remove(struct thread *td);
void sched_add_tail(struct thread *td);
void sched_add_head(struct thread *td);
void sched_add(struct thread *td0);
void sched_enter(void);
void sched_init(void);
void sched_lock(void);
void sched_unlock(void);
int sched_ack(struct thread *td, struct trapframe *tf);
int sched_park(struct thread *td);
void sched_cpu_add(struct pcpu *pcpup);
void sched_cpu_remove(struct pcpu *pcpup);

/* Thread MD part */
void md_init(int cpuid);
void md_thread_yield(void);
void md_setup_frame(struct trapframe *tf, void *entry,
    void *arg, void *terminate);
void md_thread_terminate(struct thread *td);
void md_init_secondary(int cpuid);

void send_ipi(int cpuid);

int main(void);
int app_init(void);

#endif /* !_SYS_THREAD_H_ */
