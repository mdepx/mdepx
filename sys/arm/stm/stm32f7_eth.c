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
#include <sys/systm.h>
#include <sys/mbuf.h>
#include <net/ethernet.h>

#include <machine/frame.h>
#include <dev/mii/mii.h>

#include "stm32f7_eth.h"

#define	RD4(_sc, _reg)		*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	*(volatile uint32_t *)((_sc)->base + _reg) = _val

#define	RX_DESC_SIZE	(sizeof(struct dwc_hwdesc) * RX_DESC_COUNT)
#define	TX_DESC_SIZE	(sizeof(struct dwc_hwdesc) * TX_DESC_COUNT)

struct dwc_hwdesc	txdesc_ring[TX_DESC_COUNT] __aligned(32);
struct dwc_hwdesc	rxdesc_ring[RX_DESC_COUNT] __aligned(32);

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
stm32f7_eth_intr(void *arg, struct trapframe *tf, int irq)
{

	printf("%s\n", __func__);
}

void
stm32f7_eth_wkup_intr(void *arg, struct trapframe *tf, int irq)
{

	printf("%s\n", __func__);
}

static int
dwc_miibus_read_reg(void *arg, uint32_t phy, uint32_t reg)
{
	struct stm32f7_eth_softc *sc;
	uint16_t timeout;
	uint16_t mii;
	uint32_t rv;

	sc = arg;

	mii = ((phy << MACMIIAR_PA_S)
	    | (reg << MACMIIAR_MR_S)
	    | (sc->mii_clk << MACMIIAR_CR_S)
	    | MACMIIAR_MB); /* Busy flag */
	WR4(sc, ETH_MACMIIAR, mii);

	timeout = 1000;
	rv = 0;
	do {
		if ((RD4(sc, ETH_MACMIIAR) & MACMIIAR_MB) == 0) {
			rv = RD4(sc, ETH_MACMIIDR);
			break;
		}
		udelay(10000);
	} while (--timeout);

	if (timeout == 0) {
		printf("%s: failed to read MII reg\n");
		return (-1);
	}

	return (rv);
}

static int
dwc_miibus_write_reg(void *arg, uint32_t phy, uint32_t reg, uint32_t val)
{
	struct stm32f7_eth_softc *sc;
	uint16_t timeout;
	uint16_t mii;

	sc = arg;

	mii = ((phy << MACMIIAR_PA_S)
	    | (reg << MACMIIAR_MR_S)
	    | (sc->mii_clk << MACMIIAR_CR_S)
	    | MACMIIAR_MB | MACMIIAR_MW);

	WR4(sc, ETH_MACMIIDR, val);
	WR4(sc, ETH_MACMIIAR, mii);

	timeout = 1000;
	do {
		if ((RD4(sc, ETH_MACMIIAR) & MACMIIAR_MB) == 0)
			break;
		udelay(10000);
	} while (--timeout);

	if (timeout == 0) {
		printf("%s: failed to write MII reg\n");
		return (-1);
	}

	return (0);
}

static int
smsc_lan8742a_init(struct stm32f7_eth_softc *sc)
{
	uint32_t phy_addr;
	uint32_t reg;
	uint16_t timeout;

	phy_addr = 0;

	reg = dwc_miibus_read_reg(sc, phy_addr, MII_BMCR);
	printf("%s: MII_BMCR 0x%08x\n", __func__, reg);

	reg = dwc_miibus_read_reg(sc, phy_addr, MII_BMSR);
	printf("%s: MII_BMSR 0x%08x\n", __func__, reg);

	/* Reset PHY */
	timeout = 1000;
	dwc_miibus_write_reg(sc, phy_addr, MII_BMCR, BMCR_RESET);
	do {
		reg = dwc_miibus_read_reg(sc, phy_addr, MII_BMCR);
		if ((reg & BMCR_RESET) == 0)
			break;
		udelay(10000);
	} while (--timeout);

	if (timeout == 0) {
		printf("%s: failed to reset PHY\n");
		return (-1);
	}

	reg = dwc_miibus_read_reg(sc, phy_addr, MII_BMSR);
	if (reg & BMSR_LINK)
		printf("%s: link found reg 0x%08x\n", __func__, reg);
	else
		printf("%s: link not found reg 0x%08x\n", __func__, reg);

	/* Force auto-negotiation */
	dwc_miibus_write_reg(sc, phy_addr, MII_BMCR, BMCR_AUTOEN);

	timeout = 50;
	do {
		reg = dwc_miibus_read_reg(sc, phy_addr, MII_BMSR);
		if (reg & BMSR_ACOMP)
			break;
		udelay(50000);
	} while (--timeout);
	if (timeout == 0) {
		printf("%s: auto-negotiation failed\n", __func__);
		return (-1);
	}

	printf("%s: auto-negotiation succeded (timeout %d)\n",
	    __func__, timeout);

	reg = dwc_miibus_read_reg(sc, phy_addr, 0x1f);
	printf("%s: status register: 0x%08x\n", __func__, reg);

	switch(reg & 0x001c) {
	case 0x04:
		printf("10 Half Duplex\n");
		break;
	case 0x08:
		printf("100 Half Duplex\n");
		break;
	case 0x14:
		printf("10 Full Duplex\n");
		break;
	case 0x18:
		printf("100 Full Duplex\n");
		break;
	default:
		printf("Unknown speed\n");
	}

	return (0);
}

