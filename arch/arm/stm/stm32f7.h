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

#ifndef _ARM_STM_STM32F7_H_
#define _ARM_STM_STM32F7_H_

#include <arm/stm/stm32l4_usart.h>
#include <arm/stm/stm32f4_flash.h>
#include <arm/stm/stm32f4_gpio.h>
#include <arm/stm/stm32f4_pwr.h>
#include <arm/stm/stm32f4_rcc.h>
#include <arm/stm/stm32f4_fmc.h>
#include <arm/stm/stm32f4_ltdc.h>
#include <arm/stm/stm32f4_dsi.h>
#include <arm/stm/stm32f4_timer.h>
#include <arm/stm/stm32f7_eth.h>
#include <arm/stm/stm32f7_syscfg.h>

#define	USART6_BASE	0x40011400
#define	FLASH_BASE	0x40023C00
#define	PWR_BASE	0x40007000
#define	RCC_BASE	0x40023800
#define	GPIO_BASE	0x40020000
#define	FMC_BASE	0xA0000000
#define	LTDC_BASE	0x40016800
#define	NVIC_BASE	0xE000E100
#define	TIM2_BASE	0x40000000
#define	ETH_BASE	0x40028000
#define	SYSCFG_BASE	0x40013800

#endif	/* !_ARM_STM_STM32F7_H_ */
