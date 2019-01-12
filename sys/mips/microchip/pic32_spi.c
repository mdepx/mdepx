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
#include <dev/spi/spi.h>

#include <mips/microchip/pic32_spi.h>

#define	SPI_DEBUG
#undef	SPI_DEBUG

#ifdef	SPI_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val
#define	RD2(_sc, _reg)		\
	*(volatile uint16_t *)((_sc)->base + _reg)
#define	WR2(_sc, _reg, _val)	\
	*(volatile uint16_t *)((_sc)->base + _reg) = _val
#define	RD1(_sc, _reg)		\
	*(volatile uint8_t *)((_sc)->base + _reg)
#define	WR1(_sc, _reg, _val)	\
	*(volatile uint8_t *)((_sc)->base + _reg) = _val

static int
pic32_spi_transfer(struct spi_device *dev, 
    uint8_t *out, uint8_t *in, uint32_t len)
{
	struct pic32_spi_softc *sc;
	int timeout;
	uint8_t rd;
	int i;

	sc = (struct pic32_spi_softc *)dev->sc;

	for (i = 0; i < len; i++) {
		WR1(sc, SPIBUF, out[i]);

		if (in != NULL) {
			timeout = 1000;
			do {
				if (RD4(sc, SPISTAT) & SPISTAT_SPIRBF)
					break;
			} while (--timeout);
			if (timeout == 0)
				return (-1);

			rd = RD1(sc, SPIBUF);
			in[i] = rd;

			dprintf("wr %x rd %x\n", out[i], in[i]);
		} else
			dprintf("wr %x\n", out[i]);
	}

	return (0);
}

void
pic32_spi_init(struct spi_device *dev,
    struct pic32_spi_softc *sc, uint32_t base,
    uint32_t cpu_freq, uint32_t baud_rate,
    uint32_t spicon)
{
	uint32_t reg;

	sc->base = base;

	dev->sc = sc;
	dev->transfer = pic32_spi_transfer;

	WR4(sc, SPICON, 0);
	WR4(sc, SPISTAT, 0);

	/* Baud = Frequency_Pbclk / (2 * (SPIxBRG + 1)) */
	reg = ((cpu_freq / baud_rate) / 2) - 1;
	WR4(sc, SPIBRG, reg);

	reg = spicon;
	WR4(sc, SPICON, reg);
	reg |= SPICON_ON;
	WR4(sc, SPICON, reg);
}
