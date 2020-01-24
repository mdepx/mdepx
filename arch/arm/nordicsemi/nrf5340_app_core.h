/*-
 * Copyright (c) 2019 Ruslan Bukin <br@bsdpad.com>
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

#ifndef	_ARM_NORDICSEMI_NRF5340_APP_CORE_H_
#define	_ARM_NORDICSEMI_NRF5340_APP_CORE_H_

#include <arm/nordicsemi/nrf9160_cryptocell.h>
#include <arm/nordicsemi/nrf9160_dppi.h>
#include <arm/nordicsemi/nrf9160_egu.h>
#include <arm/nordicsemi/nrf9160_gpiote.h>
#include <arm/nordicsemi/nrf9160_i2s.h>
#include <arm/nordicsemi/nrf9160_kmu.h>
#include <arm/nordicsemi/nrf9160_pdm.h>
#include <arm/nordicsemi/nrf9160_power.h>
#include <arm/nordicsemi/nrf9160_pwm.h>
#include <arm/nordicsemi/nrf9160_saadc.h>
#include <arm/nordicsemi/nrf9160_spim.h>
#include <arm/nordicsemi/nrf9160_spis.h>
#include <arm/nordicsemi/nrf9160_spu.h>
#include <arm/nordicsemi/nrf9160_timer.h>
#include <arm/nordicsemi/nrf9160_uicr.h>
#include <arm/nordicsemi/nrf9160_wdt.h>
#include <arm/nordicsemi/nrf_gpio.h>
#include <arm/nordicsemi/nrf_ipc.h>
#include <arm/nordicsemi/nrf_reset.h>
#include <arm/nordicsemi/nrf_rtc.h>
#include <arm/nordicsemi/nrf_twim.h>
#include <arm/nordicsemi/nrf_twis.h>
#include <arm/nordicsemi/nrf_uarte.h>

#define	NRF_DCNF	0x40000000 /* DCNF Domain configuration */
#define	NRF_FPU		0x40000000 /* FPU Floating Point unit interrupt control */
#define	NRF_CACHE	0x50001000 /* CACHE Cache */
#define	NRF_SPU		0x50003000 /* SPU System protection unit */
#define	NRF_OSCILLATORS	0x40004000 /* OSCILLATORS Oscillator configuration */
#define	NRF_REGULATORS	0x40004000 /* REGULATORS Regulator configuration */
#define	NRF_CLOCK	0x40005000 /* CLOCK Clock control */
#define	NRF_POWER	0x40005000 /* POWER Power control */
#define	NRF_RESET	0x40005000 /* RESET Reset control and status */
#define	NRF_CTRLAPPERI	0x40006000 /* CTRLAPPERI Control access port CPU side */
#define	NRF_SPIM0	0x40008000 /* SPIM0 SPI master 0 */
#define	NRF_SPIS0	0x40008000 /* SPIS0 SPI slave 0 */
#define	NRF_TWIM0	0x40008000 /* TWIM0 Two-wire interface master 0 */
#define	NRF_TWIS0	0x40008000 /* TWIS0 Two-wire interface slave 0 */
#define	NRF_UARTE0	0x40008000 /* UARTE0 Universal asynchronous receiver/transmitter with EasyDMA 0 */
#define	NRF_SPIM1	0x40009000 /* SPIM1 SPI master 1 */
#define	NRF_SPIS1	0x40009000 /* SPIS1 SPI slave 1 */
#define	NRF_TWIM1	0x40009000 /* TWIM1 Two-wire interface master 1 */
#define	NRF_TWIS1	0x40009000 /* TWIS1 Two-wire interface slave 1 */
#define	NRF_UARTE1	0x40009000 /* UARTE1 Universal asynchronous receiver/transmitter with EasyDMA 1 */
#define	NRF_SPIM2	0x4000A000 /* SPIM SPI master 2 (high-speed) */
#define	NRF_GPIOTE0	0x5000D000 /* GPIOTE GPIO tasks and events */
#define	NRF_SAADC	0x4000E000 /* SAADC Successive approximation analog-to-digital converter */
#define	NRF_TIMER0	0x4000F000 /* TIMER0 Timer 0 */
#define	NRF_TIMER1	0x40010000 /* TIMER1 Timer 1 */
#define	NRF_TIMER2	0x40011000 /* TIMER2 Timer 2 */
#define	NRF_RTC0	0x40014000 /* RTC0 Real time counter 0 */
#define	NRF_RTC1	0x40015000 /* RTC1 Real time counter 1 */
#define	NRF_DPPIC	0x40017000 /* DPPIC DPPI controller */
#define	NRF_WDT0	0x40018000 /* WDT0 Watchdog timer 0 */
#define	NRF_WDT1	0x40019000 /* WDT1 Watchdog timer 1 */
#define	NRF_COMP	0x4001A000 /* COMP Comparator */
#define	NRF_LPCOMP	0x4001A000 /* LPCOMP Low-power comparator */
#define	NRF_EGU0	0x4001B000 /* EGU0 Event generator unit 0 */
#define	NRF_EGU1	0x4001C000 /* EGU1 Event generator unit 1 */
#define	NRF_EGU2	0x4001D000 /* EGU2 Event generator unit 2 */
#define	NRF_EGU3	0x4001E000 /* EGU3 Event generator unit 3 */
#define	NRF_EGU4	0x4001F000 /* EGU4 Event generator unit 4 */
#define	NRF_EGU5	0x40020000 /* EGU5 Event generator unit 5 */
#define	NRF_PWM0	0x40021000 /* PWM0 Pulse width modulation unit 0 */
#define	NRF_PWM1	0x40022000 /* PWM1 Pulse width modulation unit 1 */
#define	NRF_PWM2	0x40023000 /* PWM2 Pulse width modulation unit 2 */
#define	NRF_PDM		0x40026000 /* PDM Pulse density modulation (digital microphone) interface */
#define	NRF_I2S		0x40028000 /* I2S Inter-IC sound interface */
#define	NRF_IPC		0x4002A000 /* IPC Interprocessor communication */
#define	NRF_QSPI	0x4002B000 /* QSPI External memory (quad serial peripheral) interface */
#define	NRF_NFCT	0x4002D000 /* NFCT Near field communication tag */
#define	NRF_GPIOTE1	0x4002F000 /* GPIOTE GPIO tasks and events */
#define	NRF_MUTEX	0x40030000 /* MUTEX Mutual exclusive hardware support */
#define	NRF_QDEC	0x40033000 /* QDEC Quadrature decoder */
#define	NRF_USBD	0x40036000 /* USBD Universal serial bus device */
#define	NRF_USBREG	0x40037000 /* USBREG USB regulator control */
#define	NRF_KMU		0x40039000 /* KMU Key management unit */
#define	NRF_NVMC	0x40039000 /* NVMC Non-volatile memory controller */
#define	NRF_GPIO0	0x40842500 /* GPIO0 General purpose input and output, port 0 */
#define	NRF_GPIO1	0x40842800 /* GPIO1 General purpose input and output, port 1 */
#define	NRF_CRYPTOCELL	0x50844000 /* CRYPTOCELL CryptoCell subsystem control interface */
#define	NRF_VMC		0x40081000 /* VMC Volatile memory controller */
#define	NRF_CACHEDATA	0x00F00000 /* CACHEDATA Cache data */
#define	NRF_CACHEINFO	0x00F08000 /* CACHEINFO Cache info */
#define	NRF_FICR	0x00FF0000 /* FICR Factory information configuration registers */
#define	NRF_UICR	0x00FF8000 /* UICR User information configuration registers */
#define	NRF_CTI		0xE0042000 /* CTI Cross-trigger interface */
#define	NRF_TAD		0xE0080000 /* TAD Trace and debug control */

