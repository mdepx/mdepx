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

#include <arm/nordicsemi/nrf9160_cryptocell.h>
#include <arm/nordicsemi/nrf9160_dppi.h>
#include <arm/nordicsemi/nrf9160_egu.h>
#include <arm/nordicsemi/nrf9160_gpiote.h>
#include <arm/nordicsemi/nrf9160_i2s.h>
#include <arm/nordicsemi/nrf9160_kmu.h>
#include <arm/nordicsemi/nrf9160_pdm.h>
#include <arm/nordicsemi/nrf9160_power.h>
#include <arm/nordicsemi/nrf9160_pwm.h>
#include <arm/nordicsemi/nrf9160_rtc.h>
#include <arm/nordicsemi/nrf9160_saadc.h>
#include <arm/nordicsemi/nrf9160_spim.h>
#include <arm/nordicsemi/nrf9160_spis.h>
#include <arm/nordicsemi/nrf9160_spu.h>
#include <arm/nordicsemi/nrf9160_timer.h>
#include <arm/nordicsemi/nrf9160_twim.h>
#include <arm/nordicsemi/nrf9160_twis.h>
#include <arm/nordicsemi/nrf9160_uicr.h>
#include <arm/nordicsemi/nrf9160_wdt.h>
#include <arm/nordicsemi/nrf_gpio.h>
#include <arm/nordicsemi/nrf_ipc.h>
#include <arm/nordicsemi/nrf_uarte.h>

#define	BASE_SPU	0x50003000 /* System Protection Unit */
#define	BASE_REGULATORS	0x40004000 /* Regulator configuration */
#define	BASE_CLOCK	0x40005000 /* Clock control */
#define	BASE_POWER	0x40005000 /* Power control */
#define	BASE_CTRLAPPERI 0x50006000 /* CTRL-AP-PERI */
#define	BASE_SPIM0	0x40008000 /* SPI master 0 */
#define	BASE_SPIS0	0x40008000 /* SPI slave 0 */
#define	BASE_TWIM0	0x40008000 /* Two-wire interface master 0 */
#define	BASE_TWIS0	0x40008000 /* Two-wire interface slave 0 */
#define	BASE_UARTE0	0x40008000 /* UART with EasyDMA 0 */
#define	BASE_SPIM1	0x40009000 /* SPI master 1 */
#define	BASE_SPIS1	0x40009000 /* SPI slave 1 */
#define	BASE_TWIM1	0x40009000 /* Two-wire interface master 1 */
#define	BASE_TWIS1	0x40009000 /* Two-wire interface slave 1 */
#define	BASE_UARTE1	0x40009000 /* UART with EasyDMA 1 */
#define	BASE_SPIM2	0x4000A000 /* SPI master 2 */
#define	BASE_SPIS2	0x4000A000 /* SPI slave 2 */
#define	BASE_TWIM2	0x4000A000 /* Two-wire interface master 2 */
#define	BASE_TWIS2	0x4000A000 /* Two-wire interface slave 2 */
#define	BASE_UARTE2	0x4000A000 /* UART with EasyDMA 2 */
#define	BASE_SPIM3	0x4000B000 /* SPI master 3 */
#define	BASE_SPIS3	0x4000B000 /* SPI slave 3 */
#define	BASE_TWIM3	0x4000B000 /* Two-wire interface master 3 */
#define	BASE_TWIS3	0x4000B000 /* Two-wire interface slave 3 */
#define	BASE_UARTE3	0x4000B000 /* UART with EasyDMA 3 */
#define	BASE_GPIOTE0	0x5000D000 /* Secure GPIO tasks and events */
#define	BASE_SAADC	0x4000E000 /* Analog to digital converter */
#define	BASE_TIMER0	0x4000F000 /* Timer 0 */
#define	BASE_TIMER1	0x40010000 /* Timer 1 */
#define	BASE_TIMER2	0x40011000 /* Timer 2 */
#define	BASE_RTC0	0x40014000 /* Real time counter 0 */
#define	BASE_RTC1	0x40015000 /* Real time counter 1 */
#define	BASE_DPPIC	0x40017000 /* DPPI controller */
#define	BASE_WDT	0x40018000 /* Watchdog timer */
#define	BASE_EGU0	0x4001B000 /* Event generator unit 0 */
#define	BASE_EGU1	0x4001C000 /* Event generator unit 1 */
#define	BASE_EGU2	0x4001D000 /* Event generator unit 2 */
#define	BASE_EGU3	0x4001E000 /* Event generator unit 3 */
#define	BASE_EGU4	0x4001F000 /* Event generator unit 4 */
#define	BASE_EGU5	0x40020000 /* Event generator unit 5 */
#define	BASE_PWM0	0x40021000 /* Pulse width modulation unit 0 */
#define	BASE_PWM1	0x40022000 /* Pulse width modulation unit 1 */
#define	BASE_PWM2	0x40023000 /* Pulse width modulation unit 2 */
#define	BASE_PWM3	0x40024000 /* Pulse width modulation unit 3 */
#define	BASE_PDM	0x40026000 /* PDM (digital microphone) interface */
#define	BASE_I2S	0x40028000 /* Inter-IC Sound */
#define	BASE_IPC	0x4002A000 /* Interprocessor communication */
#define	BASE_FPU	0x4002C000 /* Floating-point unit */
#define	BASE_GPIOTE1	0x40031000 /* Non Secure GPIO tasks and events */
#define	BASE_KMU	0x40039000 /* Key management unit */
#define	BASE_NVMC	0x40039000 /* Non-volatile memory controller */
#define	BASE_VMC	0x4003A000 /* Volatile memory controller */
#define	BASE_CRYPTOCELL	0x50840000 /* CryptoCell sub-system control interface*/
#define	BASE_GPIO	0x40842500 /* General purpose input and output */
#define	PERIPH_SECURE_ACCESS	(1 << 28)

