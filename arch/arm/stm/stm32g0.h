/*-
 * Copyright (c) 2023 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _ARM_STM_STM32G0_H_
#define _ARM_STM_STM32G0_H_

#include <arm/stm/stm32f4_gpio.h>
#include <arm/stm/stm32f4_timer.h>
#include <arm/stm/stm32g0_rcc.h>
#include <arm/stm/stm32l4_usart.h>
#include <arm/stm/stm32f4_i2c.h>
#include <arm/stm/stm32g0_syscfg.h>
#include <arm/stm/stm32g0_exti.h>

#define	USART2_BASE	0x40004400
#define	USART1_BASE	0x40013800
#define	RCC_BASE	0x40021000
#define	FLASH_BASE	0x40022000
#define	GPIO_BASE	0x50000000
#define	TIM1_BASE	0x40012C00
#define	NVIC_BASE	0xE000E100
#define	I2C1_BASE	0x40005400
#define	I2C2_BASE	0x40005800
#define	PWR_BASE	0x40007000
#define	I2C3_BASE	0x40008800
#define	SYSCFG_BASE	0x40010000
#define	EXTI_BASE	0x40021800

#endif	/* !_ARM_STM_STM32G0_H_ */
