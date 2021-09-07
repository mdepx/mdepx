/*-
 * Copyright (c) 2020 Ruslan Bukin <br@bsdpad.com>
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

#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/device.h>
#include <sys/list.h>
#include <sys/of.h>

static struct entry devs = LIST_INIT_STATIC(&devs);

#define	dprintf(fmt, ...)

static mdx_device_t
first(void)
{
	struct mdx_device *c;

	if (list_empty(&devs))
		return (NULL);

	c = CONTAINER_OF(devs.next, struct mdx_device, node);

	return (c);
}

static mdx_device_t
next(struct mdx_device *c0)
{
	struct mdx_device *c;

	if (c0->node.next == &devs)
		return (NULL);

	c = CONTAINER_OF(c0->node.next, struct mdx_device, node);

	return (c);
}

void *
mdx_device_alloc_softc(mdx_device_t dev, size_t size)
{

	if (dev->sc)
		return (dev->sc);

	dev->sc = zalloc(size);

	return (dev->sc);
}

#ifdef MDX_OF
static void
mdx_device_set_unit(mdx_device_t dev)
{
	struct mdx_driver *dri, *dri1;
	mdx_device_t dev1;
	int unit;

	dri = dev->dri;

	unit = -1;

	for (dev1 = first(); dev1 != NULL; dev1 = next(dev1)) {
		dri1 = dev1->dri;

		if (strcmp(dri->name, dri1->name) == 0)
			if (dev1->unit > unit)
				unit = dev1->unit;
	}

	dev->unit = unit + 1;
}

int
mdx_device_probe_and_attach(mdx_device_t dev)
{
	struct mdx_sysinit *si, *si_start, *si_end;
	struct mdx_device_ops *ops;
	struct mdx_moduledata *mod;
	mdx_driver_t *driver;
	int error;

	si_start = &__start_set_sysinit_set;
	si_end = &__stop_set_sysinit_set;

	for (si = si_start; si < si_end; si++) {
		if (si->subsystem != SI_SUB_DRIVERS)
			continue;
		mod = (struct mdx_moduledata *)si->arg;
		driver = (mdx_driver_t *)mod->data;
		dev->dri = driver;
		ops = driver->ops;
		mdx_device_set_unit(dev);
		error = ops->probe(dev);
		if (error == 0) {
			mdx_device_alloc_softc(dev, driver->size);

			error = ops->attach(dev);
			if (error == 0)
				list_append(&devs, &dev->node);
			else {
				/* TODO: free the softc */
			}
			return (error);
		}
	}

	return (-1);
}
#endif

mdx_device_t
mdx_device_lookup_by_name(const char *name, int unit)
{
	struct mdx_driver *dri;
	mdx_device_t dev;

	for (dev = first(); dev != NULL; dev = next(dev)) {
		dri = dev->dri;
		if (strcmp(name, dri->name) == 0 && dev->unit == unit)
			return (dev);
	}

	return (NULL);
}

mdx_device_t
mdx_device_lookup_by_offset(int offset)
{
	mdx_device_t dev;

	for (dev = first(); dev != NULL; dev = next(dev))
		if (dev->nodeoffset == offset)
			return (dev);

	return (NULL);
}

int
mdx_driver_module_handler(void *arg)
{
	struct mdx_compat_data *cd;
	struct mdx_driver *dri;
	int offset;
	int error __unused;

	dri = (struct mdx_driver *)arg;

	dprintf("%s: name %s\n", __func__, dri->name);

	cd = dri->compat_data;
	if (cd == NULL)
		return (-1);

	for (; cd->compatible != NULL; cd++) {
		dprintf("%s: compat %s\n", __func__, cd->compatible);
		offset = mdx_of_offset_by_compatible(cd->compatible);
		if (offset) {
			error = mdx_of_probe_and_attach(offset, NULL);
			dprintf("%s: offset %d error %d\n", __func__, offset,
			    error);
			break;
		}
	}

	return (0);
}
