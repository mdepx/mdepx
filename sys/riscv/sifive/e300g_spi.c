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

#include <dev/spi/spi.h>

#include <riscv/sifive/e300g_spi.h>

#define	RD4(_sc, _reg)		*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static int 
e300g_spi_transfer(struct spi_device *dev,
    uint8_t *out, uint8_t *in, uint32_t len)
{
	struct spi_softc *sc;
	uint32_t reg;
	int i;

	sc = (struct spi_softc *)dev->sc;

	WR4(sc, SPI_CSID, sc->cs);

	for (i = 0; i < len; i++) {
		do {
			reg = RD4(sc, SPI_TXDATA);
		} while (reg & TXDATA_FULL);

		WR4(sc, SPI_TXDATA, out[i]);

		/*
		 * TODO: add RX part
		 */
	}

	return (0);
}

void
e300g_spi_poll_txwm(struct spi_device *dev)
{
	struct spi_softc *sc;
	uint32_t ip;

	sc = (struct spi_softc *)dev->sc;

	do {
		ip = RD4(sc, SPI_IP);
	} while ((ip & IP_TXWM) == 0);
}

int
e300g_spi_init(struct spi_softc *sc, struct spi_device *dev,
    uint32_t base, uint8_t cs)
{
	uint32_t reg;

	sc->base = base;
	sc->cs = cs;

	dev->sc = sc;
	dev->transfer = e300g_spi_transfer;

	WR4(sc, SPI_SCKDIV, 8);

	/*
	 * TODO
	 * reg = (SCKMODE_PHA | SCKMODE_POL);
	 * WR4(sc, SPI_SCKMODE, reg);
	 */

	WR4(sc, SPI_CSMODE, CSMODE_AUTO);
	WR4(sc, SPI_CSDEF, 0xffff);

	reg = (8 << FMT_LEN_S);
	reg |= FMT_DIR_TX;
	WR4(sc, SPI_FMT, reg);

	WR4(sc, SPI_TXMARK, 1);

	return (0);
}
