/*-
 * Copyright (c) 2017-2020 Ruslan Bukin <br@bsdpad.com>
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

/*
 * SPI memory map.
 * SiFive E300 Platform Reference Manual, Version 1.0.1
 */

#ifndef _SYS_RISCV_SIFIVE_E300G_SPI_H_
#define _SYS_RISCV_SIFIVE_E300G_SPI_H_

#include <dev/spi/spi.h>

#define	SPI_SCKDIV		0x000	/* Serial clock divisor */
#define	SPI_SCKMODE		0x004	/* Serial clock mode */
#define	 SCKMODE_PHA		(1 << 0) /* Data is shifted on the leading edge of SCK and sampled on the trailing edge of SCK */
#define	 SCKMODE_POL		(1 << 1) /* Inactive state of SCK is logical 1 */
#define	SPI_CSID		0x010	/* Chip select ID */
#define	SPI_CSDEF		0x014	/* Chip select default */
#define	SPI_CSMODE		0x018	/* Chip select mode */
#define	 CSMODE_AUTO		0x1	/* Assert/de-assert CS at the beginning/end of each frame */
#define	 CSMODE_HOLD		0x2	/* Keep CS continuously asserted after the initial frame */
#define	 CSMODE_OFF		0x3	/* Disable hardware control of the CS pin */
#define	SPI_DELAY0		0x028	/* Delay control 0 */
#define	SPI_DELAY1		0x02C	/* Delay control 1 */
#define	SPI_FMT			0x040	/* Frame format */
#define	 FMT_PROTO_S		(0)
#define	 FMT_PROTO_M		(3 << FMT_PROTO_S)
#define	 FMT_PROTO_SINGLE	(1 << FMT_PROTO_S)
#define	 FMT_PROTO_DUAL		(2 << FMT_PROTO_S)
#define	 FMT_PROTO_QUAD		(3 << FMT_PROTO_S)
#define	 FMT_ENDIAN_LSB		(1 << 2) /* Transmit least-significant bit (LSB) first */
#define	 FMT_ENDIAN_MSB		(0) /* Transmit most-significant bit (MSB) first */
#define	 FMT_DIR_TX		(1 << 3) /* The receive FIFO is not populated. */
#define	 FMT_LEN_S		16 /* the number of bits per frame */
#define	 FMT_LEN_M		(0xf << FMT_LEN_S)
#define	SPI_TXDATA	0x048	/* Tx FIFO data */
#define	 TXDATA_FULL	(1 << 31)
#define	SPI_RXDATA	0x04C	/* Rx FIFO data */
#define	SPI_TXMARK	0x050	/* Tx FIFO watermark */
#define	SPI_RXMARK	0x054	/* Rx FIFO watermark */
#define	SPI_FCTRL	0x060	/* SPI flash interface control */
#define	SPI_FFMT	0x064	/* SPI flash instruction format */
#define	SPI_IE		0x070	/* SPI interrupt enable */
#define	SPI_IP		0x074	/* SPI interrupt pending */
#define	 IP_TXWM	(1 << 0)
#define	 IP_RXWM	(1 << 1)

struct spi_softc {
	uint32_t base;
	int cs;
};

int e300g_spi_init(mdx_device_t dev, uint32_t base);
int e300g_spi_setup(mdx_device_t dev, uint8_t cs);
void e300g_spi_poll_txwm(mdx_device_t dev);

#endif /* !_SYS_RISCV_SIFIVE_E300G_SPI_H_ */
