/*-
 * Copyright (c) 2019-2020 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/malloc.h>
#include <sys/of.h>

#include <libfdt/libfdt.h>

void *fdt;

void
mdx_of_install_dtbp(void *dtbp)
{

	fdt = dtbp;
}

static bool
mdx_of_is_enabled(int offset)
{
	const char *status;

	status = fdt_getprop(fdt, offset, "status", NULL);
	if (status == NULL)
		return (true);

	if (strcmp(status, "disabled") == 0)
		return (false);

	return (true);
}

static bool
fdt_is_compatible(int offset, const char *check)
{
	const char *compat;
	int len;

	compat = fdt_getprop(fdt, offset, "compatible", &len);
	if (compat == NULL)
		return (false);

	while (len > 0) {
		if (strcasecmp(compat, check) == 0)
			return (true);
		compat += (strlen(compat) + 1);
		len -= (strlen(compat) + 1);
	}

	return (false);
}

int
mdx_of_find_first_compatible(const char *compat)
{
	int offset;
	int depth;

	offset = 0;
	depth = 1;

	do {
		offset = fdt_next_node(fdt, offset, &depth);
		if (!mdx_of_is_enabled(offset))
			continue;

		if (fdt_is_compatible(offset, compat))
			return (offset);
	} while (offset > 0);

	return (MDX_OK);
}

void
mdx_of_probe_devices(void)
{
	mdx_device_t dev;
	const void *prop;
	int offset;
	int depth;
	int error;
	int len;

	offset = 0;
	depth = 0;

	printf("%s\n", __func__);

	do {
		prop = fdt_getprop(fdt, offset, "compatible", &len);
		if (prop && mdx_of_is_enabled(offset)) {
			dev = zalloc(sizeof(struct mdx_device));
			dev->nodeoffset = offset;
			error = mdx_device_probe_and_attach(dev);
			if (error == 0) {
				printf("device attached (depth %d): %s\n",
				    depth, prop);
			} else
				free(dev);
		}
		offset = fdt_next_node(fdt, offset, &depth);
	} while (offset > 0);
}

int
mdx_of_get_prop32(mdx_device_t dev, const char *propname, int *res)
{
	const fdt32_t *regp;

	regp = fdt_getprop(fdt, dev->nodeoffset, propname, NULL);
	if (!regp)
		return (-1);

	*res = fdt32_ld(regp);

	return (0);
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
	size_t paddr;
	int ntuples;
	int i, j;

	parent = fdt_parent_offset(fdt, dev->nodeoffset);
	if (!parent)
		return (-1);

	mdx_of_get_props(parent, &naddr, &nsize);

	regp = fdt_getprop(fdt, dev->nodeoffset, "reg", NULL);
	if (!regp)
		return (-1);

	paddr = fdt32_ld(regp);

	do {
		bus = fdt_parent_offset(fdt, parent);
		mdx_of_get_props(bus, &b_naddr, &b_nsize);

		r = fdt_getprop(fdt, parent, "ranges", &len);
		if (!r)
			break;

		if (len == 0)
			goto next;

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

			printf("paddr %x, raddr %x, rsize %x, baddr %x\n",
			    paddr, raddr, rsize, baddr);
			if (paddr < raddr || paddr >= raddr + rsize)
				continue;
			paddr = paddr - raddr + baddr;
		}

		printf("len %d, exp %d\n", len, (naddr + b_naddr + nsize));
		printf("%s: ranges found, offs %x, len %d\n",
		    __func__, parent, len);

next:
		bus = parent;
		parent = fdt_parent_offset(fdt, bus);
		mdx_of_get_props(bus, &b_naddr, &b_nsize);

	} while (parent >= 0);

	*addr = paddr;

	return (0);
}

bool
mdx_of_is_compatible(mdx_device_t dev, const char *compatstr)
{

	return (fdt_is_compatible(dev->nodeoffset, compatstr));
}

/*
 * Find an interrupt controller parent offset for the given node offset.
 */
int
mdx_of_intc_offset(int offset)
{
	int intc_offset;
	uint32_t phandle;
	const fdt32_t *regp;

	intc_offset = -1;

	printf("%s\n", __func__);

	do {
		regp = fdt_getprop(fdt, offset, "interrupt-parent", NULL);
		if (regp) {
			phandle = fdt32_ld(regp);
			intc_offset = fdt_node_offset_by_phandle(fdt, phandle);
			break;
		}
		offset = fdt_parent_offset(fdt, offset);
	} while (offset >= 0);

	if (!intc_offset)
		return (-1);

	return (intc_offset);
}
