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

#ifndef	_ARM_NORDICSEMI_NRF5340_NET_CORE_H_
#define	_ARM_NORDICSEMI_NRF5340_NET_CORE_H_

#include <arm/nordicsemi/nrf9160_cryptocell.h>
#include <arm/nordicsemi/nrf9160_dppi.h>
#include <arm/nordicsemi/nrf9160_egu.h>
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
#include <arm/nordicsemi/nrf_gpiote.h>
#include <arm/nordicsemi/nrf_ipc.h>
#include <arm/nordicsemi/nrf_reset.h>
#include <arm/nordicsemi/nrf_rtc.h>
#include <arm/nordicsemi/nrf_twim.h>
#include <arm/nordicsemi/nrf_twis.h>
#include <arm/nordicsemi/nrf_uarte.h>

#define	NRF_DCNF	0x41000000 /* Domain configuration */
#define	NRF_VREQCTRL	0x41004000 /* Voltage request control */
#define	NRF_CLOCK	0x41005000 /* Clock control */
#define	NRF_POWER	0x41005000 /* Power control */
#define	NRF_RESET	0x41005000 /* Reset status */
#define	NRF_CTRLAP	0x41006000 /* Control access port CPU side */
#define	NRF_RADIO	0x41008000 /* 2.4 GHz radio */
#define	NRF_RNG		0x41009000 /* Random number generator */
#define	NRF_GPIOTE	0x4100A000 /* GPIO tasks and events */
#define	NRF_WDT		0x4100B000 /* Watchdog timer */
#define	NRF_TIMER0	0x4100C000 /* Timer 0 */
#define	NRF_ECB		0x4100D000 /* AES ECB mode encryption */
#define	NRF_AAR		0x4100E000 /* Accelerated address resolver */
#define	NRF_CCM		0x4100E000 /* AES CCM mode encryption */
#define	NRF_DPPIC	0x4100F000 /* DPPI controller */
#define	NRF_TEMP	0x41010000 /* Temperature sensor */
#define	NRF_RTC0	0x41011000 /* Real-time counter 0 */
#define	NRF_IPC		0x41012000 /* Interprocessor communication */
#define	NRF_SPIM0	0x41013000 /* SPI master 0 */
#define	NRF_SPIS0	0x41013000 /* SPI slave 0 */
#define	NRF_TWIM0	0x41013000 /* Two-wire interface master 0 */
#define	NRF_TWIS0	0x41013000 /* Two-wire interface slave 0 */
#define	NRF_UARTE0	0x41013000 /* Universal asynchronous receiver/transmitter */
#define	NRF_EGU0	0x41014000 /* Event generator unit 0 */
#define	NRF_RTC1	0x41016000 /* Real-time counter 1 */
#define	NRF_TIMER1	0x41018000 /* Timer 1 */
#define	NRF_TIMER2	0x41019000 /* Timer 2 */
#define	NRF_SWI0	0x4101A000 /* Software interrupt 0 */
#define	NRF_SWI1	0x4101B000 /* Software interrupt 1 */
#define	NRF_SWI2	0x4101C000 /* Software interrupt 2 */
#define	NRF_SWI3	0x4101D000 /* Software interrupt 3 */
#define	NRF_ACL		0x41080000 /* Access control lists */
#define	NRF_NVMC	0x41080000 /* Non-Volatile Memory Controller */
#define	NRF_VMC		0x41081000 /* Volatile memory controller */
#define	NRF_P0		0x418C0500 /* General purpose input and output */
#define	NRF_P1		0x418C0800 /* General purpose input and output */
#define	NRF_FICR	0x01FF0000 /* Factory information configuration */
#define	NRF_UICR	0x01FF8000 /* User information configuration */
#define	NRF_CTI		0xE0042000 /* Cross-trigger interface */

#define	BASE_FICR	0x00FF0000 /* Factory information configuration */
#define	BASE_UICR	0x00FF8000 /* User information configuration */
#define	BASE_TAD	0xE0080000 /* Trace and debug control */
#define	BASE_SCS	0xE000E000 /* System Control Space */
#define	BASE_SCS_NS	0xE002E000 /* System Control Space Non-Secure */
#define	BASE_UICR	0x00FF8000 /* User information configuration */
#define	BASE_FICR	0x00FF0000 /* Factory information configuration */

#define	ID_DCNF		0
#define	ID_VREQCTRL	4
#define	ID_CLOCK	5
#define	ID_POWER	5
#define	ID_RESET	5
#define	ID_CTRLAP	6
#define	ID_RADIO	8
#define	ID_RNG		9
#define	ID_GPIOTE	10
#define	ID_WDT		11
#define	ID_TIMER0	12
#define	ID_ECB		13
#define	ID_AAR		14
#define	ID_CCM		14
#define	ID_DPPIC	15
#define	ID_TEMP		16
#define	ID_RTC0		17
#define	ID_IPC		18
#define	ID_SPIM0	19
#define	ID_SPIS0	19
#define	ID_TWIM0	19
#define	ID_TWIS0	19
#define	ID_UARTE0	19
#define	ID_EGU0		20
#define	ID_RTC1		22
#define	ID_TIMER1	24
#define	ID_TIMER2	25
#define	ID_SWI0		26
#define	ID_SWI1		27
#define	ID_SWI2		28
#define	ID_SWI3		29
#define	ID_ACL		128
#define	ID_NVMC		128
#define	ID_VMC		129
#define	ID_P0		192
#define	ID_P1		192

#endif /* !_ARM_NORDICSEMI_NRF5340_NET_CORE_H_ */
