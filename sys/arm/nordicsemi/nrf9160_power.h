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

#ifndef _ARM_NORDICSEMI_NRF9160_POWER_H_
#define _ARM_NORDICSEMI_NRF9160_POWER_H_

#define	POWER_TASKS_CONSTLAT	0x078 /* Enable constant latency mode. */
#define	POWER_TASKS_LOWPWR	0x07C /* Enable low power mode (variable latency) */
#define	POWER_SUBSCRIBE_CONSTLAT 0x0F8 /* Subscribe configuration for task CONSTLAT */
#define	POWER_SUBSCRIBE_LOWPWR	0x0FC /* Subscribe configuration for task LOWPWR */
#define	POWER_EVENTS_POFWARN	0x108 /* Power failure warning */
#define	POWER_EVENTS_SLEEPENTER	0x114 /* CPU entered WFI/WFE sleep */
#define	POWER_EVENTS_SLEEPEXIT	0x118 /* CPU exited WFI/WFE sleep */
#define	POWER_PUBLISH_POFWARN	0x188 /* Publish configuration for event POFWARN */
#define	POWER_PUBLISH_SLEEPENTER 0x194 /* Publish configuration for event SLEEPENTER */
#define	POWER_PUBLISH_SLEEPEXIT	0x198 /* Publish configuration for event SLEEPEXIT */
#define	POWER_INTEN		0x300 /* Enable or disable interrupt */
#define	POWER_INTENSET		0x304 /* Enable interrupt */
#define	POWER_INTENCLR		0x308 /* Disable interrupt */
#define	POWER_RESETREAS		0x400 /* Reset reason */
#define	POWER_POWERSTATUS	0x440 /* Modem domain power status */
#define	POWER_GPREGRET(n)	(0x51C + (n) * 0x4) /* General purpose retention register */

struct power_softc {
	size_t base;
};

void power_init(struct power_softc *sc, uint32_t base);
void power_write4(struct power_softc *sc, uint32_t reg,
    uint32_t val);

#endif /* !_ARM_NORDICSEMI_NRF9160_POWER_H_ */
