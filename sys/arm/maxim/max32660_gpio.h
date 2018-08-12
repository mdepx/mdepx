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

#ifndef _ARM_MAXIM_MAX32660_GPIO_H_
#define _ARM_MAXIM_MAX32660_GPIO_H_

enum pin_mode {
	MODE_IN,
	MODE_OUT,
	MODE_AF1,
	MODE_AF2,
	MODE_AF3,
};

#define	GPIO_AF0_SEL		0x00	/* R/W I/O and Alternate Function 1 Select Register */
#define	GPIO_OUT_EN		0x0C	/* R/W Output Enable Register */
#define	GPIO_OUT		0x18	/* R/W Output Register */
#define	GPIO_IN			0x24	/* RO Input Register */
#define	GPIO_INT_MODE		0x28	/* R/W Interrupt Mode Register */
#define	GPIO_INT_POL		0x2C	/* R/W Interrupt Polarity Select Register */
#define	GPIO_INT_EN		0x34	/* R/W Interrupt Enable Register */
#define	GPIO_INT_FL		0x40	/* R/W1C Interrupt Flag Register */
#define	GPIO_WAKE_EN		0x4C	/* R/W Wakeup Enable Register */
#define	GPIO_INT_DUAL_EDGE	0x5C	/* R/W Dual Edge Select Interrupt Register */
#define	GPIO_PULL_EN		0x60	/* R/W Input Pullup/Pulldown Select Register */
#define	GPIO_AF1_SEL		0x68	/* R/W Alternate Function 2/3 Select Register */
#define	GPIO_INHYS_EN		0xA8	/* R/W Input Hysteresis Enable Register */
#define	GPIO_SR_SEL		0xAC	/* R/W Slew Rate Select Register */
#define	GPIO_DS0_SEL		0xB0	/* R/W Drive Strength Select 0 Register */
#define	GPIO_DS1_SEL		0xB4	/* R/W Drive Strength Select 1 Register */
#define	GPIO_PULL_SEL		0xB8	/* R/W Pullup/Pulldown Enable Register */

struct max32660_gpio_softc {
	uint32_t base;
};

void max32660_gpio_init(struct max32660_gpio_softc *sc, uint32_t base);
void max32660_gpio_mode(struct max32660_gpio_softc *sc,
    uint8_t port, uint8_t pin, enum pin_mode mode);
void max32660_gpio_out(struct max32660_gpio_softc *sc,
    uint8_t port, uint8_t pin, uint8_t val);

#endif /* !_ARM_MAXIM_MAX32660_GPIO_H_ */
