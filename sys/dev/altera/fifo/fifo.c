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

#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/endian.h>

#include <machine/cpuregs.h>
#include <machine/cpufunc.h>
#include <machine/frame.h>
#include <machine/cache_mipsNN.h>
#include <machine/cache_r4k.h>

#include "fifo.h"

#define	ALTERA_FIFO_DEBUG
#undef	ALTERA_FIFO_DEBUG

#ifdef	ALTERA_FIFO_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	CACHE_FIFO_MEM_INV(_sc, _reg)		\
	mipsNN_pdcache_inv_range_128(((_sc)->fifo_base_mem + _reg), 4);

uint32_t
fifo_fill_level(struct altera_fifo_softc *sc)
{
	uint32_t reg;

	reg = RD4_FIFO_MEMC(sc,
	    A_ONCHIP_FIFO_MEM_CORE_STATUS_REG_FILL_LEVEL);

	return (le32toh(reg));
}

uint32_t
fifo_fill_level_wait(struct altera_fifo_softc *sc)
{
	uint32_t val;

	do {
		val = fifo_fill_level(sc);
	} while (val == AVALON_FIFO_TX_BASIC_OPTS_DEPTH);

	return (val);
}

int
fifo_process_rx_one(struct altera_fifo_softc *sc,
    uint64_t read_lo, uint64_t write_lo, uint32_t len)
{
	uint32_t transferred;
	uint32_t fill_level;
	uint32_t data;
	uint32_t meta;
	int timeout;
	int error;
	int sop_rcvd;
	int eop_rcvd;
	int empty;

	fill_level = fifo_fill_level(sc);
	if (fill_level == 0)
		return (0);

	dprintf("%s(%d): fill_level %d\n", __func__, sc->unit, fill_level);
	dprintf("%s: copy %x -> %x, %d bytes\n", __func__, read_lo, write_lo, len);

	write_lo |= MIPS_XKPHYS_UNCACHED_BASE;
	error = 0;
	sop_rcvd = 0;
	eop_rcvd = 0;
	empty = 0;
	transferred = 0;
	while (fill_level) {
		data = RD4_FIFO_MEM_CACHED(sc, A_ONCHIP_FIFO_MEM_CORE_DATA);
		CACHE_FIFO_MEM_INV(sc, A_ONCHIP_FIFO_MEM_CORE_DATA);

		meta = RD4_FIFO_MEM(sc, A_ONCHIP_FIFO_MEM_CORE_METADATA);
		meta = le32toh(meta);
		if ((meta & A_ONCHIP_FIFO_MEM_CORE_ERROR_MASK) ||
		    (meta & A_ONCHIP_FIFO_MEM_CORE_CHANNEL_MASK) != 0) {
			printf("error\n");
			error = -1;
			break;
		}

		/* Check for start of the packet */
		if (meta & A_ONCHIP_FIFO_MEM_CORE_SOP)
			sop_rcvd = 1;
		if (sop_rcvd == 0) {
			printf("%s: sop not received\n", __func__);
			error = -2;
			break;
		}

		empty = 0;
		if (meta & A_ONCHIP_FIFO_MEM_CORE_EOP) {
			eop_rcvd = 1;
			empty = (meta & A_ONCHIP_FIFO_MEM_CORE_EMPTY_MASK) >>
			    A_ONCHIP_FIFO_MEM_CORE_EMPTY_SHIFT;
		}

		if (empty == 0) {
			*(uint32_t *)(write_lo) = data;
			write_lo += 4;
			transferred += 4;
		} else if (empty == 1) {
			*(uint16_t *)(write_lo) = ((data >> 16) & 0xffff);
			write_lo += 2;
			transferred += 2;

			*(uint8_t *)(write_lo) = ((data >> 8) & 0xff);
			write_lo += 1;
			transferred += 1;
		} else {
			panic("implement me %d\n", empty);
		}

		if (meta & A_ONCHIP_FIFO_MEM_CORE_EOP)
			break;

		timeout = 100;
		do {
			fill_level = fifo_fill_level(sc);
		} while (fill_level == 0 && timeout--);

		if (timeout <= 0) {
			printf("%s: eop not received\n", __func__);
			/* No EOP received. Broken packet. */
			error = -3;
			break;
		}
	}

	if (error != 0)
		return (error);

	dprintf("%s: packet received, %d bytes (sop_rcvd %d eop_rcvd %d fill_level %d)\n",
	    __func__, transferred, sop_rcvd, eop_rcvd, fill_level);

	return (transferred);
}

int
fifo_process_tx_one(struct altera_fifo_softc *sc,
    int sop, int eop, uint64_t read_lo,
    uint64_t write_lo, uint32_t len)
{
	uint32_t transferred;
	uint32_t reg;
	uint32_t val;

	dprintf("%s: copy %x -> %x, %d bytes\n", __func__, read_lo, write_lo, len);
	read_lo |= MIPS_XKPHYS_UNCACHED_BASE;

	uint32_t fill_level;
	uint32_t c;
	uint32_t leftm;
	uint32_t tmp;

	transferred = 0;

	if (write_lo != 0)
		panic("error\n");

	fill_level = fifo_fill_level_wait(sc);

	/* Mem to dev */
	if (sop) {
		reg = A_ONCHIP_FIFO_MEM_CORE_SOP;
		WR4_FIFO_MEM(sc, A_ONCHIP_FIFO_MEM_CORE_METADATA, htole32(reg));
	}

	c = 0;
	while ((len - c) >= 4) {
		val = *(uint32_t *)(read_lo);
		WR4_FIFO_MEM(sc, A_ONCHIP_FIFO_MEM_CORE_DATA, val);

		fill_level += 1;
		if (fill_level == AVALON_FIFO_TX_BASIC_OPTS_DEPTH)
			fill_level = fifo_fill_level_wait(sc);

		read_lo += 4;
		transferred += 4;
		c += 4;
	}

	val = 0;
	leftm = (len - c);

	switch (leftm) {
	case 1:
		val = *(uint8_t *)(read_lo);
		val <<= 24;
		read_lo += 1;
		transferred += 1;
		break;
	case 2:
	case 3:
		val = *(uint16_t *)(read_lo);
		val <<= 16;
		read_lo += 2;
		transferred += 2;
		if (leftm == 3) {
			tmp = *(uint8_t *)(read_lo);
			val |= (tmp << 8);
			read_lo += 1;
			transferred += 1;
		}
	}

	/* Set end of packet. */
	reg = 0;
	if (eop)
		reg |= A_ONCHIP_FIFO_MEM_CORE_EOP;
	reg |= ((4 - leftm) << A_ONCHIP_FIFO_MEM_CORE_EMPTY_SHIFT);
	WR4_FIFO_MEM(sc, A_ONCHIP_FIFO_MEM_CORE_METADATA, htole32(reg));

	/* Ensure there is a FIFO entry available. */
	fill_level = fifo_fill_level_wait(sc);

	/* Final write */
	WR4_FIFO_MEM(sc, A_ONCHIP_FIFO_MEM_CORE_DATA, val);

	return (transferred);
}
