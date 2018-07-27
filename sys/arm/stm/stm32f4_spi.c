/*-
 * Copyright (c) 2018 Ruslan Bukin <br@bsdpad.com>
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
 * ARE DISCLAIMED.  IN NO SPI SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#include <dev/spi/spi.h>

#include "stm32f4_spi.h"

#define	SPI_DEBUG
#undef	SPI_DEBUG

#ifdef	SPI_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	RD2(_sc, _reg)		*(volatile uint16_t *)((_sc)->base + _reg)
#define	WR2(_sc, _reg, _val)	*(volatile uint16_t *)((_sc)->base + _reg) = _val
#define	RD1(_sc, _reg)		*(volatile uint8_t *)((_sc)->base + _reg)
#define	WR1(_sc, _reg, _val)	*(volatile uint8_t *)((_sc)->base + _reg) = _val

void
stm32f4_spi_intr(void *arg)
{
	struct stm32f4_spi_softc *sc;
	uint32_t pending;

	sc = arg;

	pending = RD2(sc, SPI_SR);
	printf("spi intr 0x%x\n", pending);
}

static void
stm32f4_spi_setup(struct stm32f4_spi_softc *sc)
{
	uint32_t reg;

	RD2(sc, SPI_SR);

	reg = RD2(sc, SPI_CR1);
	reg |= (CR1_MSTR);
	reg |= (CR1_SSI);
	reg |= (CR1_SSM);
	reg |= (CR1_SPE);
	reg |= (CR1_CPOL);
	reg |= (CR1_CPHA);
	WR2(sc, SPI_CR1, reg);
}

int
stm32f4_spi_transfer(struct spi_device *dev, uint8_t *out,
    uint8_t *in, uint32_t len)
{
	struct stm32f4_spi_softc *sc;
	int i;

	sc = dev->sc;

	for (i = 0; i < len; i++) {
		if (out != 0) {
			while ((RD2(sc, SPI_SR) & SR_FTLVL_M) == \
			    SR_FTLVL_FULL)
				;

			dprintf("txrx %02x", out[i]);
			WR1(sc, SPI_DR, out[i]);
		}

		if (in != 0) {
			while ((RD2(sc, SPI_SR) & SR_RXNE) == 0)
				;

			in[i] = RD1(sc, SPI_DR);
			dprintf(" %02x", in[i]);
		}

		dprintf("\n");
	}

	return (0);
}

void
stm32f4_spi_init(struct spi_device *dev, struct stm32f4_spi_softc *sc,
    uint32_t base)
{

	sc->base = base;
	dev->sc = sc;
	dev->transfer = stm32f4_spi_transfer;

	stm32f4_spi_setup(sc);
}
