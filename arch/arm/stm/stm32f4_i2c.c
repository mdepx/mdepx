/*-
 * Copyright (c) 2015, 2020 Ruslan Bukin <br@bsdpad.com>
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
 * ARE DISCLAIMED.  IN NO I2C SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#include <dev/i2c/i2c.h>

#include "stm32f4_i2c.h"
#include "stm32f4_i2cregs.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

#define dprintf(fmt, ...)

void
stm32f4_i2c_intr(void *arg, struct trapframe *tf, int irq)
{
	struct stm32f4_i2c_softc *sc;
	struct i2c_msg *msg;
	int pending;
	uint8_t val;
	int reg;

	sc = arg;

	msg = sc->curmsg;

	pending = RD4(sc, I2C_ISR);

	dprintf("i2c intr 0x%x\n", pending);

	if (pending & ISR_BUSY) {
		/* WR4(sc, I2C_ICR, ISR_BUSY); */
	}

	if (pending & ISR_ARLO) {
		WR4(sc, I2C_ICR, ISR_ARLO);
	}

	if (pending & ISR_NACKF) {
		WR4(sc, I2C_ICR, ISR_NACKF);
	}

	if (pending & ISR_TXIS) {
		val = *sc->curmsg->buf++;
		dprintf("write: 0x%08x\n", val);
		WR4(sc, I2C_TXDR, val);
	}

	if (pending & ISR_RXNE) {
		val = RD4(sc, I2C_RXDR);
		*msg->buf++ = val;
		dprintf("read: 0x%08x\n", val);
	}

	if (pending & ISR_TC) {
		reg = RD4(sc, I2C_CR2);
		reg |= (CR2_STOP);
		WR4(sc, I2C_CR2, reg);

		/* WR4(sc, I2C_ICR, ISR_TC); */
	}

	if (pending & ISR_STOPF) {
		WR4(sc, I2C_ICR, ISR_STOPF);

		mdx_sem_post(&sc->sem);
	}
}

static uint32_t
i2c_setup(struct stm32f4_i2c_softc *sc)
{
	int reg;

	/* TODO: set timings correctly. */

	reg =  0x01 << 28;
	reg |= 0x04 << 20;
	reg |= 0x02 << 16;
	reg |= 0x0f << 8;
	reg |= 0x13 << 0;

	reg = (30 << TIMINGR_SCLH_S);
	reg |= (30 << TIMINGR_SCLL_S);

	reg = 0x40912732;
	WR4(sc, I2C_TIMINGR, reg);

	/* WR4(sc, I2C_OAR1, 0x24); */

	reg = RD4(sc, I2C_CR1);
	reg |= (CR1_ERRIE);
	reg |= (CR1_TCIE);
	reg |= (CR1_STOPIE | CR1_NACKIE);
	reg |= (CR1_ADDRIE | CR1_TXIE | CR1_RXIE);
	reg |= (CR1_PE);
	WR4(sc, I2C_CR1, reg);

	return (0);
}

int
stm32f4_i2c_xfer(struct stm32f4_i2c_softc *sc,
    struct i2c_msg *msgs, uint32_t len)
{
	struct i2c_msg *msg;
	int reg;
	int i;

	dprintf("%s\n", __func__);

	i2c_setup(sc);

	for (i = 0; i < len; i++) {
		dprintf("buf 0x%08x\n", *msgs[i].buf);
		msg = &msgs[i];
		sc->curmsg = msg;

		reg = 0;
		reg |= (msg->slave << CR2_SADD_S);
		reg |= (msg->len << CR2_NBYTES_S);

		if (msg->flags & IIC_M_RD)
			reg |= (CR2_RD_WRN); /* Read op */

		/* reg |= (CR2_AUTOEND); */

		mdx_sem_init(&sc->sem, 0);

		reg |= (CR2_START);
		WR4(sc, I2C_CR2, reg);

		mdx_sem_wait(&sc->sem);
	};

	return (0);
}

#if 0
/* Example usage. */

static void
i2c_test(struct stm32f4_i2c_softc *sc)
{
	uint8_t data[2];
	uint16_t reg;
	uint16_t res;

	reg = 0x0000;

	struct i2c_msg msgs[] = {
		{ 0x34, IIC_M_WR, 2, (uint8_t *)&reg },
		{ 0x34, IIC_M_RD, 2, data },
	};

	i2c_xfer(msgs, 2);

	res = (data[1] | (data[0] << 8));

	printf("Read data is 0x%08x\n", res);
}
#endif

void
stm32f4_i2c_init(struct stm32f4_i2c_softc *sc, uint32_t base)
{

	sc->base = base;
}
