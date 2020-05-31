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
#include <sys/of.h>

#include <dev/intc/intc.h>

#include <libfdt/libfdt.h>

int
mdx_of_setup_intr(mdx_device_t dev, int index,
    void (*handler)(void *arg, int irq),
    void *arg)
{
	void *fdt;
	const fdt32_t *regp;
	mdx_device_t intc;
	int intc_offset;
	int ntuples;
	int error;
	int ncells;
	int len;
	int irq;

	fdt = mdx_of_get_dtbp();
	if (fdt == NULL)
		return (MDX_ERROR);

	intc_offset = mdx_of_intc_offset(dev->nodeoffset);
	if (intc_offset < 0)
		return (MDX_ERROR);

	regp = fdt_getprop(fdt, intc_offset, "#interrupt-cells", NULL);
	if (regp == NULL)
		return (MDX_ERROR);

	ncells = fdt32_ld(regp);
	if (ncells <= 0)
		return (MDX_ERROR);

	regp = fdt_getprop(fdt, dev->nodeoffset, "interrupts", &len);
	if (regp == NULL)
		return (MDX_ERROR);

	len /= sizeof(int);
	ntuples = len / ncells;
	if (index >= ntuples)
		return (MDX_ERROR);

	intc = mdx_device_lookup_by_offset(intc_offset);
	if (intc == NULL)
		return (MDX_ERROR);

	error = mdx_intc_map(intc, regp + index * ncells, ncells, &irq);
	if (error)
		return (error);

	error = mdx_intc_setup(intc, irq, handler, arg);
	if (error)
		return (error);

	mdx_intc_enable(intc, irq);

	return (0);
}
