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

#ifndef _RISCV_KENDRYTE_K210_GPIOHS_H_
#define _RISCV_KENDRYTE_K210_GPIOHS_H_

#define	GPIOHS_INPUT_VALUE	0x00
#define	GPIOHS_INPUT_ENABLE	0x04
#define	GPIOHS_OUTPUT_ENABLE	0x08
#define	GPIOHS_OUTPUT_VALUE	0x0C
#define	GPIOHS_PULLUP_ENABLE	0x10
#define	GPIOHS_DRIVE		0x14
#define	GPIOHS_RISE_IE		0x18	/* Rise interrupt enable */
#define	GPIOHS_RISE_IP		0x1C	/* Rise interrupt pending */
#define	GPIOHS_FALL_IE		0x20
#define	GPIOHS_FALL_IP		0x24
#define	GPIOHS_HIGH_IE		0x28
#define	GPIOHS_HIGH_IP		0x2C
#define	GPIOHS_LOW_IE		0x30
#define	GPIOHS_LOW_IP		0x34
#define	GPIOHS_IOF_EN		0x38
#define	GPIOHS_IOF_SEL		0x3C
#define	GPIOHS_OUTPUT_XOR	0x40

struct k210_gpiohs_softc {
	size_t base;
};

void k210_gpiohs_init(struct k210_gpiohs_softc *sc, uint32_t base);
void k210_gpiohs_set_dir(struct k210_gpiohs_softc *sc, int pin, int dir);
void k210_gpiohs_set_pin(struct k210_gpiohs_softc *sc, int pin, int val);

#endif /* !_RISCV_KENDRYTE_K210_GPIOHS_H_ */
