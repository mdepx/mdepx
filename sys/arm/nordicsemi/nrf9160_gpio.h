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

#ifndef _ARM_NORDICSEMI_NRF9160_GPIO_H_
#define _ARM_NORDICSEMI_NRF9160_GPIO_H_

#define	GPIO_OUT		0x004	/* Write GPIO port */
#define	GPIO_OUTSET		0x008	/* Set individual bits in GPIO port */
#define	GPIO_OUTCLR		0x00C	/* Clear individual bits in GPIO port */
#define	GPIO_IN			0x010	/* Read GPIO port */
#define	GPIO_DIR		0x014	/* Direction of GPIO pins */
#define	GPIO_DIRSET		0x018	/* DIR set register */
#define	GPIO_DIRCLR		0x01C	/* DIR clear register */
/*
 * Latch register indicating what GPIO pins that have met
 * the criteria set in the PIN_CNF[n].SENSE registers
 */
#define	GPIO_LATCH		0x020
/*
 * Select between default DETECT signal behaviour
 * and LDETECT mode (For non-secure pin only)
 */
#define	GPIO_DETECTMODE		0x024
/*
 * Select between default DETECT signal behaviour
 * and LDETECT mode (For secure pin only)
 */
#define	GPIO_DETECTMODE_SEC	0x028

/* Configuration of GPIO pins */
#define	GPIO_PIN_CNF(n)		(0x200 + (n) * 0x4)

struct gpio_softc {
	size_t base;
};

#endif /* !_ARM_NORDICSEMI_NRF9160_GPIO_H_ */
