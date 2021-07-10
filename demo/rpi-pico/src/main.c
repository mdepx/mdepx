#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/thread.h>
#include <dev/intc/intc.h>

#include <arm/raspberrypi/rp2040.h>

#include <app/callout_test/callout_test.h>

static void __unused
test_thr(void *arg)
{
	int n;

	n = (int)arg;

	while (1) {
		printf("cpu%d: %s%d\n", PCPU_GET(cpuid), __func__, n);
		mdx_usleep(100000 + 100000 * n);
	}
}

int
main(void)
{
	struct thread *td;

#if 0
	callout_test();
#endif

	td = mdx_thread_create("test", 1, 1000, 4096, test_thr, (void *)0);
	mdx_sched_add(td);
	td = mdx_thread_create("test1", 1, 2000, 4096, test_thr, (void *)1);
	mdx_sched_add(td);
	td = mdx_thread_create("test1", 1, 3000, 4096, test_thr, (void *)2);
	mdx_sched_add(td);

	while (1) {
		printf("cpu%d: Hello world\n", PCPU_GET(cpuid));
		mdx_usleep(1500000);
	}

	return (0);
}
