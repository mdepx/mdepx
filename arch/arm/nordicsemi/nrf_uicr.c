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
#include <sys/systm.h>

#include "nrf9160.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

#ifdef MDX_OF
static struct mdx_compat_data nrf_uicr_compat_data[] = {
	{ "nordic,nrf-uicr" },
	{ NULL },
};
#endif

void
nrf_uicr_init(mdx_device_t dev, uint32_t base)
{
	struct nrf_uicr_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(*sc));
	sc->base = base;
}

uint32_t
nrf_uicr_read(mdx_device_t dev, uint32_t reg)
{
	struct nrf_uicr_softc *sc;
	uint32_t val;

	sc = mdx_device_get_softc(dev);

	val = RD4(sc, reg);

	return (val);
}

void
nrf_uicr_write(mdx_device_t dev, uint32_t reg, uint32_t val)
{
	struct nrf_uicr_softc *sc;

	sc = mdx_device_get_softc(dev);

	WR4(sc, reg, val);
}

#ifdef MDX_OF
static int
nrf_uicr_probe(mdx_device_t dev)
{

	if (!mdx_of_is_compatible(dev, "nordic,nrf-uicr"))
		return (MDX_ERROR);

	return (MDX_OK);
}

static int
nrf_uicr_attach(mdx_device_t dev)
{
	struct nrf_uicr_softc *sc;
	int error;

	sc = mdx_device_get_softc(dev);

	error = mdx_of_get_reg(dev, 0, &sc->base, NULL);
	if (error)
		return (error);

	return (0);
}

static struct mdx_device_ops nrf_uicr_ops = {
	.probe = nrf_uicr_probe,
	.attach = nrf_uicr_attach,
};

static mdx_driver_t nrf_uicr_driver = {
	"nrf_uicr",
	&nrf_uicr_ops,
	sizeof(struct nrf_uicr_softc),
	nrf_uicr_compat_data,
};

DRIVER_MODULE(nrf_uicr, nrf_uicr_driver);
#endif
