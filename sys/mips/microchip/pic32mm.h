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

#ifndef	_MIPS_MICROCHIP_PIC32MM_H_
#define	_MIPS_MICROCHIP_PIC32MM_H_

#include <mips/microchip/pic32_adc.h>
#include <mips/microchip/pic32_uart.h>
#include <mips/microchip/pic32_spi.h>
#include <mips/microchip/pic32_port.h>
#include <mips/microchip/pic32_ccp.h>
#include <mips/microchip/pic32mm_pps.h>
#include <mips/microchip/pic32mm_cdac.h>

#define	PORTS_BASE	0xBF802600
#define	PPS_BASE	0xBF802400
#define	UART1_BASE	0xBF800600
#define	UART2_BASE	0xBF800680
#define	SPI1_BASE	0xBF808080
#define	SPI2_BASE	0xBF808100
#define	TIMER_BASE	0xBF808000
#define	CCP1_BASE	0xBF800100
#define	ADC1_BASE	0xBF800700
#define	CDAC1_BASE	0xBF800980
#define	INTC_BASE	0xBF80F000

#define	PIC32MM_CFG0	0xa4210582
#define	PIC32MM_CFG1	0x80000012
#define	PIC32MM_CFG2	0x00237068
#define	PIC32MM_CFG3	0x00000001

#define	PIC32MM_DEVCFG	PIC32_DEVCFG(PIC32MM_CFG0, PIC32MM_CFG1, \
				     PIC32MM_CFG2, PIC32MM_CFG3)

#endif /* !_MIPS_MICROCHIP_PIC32MM_H_ */