#define	BASE_FICR	0x00FF0000 /* Factory information configuration */
#define	BASE_UICR	0x00FF8000 /* User information configuration */
#define	BASE_TAD	0xE0080000 /* Trace and debug control */
#define	BASE_SCS	0xE000E000 /* System Control Space */
#define	BASE_SCS_NS	0xE002E000 /* System Control Space Non-Secure */
#define	BASE_UICR	0x00FF8000 /* User information configuration */
#define	BASE_FICR	0x00FF0000 /* Factory information configuration */

#define	ID_SPU		3
#define	ID_REGULATORS	4
#define	ID_CLOCK	5
#define	ID_POWER	5
#define	ID_CTRLAPPERI	6
#define	ID_SPIM0	8
#define	ID_SPIS0	8
#define	ID_TWIM0	8
#define	ID_TWIS0	8
#define	ID_UARTE0	8
#define	ID_SPIM1	9
#define	ID_SPIS1	9
#define	ID_TWIM1	9
#define	ID_TWIS1	9
#define	ID_UARTE1	9
#define	ID_SPIM2	10
#define	ID_SPIS2	10
#define	ID_TWIM2	10
#define	ID_TWIS2	10
#define	ID_UARTE2	10
#define	ID_SPIM3	11
#define	ID_SPIS3	11
#define	ID_TWIM3	11
#define	ID_TWIS3	11
#define	ID_UARTE3	11
#define	ID_GPIOTE0	13
#define	ID_SAADC	14
#define	ID_TIMER0	15
#define	ID_TIMER1	16
#define	ID_TIMER2	17
#define	ID_RTC0		20
#define	ID_RTC1		21
#define	ID_DPPIC	23
#define	ID_WDT		24
#define	ID_EGU0		27
#define	ID_EGU1		28
#define	ID_EGU2		29
#define	ID_EGU3		30
#define	ID_EGU4		31
#define	ID_EGU5		32
#define	ID_PWM0		33
#define	ID_PWM1		34
#define	ID_PWM2		35
#define	ID_PWM3		36
#define	ID_PDM		38
#define	ID_I2S		40
#define	ID_IPC		42
#define	ID_FPU		44
#define	ID_GPIOTE1	49
#define	ID_KMU		57
#define	ID_NVMC		57
#define	ID_VMC		58
#define	ID_CRYPTOCELL	64
#define	ID_GPIO		66

#endif /* !_ARM_NORDICSEMI_NRF9160_H_ */
