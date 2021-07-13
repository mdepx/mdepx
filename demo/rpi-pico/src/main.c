#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/spinlock.h>
#include <dev/intc/intc.h>

#include <arm/raspberrypi/rp2040.h>

#include <app/callout_test/callout_test.h>

struct spinlock l;

static void __unused
test_thr(void *arg)
{
	int n;

	n = (int)arg;

	while (1) {
		sl_lock(&l);
		printf("cpu%d: %s%d\n", PCPU_GET(cpuid), __func__, n);
		sl_unlock(&l);
		mdx_usleep(10000 + 100000 * n);
	}
}

int
main(void)
{
	struct thread *td;

	sl_init(&l);

#if 0
	callout_test();
#endif

	td = mdx_thread_create("test", 1, 1000, 4096, test_thr, (void *)0);
	if (td)
		mdx_sched_add(td);
	td = mdx_thread_create("test1", 1, 2000, 4096, test_thr, (void *)1);
	if (td)
		mdx_sched_add(td);
	td = mdx_thread_create("test1", 1, 3000, 4096, test_thr, (void *)2);
	if (td)
		mdx_sched_add(td);

	while (1) {
		sl_lock(&l);
		printf("cpu%d: Hello world\n", PCPU_GET(cpuid));
		sl_unlock(&l);
		mdx_usleep(50000);
	}

	return (0);
}
