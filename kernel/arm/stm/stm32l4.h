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

#ifndef _ARM_STM_STM32F4_H_
#define _ARM_STM_STM32F4_H_

#include <arm/stm/stm32l4_exti.h>
#include <arm/stm/stm32l4_usart.h>
#include <arm/stm/stm32f4_flash.h>
#include <arm/stm/stm32f4_gpio.h>
#include <arm/stm/stm32l4_pwr.h>
#include <arm/stm/stm32f4_timer.h>
#include <arm/stm/stm32f4_spi.h>
#include <arm/stm/stm32l4_syscfg.h>
#include <arm/stm/stm32l4_rcc.h>
#include <arm/stm/stm32l4_rtc.h>
#include <arm/stm/stm32f4_dsi.h>

#define	USART1_BASE	0x40013800
#define	USART2_BASE	0x40004400
#define	USART3_BASE	0x40004800
#define	USART4_BASE	0x40004C00

#define	ADC_BASE	0x50040000
#define	FLASH_BASE	0x40022000
#define	PWR_BASE	0x40007000
#define	GPIO_BASE	0x48000000
#define	NVIC_BASE	0xE000E000
#define	RCC_BASE	0x40021000
#define	TIM1_BASE	0x40012C00
#define	SPI1_BASE	0x40013000
#define	SPI2_BASE	0x40003800
#define	SPI3_BASE	0x40003C00
#define	EXTI_BASE	0x40010400
#define	SYSCFG_BASE	0x40010000
#define	RTC_BASE	0x40002800
#define	DSIHOST_BASE	0x40016C00

#endif	/* !_ARM_STM_STM32F4_H_ */
