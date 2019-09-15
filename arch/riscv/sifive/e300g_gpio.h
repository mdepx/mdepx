/*-
 * Copyright (c) 2017 Ruslan Bukin <br@bsdpad.com>
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

/*
 * SiFive GPIO Register Offsets.
 * SiFive E300 Platform Reference Manual, Version 1.0.1
 */

#ifndef	_SYS_RISCV_SIFIVE_E300G_GPIO_H_
#define	_SYS_RISCV_SIFIVE_E300G_GPIO_H_

#define	GPIO_VALUE		0x000 /* pin value */
#define	GPIO_INPUT_EN		0x004 /* pin input enable */
#define	GPIO_OUTPUT_EN		0x008 /* pin output enable */
#define	GPIO_PORT		0x00C /* output port value */
#define	GPIO_PUE		0x010 /* internal pull-up enable */
#define	GPIO_DS			0x014 /* Pin Drive Strength */
#define	GPIO_RISE_IE		0x018 /* rise interrupt enable */
#define	GPIO_RISE_IP		0x01C /* rise interrupt pending */
#define	GPIO_FALL_IE		0x020 /* fall interrupt enable */
#define	GPIO_FALL_IP		0x024 /* fall interrupt pending */
#define	GPIO_HIGH_IE		0x028 /* high interrupt enable */
#define	GPIO_HIGH_IP		0x02C /* high interrupt pending */
#define	GPIO_LOW_IE		0x030 /* low interrupt enable */
#define	GPIO_LOW_IP		0x034 /* low interrupt pending */
#define	GPIO_IOF_EN		0x038 /* HW I/O Function enable */
#define	GPIO_IOF_SEL		0x03C /* HW I/O Function select */
#define	 IOF0_SPI1_SS0		(1 << 2)
#define	 IOF0_SPI1_MOSI		(1 << 3)
#define	 IOF0_SPI1_MISO		(1 << 4)
#define	 IOF0_SPI1_SCK		(1 << 5)
#define	 IOF0_SPI1_SS2		(1 << 9)
#define	 IOF0_SPI1_SS3		(1 << 10)
#define	 IOF0_UART0_RX		(1 << 16)
#define	 IOF0_UART0_TX		(1 << 17)
#define	GPIO_OUT_XOR		0x040 /* Output XOR (invert) */

struct gpio_softc {
	uint32_t base;
};

int e300g_gpio_init(struct gpio_softc *sc, uint32_t base);
void e300g_gpio_output_enable(struct gpio_softc *sc, int port, int enable);
void e300g_gpio_port(struct gpio_softc *sc, int port, int value);
void e300g_iof0_enable(struct gpio_softc *sc, uint32_t val);
void e300g_iof1_enable(struct gpio_softc *sc, uint32_t val);

#endif /* !_SYS_RISCV_SIFIVE_E300G_GPIO_H_ */
