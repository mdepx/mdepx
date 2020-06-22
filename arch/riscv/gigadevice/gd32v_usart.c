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

#include <dev/uart/uart.h>

#include <riscv/gigadevice/gd32v_usart.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
gd32v_usart_putc(mdx_device_t dev, int c)
{
	struct gd32v_usart_softc *sc;

	sc = mdx_device_get_softc(dev);

	while ((RD4(sc, USART_STAT) & USART_STAT_TBE) == 0)
		;

	WR4(sc, USART_DATA, c);
}

static int
gd32v_usart_getc(mdx_device_t dev)
{
	struct gd32v_usart_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	while ((RD4(sc, USART_STAT) & USART_STAT_RBNE) == 0)
		;

	reg = RD4(sc, USART_DATA);

	return (reg);
}

static bool
gd32v_usart_rxready(mdx_device_t dev)
{
	struct gd32v_usart_softc *sc;

	sc = mdx_device_get_softc(dev);

	if (RD4(sc, USART_STAT) & USART_STAT_RBNE)
		return (true);

	return (false);
}

static void
gd32v_usart_setup(mdx_device_t dev, int baud_rate,
    uart_databits_t databits,
    uart_stopbits_t stopbits,
    uart_parity_t parity)
{
	struct gd32v_usart_softc *sc;
	uint32_t reg;

	sc = mdx_device_get_softc(dev);

	WR4(sc, USART_CTL0, 0);
	WR4(sc, USART_CTL1, 0);
	WR4(sc, USART_CTL2, 0);

	reg = (sc->cpu_freq / baud_rate);
	WR4(sc, USART_BAUD, reg);

	reg = USART_CTL0_TEN | USART_CTL0_REN;
	WR4(sc, USART_CTL0, reg);
	WR4(sc, USART_CTL0, reg | USART_CTL0_UEN);
}

static struct mdx_uart_ops gd32v_usart_ops = {
	.putc = gd32v_usart_putc,
	.getc = gd32v_usart_getc,
	.rxready = gd32v_usart_rxready,
	.setup = gd32v_usart_setup,
};

int
gd32v_usart_init(mdx_device_t dev, uint32_t base,
    uint32_t cpu_freq)
{
	struct gd32v_usart_softc *sc;

	sc = mdx_device_alloc_softc(dev, sizeof(struct gd32v_usart_softc));
	sc->base = base;
	sc->cpu_freq = cpu_freq;

	dev->ops = &gd32v_usart_ops;

	return (0);
}
