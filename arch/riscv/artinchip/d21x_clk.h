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

#ifndef _RISCV_ARTINCHIP_D21X_CLK_H_
#define	_RISCV_ARTINCHIP_D21X_CLK_H_

/* Fixed rate. */
#define	D21X_CLK_DUMMY		0
#define	D21X_CLK_OSC24M		1
#define	D21X_CLK_OSC32K		2
#define	D21X_CLK_RC1M		3
/* PLL clock. */
#define	D21X_CLK_PLL_INT0	4
#define	D21X_CLK_PLL_INT1	5
#define	D21X_CLK_PLL_FRA0	6
#define	D21X_CLK_PLL_FRA1	7
#define	D21X_CLK_PLL_FRA2	8
/* Fixed factor clock. */
#define	D21X_CLK_AXI0_SRC1	9
#define	D21X_CLK_AHB0_SRC1	10
#define	D21X_CLK_APB0_SRC1	11
#define	D21X_CLK_APB1_SRC1	12
#define	D21X_CLK_CPU_SRC1	13
#define	D21X_CLK_DM_SRC1	14
/* Sysem clock. */
#define	D21X_CLK_AXI0		15
#define	D21X_CLK_AHB0		16
#define	D21X_CLK_APB0		17
#define	D21X_CLK_APB1		18
#define	D21X_CLK_CPU		19
/* Peripheral clock. */
#define	D21X_CLK_DMA		20
#define	D21X_CLK_CE		21
#define	D21X_CLK_USBD		22
#define	D21X_CLK_USBH0		23
#define	D21X_CLK_USBH1		24
#define	D21X_CLK_USB_PHY0	25
#define	D21X_CLK_USB_PHY1	26
#define	D21X_CLK_GMAC0		27
#define	D21X_CLK_GMAC1		28
#define	D21X_CLK_QSPI0		29
#define	D21X_CLK_QSPI1		30
#define	D21X_CLK_QSPI2		31
#define	D21X_CLK_QSPI3		32
#define	D21X_CLK_SDMC0		33
#define	D21X_CLK_SDMC1		34
#define	D21X_CLK_SDMC2		35
#define	D21X_CLK_SYSCFG		36
#define	D21X_CLK_RTC		37
#define	D21X_CLK_SPIENC		38
#define	D21X_CLK_I2S0		39
#define	D21X_CLK_I2S1		40
#define	D21X_CLK_CODEC		41
#define	D21X_CLK_RGB		42
#define	D21X_CLK_LVDS		43
#define	D21X_CLK_MIPIDSI	44
#define	D21X_CLK_DE		45
#define	D21X_CLK_GE		46
#define	D21X_CLK_VE		47
#define	D21X_CLK_WDT		48
#define	D21X_CLK_SID		49
#define	D21X_CLK_GTC		50
#define	D21X_CLK_GPIO		51
#define	D21X_CLK_UART0		52
#define	D21X_CLK_UART1		53
#define	D21X_CLK_UART2		54
#define	D21X_CLK_UART3		55
#define	D21X_CLK_UART4		56
#define	D21X_CLK_UART5		57
#define	D21X_CLK_UART6		58
#define	D21X_CLK_UART7		59
#define	D21X_CLK_I2C0		60
#define	D21X_CLK_I2C1		61
#define	D21X_CLK_I2C2		62
#define	D21X_CLK_I2C3		63
#define	D21X_CLK_CAN0		64
#define	D21X_CLK_CAN1		65
#define	D21X_CLK_PWM		66
#define	D21X_CLK_ADCIM		67
#define	D21X_CLK_GPAI		68
#define	D21X_CLK_RTP		69
#define	D21X_CLK_TSEN		70
#define	D21X_CLK_CIR		71
#define	D21X_CLK_DVP		72
#define	D21X_CLK_PBUS		73
#define	D21X_CLK_MTOP		74
#define	D21X_CLK_DM		75
#define	D21X_CLK_PWMCS		76
#define	D21X_CLK_PSADC		77
/* Display clock. */
#define	D21X_CLK_PIX		78
#define	D21X_CLK_SCLK		79
/* Output clock. */
#define	D21X_CLK_OUT0		80
#define	D21X_CLK_OUT1		81
#define	D21X_CLK_OUT2		82
#define	D21X_CLK_OUT3		83
#define	D21X_CLK_N		84 /* N clocks */

#endif /* !_RISCV_ARTINCHIP_D21X_CLK_H_ */
