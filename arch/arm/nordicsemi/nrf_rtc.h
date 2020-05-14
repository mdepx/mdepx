/*-
 * Copyright (c) 2018-2020 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _ARM_NORDICSEMI_NRF9160_RTC_H_
#define _ARM_NORDICSEMI_NRF9160_RTC_H_

#include <sys/time.h>

#define	RTC_TASKS_START		0x000	/* Start RTC COUNTER */
#define	RTC_TASKS_STOP		0x004	/* Stop RTC COUNTER */
#define	RTC_TASKS_CLEAR		0x008	/* Clear RTC COUNTER */
#define	RTC_TASKS_TRIGOVRFLW	0x00C	/* Set COUNTER to 0xFFFFF0 */
#define	RTC_EVENTS_TICK		0x100	/* Event on COUNTER increment */
#define	RTC_EVENTS_OVRFLW	0x104	/* Event on COUNTER overflow */
#define	RTC_EVENTS_COMPARE(n)	(0x140 + (n) * 0x4) /* Compare event on CCn match */
#define	RTC_INTENSET		0x304	/* Enable interrupt */
#define	 INTENSET_TICK		(1 << 0)
#define	 INTENSET_OVRFLW	(1 << 1)
#define	 INTENSET_COMPARE_S	16
#define	RTC_INTENCLR		0x308	/* Disable interrupt */
#define	RTC_EVTEN		0x340	/* Enable or disable event routing */
#define	RTC_EVTENSET		0x344	/* Enable event routing */
#define	RTC_EVTENCLR		0x348	/* Disable event routing */
#define	RTC_COUNTER		0x504	/* Current COUNTER value */
#define	RTC_PRESCALER		0x508	/* 12 bit prescaler for COUNTER frequency (32768/(PRESCALER+1)) */
#define	RTC_CC(n)		(0x540 + (n) * 0x4) /* Compare register n */

struct nrf_rtc_softc {
	size_t base;
	struct timespec ts;
	uint16_t prescaler;
	uint32_t freq;
	uint32_t period;
};

void nrf_rtc_init(mdx_device_t dev, uint32_t base, uint16_t prescaler);
void nrf_rtc_intr(void *arg, int irq);

struct rtc_driver nrf_rtc_driver;

#endif /* !_ARM_NORDICSEMI_NRF9160_RTC_H_ */
