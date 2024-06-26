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

#ifndef _RISCV_ARTINCHIP_D21X_CMU_H_
#define	_RISCV_ARTINCHIP_D21X_CMU_H_

/* Fixed rate. */
#define	D21X_CLK_ID_DUMMY	0
#define	D21X_CLK_ID_OSC24M	1
#define	D21X_CLK_ID_OSC32K	2
#define	D21X_CLK_ID_RC1M	3
/* PLL clock. */
#define	D21X_CLK_ID_PLL_INT0	4
#define	D21X_CLK_ID_PLL_INT1	5
#define	D21X_CLK_ID_PLL_FRA0	6
#define	D21X_CLK_ID_PLL_FRA1	7
#define	D21X_CLK_ID_PLL_FRA2	8
/* Fixed factor clock. */
#define	D21X_CLK_ID_AXI0_SRC1	9
#define	D21X_CLK_ID_AHB0_SRC1	10
#define	D21X_CLK_ID_APB0_SRC1	11
#define	D21X_CLK_ID_APB1_SRC1	12
#define	D21X_CLK_ID_CPU_SRC1	13
#define	D21X_CLK_ID_DM_SRC1	14
/* Sysem clock. */
#define	D21X_CLK_ID_AXI0	15
#define	D21X_CLK_ID_AHB0	16
#define	D21X_CLK_ID_APB0	17
#define	D21X_CLK_ID_APB1	18
#define	D21X_CLK_ID_CPU		19
/* Peripheral clock. */
#define	D21X_CLK_ID_DMA		20
#define	D21X_CLK_ID_CE		21
#define	D21X_CLK_ID_USBD	22
#define	D21X_CLK_ID_USBH0	23
#define	D21X_CLK_ID_USBH1	24
#define	D21X_CLK_ID_USB_PHY0	25
#define	D21X_CLK_ID_USB_PHY1	26
#define	D21X_CLK_ID_GMAC0	27
#define	D21X_CLK_ID_GMAC1	28
#define	D21X_CLK_ID_QSPI0	29
#define	D21X_CLK_ID_QSPI1	30
#define	D21X_CLK_ID_QSPI2	31
#define	D21X_CLK_ID_QSPI3	32
#define	D21X_CLK_ID_SDMC0	33
#define	D21X_CLK_ID_SDMC1	34
#define	D21X_CLK_ID_SDMC2	35
#define	D21X_CLK_ID_SYSCFG	36
#define	D21X_CLK_ID_RTC		37
#define	D21X_CLK_ID_SPIENC	38
#define	D21X_CLK_ID_I2S0	39
#define	D21X_CLK_ID_I2S1	40
#define	D21X_CLK_ID_CODEC	41
#define	D21X_CLK_ID_RGB		42
#define	D21X_CLK_ID_LVDS	43
#define	D21X_CLK_ID_MIPIDSI	44
#define	D21X_CLK_ID_DE		45
#define	D21X_CLK_ID_GE		46
#define	D21X_CLK_ID_VE		47
#define	D21X_CLK_ID_WDT		48
#define	D21X_CLK_ID_SID		49
#define	D21X_CLK_ID_GTC		50
#define	D21X_CLK_ID_GPIO	51
#define	D21X_CLK_ID_UART0	52
#define	D21X_CLK_ID_UART1	53
#define	D21X_CLK_ID_UART2	54
#define	D21X_CLK_ID_UART3	55
#define	D21X_CLK_ID_UART4	56
#define	D21X_CLK_ID_UART5	57
#define	D21X_CLK_ID_UART6	58
#define	D21X_CLK_ID_UART7	59
#define	D21X_CLK_ID_I2C0	60
#define	D21X_CLK_ID_I2C1	61
#define	D21X_CLK_ID_I2C2	62
#define	D21X_CLK_ID_I2C3	63
#define	D21X_CLK_ID_CAN0	64
#define	D21X_CLK_ID_CAN1	65
#define	D21X_CLK_ID_PWM		66
#define	D21X_CLK_ID_ADCIM	67
#define	D21X_CLK_ID_GPAI	68
#define	D21X_CLK_ID_RTP		69
#define	D21X_CLK_ID_TSEN	70
#define	D21X_CLK_ID_CIR		71
#define	D21X_CLK_ID_DVP		72
#define	D21X_CLK_ID_PBUS	73
#define	D21X_CLK_ID_MTOP	74
#define	D21X_CLK_ID_DM		75
#define	D21X_CLK_ID_PWMCS	76
#define	D21X_CLK_ID_PSADC	77
/* Display clock. */
#define	D21X_CLK_ID_PIX		78
#define	D21X_CLK_ID_SCLK	79
/* Output clock. */
#define	D21X_CLK_ID_OUT0	80
#define	D21X_CLK_ID_OUT1	81
#define	D21X_CLK_ID_OUT2	82
#define	D21X_CLK_ID_OUT3	83
#define	D21X_CLK_ID_N		84 /* N clocks */

#define	D21X_CLK_FREQ_CLK1	48000000
#define	D21X_CLK_FREQ_CLK2	100000000
#define	D21X_CLK_FREQ_CLK3	24576000
#define	D21X_CLK_FREQ_CLK4	62500000
#define	D21X_CLK_FREQ_CLK5	48000000
#define	D21X_CLK_FREQ_CLK6	60000000

