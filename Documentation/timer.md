# MDX timer

The MDX timer is a simple software timer built around mdx_callout framework.
The implementation features periodic operation, so unlike callouts, the mdx timer automatically restarts after specified period has elapsed.

## Methods

* * *
    struct mdx_timer *
    mdx_timer_create(void);

Creates a timer.

* * *
    int
    mdx_timer_start(struct mdx_timer *timer, int period_us);

Starts a timer with a given period in microseconds.

* * *
    uint64_t
    mdx_timer_count(struct mdx_timer *timer);

Returns time in microseconds passed since timer started.
