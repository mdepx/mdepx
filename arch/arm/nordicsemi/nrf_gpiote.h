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

#ifndef _ARM_NORDICSEMI_NRF9160_GPIOTE_H_
#define _ARM_NORDICSEMI_NRF9160_GPIOTE_H_

#define	GPIOTE_TASKS_OUT(n)	(0x000 + (n) * 0x4)	/* Task for writing to pin specified in CONFIG[0].PSEL. Action on pin is configured in CONFIG[0].POLARITY. */
#define	GPIOTE_TASKS_SET(n)	(0x030 + (n) * 0x4)	/* Task for writing to pin specified in CONFIG[0].PSEL. Action on pin is to set it high. */
#define	GPIOTE_TASKS_CLR(n)	(0x060 + (n) * 0x4)	/* Task for writing to pin specified in CONFIG[0].PSEL. Action on pin is to set it low. */
#define	GPIOTE_SUBSCRIBE_OUT(n)	(0x080 + (n) * 0x4)	/* Subscribe configuration for task OUT[0] */
#define	GPIOTE_SUBSCRIBE_SET(n)	(0x0B0 + (n) * 0x4)	/* Subscribe configuration for task SET[0] */
#define	GPIOTE_SUBSCRIBE_CLR(n)	(0x0E0 + (n) * 0x4)	/* Subscribe configuration for task CLR[0] */
#define	GPIOTE_EVENTS_IN(n)	(0x100 + (n) * 0x4)	/* Event generated from pin specified in CONFIG[0].PSEL */
#define	GPIOTE_EVENTS_PORT	0x17C			/* Event generated from multiple input GPIO pins with SENSE mechanism enabled */
#define	GPIOTE_PUBLISH_IN(n)	(0x180 + (n) * 0x4)	/* Publish configuration for event IN[0] */
#define	GPIOTE_PUBLISH_PORT	0x1FC			/* Publish configuration for event PORT */
#define	GPIOTE_INTENSET		0x304			/* Enable interrupt */
#define	GPIOTE_INTENCLR		0x308			/* Disable interrupt */
#define	GPIOTE_CONFIG(n)	(0x510 + (n) * 0x4)	/* Configuration for OUT[n], SET[n] and CLR[n] tasks and IN[n] event */
#define	 CONFIG_MODE_S		0 /* Mode */
#define	 CONFIG_MODE_M		(0x3 << CONFIG_MODE_S)
#define	 CONFIG_MODE_DIS	(0 << CONFIG_MODE_S) /* Disabled */
#define	 CONFIG_MODE_EVENT	(1 << CONFIG_MODE_S) /* Event mode */
#define	 CONFIG_MODE_TASK	(3 << CONFIG_MODE_S) /* Task mode */
#define	 CONFIG_PSEL_S		8 /* GPIO number associated with SET[n], CLR[n] and OUT[n] tasks and IN[n] event */
#define	 CONFIG_PSEL_M		(0x1f << CONFIG_PSEL_S)
#define	 CONFIG_POLARITY_S	16
#define	 CONFIG_POLARITY_M	(0x3 << CONFIG_POLARITY_S)
#define	 CONFIG_POLARITY_NONE	(0x0 << CONFIG_POLARITY_S)
#define	 CONFIG_POLARITY_LOTOHI	(0x1 << CONFIG_POLARITY_S)
#define	 CONFIG_POLARITY_HITOLO	(0x2 << CONFIG_POLARITY_S)
#define	 CONFIG_POLARITY_TOGGLE	(0x3 << CONFIG_POLARITY_S)
#define	 CONFIG_OUTINIT		(1 << 20) /* Initial value of the output */

struct nrf_gpiote_softc {
	size_t base;
};

void nrf_gpiote_init(struct nrf_gpiote_softc *sc, uint32_t base);

#endif /* !_ARM_NORDICSEMI_NRF9160_GPIOTE_H_ */
