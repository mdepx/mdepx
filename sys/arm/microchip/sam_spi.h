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

#ifndef _ARM_MICROCHIP_SAM_SPI_H_
#define _ARM_MICROCHIP_SAM_SPI_H_

#define	SPI_CTRLA	0x00 /* Control A */
#define	SPI_CTRLB	0x04 /* Control B */
#define	SPI_CTRLC	0x08 /* Control C */
#define	SPI_BAUD	0x0C /* Baud */
#define	SPI_INTENCLR	0x14 /* Interrupt Enable Clear */
#define	SPI_INTENSET	0x16 /* Interrupt Enable Set */
#define	SPI_INTFLAG	0x18 /* Interrupt Flag Status and Clear */
#define	SPI_STATUS	0x1A /* Status */
#define	SPI_SYNCBUSY	0x1C /* Synchronization Busy */
#define	SPI_LENGTH	0x22 /* Length */
#define	SPI_ADDR	0x24 /* Address */
#define	SPI_DATA	0x28 /* Data */
#define	SPI_DBGCTRL	0x30 /* Debug Control */

struct sam_spi_softc {
	uint32_t base;
};

#endif /* !_ARM_MICROCHIP_SAM_SPI_H_ */
