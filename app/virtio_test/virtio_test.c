#include <stdio.h>

#include <sys/systm.h>

#include <dev/virtio/virtio.h>
#include <dev/virtio/virtio-blk.h>

#include "virtio_test.h"

static uint8_t buf[512];

int
virtio_test(void *mmio_base)
{
	struct virtio_device *vd;
	int err;

	printf("%s\n", __func__);

	vd = virtio_setup_vd(mmio_base);
	if (vd == NULL) {
		printf("%s: could not setup virtio device\n", __func__);
		return (-1);
	}

	err = virtioblk_init(vd);
	if (err <= 0) {
		printf("%s: could not setup virtio block\n", __func__);
		return (-1);
	}

	printf("%s: Virtio block device initialized, block size %d\n",
	    __func__, err);

	/* Test */
	buf[0] = 1;
	buf[1] = 2;

	err = virtioblk_transfer(vd, (void *)buf, 0, 1, 1 /* write */);

	printf("%s: Transferred %d blocks.\n", __func__, err);

	return (0);
}
