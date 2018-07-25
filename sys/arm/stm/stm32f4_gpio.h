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

#define	GPIO_MODER(n)	(0x00 + 0x400 * (n))
#define	GPIO_OTYPER(n)	(0x04 + 0x400 * (n))
#define	GPIO_OSPEEDR(n)	(0x08 + 0x400 * (n))
#define	GPIO_BSSR(n)	(0x18 + 0x400 * (n))
#define	GPIO_AFRL(n)	(0x20 + 0x400 * (n))
#define	GPIO_AFRH(n)	(0x24 + 0x400 * (n))
#define	GPIO_PUPDR(n)	(0x0C + 0x400 * (n))

#define	MODE_INP	0
#define	MODE_OUT	1
#define	MODE_ALT	2
#define	MODE_ANA	3

#define	FLOAT		0
#define	PULLUP		1
#define	PULLDOWN	2

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
};

struct gpio_pin {
	uint32_t	port;
	uint32_t	pin;
	uint32_t	mode;
	uint32_t	alt;
	uint32_t	pupdr;
};

struct stm32f4_gpio_softc {
	uint32_t base;
};

int stm32f4_gpio_init(struct stm32f4_gpio_softc *sc, uint32_t base,
    uint32_t cpu_freq, uint32_t baud_rate);

void pin_configure(struct stm32f4_gpio_softc *sc, const struct gpio_pin *pins);
void pin_set(struct stm32f4_gpio_softc *sc, uint32_t port,
    uint32_t pin, uint32_t enable);

#endif /* !_ARM_STM_STM32F4_GPIO_H_ */
