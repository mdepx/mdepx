/*-
 * Copyright (c) 2021 Ruslan Bukin <br@bsdpad.com>
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

#ifndef	_RP2040_RESETS_H_
#define	_RP2040_RESETS_H_

#define	RP2040_RESETS_RESET		0x00		/* 1: reset asserted */
#define	 RESETS_RESET_ADC		(1 << 0)
#define	 RESETS_RESET_BUS		(1 << 1)
#define	 RESETS_RESET_DMA		(1 << 2)
#define	 RESETS_RESET_I2C0		(1 << 3)
#define	 RESETS_RESET_I2C1		(1 << 4)
#define	 RESETS_RESET_IOBANK0		(1 << 5)
#define	 RESETS_RESET_IOBANK1		(1 << 6)
#define	 RESETS_RESET_JTAG		(1 << 7)
#define	 RESETS_RESET_PADS_BANK0	(1 << 8)
#define	 RESETS_RESET_PADS_QSPI		(1 << 9)
#define	 RESETS_RESET_PIO0		(1 << 10)
#define	 RESETS_RESET_PIO1		(1 << 11)
#define	 RESETS_RESET_PLL_SYS		(1 << 12)
#define	 RESETS_RESET_PLL_USB		(1 << 13)
#define	 RESETS_RESET_PWM		(1 << 14)
#define	 RESETS_RESET_RTC		(1 << 15)
#define	 RESETS_RESET_SPI0		(1 << 16)
#define	 RESETS_RESET_SPI1		(1 << 17)
#define	 RESETS_RESET_SYSCONFIG		(1 << 18)
#define	 RESETS_RESET_SYSINFO		(1 << 19)
#define	 RESETS_RESET_TBMANAGER		(1 << 20)
#define	 RESETS_RESET_TIMER		(1 << 21)
#define	 RESETS_RESET_UART0		(1 << 22)
#define	 RESETS_RESET_UART1		(1 << 23)
#define	 RESETS_RESET_USB		(1 << 24)
#define	RP2040_RESETS_WDSEL		0x04		/* Watchdog select. */
#define	RP2040_RESETS_DONE		0x08		/* 1: ready */

#endif /* !_RP2040_RESETS_H_ */
