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

#ifndef _ARM_STM_STM32F4_GPIO_H_
#define _ARM_STM_STM32F4_GPIO_H_

#define	GPIO_MODER(n)	(0x00 + 0x400 * (n))	/* port mode */
#define	 MODE_INP	0	/* Input mode */
#define	 MODE_OUT	1	/* General purpose output mode */
#define	 MODE_ALT	2	/* Alternate function mode */
#define	 MODE_ANA	3	/* Analog mode (reset state) */
#define	GPIO_OTYPER(n)	(0x04 + 0x400 * (n))	/* port output type */
#define	 OT_PP		0	/* Push-pull */
#define	 OT_OD		1	/* Opendrain */
#define	GPIO_OSPEEDR(n)	(0x08 + 0x400 * (n))	/* port output speed */
#define	 OSPEEDR_M	3	/* Mask */
#define	 OS_VL		0	/* Very low */
#define	 OS_L		1	/* Low */
#define	 OS_H		2	/* High */
#define	 OS_VH		3	/* Very High */
#define	GPIO_PUPDR(n)	(0x0C + 0x400 * (n))	/* port pull-up/pull-down */
#define	 PUPDR_M	3
#define	 FLOAT		0	/* No pull-up, pull-down */
#define	 PULLUP		1
#define	 PULLDOWN	2
#define	GPIO_IDR(n)	(0x10 + 0x400 * (n))	/* port input data */
#define	GPIO_ODR(n)	(0x14 + 0x400 * (n))	/* port output data */
#define	GPIO_BSRR(n)	(0x18 + 0x400 * (n))	/* port bit set/reset */
#define	GPIO_LCKR(n)	(0x1C + 0x400 * (n))	/* port configuration lock */
#define	GPIO_AFRL(n)	(0x20 + 0x400 * (n))	/* alternate function low */
#define	GPIO_AFRH(n)	(0x24 + 0x400 * (n))	/* alternate function high */
#define	GPIO_BRR(n)	(0x28 + 0x400 * (n))	/* port bit reset */

enum {
	PORT_A,
	PORT_B,
	PORT_C,
	PORT_D,
	PORT_E,
	PORT_F,
	PORT_G,
	PORT_H,
	PORT_I,
	PORT_J,
	PORT_K,
	PORT_L,
	PORT_M,
	PORT_N,
	PORT_O,
	PORT_P,
	PORT_Q,
};

struct stm32_gpio_pin {
	uint32_t	port;
	uint32_t	pin;
	uint32_t	mode;		/* mode */
	uint32_t	alt;		/* alternate function ID */
	uint32_t	otyper;		/* port output type */
	uint32_t	ospeedr;	/* port output speed */
	uint32_t	pupdr;		/* port pull-up/pull-down */
};

#define	PINS_END	{ -1, -1, -1, -1, -1, -1, -1}


struct stm32f4_gpio_softc {
	uint32_t base;
};

int stm32f4_gpio_init(struct stm32f4_gpio_softc *sc, uint32_t base);
void pin_configure(struct stm32f4_gpio_softc *sc,
    const struct stm32_gpio_pin *pins);
void pin_set(struct stm32f4_gpio_softc *sc, uint32_t port,
    uint32_t pin, uint32_t enable);
int pin_get(struct stm32f4_gpio_softc *sc, uint32_t port, uint32_t pin);

#endif /* !_ARM_STM_STM32F4_GPIO_H_ */
