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
#include <sys/systm.h>

#include "nrf_nvmc.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
nrf_nvmc_init(mdx_device_t dev, uint32_t base)
{
	struct nrf_nvmc_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(*sc));
	sc->base = base;
}

void
nrf_nvmc_icache_control(mdx_device_t dev, bool enable)
{
	struct nrf_nvmc_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	reg = RD4(sc, NVMC_ICACHECNF);
	if (enable)
		reg |= ICACHECNF_CACHEEN;
	else
		reg &= ~ICACHECNF_CACHEEN;
	WR4(sc, NVMC_ICACHECNF, reg);
}

#ifdef MDX_OF
static int
nrf_nvmc_probe(mdx_device_t dev)
{

	if (!mdx_of_is_compatible(dev, "nordic,nrf91-flash-controller"))
		return (MDX_ERROR);

	return (MDX_OK);
}

static int
nrf_nvmc_attach(mdx_device_t dev)
{
	struct nrf_nvmc_softc *sc;
	int error;

	sc = mdx_device_get_softc(dev);

	error = mdx_of_get_reg(dev, 0, &sc->base, NULL);
	if (error)
		return (error);

	return (0);
}

static struct mdx_device_ops nrf_nvmc_ops = {
	.probe = nrf_nvmc_probe,
	.attach = nrf_nvmc_attach,
};

static mdx_driver_t nrf_nvmc_driver = {
	"nrf_nvmc",
	&nrf_nvmc_ops,
	sizeof(struct nrf_nvmc_softc),
};

DRIVER_MODULE(nrf_nvmc, nrf_nvmc_driver);
#endif
