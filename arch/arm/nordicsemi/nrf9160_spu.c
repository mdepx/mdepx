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

void
nrf_spu_periph_set_attr(mdx_device_t dev, int periph_id,
    bool secure_attr, bool secure_dma)
{
	struct nrf_spu_softc *sc;
	int reg;

	sc = mdx_device_get_softc(dev);

	reg = RD4(sc, SPU_PERIPHID_PERM(periph_id));
	if ((reg & PERIPHID_PRESENT) == 0)
		return;

	if ((reg & PERIPHID_SECMAP_M) != PERIPHID_SECMAP_USER_SELECT &&
	    (reg & PERIPHID_SECMAP_M) != PERIPHID_SECMAP_SPLIT)
		return;

	if (secure_attr)
		reg |= PERIPHID_SECATTR;
	else
		reg &= ~PERIPHID_SECATTR;
	if (secure_dma)
		reg |= PERIPHID_DMASEC;
	else
		reg &= ~PERIPHID_DMASEC;
	WR4(sc, SPU_PERIPHID_PERM(periph_id), reg);
}

void
nrf_spu_flash_set_perm(mdx_device_t dev, int region_id,
    bool secure)
{
	struct nrf_spu_softc *sc;
	int reg;

	sc = mdx_device_get_softc(dev);

	reg = PERM_EXECUTE | PERM_WRITE | PERM_READ;

	if (secure)
		reg |= PERM_SECATTR;

	reg |= PERM_LOCK;

	WR4(sc, SPU_FLASHREGION_PERM(region_id), reg);
}

void
nrf_spu_sram_set_perm(mdx_device_t dev, int region_id,
    bool secure)
{
	struct nrf_spu_softc *sc;
	int reg;

	sc = mdx_device_get_softc(dev);

	reg = PERM_EXECUTE | PERM_WRITE | PERM_READ;

	if (secure)
		reg |= PERM_SECATTR;

	reg |= PERM_LOCK;

	WR4(sc, SPU_RAMREGION_PERM(region_id), reg);
}

/* Configure Non-Secure Callable (NSC) flash region. */
void
nrf_spu_flashnsc(mdx_device_t dev, int nsc_region,
    int flash_region, int size, bool lock)
{
	struct nrf_spu_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	/* Configure REGION */
	reg = (flash_region << FLASHNSC_REGION_REGION_S);
	if (lock)
		reg |= (FLASHNSC_REGION_LOCK);
	WR4(sc, SPU_FLASHNSC_REGION(nsc_region), reg);

	/* Configure SIZE */
	reg = 0;
	switch (size) {
	case 32:
		reg |= FLASHNSC_SIZE_SIZE_32;
		break;
	case 64:
		reg |= FLASHNSC_SIZE_SIZE_64;
		break;
	case 128:
		reg |= FLASHNSC_SIZE_SIZE_128;
		break;
	case 256:
		reg |= FLASHNSC_SIZE_SIZE_256;
		break;
	case 512:
		reg |= FLASHNSC_SIZE_SIZE_512;
		break;
	case 1024:
		reg |= FLASHNSC_SIZE_SIZE_1024;
		break;
	case 2048:
		reg |= FLASHNSC_SIZE_SIZE_2048;
		break;
	case 4096:
		reg |= FLASHNSC_SIZE_SIZE_4096;
		break;
	default:
		/* Disabled */
		reg |= FLASHNSC_SIZE_DISABLED;
		break;
	}
	if (lock)
		reg |= (FLASHNSC_SIZE_LOCK);
	WR4(sc, SPU_FLASHNSC_SIZE(nsc_region), reg);
}

void
nrf_spu_gpio_set_perm(mdx_device_t dev, int region_id,
    int perm)
{
	struct nrf_spu_softc *sc;

	sc = mdx_device_get_softc(dev);

	WR4(sc, SPU_GPIOPORT_PERM(region_id), perm);
}

void
nrf_spu_extdomain(mdx_device_t dev, bool secure, bool lock)
{
	struct nrf_spu_softc *sc;
	int reg;

	sc = mdx_device_get_softc(dev);

	reg = EXTDOMAIN_USERMAPPING_US;
	if (secure)
		reg |= EXTDOMAIN_SECATTR_SECURE;

	if (lock)
		reg |= EXTDOMAIN_LOCK;

	WR4(sc, SPU_EXTDOMAIN_PERM(0), reg);
}

void
nrf_spu_init(mdx_device_t dev, uint32_t base)
{
	struct nrf_spu_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(*sc));
	sc->base = base;
}

#ifdef MDX_FDT
static int
nrf_spu_probe(mdx_device_t dev)
{

	if (!mdx_of_is_compatible(dev, "nordic,nrf-spu"))
		return (MDX_ERROR);

	return (MDX_OK);
}

static int
nrf_spu_attach(mdx_device_t dev)
{
	struct nrf_spu_softc *sc;
	int error;

	sc = mdx_device_get_softc(dev);

	error = mdx_of_get_reg(dev, 0, &sc->base, NULL);
	if (error)
		return (error);

	return (0);
}

static struct mdx_device_ops nrf_spu_ops = {
	.probe = nrf_spu_probe,
	.attach = nrf_spu_attach,
};

static mdx_driver_t nrf_spu_driver = {
	"nrf_spu",
	&nrf_spu_ops,
	sizeof(struct nrf_spu_softc),
};

DRIVER_MODULE(nrf_spu, nrf_spu_driver);
#endif
