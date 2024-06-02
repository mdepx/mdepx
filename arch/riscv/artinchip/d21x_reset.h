/*-
 * Copyright (c) 2024 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _RISCV_ARTINCHIP_D21X_RESET_H_
#define _RISCV_ARTINCHIP_D21X_RESET_H_

#define	RESET_DMA		0
#define	RESET_CE		1
#define	RESET_USBD		2
#define	RESET_USBH0		3
#define	RESET_USBH1		4
#define	RESET_USBPHY0		5
#define	RESET_USBPHY1		6
#define	RESET_GMAC0		7
#define	RESET_GMAC1		8
#define	RESET_QSPI0		9
#define	RESET_QSPI1		10
#define	RESET_QSPI2		11
#define	RESET_QSPI3		12
#define	RESET_SDMMC0		13
#define	RESET_SDMMC1		14
#define	RESET_SDMMC2		15
#define	RESET_SYSCFG		16
#define	RESET_RTC		17
#define	RESET_SPIENC		18
#define	RESET_I2S0		19
#define	RESET_I2S1		20
#define	RESET_CODEC		21
#define	RESET_RGB		22
#define	RESET_LVDS		23
#define	RESET_MIPIDSI		24
#define	RESET_DE		25
#define	RESET_GE		26
#define	RESET_VE		27
#define	RESET_WDT		28
#define	RESET_SID		29
#define	RESET_GTC		30
#define	RESET_GPIO		31
#define	RESET_UART0		32
#define	RESET_UART1		33
#define	RESET_UART2		34
#define	RESET_UART3		35
#define	RESET_UART4		36
#define	RESET_UART5		37
#define	RESET_UART6		38
#define	RESET_UART7		39
#define	RESET_I2C0		40
#define	RESET_I2C1		41
#define	RESET_I2C2		42
#define	RESET_I2C3		43
#define	RESET_CAN0		44
#define	RESET_CAN1		45
#define	RESET_PWM		46
#define	RESET_ADCIM		47
#define	RESET_GPAI		48
#define	RESET_RTP		49
#define	RESET_TSEN		50
#define	RESET_CIR		51
#define	RESET_DVP		52
#define	RESET_MTOP		53
#define	RESET_PBUS		54
#define	RESET_PWMCS		55
#define	RESET_PSADC		56
#define	RESET_N			57 /* N resets total. */

#endif /* !_RISCV_ARTINCHIP_D21X_RESET_H_ */
