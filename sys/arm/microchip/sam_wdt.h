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

#ifndef _ARM_MICROCHIP_SAM_WDT_H_
#define _ARM_MICROCHIP_SAM_WDT_H_

#define	WDT_CTRLA	0x00	/* Control A */
#define	WDT_CONFIG	0x01	/* Configuration */
#define	WDT_EWCTRL	0x02	/* Early Warning Control */
#define	WDT_INTENCLR	0x04	/* Interrupt Enable Clear */
#define	WDT_INTENSET	0x05	/* Interrupt Enable Set */
#define	WDT_INTFLAG	0x06	/* Interrupt Flag Status and Clear */
#define	WDT_SYNCBUSY	0x08	/* Synchronization Busy */
#define	WDT_CLEAR	0x0C	/* Clear */

struct sam_wdt_softc {
	uint32_t base;
};

#endif /* !_ARM_MICROCHIP_SAM_WDT_H_ */
