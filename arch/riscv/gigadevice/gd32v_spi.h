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
#define	 CTL0_FF16	(1 << 11) /* Data frame format */
#define	 CTL0_SWNSSEN	(1 << 9) /* NSS software mode enable */
#define	 CTL0_SWNSS	(1 << 8) /* NSS pin selection in NSS software mode */
#define	 CTL0_LF	(1 << 7) /* LSB first mode */
#define	 CTL0_SPIEN	(1 << 6) /* SPI enable */
#define	 CTL0_PSC_S	3
#define	 CTL0_PSC_M	(0x7 << CTL0_PSC_S)
#define	 CTL0_PSC_2	(0 << CTL0_PSC_S)
#define	 CTL0_PSC_4	(1 << CTL0_PSC_S)
#define	 CTL0_PSC_8	(2 << CTL0_PSC_S)
#define	 CTL0_PSC_16	(3 << CTL0_PSC_S)
#define	 CTL0_PSC_32	(4 << CTL0_PSC_S)
#define	 CTL0_PSC_64	(5 << CTL0_PSC_S)
#define	 CTL0_PSC_128	(6 << CTL0_PSC_S)
#define	 CTL0_PSC_256	(7 << CTL0_PSC_S)
#define	 CTL0_MSTMOD	(1 << 2) /* Master mode enable */
#define	 CTL0_CKPL	(1 << 1) /* Clock polarity selection */
#define	 CTL0_CKPH	(1 << 0) /* Clock phase selection */
#define	SPI_CTL1	0x04	/* Control register 1 */
#define	 CTL1_DMATEN	(1 << 1) /* Transmit buffer DMA enable */
#define	 CTL1_DMAREN	(1 << 0) /* Receive buffer DMA enable */
#define	SPI_STAT	0x08	/* Status register */
#define	 STAT_TRANS	(1 << 7) /* Transmitting ongoing bit */
#define	 STAT_TBE	(1 << 1) /* Transmit buffer empty */
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

void gd32v_spi_init(mdx_device_t dev, uint32_t base);
void gd32v_spi_intr(void *arg);

#endif /* !_RISCV_GIGADEVICE_GD32V_SPI_H_ */
