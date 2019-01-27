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

#ifndef _ARM_NXP_LPC43XX_H_
#define _ARM_NXP_LPC43XX_H_

#include <arm/nxp/lpc43xx_usart.h>

/* APB3 peripherals */
#define	BASE_ADC1		0x400E4000
#define	BASE_ADC0		0x400E3000
#define	BASE_C_CAN0		0x400E2000
#define	BASE_DAC		0x400E1000
#define	BASE_I2C1		0x400E0000

/* APB2 peripherals */
#define	BASE_GIMA		0x400C7000
#define	BASE_QEI		0x400C6000
#define	BASE_SSP1		0x400C5000
#define	BASE_timer3		0x400C4000
#define	BASE_timer2		0x400C3000
#define	BASE_USART3		0x400C2000
#define	BASE_USART2		0x400C1000
#define	BASE_RIT		0x400C0000

/* APB1 peripherals */
#define	BASE_C_CAN1		0x400A4000
#define	BASE_I2S1		0x400A3000
#define	BASE_I2S0		0x400A2000
#define	BASE_I2C0		0x400A1000
#define	BASE_MOTOCONPWM		0x400A0000

/* APB0 peripherals */
#define	BASE_GPIO_GROUP1	0x40089000
#define	BASE_GPIO_GROUP0	0x40088000
#define	BASE_GPIO		0x40087000
#define	BASE_SCU		0x40086000
#define	BASE_timer1		0x40085000
#define	BASE_timer0		0x40084000
#define	BASE_SSP0		0x40083000
#define	BASE_UART1		0x40082000
#define	BASE_USART0		0x40081000
#define	BASE_WWDT		0x40080000

#endif /* !_ARM_NXP_LPC43XX_H_ */
