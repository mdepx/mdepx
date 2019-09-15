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
#define	GPIO_LATCH		0x020	/* Latch register indicating what GPIO pins that have met the criteria set in the PIN_CNF[n].SENSE registers */
#define	GPIO_DETECTMODE		0x024	/* Select between default DETECT signal behaviour and LDETECT mode (For non-secure pin only) */
#define	GPIO_DETECTMODE_SEC	0x028	/* Select between default DETECT signal behaviour and LDETECT mode (For secure pin only) */
#define	GPIO_PIN_CNF(n)		(0x200 + (n) * 0x4)	/* Configuration of GPIO pins */
#define	 CNF_RW_DIR_OUT		(1 << 0) /* Configure pin as an output pin */
#define	 CNF_RW_INPUT_DIS	(1 << 1) /* Disconnect input buffer */
#define	 CNF_RW_PULL_S		2 /* Pull configuration */
#define	 CNF_RW_PULL_M		(0x3 << CNF_RW_PULL_S)
#define	 CNF_RW_PULL_DISABLED	(0x0 << CNF_RW_PULL_S)
#define	 CNF_RW_PULL_DOWN	(0x1 << CNF_RW_PULL_S)
#define	 CNF_RW_PULL_UP		(0x2 << CNF_RW_PULL_S)
#define	 CNF_RW_DRIVE_S		8 /* Drive configuration */
#define	 CNF_RW_DRIVE_M		(0x7 << CNF_RW_DRIVE_S)
#define	 CNF_RW_DRIVE_S0S1	(0 << CNF_RW_DRIVE_S) /* Std 0, std 1 */
#define	 CNF_RW_DRIVE_H0S1	(1 << CNF_RW_DRIVE_S) /* High drive 0, std 1 */
#define	 CNF_RW_DRIVE_S0H1	(2 << CNF_RW_DRIVE_S) /* Std 0, high drive 1 */
#define	 CNF_RW_DRIVE_H0H1	(3 << CNF_RW_DRIVE_S) /* High drive 0, 1 */
#define	 CNF_RW_DRIVE_D0S1	(4 << CNF_RW_DRIVE_S) /* Disconnect 0, std 1 */
#define	 CNF_RW_DRIVE_D0H1	(5 << CNF_RW_DRIVE_S) /* Dis 0, high drive 1 */
#define	 CNF_RW_DRIVE_S0D1	(6 << CNF_RW_DRIVE_S) /* Std 0, disconnect 1 */
#define	 CNF_RW_DRIVE_H0D1	(6 << CNF_RW_DRIVE_S) /* High drive 0, dis 1 */
#define	 CNF_RW_SENSE_S		16 /* Pin sensing mechanism */
#define	 CNF_RW_SENSE_M		(0x7 << CNF_RW_SENSE_S)
#define	 CNF_RW_SENSE_DISABLED	(0 << CNF_RW_SENSE_S)
#define	 CNF_RW_SENSE_HIGH	(2 << CNF_RW_SENSE_S)
#define	 CNF_RW_SENSE_LOW	(3 << CNF_RW_SENSE_S)

struct gpio_softc {
	size_t base;
};

#endif /* !_ARM_NORDICSEMI_NRF9160_GPIO_H_ */
