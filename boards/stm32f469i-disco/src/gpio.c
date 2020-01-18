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

#include <sys/cdefs.h>

#include <arm/stm/stm32f4_gpio.h>

#include "gpio.h"

static const struct gpio_pin uart_pins[] = {
	{ PORT_G,  9, MODE_ALT, 8, FLOAT }, /* USART6_RX */
	{ PORT_G, 14, MODE_ALT, 8, FLOAT }, /* USART6_TX */
	{ -1, -1, -1, -1, -1 },
};

static const struct gpio_pin dram_pins[] = {
	{ PORT_B,  5, MODE_ALT, 12, FLOAT }, /* SDCKE1 */
	{ PORT_B,  6, MODE_ALT, 12, FLOAT }, /* SDNE1 */

	{ PORT_C,  0, MODE_ALT, 12, FLOAT }, /* FMC_SDNWE */
	{ PORT_D,  0, MODE_ALT, 12, FLOAT }, /* FMC_D2 */
	{ PORT_D,  1, MODE_ALT, 12, FLOAT }, /* FMC_D3 */
	{ PORT_D,  8, MODE_ALT, 12, FLOAT }, /* FMC_D13 */
	{ PORT_D,  9, MODE_ALT, 12, FLOAT }, /* FMC_D14 */
	{ PORT_D, 10, MODE_ALT, 12, FLOAT }, /* FMC_D15 */
	{ PORT_D, 14, MODE_ALT, 12, FLOAT }, /* FMC_D0 */
	{ PORT_D, 15, MODE_ALT, 12, FLOAT }, /* FMC_D1 */

	{ PORT_E,  0, MODE_ALT, 12, FLOAT }, /* FMC_NBL0 */
	{ PORT_E,  1, MODE_ALT, 12, FLOAT }, /* FMC_NBL1 */
	{ PORT_E,  7, MODE_ALT, 12, FLOAT }, /* FMC_D4 */
	{ PORT_E,  8, MODE_ALT, 12, FLOAT }, /* FMC_D5 */
	{ PORT_E,  9, MODE_ALT, 12, FLOAT }, /* FMC_D6 */
	{ PORT_E, 10, MODE_ALT, 12, FLOAT }, /* FMC_D7 */
	{ PORT_E, 11, MODE_ALT, 12, FLOAT }, /* FMC_D8 */
	{ PORT_E, 12, MODE_ALT, 12, FLOAT }, /* FMC_D9 */
	{ PORT_E, 13, MODE_ALT, 12, FLOAT }, /* FMC_D10 */
	{ PORT_E, 14, MODE_ALT, 12, FLOAT }, /* FMC_D11 */
	{ PORT_E, 15, MODE_ALT, 12, FLOAT }, /* FMC_D12 */

	{ PORT_F,  0, MODE_ALT, 12, FLOAT }, /* A0 */
	{ PORT_F,  1, MODE_ALT, 12, FLOAT }, /* A1 */
	{ PORT_F,  2, MODE_ALT, 12, FLOAT }, /* A2 */
	{ PORT_F,  3, MODE_ALT, 12, FLOAT }, /* A3 */
	{ PORT_F,  4, MODE_ALT, 12, FLOAT }, /* A4 */
	{ PORT_F,  5, MODE_ALT, 12, FLOAT }, /* A5 */
	{ PORT_F, 11, MODE_ALT, 12, FLOAT }, /* FMC_SDNRAS */
	{ PORT_F, 12, MODE_ALT, 12, FLOAT }, /* A6 */
	{ PORT_F, 13, MODE_ALT, 12, FLOAT }, /* A7 */
	{ PORT_F, 14, MODE_ALT, 12, FLOAT }, /* A8 */
	{ PORT_F, 15, MODE_ALT, 12, FLOAT }, /* A9 */

	{ PORT_G,  0, MODE_ALT, 12, FLOAT }, /* A10 */
	{ PORT_G,  1, MODE_ALT, 12, FLOAT }, /* A11 */
	{ PORT_G,  4, MODE_ALT, 12, FLOAT }, /* A14 / BA0 */
	{ PORT_G,  5, MODE_ALT, 12, FLOAT }, /* A15 / BA1 */
	{ PORT_G,  8, MODE_ALT, 12, FLOAT }, /* SDCLK */
	{ PORT_G, 15, MODE_ALT, 12, FLOAT }, /* SDNCAS */

	{ PORT_H,  2, MODE_ALT, 12, FLOAT }, /* SDCKE0 */
	{ PORT_H,  3, MODE_ALT, 12, FLOAT }, /* SDNE0 */
	{ PORT_H,  8, MODE_ALT, 12, FLOAT }, /* D16 */
	{ PORT_H,  9, MODE_ALT, 12, FLOAT }, /* D17 */
	{ PORT_H, 10, MODE_ALT, 12, FLOAT }, /* D18 */
	{ PORT_H, 11, MODE_ALT, 12, FLOAT }, /* D19 */
	{ PORT_H, 12, MODE_ALT, 12, FLOAT }, /* D20 */
	{ PORT_H, 13, MODE_ALT, 12, FLOAT }, /* D21 */
	{ PORT_H, 14, MODE_ALT, 12, FLOAT }, /* D22 */
	{ PORT_H, 15, MODE_ALT, 12, FLOAT }, /* D23 */

	{ PORT_I,  0, MODE_ALT, 12, FLOAT }, /* D24 */
	{ PORT_I,  1, MODE_ALT, 12, FLOAT }, /* D25 */
	{ PORT_I,  2, MODE_ALT, 12, FLOAT }, /* D26 */
	{ PORT_I,  3, MODE_ALT, 12, FLOAT }, /* D27 */
	{ PORT_I,  4, MODE_ALT, 12, FLOAT }, /* NBL2 */
	{ PORT_I,  5, MODE_ALT, 12, FLOAT }, /* NBL3 */
	{ PORT_I,  6, MODE_ALT, 12, FLOAT }, /* D28 */
	{ PORT_I,  7, MODE_ALT, 12, FLOAT }, /* D29 */
	{ PORT_I,  9, MODE_ALT, 12, FLOAT }, /* D30 */
	{ PORT_I, 10, MODE_ALT, 12, FLOAT }, /* D31 */
	{ -1, -1, -1, -1, -1 },
};

static const struct gpio_pin dsi_pins[] = {
	{ PORT_A, 3, MODE_OUT, 0, FLOAT }, /* LCD_BL_CTRL */
	{ PORT_H, 7, MODE_OUT, 0, FLOAT }, /* LCD_RESET */
	{ PORT_J, 2, MODE_ALT, 13, FLOAT }, /* DSI_TE */
	{ -1, -1, -1, -1, -1 },
};


void
gpio_config(struct stm32f4_gpio_softc *sc)
{

	pin_configure(sc, uart_pins);
	pin_configure(sc, dram_pins);
	pin_configure(sc, dsi_pins);
}
