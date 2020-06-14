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

#ifndef _RISCV_GIGADEVICE_GD32V_SPI_H_
#define _RISCV_GIGADEVICE_GD32V_SPI_H_

#include <sys/sem.h>

#define	SPI_CTL0	0x00	/* Control register 0 */
#define	SPI_CTL1	0x04	/* Control register 1 */
#define	SPI_STAT	0x08	/* Status register */
#define	SPI_DATA	0x0C	/* Data register */
#define	SPI_CRCPOLY	0x10	/* CRC polynomial register */
#define	SPI_RCRC	0x14	/* RX CRC register */
#define	SPI_TCRC	0x18	/* TX CRC register */
#define	SPI_I2SCTL	0x1C	/* I2S control register */
#define	SPI_I2SPSC	0x20	/* I2S clock prescaler register */

struct gd32v_spi_softc {
	uint32_t base;
	mdx_sem_t sem;
};

int gd32v_spi_init(mdx_device_t dev, uint32_t base);

#endif /* !_RISCV_GIGADEVICE_GD32V_SPI_H_ */
