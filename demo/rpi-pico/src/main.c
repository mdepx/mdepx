#include <sys/cdefs.h>
#include <sys/systm.h>

#include <app/callout_test/callout_test.h>

int
main(void)
{

#if 0
	callout_test();
#endif

	while (1) {
		printf("Hello world\n");
		mdx_usleep(1000000);
	}

	return (0);
}
