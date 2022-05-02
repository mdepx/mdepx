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

#include <dev/i2c/i2c.h>

#include <riscv/kendryte/k210_i2c.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static int
k210_i2c_xfer(mdx_device_t dev, struct i2c_msg *msgs, int len)
{
	struct k210_i2c_softc *sc;
	struct i2c_msg *msg;
	uint32_t reg;
	int timeout;
	int error;
	int cmd;
	int i;
	int j;

	sc = mdx_device_get_softc(dev);
	error = 0;

	critical_enter();

	for (i = 0; i < len; i++) {
		msg = &msgs[i];

		WR4(sc, K210_I2C_TAR, msg->slave);

		cmd = 0;

		if (msg->flags & IIC_M_RD) {
			for (j = 0; j < msg->len; j++) {
				if (((msg->flags & IIC_M_NOSTOP) == 0) &&
				    (j == msg->len - 1))
					cmd |= (1 << 9);	/* Stop bit */

				WR4(sc, K210_I2C_DATA_CMD,
				    K210_I2C_DATA_CMD_CMD | cmd);

				timeout = 10000;
				do {
					reg = RD4(sc, K210_I2C_RXFLR);
					if (reg != 0)
						break;
				} while (timeout--);
				if (timeout < 0) {
					error = -2;
					goto out;
				}

				msg->buf[j] = RD4(sc, K210_I2C_DATA_CMD);
			}
		} else {
			for (j = 0; j < msg->len; j++) {
				timeout = 10000;
				do {
					reg = RD4(sc, K210_I2C_TXFLR);
					if (reg < 8)
						break;
				} while (timeout--);
				if (timeout < 0) {
					error = -3;
					goto out;
				}
				if (((msg->flags & IIC_M_NOSTOP) == 0) &&
				    (j == msg->len - 1))
					cmd |= (1 << 9);	/* Stop bit */
				WR4(sc, K210_I2C_DATA_CMD, cmd | msg->buf[j]);
			}
			reg = RD4(sc, K210_I2C_TX_ABRT_SOURCE);
			if (reg != 0) {
				error = -1;
				goto out;
			}

			/* Ensure all entries transmitted. */
			timeout = 10000;
			do {
				reg = RD4(sc, K210_I2C_STATUS);
				if (reg & I2C_STATUS_TFE)
					break;
			} while (timeout--);
			if (timeout < 0) {
				error = -4;
				goto out;
			}
		}
	}

out:
	critical_exit();

	return (error);
}

static struct mdx_i2c_ops k210_i2c_ops = {
	.xfer = k210_i2c_xfer,
};

void
k210_i2c_init(mdx_device_t dev, uint32_t base)
{
	struct k210_i2c_softc *sc;

	sc = mdx_device_get_softc(dev);
	sc->base = base;

	dev->ops = &k210_i2c_ops;
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
