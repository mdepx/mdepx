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
#include <sys/of.h>

#include <dev/i2c/i2c.h>

#include "nrf_twim.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

#define	TWIM_TIMEOUT	10000	/* usec */

#ifdef MDX_OF
static struct mdx_compat_data nrf_twim_compat_data[] = {
	{ "nordic,nrf-twim" },
	{ NULL },
};
#endif

void
nrf_twim_intr(void *arg, int irq)
{
	struct nrf_twim_softc *sc;

	sc = arg;

	if (RD4(sc, TWIM_EVENTS_LASTTX)) {
		WR4(sc, TWIM_EVENTS_LASTTX, 0);
		mdx_sem_post(&sc->sem_tx);
	}

	if (RD4(sc, TWIM_EVENTS_LASTRX)) {
		WR4(sc, TWIM_EVENTS_LASTRX, 0);
		mdx_sem_post(&sc->sem_rx);
	}

	if (RD4(sc, TWIM_EVENTS_STOPPED)) {
		WR4(sc, TWIM_EVENTS_STOPPED, 0);
		mdx_sem_post(&sc->sem_stop);
	}
}

static int
nrf_twim_xfer(mdx_device_t dev, struct i2c_msg *msgs, int len)
{
	struct nrf_twim_softc *sc;
	struct i2c_msg *msg;
	int err;
	int i;

	sc = mdx_device_get_softc(dev);

	/* TODO: handle errors. */

	for (i = 0; i < len; i++) {
		msg = &msgs[i];

		WR4(sc, TWIM_ADDRESS, msg->slave);

		if (msg->flags & IIC_M_RD) {
			WR4(sc, TWIM_RXD_MAXCNT, msg->len);
			WR4(sc, TWIM_RXD_PTR, (uint32_t)msg->buf);

			mdx_sem_init(&sc->sem_rx, 0);
			WR4(sc, TWIM_TASKS_STARTRX, 1);
			err = mdx_sem_timedwait(&sc->sem_rx, TWIM_TIMEOUT);
			if (err == 0)
				return (-1);

		} else {
			WR4(sc, TWIM_TXD_MAXCNT, msg->len);
			WR4(sc, TWIM_TXD_PTR, (uint32_t)msg->buf);

			mdx_sem_init(&sc->sem_tx, 0);
			WR4(sc, TWIM_TASKS_STARTTX, 1);
			err = mdx_sem_timedwait(&sc->sem_tx, TWIM_TIMEOUT);
			if (err == 0)
				return (-1);
		}

		if ((msg->flags & IIC_M_NOSTOP) == 0) {
			mdx_sem_init(&sc->sem_stop, 0);
			WR4(sc, TWIM_TASKS_STOP, 1);
			err = mdx_sem_timedwait(&sc->sem_stop, TWIM_TIMEOUT);
			if (err == 0)
				return (-1);
		}
	}

	return (0);
}

void
nrf_twim_setup(mdx_device_t dev, struct nrf_twim_conf *conf)
{
	struct nrf_twim_softc *sc;

	sc = mdx_device_get_softc(dev);

	WR4(sc, TWIM_FREQUENCY, conf->freq);
	WR4(sc, TWIM_PSEL_SCL, conf->pin_scl);
	WR4(sc, TWIM_PSEL_SDA, conf->pin_sda);
	WR4(sc, TWIM_INTEN, INTEN_LASTTX | INTEN_LASTRX | INTEN_STOPPED);
	WR4(sc, TWIM_ENABLE, TWIM_ENABLE_EN);
}

static struct mdx_i2c_ops nrf_twim_i2c_ops = {
	.xfer = nrf_twim_xfer,
};

void
nrf_twim_init(mdx_device_t dev, uint32_t base)
{
	struct nrf_twim_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(*sc));
	sc->base = base;

	dev->ops = &nrf_twim_i2c_ops;
}

#ifdef MDX_OF
static int
nrf_twim_probe(mdx_device_t dev)
{

	if (!mdx_of_is_compatible(dev, "nordic,nrf-twim"))
		return (MDX_ERROR);

	return (MDX_OK);
}

static int
nrf_twim_attach(mdx_device_t dev)
{
	struct nrf_twim_softc *sc;
	uint32_t sda, scl, clk;
	int error;

	sc = mdx_device_get_softc(dev);

	error = mdx_of_get_reg(dev, 0, &sc->base, NULL);
	if (error)
		return (error);

	error = mdx_of_dev_get_prop32(dev, "sda-pin", &sda, NULL);
	if (error)
		return (error);

	error = mdx_of_dev_get_prop32(dev, "scl-pin", &scl, NULL);
	if (error)
		return (error);

	error = mdx_of_dev_get_prop32(dev, "clock-frequency", &clk, NULL);
	if (error)
		return (error);

	mdx_of_setup_intr(dev, 0, nrf_twim_intr, sc);

	switch (clk) {
	case 100000:
		WR4(sc, TWIM_FREQUENCY, TWIM_FREQ_K100);
		break;
	default:
		panic("unknown clock frequency");
	};

	WR4(sc, TWIM_PSEL_SCL, scl);
	WR4(sc, TWIM_PSEL_SDA, sda);
	WR4(sc, TWIM_INTEN, INTEN_LASTTX | INTEN_LASTRX | INTEN_STOPPED);
	WR4(sc, TWIM_ENABLE, TWIM_ENABLE_EN);

	dev->ops = &nrf_twim_i2c_ops;

	return (0);
}

static struct mdx_device_ops nrf_twim_ops = {
	.probe = nrf_twim_probe,
	.attach = nrf_twim_attach,
};

static mdx_driver_t nrf_twim_driver = {
	"nrf_twim",
	&nrf_twim_ops,
	sizeof(struct nrf_twim_softc),
	nrf_twim_compat_data,
};

DRIVER_MODULE(nrf_twim, nrf_twim_driver);
#endif