static inline uint32_t
next_txidx(struct stm32f7_eth_softc *sc, uint32_t curidx)
{

	return ((curidx + 1) % TX_DESC_COUNT);
}

static inline uint32_t
next_rxidx(struct stm32f7_eth_softc *sc, uint32_t curidx)
{

	return ((curidx + 1) % RX_DESC_COUNT);
}

static void
setup_txdesc(struct stm32f7_eth_softc *sc)
{
	uint16_t nidx;
	uint16_t idx;

	sc->tx_idx_head = 0;
	sc->tx_idx_tail = 0;

	for (idx = 0; idx < TX_DESC_COUNT; idx++) {
		sc->txdesc_ring[idx].tdes0 = DDESC_TDES0_TXCHAIN;
		sc->txdesc_ring[idx].tdes1 = 0;
		nidx = next_txidx(sc, idx);
		sc->txdesc_ring[idx].addr_next = (uint32_t)(sc->txdesc_ring) +
		    (nidx * sizeof(struct dwc_hwdesc));
	}
}

static void
setup_rxdesc0(struct stm32f7_eth_softc *sc, int idx)
{
	struct mbuf *m;
	uint16_t nidx;
	uint16_t len;

	len = 2050;

	m = m_alloc(len);
	sc->rxbuf[idx] = m;
	sc->rxdesc_ring[idx].addr = (uint32_t)m->m_data;
	nidx = next_rxidx(sc, idx);
	sc->rxdesc_ring[idx].addr_next = (uint32_t)sc->rxdesc_ring +
	    (nidx * sizeof(struct dwc_hwdesc));
	sc->rxdesc_ring[idx].tdes1 = (DDESC_RDES1_CHAINED | len);
	sc->rxdesc_ring[idx].tdes0 = (DDESC_RDES0_OWN);
}

static void
setup_rxdesc(struct stm32f7_eth_softc *sc)
{
	uint16_t i;

	for (i = 0; i < RX_DESC_COUNT; i++)
		setup_rxdesc0(sc, i);
}

int
stm32f7_eth_setup(struct stm32f7_eth_softc *sc,
    uint8_t *new_hwaddr)
{
	uint32_t reg;
	uint16_t timeout;

	/* Unreset DMA bus */
	reg = RD4(sc, ETH_DMABMR);
	reg &= ~(DMABMR_SR);
	WR4(sc, ETH_DMABMR, reg);

	udelay(10000);

	/* Reset DMA bus */
	reg = RD4(sc, ETH_DMABMR);
	if (reg & DMABMR_SR) {
		printf("%s: Device is in reset\n", __func__);
		return (-1);
	}
	reg |= (DMABMR_SR);
	WR4(sc, ETH_DMABMR, reg);

	timeout = 100;
	do {
		if ((RD4(sc, ETH_DMABMR) & DMABMR_SR) == 0)
			break;
		udelay(10000);
	} while (timeout--);

	if (timeout == 0) {
		printf("%s: Can't reset DMA bus\n", __func__);
		return (-1);
	}

	if (new_hwaddr != NULL)
		dwc_set_hwaddr(sc, new_hwaddr);
	else
		dwc_set_hwaddr(sc, sc->hwaddr);

	reg = RD4(sc, ETH_DMABMR);
	reg |= (32 << DMABMR_PBL_S);
	WR4(sc, ETH_DMABMR, reg);

	/* DMA must be halted while changing descriptor list addresses. */
	reg = RD4(sc, ETH_DMAOMR);
	reg &= ~(DMAOMR_ST | DMAOMR_SR);
	WR4(sc, ETH_DMAOMR, reg);

	if (smsc_lan8742a_init(sc) != 0)
		return (-1);

	/* Setup descriptors */

	/* Stop DMA */
	reg = RD4(sc, ETH_DMAOMR);
	reg &= ~(DMAOMR_ST | DMAOMR_SR);
	WR4(sc, ETH_DMAOMR, reg);

	sc->txdesc_ring = &txdesc_ring[0];
	sc->rxdesc_ring = &rxdesc_ring[0];

	setup_txdesc(sc);
	setup_rxdesc(sc);

	WR4(sc, ETH_DMATDLAR, (uint32_t)sc->txdesc_ring);
	WR4(sc, ETH_DMARDLAR, (uint32_t)sc->rxdesc_ring);

	return (0);
}

void
stm32f7_eth_init(struct stm32f7_eth_softc *sc, uint32_t base)
{

	sc->base = base;
	sc->mii_clk = 4;

	dwc_get_hwaddr(sc, sc->hwaddr);
}
