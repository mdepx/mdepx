/*-
 * Copyright (c) 2019-2021 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/console.h>
#include <sys/malloc.h>
#include <sys/of.h>

#include <libfdt/libfdt.h>

extern void *fdt;

int
mdx_of_probe_and_attach(struct mdx_driver *dri, int offset, mdx_device_t *dev0)
{
	const void *prop;
	mdx_device_t dev;
	int len;
	int error;

	if (mdx_of_node_is_enabled(offset) == false)
		return (-1);

	if (mdx_device_lookup_by_offset(offset) != NULL)
		return (MDX_EEXIST);

	prop = fdt_getprop(fdt, offset, "compatible", &len);
	if (prop == NULL)
		return (-2);

	dev = zalloc(sizeof(struct mdx_device));
	if (dev == NULL)
		panic("could not allocate device");
	dev->dri = dri;
	dev->nodeoffset = offset;

	error = mdx_device_probe_and_attach(dev);
	if (error) {
		free(dev);
		return (MDX_ERROR);
	}

	printf("device attached: %s (offset %x)\n", prop, offset);

	if (dev0)
		*dev0 = dev;

	return (MDX_OK);
}

const char *
mdx_of_dev_get_prop(mdx_device_t dev, const char *propname, int *len)
{
	const char *regp;

	regp = fdt_getprop(fdt, dev->nodeoffset, propname, len);

	return (regp);
}

int
mdx_of_dev_get_prop32(mdx_device_t dev, const char *propname,
    uint32_t *res, int *len)
{

	return (mdx_of_get_prop32(dev->nodeoffset, propname, res, len));
}

static int
mdx_of_get_props(int offset, uint32_t *addr, uint32_t *size)
{
	const fdt32_t *regp;

	regp = fdt_getprop(fdt, offset, "#address-cells", NULL);
	if (!regp)
		return (-1);
	*addr = fdt32_ld(regp);

	regp = fdt_getprop(fdt, offset, "#size-cells", NULL);
	if (!regp)
		return (-1);
	*size = fdt32_ld(regp);

	return (0);
}

int
mdx_of_get_reg(mdx_device_t dev, int index,
    size_t *addr, size_t *size)
{
	const fdt32_t *regp, *r;
	int parent, bus;
	int len;
	uint32_t naddr, nsize, b_naddr, b_nsize;
	uint64_t raddr, rsize, baddr;
	size_t paddr, psize;
	int ntuples;
	int i, j;

	parent = fdt_parent_offset(fdt, dev->nodeoffset);
	if (!parent)
		parent = fdt_path_offset(fdt, "/");

	mdx_of_get_props(parent, &naddr, &nsize);

	regp = fdt_getprop(fdt, dev->nodeoffset, "reg", NULL);
	if (!regp)
		return (-1);

	paddr = 0;
	for (i = 0; i < naddr; i++)
		paddr = ((uint64_t)paddr << 32) | fdt32_ld(regp++);

	psize = 0;
	for (i = 0; i < nsize; i++)
		psize = ((uint64_t)psize << 32) | fdt32_ld(regp++);

	for (bus = fdt_parent_offset(fdt, parent);
	    bus > 0;
	    parent = bus, bus = fdt_parent_offset(fdt, parent)) {
		mdx_of_get_props(bus, &b_naddr, &b_nsize);

		r = fdt_getprop(fdt, parent, "ranges", &len);
		if (!r)
			break;

		if (len == 0)
			continue;

		len /= sizeof(int);

		ntuples = len / (naddr + b_naddr + nsize);
		for (i = 0; i < ntuples; i++) {
			raddr = 0;
			for (j = 0; j < naddr; j++)
				raddr = ((uint64_t)raddr << 32) | fdt32_ld(r++);
			baddr = 0;
			for (j = 0; j < b_naddr; j++)
				baddr = ((uint64_t)baddr << 32) | fdt32_ld(r++);
			rsize = 0;
			for (j = 0; j < nsize; j++)
				rsize = ((uint64_t)rsize << 32) | fdt32_ld(r++);

			if (paddr < raddr || paddr >= raddr + rsize)
				continue;
			paddr = paddr - raddr + baddr;
		}
	}

	*addr = paddr;
	if (size)
		*size = psize;

	return (0);
}

bool
mdx_of_is_compatible(mdx_device_t dev, const char *compatstr)
{

	return (mdx_of_node_is_compatible(dev->nodeoffset, compatstr));
}

bool
mdx_of_dev_uart_chosen(mdx_device_t dev)
{
	int chosen;

	chosen = mdx_of_chosen_path_offset();
	if (chosen < 0 || chosen != dev->nodeoffset)
		return (false);

	return (true);
}
