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

#include <sys/cdefs.h>
#include <sys/systm.h>

#include <riscv/kendryte/k210_i2c.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

int
k210_i2c_xfer(void *arg, struct i2c_msg *msgs, int len)
{
	struct k210_i2c_softc *sc;
	struct i2c_msg *msg;
	uint32_t reg;
	int error;
	int i;
	int j;

	sc = arg;
	error = 0;

	critical_enter();

	for (i = 0; i < len; i++) {
		msg = &msgs[i];

		WR4(sc, K210_I2C_TAR, msg->slave);

		if (msg->flags & IIC_M_RD) {
			for (j = 0; j < msg->len; j++) {
				while (RD4(sc, K210_I2C_TXFLR) >= 8) ;
				WR4(sc, K210_I2C_DATA_CMD,
				    K210_I2C_DATA_CMD_CMD);

				while (RD4(sc, K210_I2C_RXFLR) == 0) ;
				msg->buf[j] = RD4(sc, K210_I2C_DATA_CMD);
			}
		} else {
			for (j = 0; j < msg->len; j++) {
				while (RD4(sc, K210_I2C_TXFLR) >= 8);
				WR4(sc, K210_I2C_DATA_CMD, msg->buf[j]);
			}
			reg = RD4(sc, K210_I2C_TX_ABRT_SOURCE);
			if (reg != 0) {
				error = -1;
				goto out;
			}

			/* Ensure all entries transmitted. */
			do {
				reg = RD4(sc, K210_I2C_STATUS);
			} while ((reg & I2C_STATUS_TFE) == 0);
		}

		if ((msg->flags & IIC_M_NOSTOP) == 0) {

			do {
				reg = RD4(sc, K210_I2C_STATUS);
			} while (reg & I2C_STATUS_ACTIVITY);


			/*
			 * TODO: not sure how to generate STOP condition.
			 * Do a 25ms timeout which helps.
			 */

			critical_exit();
			mdx_usleep(25000);
			critical_enter();
		}
	}

out:
	critical_exit();

	return (error);
}

void
k210_i2c_init(struct k210_i2c_softc *sc, uint32_t base)
{

	sc->base = base;
}

void
k210_i2c_configure_master(struct k210_i2c_softc *sc,
    uint32_t v_i2c_freq, uint32_t i2c_clk)
{
	uint32_t v_period_clk_cnt;
	uint32_t reg;

	v_period_clk_cnt = v_i2c_freq / i2c_clk / 2;
	if (v_period_clk_cnt == 0)
		v_period_clk_cnt = 1;

	WR4(sc, K210_I2C_ENABLE, 0);

	reg = I2C_CON_MASTER | I2C_CON_SLAVE_DISABLE | I2C_CON_RESTART_EN;
	reg |= (1 << I2C_CON_SPEED_S);	/* Normal speed */
	WR4(sc, K210_I2C_CON, reg);

	WR4(sc, K210_I2C_SS_SCL_HCNT, (v_period_clk_cnt << I2C_SS_SCL_HCNT_S));
	WR4(sc, K210_I2C_SS_SCL_LCNT, (v_period_clk_cnt << I2C_SS_SCL_LCNT_S));
	WR4(sc, K210_I2C_INTR_MASK, I2C_INTR_MASK_STOP_DET);
	WR4(sc, K210_I2C_RX_TL, 0);
	WR4(sc, K210_I2C_TX_TL, 0);
	WR4(sc, K210_I2C_DMA_CR, 3);
	WR4(sc, K210_I2C_DMA_RDLR, 0);
	WR4(sc, K210_I2C_DMA_TDLR, 4);

	WR4(sc, K210_I2C_ENABLE, I2C_ENABLE_ENABLE);
}
