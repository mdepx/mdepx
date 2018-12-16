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

#ifndef _ARM_NORDICSEMI_NRF9160_H_
#define _ARM_NORDICSEMI_NRF9160_H_

#define	SPU_BASE		0x50003000	/* System Protection Unit */
#define	REGULATORS_BASE		0x40004000	/* Regulator configuration */
#define	CLOCK_BASE		0x40005000	/* Clock control */
#define	POWER_BASE		0x40005000	/* Power control */
#define	CTRL_AP_PERI_BASE	0x50006000	/* CTRL-AP-PERI */
#define	SPIM0_BASE		0x40008000	/* SPI master 0 */
#define	SPIS0_BASE		0x40008000	/* SPI slave 0 */
#define	TWIM0_BASE		0x40008000	/* Two-wire interface master 0 */
#define	TWIS0_BASE		0x40008000	/* Two-wire interface slave 0 */
#define	UARTE0_BASE		0x40008000	/* Universal asynchronous receiver/transmitter with EasyDMA 0 */
#define	SPIM1_BASE		0x40009000	/* SPI master 1 */
#define	SPIS1_BASE		0x40009000	/* SPI slave 1 */
#define	TWIM1_BASE		0x40009000	/* Two-wire interface master 1 */
#define	TWIS1_BASE		0x40009000	/* Two-wire interface slave 1 */
#define	UARTE1_BASE		0x40009000	/* Universal asynchronous receiver/transmitter with EasyDMA 1 */
#define	SPIM2_BASE		0x4000A000	/* SPI master 2 */
#define	SPIS2_BASE		0x4000A000	/* SPI slave 2 */
#define	TWIM2_BASE		0x4000A000	/* Two-wire interface master 2 */
#define	TWIS2_BASE		0x4000A000	/* Two-wire interface slave 2 */
#define	UARTE2_BASE		0x4000A000	/* Universal asynchronous receiver/transmitter with EasyDMA 2 */
#define	SPIM3_BASE		0x4000B000	/* SPI master 3 */
#define	SPIS3_BASE		0x4000B000	/* SPI slave 3 */
#define	TWIM3_BASE		0x4000B000	/* Two-wire interface master 3 */
#define	TWIS3_BASE		0x4000B000	/* Two-wire interface slave 3 */
#define	UARTE3_BASE		0x4000B000	/* Universal asynchronous receiver/transmitter with EasyDMA 3 */
#define	GPIOTE0_BASE		0x5000D000	/* Secure GPIO tasks and events */
#define	SAADC_BASE		0x4000E000	/* Analog to digital converter */
#define	TIMER0_BASE		0x4000F000	/* Timer 0 */
#define	TIMER1_BASE		0x40010000	/* Timer 1 */
#define	TIMER2_BASE		0x40011000	/* Timer 2 */
#define	RTC0_BASE		0x40014000	/* Real time counter 0 */
#define	RTC1_BASE		0x40015000	/* Real time counter 1 */
#define	DPPIC_BASE		0x40017000	/* DPPI controller */
#define	WDT_BASE		0x40018000	/* Watchdog timer */
#define	EGU0_BASE		0x4001B000	/* Event generator unit 0 */
#define	EGU1_BASE		0x4001C000	/* Event generator unit 1 */
#define	EGU2_BASE		0x4001D000	/* Event generator unit 2 */
#define	EGU3_BASE		0x4001E000	/* Event generator unit 3 */
#define	EGU4_BASE		0x4001F000	/* Event generator unit 4 */
#define	EGU5_BASE		0x40020000	/* Event generator unit 5 */
#define	PWM0_BASE		0x40021000	/* Pulse width modulation unit 0 */
#define	PWM1_BASE		0x40022000	/* Pulse width modulation unit 1 */
#define	PWM2_BASE		0x40023000	/* Pulse width modulation unit 2 */
#define	PWM3_BASE		0x40024000	/* Pulse width modulation unit 3 */
#define	PDM_BASE		0x40026000	/* Pulse density modulation (digital microphone) interface */
#define	I2S_BASE		0x40028000	/* Inter-IC Sound */
#define	IPC_BASE		0x4002A000	/* Interprocessor communication */
#define	FPU_BASE		0x4002C000	/* Floating-point unit */
#define	GPIOTE1_BASE		0x40031000	/* Non Secure GPIO tasks and events */
#define	KMU_BASE		0x40039000	/* Key management unit */
#define	NVMC_BASE		0x40039000	/* Non-volatile memory controller */
#define	VMC_BASE		0x4003A000	/* Volatile memory controller */
#define	CRYPTOCELL_BASE		0x50840000	/* CryptoCell sub-system control interface */
#define	P0_BASE			0x40842500	/* General purpose input and output */
#define	FICR_BASE		0x00FF0000	/* Factory information configuration */
#define	UICR_BASE		0x00FF8000	/* User information configuration */
#define	TAD_BASE		0xE0080000	/* Trace and debug control */
#define	NRF_SECURE_ACCESS	(1 << 28)

#endif /* !_ARM_NORDICSEMI_NRF9160_H_ */
