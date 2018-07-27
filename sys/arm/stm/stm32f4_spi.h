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

#ifndef _ARM_STM_STM32F4_SPI_H_
#define _ARM_STM_STM32F4_SPI_H_

#define	SPI_CR1		0x00 /* control register 1 */
#define	 CR1_BIDIMODE	(1 << 15) /* Bidirectional data mode enable */
#define	 CR1_BIDIOE	(1 << 14) /* Output enable in bidirectional mode */
#define	 CR1_CRCEN	(1 << 13) /* Hardware CRC calculation enable */
#define	 CR1_CRCNEXT	(1 << 12) /* CRC transfer next */
#define	 CR1_DFF	(1 << 11) /* Data frame format */
#define	 CR1_RXONLY	(1 << 10) /* Receive only mode enable */
#define	 CR1_SSM	(1 << 9) /* Software slave management */
#define	 CR1_SSI	(1 << 8) /* Internal slave select */
#define	 CR1_LSBFIRST	(1 << 7) /* Frame format */
#define	 CR1_SPE	(1 << 6) /* SPI enable */
#define	 CR1_BR_S	3 /* Baud rate control */
#define	 CR1_BR_M	(0x7 << CR1_BR_S)
#define	 CR1_MSTR	(1 << 2) /* Master selection */
#define	 CR1_CPOL	(1 << 1) /* Clock polarity */
#define	 CR1_CPHA	(1 << 0) /* Clock phase */
#define	SPI_CR2		0x04 /* SPI control register 2 */
#define	 CR2_TXEIE	(1 << 7) /* Tx buffer empty interrupt enable */
#define	 CR2_RXNEIE	(1 << 6) /* RX buffer not empty interrupt enable */
#define	 CR2_ERRIE	(1 << 5) /* Error interrupt enable */
#define	 CR2_FRF	(1 << 4) /* Frame format */
#define	 CR2_SSOE	(1 << 2) /* SS output enable */
#define	 CR2_TXDMAEN	(1 << 1) /* Tx buffer DMA enable */
#define	 CR2_RXDMAEN	(1 << 0) /* Rx buffer DMA enable */
#define	SPI_SR		0x08 /* SPI status register */
#define	 SR_FTLVL_S	11	/* FIFO Transmission Level */
#define	 SR_FTLVL_M	(0x3 << SR_FTLVL_S)
#define	 SR_FTLVL_FULL	(0x3 << SR_FTLVL_S)
#define	 SR_TXE		(1 << 1) /* Transmit buffer empty */
#define	 SR_RXNE	(1 << 0) /* Receive buffer not empty */
#define	SPI_DR		0x0C /* SPI data register */
#define	SPI_CRCPR	0x10 /* SPI CRC polynomial register */
#define	SPI_RXCRCR	0x14 /* SPI RX CRC register */
#define	SPI_TXCRCR	0x18 /* SPI TX CRC register */
#define	SPI_I2SCFGR	0x1C /* SPI_I2S configuration register */
#define	SPI_I2SPR	0x20 /* SPI_I2S prescaler register */

struct stm32f4_spi_softc {
	uint32_t base;
};

void stm32f4_spi_init(struct spi_device *dev, struct stm32f4_spi_softc *sc,
    uint32_t base);
int stm32f4_spi_transfer(struct spi_device *dev, uint8_t *txcmd,
    uint8_t *rxcmd, uint32_t nmsg);
void stm32f4_spi_intr(void *arg);

#endif /* !_ARM_STM_STM32F4_SPI_H_ */
