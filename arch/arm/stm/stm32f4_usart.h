/*-
 * Copyright (c) 2018-2023 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _ARM_STM_STM32F4_USART_H_
#define _ARM_STM_STM32F4_USART_H_

#define	USART_SR		0x00 /* Status register */
#define	 USART_SR_RXNE		(1 << 5) /* Read data register not empty */
#define	 USART_SR_TXE		(1 << 7) /* Transmit data register empty */
#define	USART_DR		0x04 /* Data register */
#define	USART_BRR		0x08 /* Baud rate register */
#define	USART_CR1		0x0C /* Control register 1 */
#define	 USART_CR1_UE		(1 << 13) /* USART enable */
#define	 USART_CR1_RXNEIE	(1 << 5) /* RXNE interrupt enable */
#define	 USART_CR1_TE		(1 << 3) /* Transmitter enable */
#define	 USART_CR1_RE		(1 << 2) /* Receiver enable */
#define	USART_CR2		0x10 /* Control register 2 */
#define	USART_CR3		0x14 /* Control register 3 */
#define	 CR3_DMAT		(1 << 7) /* DMA enable transmitter */
#define	 CR3_DMAR		(1 << 6) /* DMA enable receiver */
#define	USART_GTPR		0x18 /* Guard time and prescaler register */

struct stm32f4_usart_softc {
	uint32_t base;
	void (*cb)(uint8_t ch);
	int cb_configured;
};

int stm32f4_usart_init(struct stm32f4_usart_softc *sc, uint32_t base,
    uint32_t cpu_freq, uint32_t baud_rate);
void stm32f4_usart_putc(struct stm32f4_usart_softc *sc, char c);
void stm32f4_usart_intr(void *arg, int irq);
void stm32f4_usart_setup_receiver(struct stm32f4_usart_softc *sc, int dma,
    void (*cb)(uint8_t ch));

#endif /* !_ARM_STM_STM32F4_USART_H_ */
