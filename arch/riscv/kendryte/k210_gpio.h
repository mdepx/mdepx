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

#ifndef _RISCV_KENDRYTE_K210_GPIO_H_
#define _RISCV_KENDRYTE_K210_GPIO_H_

#include <dev/gpio/gpio.h>

/*
 * GPIOHS: 32 pins 3 Mbps
 * GPIO: 8 pins 1 Mbps
 */

#define	GPIO_DATA_OUTPUT	0x00
#define	GPIO_DIRECTION		0x04
#define	GPIO_SOURCE		0x08
#define	GPIO_INT_ENABLE		0x30
#define	GPIO_INT_MASK		0x34
#define	GPIO_INT_LEVEL		0x38
#define	GPIO_INT_POLARITY	0x3C
#define	GPIO_INT_STATUS		0x40
#define	GPIO_INT_STATUS_RAW	0x44
#define	GPIO_INT_DEBOUNCE	0x48
#define	GPIO_INT_CLEAR		0x4C
#define	GPIO_DATA_INPUT		0x50
#define	GPIO_SYNC_LEVEL		0x60
#define	GPIO_ID_CODE		0x64
#define	GPIO_INT_BOTHEDGE	0x68

struct k210_gpio_softc {
	size_t base;
};

void k210_gpio_init(mdx_device_t dev, uint32_t base);

#endif /* !_RISCV_KENDRYTE_K210_GPIO_H_ */
