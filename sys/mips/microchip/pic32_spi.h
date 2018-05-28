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

#ifndef _MIPS_MICROCHIP_PIC32_SPI_H_
#define _MIPS_MICROCHIP_PIC32_SPI_H_

#include <dev/spi/spi.h>

#define	SPICON	0x00
#define	 SPICON_MSSEN	(1 << 28)	/* MSSEN: Master Mode Slave Select Enable bit */
#define	 SPICON_ON	(1 << 15)	/* Module On bit */
#define	 SPICON_DISSDO	(1 << 12)	/* Disable SDOx Pin bit */
#define	 SPICON_MODE32	(1 << 11)
#define	 SPICON_MODE16	(1 << 10)
#define	 SPICON_SMP	(1 << 9)	/* Data Input Sample Phase bit */
#define	 SPICON_CKE	(1 << 8)	/* Clock Edge Select bit */
#define	 SPICON_SSEN	(1 << 7)	/* Slave Select Enable (Slave mode) bit */
#define	 SPICON_CKP	(1 << 6)	/* Clock Polarity Select */
#define	 SPICON_MSTEN	(1 << 5)	/* Master Mode Enable bit */
#define	SPISTAT	0x10
#define	 SPISTAT_SPITBF	(1 << 1)	/* Transmit Buffer Full Status bit */
#define	 SPISTAT_SPIRBF	(1 << 0)	/* Receive Buffer Full Status bit */
#define	SPIBUF	0x20
#define	SPIBRG	0x30
#define	SPICON2	0x40

struct pic32_spi_softc {
	uint32_t base;
};

void pic32_spi_init(struct spi_device *dev, struct pic32_spi_softc *sc,
    uint32_t base, uint32_t cpu_freq, uint32_t baud_rate, uint32_t spicon);

#endif /* !_MIPS_MICROCHIP_PIC32_SPI_H_ */
