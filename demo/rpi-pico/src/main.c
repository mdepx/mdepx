#include <sys/cdefs.h>
#include <sys/systm.h>

int
main(void)
{

	while (1) {
		printf("Hello world\n");
		mdx_usleep(1000000);
	}

	return (0);
}
