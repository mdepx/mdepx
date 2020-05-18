/*-
 * Copyright (c) 2019 Ruslan Bukin <br@bsdpad.com>
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

static void *fdt;

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

	if (strcmp(status, "disabled"))
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
		if (prop) {
			dev = zalloc(sizeof(struct mdx_device));
			dev->node = offset;
			error = mdx_device_probe_and_attach(dev);
			if (error != 0)
				free(dev);
			else
				printf("device attached (depth %d): %s\n",
				    depth, prop);
		}
		offset = fdt_next_node(fdt, offset, &depth);
	} while (offset > 0);
}

int
mdx_of_get_reg(mdx_device_t dev, int index,
    size_t *addr, size_t *size)
{
	const fdt32_t *regp;
	uint32_t reg;

	/* TODO: address cells, ranges, ... */

	regp = fdt_getprop(fdt, dev->node, "reg", NULL);
	if (!regp)
		return (-1);

	reg = fdt32_ld(regp);

	*addr = reg;

	return (0);
}

bool
mdx_of_is_compatible(mdx_device_t dev, const char *compatstr)
{

	return (fdt_is_compatible(dev->node, compatstr));
}
