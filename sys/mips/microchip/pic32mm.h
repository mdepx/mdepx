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
#define	CCP2_BASE	0xBF800200
#define	CCP3_BASE	0xBF800300
#define	ADC1_BASE	0xBF800700
#define	CDAC1_BASE	0xBF800980
#define	INTC_BASE	0xBF80F000

#define	PIC32MM_CFG0	0xa4210582
#define	PIC32MM_CFG1	0x80000012
#define	PIC32MM_CFG2	0x00237068
#define	PIC32MM_CFG3	0x00000001

#define	PIC32MM_DEVCFG	PIC32_DEVCFG(PIC32MM_CFG0, PIC32MM_CFG1, \
				     PIC32MM_CFG2, PIC32MM_CFG3)

/* Interrupt controller vectors */
#define	_CORE_TIMER_VECTOR		0
#define	_CORE_SOFTWARE_0_VECTOR		1
#define	_CORE_SOFTWARE_1_VECTOR		2
#define	_EXTERNAL_0_VECTOR		3
#define	_EXTERNAL_1_VECTOR		4
#define	_EXTERNAL_2_VECTOR		5
#define	_EXTERNAL_3_VECTOR		6
#define	_EXTERNAL_4_VECTOR		7
#define	_CHANGE_NOTICE_A_VECTOR		8
#define	_CHANGE_NOTICE_B_VECTOR		9
#define	_CHANGE_NOTICE_C_VECTOR		10
#define	_TIMER_1_VECTOR			11
#define	_COMPARATOR_1_VECTOR		12
#define	_COMPARATOR_2_VECTOR		13
#define	_RTCC_VECTOR			14
#define	_ADC_VECTOR			15
#define	_CRC_VECTOR			16
#define	_HLVD_VECTOR			17
#define	_CLC1_VECTOR			18
#define	_CLC2_VECTOR			19
#define	_SPI1_ERR_VECTOR		20
#define	_SPI1_TX_VECTOR			21
#define	_SPI1_RX_VECTOR			22
#define	_UART1_RX_VECTOR		23
#define	_UART1_TX_VECTOR		24
#define	_UART1_ERR_VECTOR		25
#define	_CCP1_VECTOR			29
#define	_CCT1_VECTOR			30
#define	_CCP2_VECTOR			31
#define	_CCT2_VECTOR			32
#define	_CCP3_VECTOR			33
#define	_CCT3_VECTOR			34
#define	_SPI2_ERR_VECTOR		37
#define	_SPI2_TX_VECTOR			38
#define	_SPI2_RX_VECTOR			39
#define	_UART2_RX_VECTOR		40
#define	_UART2_TX_VECTOR		41
#define	_UART2_ERR_VECTOR		42
#define	_NVM_VECTOR			46
#define	_PERFORMANCE_COUNTER_VECTOR	47

#endif /* !_MIPS_MICROCHIP_PIC32MM_H_ */