#define	BASE_FICR	0x00FF0000 /* Factory information configuration */
#define	BASE_UICR	0x00FF8000 /* User information configuration */
#define	BASE_TAD	0xE0080000 /* Trace and debug control */
#define	BASE_SCS	0xE000E000 /* System Control Space */
#define	BASE_SCS_NS	0xE002E000 /* System Control Space Non-Secure */
#define	BASE_UICR	0x00FF8000 /* User information configuration */
#define	BASE_FICR	0x00FF0000 /* Factory information configuration */

#define	NRF_SECURE	(1 << 28)

#define	ID_DCNF		0
#define	ID_FPU		0
#define	ID_CACHE	1
#define	ID_SPU		3
#define	ID_OSCILLATORS	4
#define	ID_REGULATORS	4
#define	ID_CLOCK	5
#define	ID_POWER	5
#define	ID_RESET	5
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
#define	ID_GPIOTE0	13
#define	ID_SAADC	14
#define	ID_TIMER0	15
#define	ID_TIMER1	16
#define	ID_TIMER2	17
#define	ID_RTC0		20
#define	ID_RTC1		21
#define	ID_DPPIC	23
#define	ID_WDT0		24
#define	ID_WDT1		25
#define	ID_COMP		26
#define	ID_LPCOMP	26
#define	ID_EGU0		27
#define	ID_EGU1		28
#define	ID_EGU2		29
#define	ID_EGU3		30
#define	ID_EGU4		31
#define	ID_EGU5		32
#define	ID_PWM0		33
#define	ID_PWM1		34
#define	ID_PWM2		35
#define	ID_PDM		38
#define	ID_I2S		40
#define	ID_IPC		42
#define	ID_QSPI		43
#define	ID_NFCT		45
#define	ID_GPIOTE1	47
#define	ID_MUTEX	48
#define	ID_QDEC		51
#define	ID_USBD		54
#define	ID_USBREG	55
#define	ID_KMU		57
#define	ID_NVMC		57
#define	ID_GPIO0	66
#define	ID_GPIO1	66
#define	ID_CRYPTOCELL	68
#define	ID_VMC		129

#endif /* !_ARM_NORDICSEMI_NRF5340_APP_CORE_H_ */
