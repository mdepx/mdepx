# Callout framework

## Introduction

The callout framework is used to schedule a call to an arbitrary function after a specific time interval in the future.

Consumers require to allocate and initialize struct callout for every function invocation.

Callouts are one-shot mode only. If a periodic callout is required, a user can reschedule the callout in the called function.

Callout framework allows to cancel a pending call.

Callout framework is lock-free, except in the SMP (Symmetric Multi Processing) case when we need to cancel a callout that belongs to another CPU (a spinlock is used in that case).

## Methods

* * *
    void mdx_callout_init(struct mdx_callout *c);

Used to initialize a callout.

* * *
    int mdx_callout_set(struct mdx_callout *c, uint32_t ticks,
        void (*func)(void *arg), void *arg);

Used to schedule a function invocation for a callout c. The time interval is provided in raw timer ticks. The same argument arg given to *mdx_callout_set()* will be passed to the invoked function func.

* * *
    int mdx_callout_cancel(struct mdx_callout *c);

Used to cancel a callout c.

## A timer device

The framework requires a hardware timer. A timer device must be free running, upcounting with automatic value reloading.

* * *
    int mdx_callout_register(struct mi_timer *mt);

Used to register a hardware timer that will drive the callout framework.

* * *
    int mdx_callout_callback(struct mi_timer *mt);

The callback function that must be called in the interrupt handler of a timer device.

### MI Timer

The struct mi_timer has several fields that should be provided by the timer driver:

    void (*start)(void *arg, uint32_t ticks);

Start function will be used by callout framework to schedule an interrupt.
A timer driver must read current timer count, add ticks and set the result value to a timer compare register. The driver must ensure that the result value written to the compare register is less than current count, so the scheduled interrupt will not be in the past.

    void (*stop)(void *arg);

Stop function will be used to inform the driver that there is no pending callouts configured, so timer could stop free run operation.

    uint32_t (*count)(void *arg);

The current counting value must be provided by this call.

    void *arg;

An argument arg that will be passed to start(), stop() and count() functions. Usually a driver software context.

    uint32_t maxcnt;

The driver must set the maximum counting value supported by the device. A 32-bit timer should set 0xffffffff.