#define	D21X_CLOCK_120M		120000000
#define	D21X_CLOCK_100M		100000000
#define	D21X_CLOCK_72M		72000000
#define	D21X_CLOCK_60M		60000000
#define	D21X_CLOCK_50M		50000000
#define	D21X_CLOCK_30M		30000000
#define	D21X_CLOCK_24M		24000000
#define	D21X_CLOCK_12M		12000000
#define	D21X_CLOCK_4M		4000000
#define	D21X_CLOCK_1M		1000000
#define	D21X_CLOCK_32K		32768

#define	D21X_PLL_INT0_GEN	0x0000
#define	D21X_PLL_INT1_GEN	0x0004
#define	D21X_PLL_FRA0_GEN	0x0020
#define	D21X_PLL_FRA1_GEN	0x0024
#define	D21X_PLL_FRA2_GEN	0x0028
#define	D21X_PLL_INT0_CFG	0x0040
#define	D21X_PLL_INT1_CFG	0x0044
#define	D21X_PLL_FRA0_CFG	0x0060
#define	D21X_PLL_FRA1_CFG	0x0064
#define	D21X_PLL_FRA2_CFG	0x0068
#define	D21X_PLL_FRA0_SDM	0x0080
#define	D21X_PLL_FRA1_SDM	0x0084
#define	D21X_PLL_FRA2_SDM	0x0088
#define	D21X_PLL_COM		0x00A0
#define	D21X_PLL_IN		0x00A4

#define	D21X_CLK_OUT0		0x00E0
#define	D21X_CLK_OUT1		0x00E4
#define	D21X_CLK_OUT2		0x00E8
#define	D21X_CLK_OUT3		0x00EC
#define	D21X_CLK_AXI0		0x0100
#define	D21X_CLK_AHB0		0x0110
#define	D21X_CLK_APB0		0x0120
#define	D21X_CLK_APB1		0x0124
#define	D21X_CLK_CPU		0x0200
#define	D21X_CLK_DM		0x0204
#define	D21X_CLK_WDT		0x020C
#define	D21X_CLK_DDR		0x0210
#define	D21X_CLK_DISP		0x0220
#define	D21X_CLK_DMA		0x0410
#define	D21X_CLK_CE		0x0418
#define	D21X_CLK_USBD		0x041C
#define	D21X_CLK_USBH0		0x0420
#define	D21X_CLK_USBH1		0x0424
#define	D21X_CLK_USB_PHY0	0x0430
#define	D21X_CLK_USB_PHY1	0x0434
#define	D21X_CLK_GMAC0		0x0440
#define	D21X_CLK_GMAC1		0x0444
#define	D21X_CLK_QSPI0		0x0460
#define	D21X_CLK_QSPI1		0x0464
#define	D21X_CLK_QSPI2		0x0468
#define	D21X_CLK_QSPI3		0x046C
#define	D21X_CLK_SDMC0		0x0470
#define	D21X_CLK_SDMC1		0x0474
#define	D21X_CLK_SDMC2		0x0478
#define	D21X_CLK_PBUS		0x04A0
#define	D21X_CLK_SYSCFG		0x0800
#define	D21X_CLK_SPIENC		0x0810
#define	D21X_CLK_PWMCS		0x0814
#define	D21X_CLK_PSADC		0x0818
#define	D21X_CLK_MTOP		0x081C
#define	D21X_CLK_I2S0		0x0820
#define	D21X_CLK_I2S1		0x0824
#define	D21X_CLK_CODEC		0x0830
#define	D21X_CLK_GPIO		0x083C
#define	D21X_CLK_UART0		0x0840
#define	D21X_CLK_UART1		0x0844
#define	D21X_CLK_UART2		0x0848
#define	D21X_CLK_UART3		0x084C
#define	D21X_CLK_UART4		0x0850
#define	D21X_CLK_UART5		0x0854
#define	D21X_CLK_UART6		0x0858
#define	D21X_CLK_UART7		0x085C
#define	D21X_CLK_RGB		0x0880
#define	D21X_CLK_LVDS		0x0884
#define	D21X_CLK_MIPID		0x0888
#define	D21X_CLK_DVP		0x0890
#define	D21X_CLK_DE		0x08C0
#define	D21X_CLK_GE		0x08C4
#define	D21X_CLK_VE		0x08C8
#define	D21X_CLK_SID		0x0904
#define	D21X_CLK_RTC		0x0908
#define	D21X_CLK_GTC		0x090C
#define	D21X_CLK_I2C0		0x0960
#define	D21X_CLK_I2C1		0x0964
#define	D21X_CLK_I2C2		0x0968
#define	D21X_CLK_I2C3		0x096C
#define	D21X_CLK_CAN0		0x0980
#define	D21X_CLK_CAN1		0x0984
#define	D21X_CLK_PWM		0x0990
#define	D21X_CLK_ADCIM		0x09A0
#define	D21X_CLK_GPAI		0x09A4
#define	D21X_CLK_RTP		0x09A8
#define	D21X_CLK_TSEN		0x09AC
#define	D21X_CLK_CIR		0x09B0
#define	D21X_CLK_VER		0x0FFC

#define	MOD_RSTN		(1 << 13) /* Module reset control */
#define	MOD_BUS_EN		(1 << 12) /* Bus clock enable */
#define	MOD_CLK_EN		(1 << 8) /* Module clock enable */
#define	MOD_CLK_DIV_S		0
#define	MOD_CLK_DIV_M		(0x1f << MOD_CLK_DIV_S)

struct d21x_cmu_softc {
	size_t base;
};

void d21x_cmu_init(mdx_device_t dev, uint32_t base);
void d21x_cmu_clk_enable(mdx_device_t dev, uint32_t clk_reg, uint32_t div);

#endif /* !_RISCV_ARTINCHIP_D21X_CMU_H_ */
