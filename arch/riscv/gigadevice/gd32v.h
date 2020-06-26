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

#ifndef _RISCV_GIGADEVICE_GD32V_H_
#define _RISCV_GIGADEVICE_GD32V_H_

#include <riscv/gigadevice/gd32v_dma.h>
#include <riscv/gigadevice/gd32v_exmc.h>
#include <riscv/gigadevice/gd32v_exti.h>
#include <riscv/gigadevice/gd32v_fmc.h>
#include <riscv/gigadevice/gd32v_gpio.h>
#include <riscv/gigadevice/gd32v_i2c.h>
#include <riscv/gigadevice/gd32v_rcu.h>
#include <riscv/gigadevice/gd32v_spi.h>
#include <riscv/gigadevice/gd32v_timer.h>
#include <riscv/gigadevice/gd32v_usart.h>

#define	BASE_RCU	0x40021000
#define	BASE_GFIO	0x40010000
#define	BASE_GPIOA	0x40010800
#define	BASE_GPIOB	0x40010C00
#define	BASE_GPIOC	0x40011000
#define	BASE_GPIOD	0x40011400
#define	BASE_GPIOE	0x40011800
#define	BASE_USART0	0x40013800
#define	BASE_USART1	0x40004400
#define	BASE_USART2	0x40004800
#define	BASE_UART3	0x40004C00
#define	BASE_UART4	0x40005000
#define	BASE_EXTI	0x40010400
#define	BASE_TIMER0	0x40012C00
#define	BASE_TIMER1	0x40000000
#define	BASE_TIMER2	0x40000400
#define	BASE_TIMER3	0x40000800
#define	BASE_TIMER4	0x40000C00
#define	BASE_TIMER5	0x40001000
#define	BASE_TIMER6	0x40001400
#define	BASE_ECLIC	0xd2000000
#define	BASE_I2C0	0x40005400
#define	BASE_I2C1	0x40005800
#define	BASE_SPI0	0x40013000
#define	BASE_SPI1	0x40003800
#define	BASE_SPI2	0x40003C00
#define	BASE_I2S1	0x40003800
#define	BASE_I2S2	0x40003C00
#define	BASE_DMA0	0x40020000
#define	BASE_DMA1	0x40020400
#define	BASE_FMC	0x40022000
#define	BASE_EXMC	0xA0000000

#endif	/* !_RISCV_GIGADEVICE_GD32V_H_ */
