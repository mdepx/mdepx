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

#ifndef _RISCV_ARTINCHIP_D21X_H_
#define _RISCV_ARTINCHIP_D21X_H_

#if 0
#include <riscv/artinchip/d21x_uart.h>
#endif

#define	BASE_BROM	0x00000000
#define	BASE_SRAM	0x00100000
#define	BASE_DMA	0x10000000
#define	BASE_CE		0x10020000
#define	BASE_USB_DEV	0x10200000
#define	BASE_USB_HOST0	0x10210000
#define	BASE_USB_HOST1	0x10220000
#define	BASE_GMAC0	0x10280000
#define	BASE_GMAC1	0x10290000
#define	BASE_QSPI0	0x10400000
#define	BASE_QSPI1	0x10410000
#define	BASE_QSPI2	0x10420000
#define	BASE_QSPI3	0x10430000
#define	BASE_SDMC0	0x10440000
#define	BASE_SDMC1	0x10450000
#define	BASE_SDMC2	0x10460000
#define	BASE_PBUS	0x107F0000
#define	BASE_SYSCFG	0x18000000
#define	BASE_CMU	0x18020000
#define	BASE_CMT	0x18028000
#define	BASE_SPI_ENC	0x18100000
#define	BASE_PWMCS	0x18200000
#define	BASE_PSADC	0x18210000
#define	BASE_DDR_CTL	0x18400000
#define	BASE_MTOP	0x184FF000
#define	BASE_DDR_PHY	0x18500000
#define	BASE_I2S0	0x18600000
#define	BASE_I2S1	0x18601000
#define	BASE_AUDIO	0x18610000
#define	BASE_GPIO	0x18700000
#define	BASE_PMT	0x1870F000
#define	BASE_UART0	0x18710000
#define	BASE_UART1	0x18711000
#define	BASE_UART2	0x18712000
#define	BASE_UART3	0x18713000
#define	BASE_UART4	0x18714000
#define	BASE_UART5	0x18715000
#define	BASE_UART6	0x18716000
#define	BASE_UART7	0x18717000
#define	BASE_UART(n)	(BASE_UART0 + (n) * 0x1000)
#define	BASE_LCD	0x18800000
#define	BASE_LVDS	0x18810000
#define	BASE_MIPI_DSI	0x18820000
#define	BASE_DVP	0x18830000
#define	BASE_MIPI_CSI	0x18840000
#define	BASE_DE		0x18A00000
#define	BASE_GE		0x18B00000
#define	BASE_VE		0x18C00000
#define	BASE_WDT	0x19000000
#define	BASE_WRI	0x1900F000
#define	BASE_SID	0x19010000
#define	BASE_RTC	0x19030000
#define	BASE_GTC	0x19050000
#define	BASE_I2C0	0x19220000
#define	BASE_I2C1	0x19221000
#define	BASE_I2C2	0x19222000
#define	BASE_I2C3	0x19223000
#define	BASE_CAN0	0x19230000
#define	BASE_CAN1	0x19231000
#define	BASE_PWM	0x19240000
#define	BASE_ADCIM	0x19250000
#define	BASE_GPAI	0x19251000
#define	BASE_RTP	0x19252000
#define	BASE_TSEN	0x19253000
#define	BASE_CIR	0x19260000
#define	BASE_C906_CPU	0x20000000
#define	BASE_C906_CORE	C906_CPU_BASE
#define	BASE_C906_PLIC	C906_CPU_BASE
#define	BASE_C906_CINT	(C906_CPU_BASE + 0x04000000)
#define	BASE_DRAM	0x40000000

#define	ID_DMA			32
#define	ID_CE			33
#define	ID_USB_DEV		34
#define	ID_USB_HOST0_EHCI	35
#define	ID_USB_HOST0_OHCI	36
#define	ID_USB_HOST1_EHCI	37
#define	ID_USB_HOST1_OHCI	38
#define	ID_GMAC0		39
#define	ID_GMAC1		40
#define	ID_QSPI0		44
#define	ID_QSPI1		45
#define	ID_QSPI2		42
#define	ID_QSPI3		43
#define	ID_SDMC0		46
#define	ID_SDMC1		47
#define	ID_SDMC2		48
#define	ID_SPI_ENC		41
#define	ID_PWMCS_FAULT		24
#define	ID_PWMCS_PWM		25
#define	ID_PWMCS_CAP		26
#define	ID_PWMCS_QEP		27
#define	ID_PSADC		28
#define	ID_MTOP			51
#define	ID_I2S0			52
#define	ID_I2S1			53
#define	ID_AUDIO		54
#define	ID_GPIO			68 /* 68 - 75 */
#define	ID_UART0		76
#define	ID_UART1		77
#define	ID_UART2		78
#define	ID_UART3		79
#define	ID_UART4		80
#define	ID_UART5		81
#define	ID_UART6		82
#define	ID_UART7		83
#define	ID_LCD			55
#define	ID_MIPI_DSI		56
#define	ID_DVP			57
#define	ID_MIPI_CSI		58
#define	ID_DE			59
#define	ID_GE			60
#define	ID_VE			61
#define	ID_WDT			64
#define	ID_RTC			50
#define	ID_I2C0			84
#define	ID_I2C1			85
#define	ID_I2C2			86
#define	ID_I2C3			87
#define	ID_CAN0			88
#define	ID_CAN1			89
#define	ID_PWM			90
#define	ID_GPAI			92
#define	ID_RTP			93
#define	ID_TSEN			94
#define	ID_CIR			95

#endif /* !_RISCV_ARTINCHIP_D21X_H_ */
