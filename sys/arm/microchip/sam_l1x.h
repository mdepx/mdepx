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

#ifndef _ARM_MICROCHIP_SAM_L1X_H_
#define _ARM_MICROCHIP_SAM_L1X_H_

#include <arm/microchip/sam_usart.h>
#include <arm/microchip/sam_spi.h>

/* AHB-APB Bridge A */
#define	BASE_PAC	0x40000000
#define	BASE_PM		0x40000400
#define	BASE_MCLK	0x40000800
#define	BASE_RSTC	0x40000C00
#define	BASE_OSCCTRL	0x40001000
#define	BASE_OSC32KCTRL	0x40001400
#define	BASE_SUPC	0x40001800
#define	BASE_GCLK	0x40001C00
#define	BASE_WDT	0x40002000
#define	BASE_RTC	0x40002400
#define	BASE_EIC	0x40002800
#define	BASE_FREQM	0x40002C00
#define	BASE_PORT	0x40003000
#define	BASE_AC		0x40003400

/* AHB-APB Bridge B */
#define	BASE_DSU	0x41002000
#define	BASE_NVMCTRL	0x41004000
#define	BASE_DMAC	0x41006000
#define	BASE_HMATRIXHS	0x41008000

/* AHB-APB Bridge C */
#define	BASE_EVSYS	0x42000000
#define	BASE_SERCOM0	0x42000400
#define	BASE_SERCOM1	0x42000800
#define	BASE_SERCOM2	0x42000C00
#define	BASE_TC0	0x42001000
#define	BASE_TC1	0x42001400
#define	BASE_TC2	0x42001800
#define	BASE_ADC	0x42001C00
#define	BASE_DAC	0x42002000
#define	BASE_PTC	0x42002400
#define	BASE_TRNG	0x42002800
#define	BASE_CCL	0x42002C00
#define	BASE_OPAMP	0x42003000
#define	BASE_TRAM	0x42003400

#endif /* !_ARM_MICROCHIP_SAM_L1X_H_ */
