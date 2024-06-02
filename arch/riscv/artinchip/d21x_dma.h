/*-
 * Copyright (c) 2024 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _RISCV_ARTINCHIP_D21X_DMA_H_
#define	_RISCV_ARTINCHIP_D21X_DMA_H_

#define	DMA_ID_SRAM		0
#define	DMA_ID_DRAM		1
#define	DMA_ID_PSADC_Q1		4
#define	DMA_ID_PSADC_Q2		5
#define	DMA_ID_SPI2		8
#define	DMA_ID_SPI3		9
#define	DMA_ID_SPI0		10
#define	DMA_ID_SPI1		11
#define	DMA_ID_I2S0		12
#define	DMA_ID_I2S1		13
#define	DMA_ID_AUDIO_DMIC	14
#define	DMA_ID_AUDIO_ADC	15
#define	DMA_ID_UART0		16
#define	DMA_ID_UART1		17
#define	DMA_ID_UART2		18
#define	DMA_ID_UART3		19
#define	DMA_ID_UART4		20
#define	DMA_ID_UART5		21
#define	DMA_ID_UART6		22
#define	DMA_ID_UART7		23
#define	DMA_ID_UART(n)		(DMA_ID_UART0 + (n))

#endif /* !_RISCV_ARTINCHIP_D21X_DMA_H_ */
