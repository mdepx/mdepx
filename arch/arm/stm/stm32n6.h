/*-
 * Copyright (c) 2025 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _ARM_STM_STM32N6_H_
#define _ARM_STM_STM32N6_H_

#include <arm/stm/stm32n6_rcc.h>
#include <arm/stm/stm32l4_usart.h>
#include <arm/stm/stm32f4_gpio.h>
#include <arm/stm/stm32f4_timer.h>
#include <arm/stm/stm32n6_ltdc.h>
#include <arm/stm/stm32n6_xspi.h>
#include <arm/stm/stm32n6_pwr.h>
#include <arm/stm/stm32n6_risaf.h>
#include <arm/stm/stm32n6_ramcfg.h>

#define	USART1_BASE	0x42001000
#define	USART2_BASE	0x40004400
#define	UART5_BASE	0x40005000

#define	GPIO_BASE	0x46020000
#define	RCC_BASE	0x46028000
#define	TIM1_BASE	0x42000000	/* 16-bit. */
#define	TIM2_BASE	0x40000000	/* 32-bit. */
#define	NVIC_BASE	0xE000E100
#define	LTDC_BASE	0x48001000
#define	DCMIPP_BASE	0x48002000
#define	GFXTIM_BASE	0x48004000
#define	VENC_BASE	0x48005000	/* H264/JPEG encoder */
#define	CSI_BASE	0x48006000	/* CSI2 HOST wrapper */
#define	SYSCFG_BASE	0x46008000
#define	DMA2D_BASE	0x48021000
#define	XSPI1_BASE	0x48025000
#define	PWR_BASE	0x46024800
#define	RAMCFG_BASE	0x42023000
#define	RISAF11_BASE	0x44030000	/* XSPI1 */
#define	RISAF4_BASE	0x44029000	/* NPU Master 0 */
#define	RISAF5_BASE	0x4402A000	/* NPU Master 1 */
#define	RISAF6_BASE	0x4402B000	/* CPU Master */

#endif	/* !_ARM_STM_STM32N6_H_ */
