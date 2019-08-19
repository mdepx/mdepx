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

#ifndef _ARM_NORDICSEMI_NRF9160_WDT_H_
#define _ARM_NORDICSEMI_NRF9160_WDT_H_

#define	WDT_TASKS_START		0x000	/* Start the watchdog */
#define	WDT_SUBSCRIBE_START	0x080	/* Subscribe configuration for task START */
#define	WDT_EVENTS_TIMEOUT	0x100	/* Watchdog timeout */
#define	WDT_PUBLISH_TIMEOUT	0x180	/* Publish configuration for event TIMEOUT */
#define	WDT_INTENSET		0x304	/* Enable interrupt */
#define	WDT_INTENCLR		0x308	/* Disable interrupt */
#define	WDT_RUNSTATUS		0x400	/* Run status */
#define	WDT_REQSTATUS		0x404	/* Request status */
#define	WDT_CRV			0x504	/* Counter reload value */
#define	WDT_RREN		0x508	/* Enable register for reload request registers */
#define	WDT_CONFIG		0x50C	/* Configuration register */
#define	WDT_RR(n)		(0x600 + (n) * 0x4)	/* Reload request n (0 to 7) */

struct wdt_softc {
	size_t base;
};

#endif /* !_ARM_NORDICSEMI_NRF9160_WDT_H_ */
