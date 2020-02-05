# Thread module

## Introduction

Thread API manages a struct thread. Threads are used by the scheduler.

## Methods

* * *
    mdx_thread_create(const char *name, int prio, uint32_t quantum,
        uint32_t stack_size, void *entry, void *arg);

Dynamicaly allocates a thread and a stack space for it. Stack size provided in bytes, quantum in raw timer ticks. Thread's priority *prio*, name *name*, entry point *entry* and a user argument *arg* should also be provided.

Returns a pointer to the allocated thread or NULL if allocation fails.

Successfully allocated thread can be added to the scheduler using *mdx_sched_add(td)* immediately.

* * *
    mdx_thread_setup(struct thread *td, const char *name,
        int prio, uint32_t quantum, void *entry, void *arg);

Used to configure a statically allocated thread *td* with name *name*, priority *prio*, entry point *entry* and argument for the entry point *arg*. Thread quantum specified in raw timer ticks.

A thread must have td->td_stack allocated and td->td_stack_size set before calling this function.

* * *
    mdx_thread_terminate(void)

Used by a thread to terminate itself at any given time. Could not be used in the exception time.

This is a function a thread goes into after returning from main().
