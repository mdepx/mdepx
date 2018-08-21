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
 * ARE DISCLAIMED.  IN NO SPI SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#include <net/ethernet.h>

#include "stm32f7_eth.h"

#define	RD4(_sc, _reg)		*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static void
dwc_get_hwaddr(struct stm32f7_eth_softc *sc, uint8_t *hwaddr)
{
	uint32_t hi, lo;

	lo = RD4(sc, ETH_MACALR(0));
	hi = RD4(sc, ETH_MACAHR(0)) & 0xffff;

	if ((lo != 0xffffffff) || (hi != 0xffff)) {
		hwaddr[0] = (lo >>  0) & 0xff;
		hwaddr[1] = (lo >>  8) & 0xff;
		hwaddr[2] = (lo >> 16) & 0xff;
		hwaddr[3] = (lo >> 24) & 0xff;
		hwaddr[4] = (hi >>  0) & 0xff;
		hwaddr[5] = (hi >>  8) & 0xff;
	} else {
		hwaddr[0] = 'o';
		hwaddr[1] = 's';
		hwaddr[2] = 'f';
		hwaddr[3] = 'i';
		hwaddr[4] = 'v';
		hwaddr[5] = 'e';
	}
}

static void
dwc_set_hwaddr(struct stm32f7_eth_softc *sc, uint8_t *hwaddr)
{
	uint32_t hi, lo;

	lo = hwaddr[0] | (hwaddr[1] << 8) | (hwaddr[2] << 16) |
	    (hwaddr[3] << 24);
	hi = hwaddr[4] | (hwaddr[5] << 8);

	WR4(sc, ETH_MACALR(0), lo);
	WR4(sc, ETH_MACAHR(0), hi);
	WR4(sc, ETH_MACFFR, (MACFFR_PM | MACFFR_RA));
}

void
stm32f7_eth_init(struct stm32f7_eth_softc *sc, uint32_t base)
{
	uint8_t hwaddr[ETHER_ADDR_LEN];

	sc->base = base;

	dwc_get_hwaddr(sc, hwaddr);
	dwc_set_hwaddr(sc, hwaddr);
}
