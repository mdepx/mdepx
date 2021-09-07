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
#include <sys/systm.h>
#include <sys/console.h>
#include <sys/malloc.h>
#include <sys/of.h>

#include <libfdt/libfdt.h>

void *fdt;

void
mdx_of_install_dtbp(void *dtbp)
{

	fdt = dtbp;
}

void *
mdx_of_get_dtbp(void)
{

	return (fdt);
}

int
mdx_of_check_header(void)
{

	return (fdt_check_header(fdt));
}

int
mdx_of_ld32(const void *regp)
{
	int ret;

	ret = fdt32_ld(regp);

	return (ret);
}

bool
mdx_of_node_is_enabled(int offset)
{
	const char *status;

	status = fdt_getprop(fdt, offset, "status", NULL);
	if (status == NULL)
		return (true);

	if (strcmp(status, "disabled") == 0)
		return (false);

	return (true);
}

bool
mdx_of_node_is_compatible(int offset, const char *check)
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
		if (!mdx_of_node_is_enabled(offset))
			continue;

		if (mdx_of_node_is_compatible(offset, compat))
			return (offset);
	} while (offset > 0);

	return (MDX_OK);
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

	do {
		regp = fdt_getprop(fdt, offset, "interrupt-parent", NULL);
		if (regp) {
			phandle = fdt32_ld(regp);
			intc_offset = fdt_node_offset_by_phandle(fdt, phandle);
			break;
		}
		offset = fdt_parent_offset(fdt, offset);
	} while (offset >= 0);

	if (intc_offset < 0)
		return (-1);

	return (intc_offset);
}

int
mdx_of_node_offset_by_phandle(uint32_t phandle)
{
	int offset;

	offset = fdt_node_offset_by_phandle(fdt, phandle);

	return (offset);
}

int
mdx_of_parent_offset(int nodeoffset)
{
	int offset;

	offset = fdt_parent_offset(fdt, nodeoffset);

	return (offset);
}

int
mdx_of_get_prop32(int nodeoffset, const char *propname,
    uint32_t *res, int *len)
{
	const uint32_t *regp;

	regp = fdt_getprop(fdt, nodeoffset, propname, len);
	if (!regp)
		return (-1);

	*res = fdt32_ld(regp);

	return (0);
}

int
mdx_of_offset_by_compatible(const char *compatible)
{

	return (fdt_node_offset_by_compatible(fdt, 0, compatible));
}
