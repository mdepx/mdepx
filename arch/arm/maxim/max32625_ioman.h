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

#ifndef _ARM_MAXIM_MAX32625_IOMAN_H_
#define _ARM_MAXIM_MAX32625_IOMAN_H_

#define	IOMAN_WUD_REQ0		0x000	/* R/W Wakeup Detect Mode Request Register 0 (P0/P1/P2/P3) */
#define	IOMAN_WUD_REQ1		0x004	/* R/W Wakeup Detect Mode Request Register 1 (P4) */
#define	IOMAN_WUD_ACK0		0x008	/* R/O Wakeup Detect Mode Acknowledge Register 0 (P0/P1/P2/P3) */
#define	IOMAN_WUD_ACK1		0x00C	/* R/O Wakeup Detect Mode Acknowledge Register 1 (P4) */
#define	IOMAN_ALI_REQ0		0x010	/* R/W Analog Input Request Register 0 */
#define	IOMAN_ALI_REQ1		0x014	/* R/W Analog Input Request Register 1 */
#define	IOMAN_ALI_ACK0		0x018	/* R/O Analog Input Acknowledge Register 0 */
#define	IOMAN_ALI_ACK1		0x01C	/* R/O Analog Input Acknowledge Register 1 */
#define	IOMAN_ALI_CONNECT0	0x020	/* R/W Analog I/O Connection Control Register 0 */
#define	IOMAN_ALI_CONNECT1	0x024	/* R/W Analog I/O Connection Control Register 1 */
#define	IOMAN_SPIX_REQ		0x028	/* R/W SPIX I/O Mode Request */
#define	IOMAN_SPIX_ACK		0x02C	/* R/O SPIX I/O Mode Acknowledge */
#define	IOMAN_UART0_REQ		0x030	/* R/W UART0 I/O Mode Request */
#define	IOMAN_UART0_ACK		0x034	/* R/O UART0 I/O Mode Acknowledge */
#define	IOMAN_UART1_REQ		0x038	/* R/W UART1 I/O Mode Request */
#define	IOMAN_UART1_ACK		0x03C	/* R/O UART1 I/O Mode Acknowledge */
#define	IOMAN_UART2_REQ		0x040	/* R/W UART2 I/O Mode Request */
#define	IOMAN_UART2_ACK		0x044	/* R/O UART2 I/O Mode Acknowledge */
#define	IOMAN_UART3_REQ		0x048	/* R/W UART3 I/O Mode Request (N/A) */
#define	IOMAN_UART3_ACK		0x04C	/* R/O UART3 I/O Mode Acknowledge (N/A) */
#define	IOMAN_UART_REQ(n)	(0x030 + 0x4 * (n))	/* R/W UARTn I/O Mode Request */
#define	IOMAN_UART_ACK(n)	(0x034 + 0x4 * (n))	/* R/O UARTn I/O Mode Acknowledge */
#define	 UART_REQ_IO_REQ	(1 << 4)	/* Requests UARTn mode for TX and RX pins. */
#define	IOMAN_I2CM0_REQ		0x050	/* R/W I2C Master 0 I/O Request */
#define	IOMAN_I2CM0_ACK		0x054	/* R/O I2C Master 0 I/O Acknowledge */
#define	IOMAN_I2CM1_REQ		0x058	/* R/W I2C Master 1 I/O Request */
#define	IOMAN_I2CM1_ACK		0x05C	/* R/O I2C Master 1 I/O Acknowledge */
#define	IOMAN_I2CM2_REQ		0x060	/* R/W I2C Master 2 I/O Request (N/A) */
#define	IOMAN_I2CM2_ACK		0x064	/* R/O I2C Master 2 I/O Acknowledge (N/A) */
#define	IOMAN_I2CS_REQ		0x068	/* R/W I2C Slave I/O Request */
#define	IOMAN_I2CS_ACK		0x06C	/* R/O I2C Slave I/O Acknowledge */
#define	IOMAN_SPIM0_REQ		0x070	/* R/W SPI Master 0 I/O Mode Request */
#define	IOMAN_SPIM0_ACK		0x074	/* R/O SPI Master 0 I/O Mode Acknowledge */
#define	IOMAN_SPIM1_REQ		0x078	/* R/W SPI Master 1 I/O Mode Request */
#define	IOMAN_SPIM1_ACK		0x07C	/* R/O SPI Master 1 I/O Mode Acknowledge */
#define	IOMAN_SPIM2_REQ		0x080	/* R/W SPI Master 2 I/O Mode Request */
#define	IOMAN_SPIM2_ACK		0x084	/* R/O SPI Master 2 I/O Mode Acknowledge */
#define	IOMAN_SPIB_REQ		0x088	/* R/W SPI Bridge I/O Mode Request (Reserved) */
#define	IOMAN_SPIB_ACK		0x08C	/* R/O SPI Bridge I/O Mode Acknowledge (Reserved) */
#define	IOMAN_OWM_REQ		0x090	/* R/W 1-Wire Master I/O Mode Request */
#define	IOMAN_OWM_ACK		0x094	/* R/W 1-Wire Master I/O Mode Acknowledge */
#define	IOMAN_SPIS_REQ		0x098	/* R/W SPI Slave I/O Mode Request */
#define	IOMAN_SPIS_ACK		0x09C	/* R/O SPI Slave I/O Mode Acknowledge */
#define	IOMAN_USE_VDDIOH_0	0x100	/* R/W Enable VDDIOH Register 0 */
#define	IOMAN_USE_VDDIOH_1	0x104	/* R/W Enable VDDIOH Register 1 */
#define	IOMAN_PAD_MODE		0x110	/* R/W Pad Mode Control Register (Reserved) */

struct max32625_ioman_softc {
	uint32_t base;
};

void max32625_ioman_init(struct max32625_ioman_softc *sc, uint32_t base);
void max32625_ioman_write(struct max32625_ioman_softc *sc,
    uint32_t reg, uint32_t val);

#endif /* !_ARM_MAXIM_MAX32625_IOMAN_H_ */
