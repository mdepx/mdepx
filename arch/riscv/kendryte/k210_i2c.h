/*-
 * Copyright (c) 2020 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _RISCV_KENDRYTE_K210_I2C_H_
#define _RISCV_KENDRYTE_K210_I2C_H_

#include <dev/i2c/i2c.h>

#define	K210_I2C_CON		0x00	/* Control */
#define	 I2C_CON_MASTER		(1 << 0) /* Master mode */
#define	 I2C_CON_SPEED_S	1
#define	 I2C_CON_SPEED_M	(0x7 << I2C_CON_SPEED_S)
#define	 I2C_CON_10BITSLAVE	(1 << 3) /* Slave is 10 bit address */
#define	 I2C_CON_RESTART_EN	(1 << 5)
#define	 I2C_CON_SLAVE_DISABLE	(1 << 6)
#define	 I2C_CON_STOP_DET_IFAD	(1 << 7) /* Stop det if addressed */
#define	 I2C_CON_TX_EMPTY_CTRL	(1 << 8)
#define	K210_I2C_TAR		0x04	/* Target Address */
#define	K210_I2C_SAR		0x08	/* Slave Address */
#define	K210_I2C_RESV1		0x0c	/* reserved */
#define	K210_I2C_DATA_CMD	0x10	/* Data Buffer and Command */
#define	K210_I2C_DATA_CMD_CMD	(1 << 8) /* Data Buffer and Command */
#define	K210_I2C_SS_SCL_HCNT	0x14	/* Std Speed Clock SCL High Count */
#define	 I2C_SS_SCL_HCNT_S	0
#define	 I2C_SS_SCL_HCNT_M	(0xffff << I2C_SS_SCL_HCNT_S)
#define	K210_I2C_SS_SCL_LCNT	0x18	/* Std Speed Clock SCL Low Count */
#define	 I2C_SS_SCL_LCNT_S	0
#define	 I2C_SS_SCL_LCNT_M	(0xffff << I2C_SS_SCL_LCNT_S)
#define	K210_I2C_INTR_STAT	0x2c	/* Interrupt Status */
#define	K210_I2C_INTR_MASK	0x30	/* Interrupt Mask */
#define	 I2C_INTR_MASK_STOP_DET	(1 << 9)
#define	K210_I2C_RAW_INTR_STAT	0x34	/* Raw Interrupt Status */
#define	K210_I2C_RX_TL		0x38	/* Receive FIFO Threshold */
#define	K210_I2C_TX_TL		0x3c	/* Transmit FIFO Threshold */
#define	K210_I2C_CLR_INTR	0x40	/* Clear Combined and Individual Int.*/
#define	K210_I2C_CLR_RX_UNDER	0x44	/* Clear RX_UNDER Interrupt */
#define	K210_I2C_CLR_RX_OVER	0x48	/* Clear RX_OVER Interrupt */
#define	K210_I2C_CLR_TX_OVER	0x4c	/* Clear TX_OVER Interrupt */
#define	K210_I2C_CLR_RD_REQ	0x50	/* Clear RD_REQ Interrupt */
#define	K210_I2C_CLR_TX_ABRT	0x54	/* Clear TX_ABRT Interrupt */
#define	K210_I2C_CLR_RX_DONE	0x58	/* Clear RX_DONE Interrupt */
#define	K210_I2C_CLR_ACTIVITY	0x5c	/* Clear ACTIVITY Interrupt */
#define	K210_I2C_CLR_STOP_DET	0x60	/* Clear STOP_DET Interrupt */
#define	K210_I2C_CLR_START_DET	0x64	/* Clear START_DET Interrupt */
#define	K210_I2C_CLR_GEN_CALL	0x68	/* Clear GEN_CALL Interrupt */
#define	K210_I2C_ENABLE		0x6c	/* Enable */
#define	 I2C_ENABLE_ENABLE	(1 << 0)
#define	K210_I2C_STATUS		0x70	/* Status */
#define	 I2C_STATUS_ACTIVITY	(1 << 0)
#define	 I2C_STATUS_TFNF	(1 << 1)	/* Transmit FIFO not full */
#define	 I2C_STATUS_TFE		(1 << 2)	/* Transmit FIFO empty */
#define	 I2C_STATUS_RFNF	(1 << 3)	/* Receive FIFO not full */
#define	 I2C_STATUS_RFF		(1 << 4)	/* Receive FIFO full */
#define	 I2C_STATUS_MST_ACTIVITY (1 << 5)
#define	 I2C_STATUS_SLV_ACTIVITY (1 << 6)
#define	K210_I2C_TXFLR		0x74	/* Transmit FIFO Level */
#define	K210_I2C_RXFLR		0x78	/* Receive FIFO Level */
#define	K210_I2C_SDA_HOLD	0x7c	/* SDA Hold Time Length */
#define	K210_I2C_TX_ABRT_SOURCE	0x80	/* Transmit Abort Source */
#define	K210_I2C_RESV3		0x84	/* reserved */
#define	K210_I2C_DMA_CR		0x88	/* DMA Control */
#define	K210_I2C_DMA_TDLR	0x8c	/* DMA Transmit Data Level */
#define	K210_I2C_DMA_RDLR	0x90	/* DMA Receive Data Level */
#define	K210_I2C_SDA_SETUP	0x94	/* SDA Setup */
#define	K210_I2C_GENERAL_CALL	0x98	/* ACK General Call */
#define	K210_I2C_ENABLE_STATUS	0x9c	/* Enable Status */
#define	K210_I2C_FS_SPKLEN	0xa0	/* SS,FS or FM+ spike suppress limit */
#define	K210_I2C_COMP_PARAM_1	0xf4	/* Component Parameter 1 */
#define	K210_I2C_COMP_VERSION	0xf8	/* Component Version */
#define	K210_I2C_COMP_TYPE	0xfc	/* Component Type */

struct k210_i2c_softc {
	size_t base;
};

void k210_i2c_init(struct k210_i2c_softc *sc, uint32_t base);
void k210_i2c_configure_master(struct k210_i2c_softc *sc,
    uint32_t v_i2c_freq, uint32_t i2c_clk);
int k210_i2c_xfer(void *arg, struct i2c_msg *msgs, int len);

#endif /* !_RISCV_KENDRYTE_K210_I2C_H_ */
