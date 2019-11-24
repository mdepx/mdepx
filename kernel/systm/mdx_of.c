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
#include <sys/of.h>

#include <libfdt/libfdt.h>

static bool
fdt_is_enabled(void *dtb, int offset)
{
	const char *status;

	status = fdt_getprop(dtb, offset, "status", NULL);
	if (status == NULL)
		return (true);

	if (strcmp(status, "disabled"))
		return (false);

	return (true);
}

static bool
fdt_is_compatible(void *dtb, int offset, const char *check)
{
	const char *compat;
	int len;

	compat = fdt_getprop(dtb, offset, "compatible", &len);
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
fdt_find_first_compatible(void *dtb, const char *compat)
{
	int offset;
	int depth;

	offset = 0;
	depth = 1;

	do {
		offset = fdt_next_node(dtb, offset, &depth);
		if (!fdt_is_enabled(dtb, offset))
			continue;

		if (fdt_is_compatible(dtb, offset, compat))
			return (offset);
	} while (offset > 0);

	return (MDX_OK);
}
