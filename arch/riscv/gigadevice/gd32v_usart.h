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

#ifndef _RISCV_GIGADEVICE_GD32V_USART_H_
#define _RISCV_GIGADEVICE_GD32V_USART_H_

#define	USART_STAT		0x00 /* Status register */
#define	 USART_STAT_TBE		(1 << 7) /* Transmit data buffer empty */
#define	 USART_STAT_TC		(1 << 6) /* Transmission complete */
#define	 USART_STAT_RBNE	(1 << 5) /* Read data buffer not empty */
#define	USART_DATA		0x04 /* Data register */
#define	USART_BAUD		0x08 /* Baud rate register */
#define	USART_CTL0		0x0C /* Control register 0 */
#define	 USART_CTL0_UEN		(1 << 13) /* USART enable */
#define	 USART_CTL0_RBNEIE	(1 << 5) /* RBNE interrupt enable */
#define	 USART_CTL0_TEN		(1 << 3) /* Transmitter enable */
#define	 USART_CTL0_REN		(1 << 2) /* Receiver enable */
#define	USART_CTL1		0x10
#define	USART_CTL2		0x14
#define	USART_CR1		0x10 /* Control register 1 */
#define	USART_CR2		0x14 /* Control register 2 */
#define	USART_GR		0x18 /* Guard time and prescaler register */

struct gd32v_usart_softc {
	uint32_t base;
	int cpu_freq;
};

int gd32v_usart_init(mdx_device_t dev, uint32_t base, uint32_t cpu_freq);
void gd32v_usart_putc(mdx_device_t dev, int c);

#endif /* !_RISCV_GIGADEVICE_GD32V_USART_H_ */
