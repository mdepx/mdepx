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

#ifndef _ARM_STM_STM32L4_USART_H_
#define _ARM_STM_STM32L4_USART_H_

#define	USART_CR1		0x00 /* Control register 1 */
#define	 CR1_M1			(1 << 28) /* Word length */
#define	 CR1_EOBIE		(1 << 27) /* End of Block interrupt enable */
#define	 CR1_RTOIE		(1 << 26) /* Receiver timeout interrupt enable */
#define	 CR1_DEAT_S		21 /* Driver Enable assertion time */
#define	 CR1_DEAT_M		(0x1f << CR1_DEAT_S)
#define	 CR1_DEDT_S		16 /* Driver Enable de-assertion time */
#define	 CR1_DEDT_M		(0x1f << CR1_DEDT_S)
#define	 CR1_OVER8		(1 << 15) /* Oversampling mode */
#define	 CR1_CMIE		(1 << 14) /* Character match interrupt enable */
#define	 CR1_MME		(1 << 13) /* Mute mode enable */
#define	 CR1_M0			(1 << 12) /* Word length */
#define	 CR1_WAKE		(1 << 11) /* Receiver wakeup method */
#define	 CR1_PCE		(1 << 10) /* Parity control enable */
#define	 CR1_PS_ODD		(1 << 9) /* Parity selection */
#define	 CR1_PEIE		(1 << 8) /* PE interrupt enable */
#define	 CR1_TXEIE		(1 << 7) /* interrupt enable */
#define	 CR1_TCIE		(1 << 6) /* Transmission complete interrupt enable */
#define	 CR1_RXNEIE		(1 << 5) /* RXNE interrupt enable */
#define	 CR1_IDLEIE		(1 << 4) /* IDLE interrupt enable */
#define	 CR1_TE			(1 << 3) /* Transmitter enable */
#define	 CR1_RE			(1 << 2) /* Receiver enable */
#define	 CR1_UESM		(1 << 1) /* USART enable in Stop mode */
#define	 CR1_UE			(1 << 0) /* USART enable */
#define	USART_CR2		0x04 /* Control register 2 */
#define	USART_CR3		0x08 /* Control register 3 */
#define	USART_BRR		0x0C /* Baud rate register */
#define	USART_GTPR		0x10 /* Guard time and prescaler register */
#define	USART_ISR		0x1C /* Interrupt and status register */
#define	 ISR_RXNE		(1 << 5) /* Read data register not empty */
#define	 ISR_TXE		(1 << 7) /* Transmit data register empty */
#define	USART_RDR		0x24 /* Receive data register */
#define	USART_TDR		0x28 /* Transmit data register */

struct stm32l4_usart_softc {
	uint32_t base;
};

int stm32l4_usart_init(struct stm32l4_usart_softc *sc, uint32_t base,
    uint32_t cpu_freq, uint32_t baud_rate);
void stm32l4_usart_putc(struct stm32l4_usart_softc *sc, char c);

#endif /* !_ARM_STM_STM32L4_USART_H_ */
