# Scheduler

The MDX scheduler manages CPU resources and decides which task will be run at any particular time.

## Algorithm

Round-robin priority-based scheduling algorithm is used.

Threads with highest priority run first.

The maximum number of priorities specified by nprio (*MDX_SCHED_NPRIO*) config macro.

The highest priority is 0 and could be set to the idle threads only.

An idle thread (one per each CPU) is created by the OS initialization.

## Multitasking

Threads could be fully-preemptive (quantum > 0) and cooperative (quantum = 0).

The scheduler is tick-less, which means there is no periodic timer interrupt configured and the timer set to a thread's deadline only. In case of cooperative threads the timer is not set at all and a thread must leave the cpu voluntarily.

Regardless of the multitasking style threads could block on sleep, IO or semaphore waiting. An involuntarily context switch will be held in that case.

## Symmetric Multi Processing

The mdepx features SMP support for the systems with multiple CPUs. The processors should be identical and should share the same memory.

The SMP feature is currently supported on RISC-V architecture only.

## Methods

* * *
    void mdx_sched_add(struct thread *td);

Adds a thread *td* to the scheduler run queue.
