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
#include <sys/uio.h>

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

void
fifo_interrupts_disable(struct altera_fifo_softc *sc)
{

	WR4_FIFO_MEMC(sc, A_ONCHIP_FIFO_MEM_CORE_STATUS_REG_INT_ENABLE, 0);
}

void
fifo_interrupts_enable(struct altera_fifo_softc *sc, int mask)
{

	WR4_FIFO_MEMC(sc, A_ONCHIP_FIFO_MEM_CORE_STATUS_REG_INT_ENABLE,
	    htole32(mask));
}

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

void
altera_fifo_intr(void *arg)
{
	struct altera_fifo_softc *sc;
	uint32_t reg0;
	uint32_t reg;
	uint32_t err;

	sc = arg;

	reg0 = RD4_FIFO_MEMC(sc, A_ONCHIP_FIFO_MEM_CORE_STATUS_REG_EVENT);
	reg = le32toh(reg0);

	dprintf("%s: reg %x\n", __func__, reg);

	if (reg & (A_ONCHIP_FIFO_MEM_CORE_EVENT_OVERFLOW |
	    A_ONCHIP_FIFO_MEM_CORE_EVENT_UNDERFLOW)) {
		/* Errors */
		err = (((reg & A_ONCHIP_FIFO_MEM_CORE_ERROR_MASK) >> \
		    A_ONCHIP_FIFO_MEM_CORE_ERROR_SHIFT) & 0xff);
		dprintf("%s: reg %x err %x\n", __func__, reg, err);
	}

	if (sc->cb != NULL)
		sc->cb(sc->cb_arg);

	if (reg0 != 0)
		WR4_FIFO_MEMC(sc,
		    A_ONCHIP_FIFO_MEM_CORE_STATUS_REG_EVENT, reg0);
}

int
fifo_process_tx(struct altera_fifo_softc *sc,
    struct iovec *iov, int iovcnt)
{
	uint32_t fill_level;
	uint64_t read_lo;
	uint64_t read_buf;
	uint32_t missing;
	uint32_t word;
	uint32_t len;
	int got_bits;
	int reg;
	int i;

	/* Set start of packet */
	WR4_FIFO_MEM(sc, A_ONCHIP_FIFO_MEM_CORE_METADATA,
	    htole32(A_ONCHIP_FIFO_MEM_CORE_SOP));

	read_buf = 0;
	got_bits = 0;

	for (i = 0; i < iovcnt; i++) {
		read_lo = (uint64_t)iov[i].iov_base;
		len = iov[i].iov_len;

		dprintf("%s: copy %lx -> 0, %d bytes\n",
		    __func__, read_lo, len);

		if (read_lo & 1) {
			read_buf = (read_buf << 8) | *(uint8_t *)read_lo;
			got_bits += 8;
			read_lo += 1;
			len -= 1;
		}

		if (len >= 2 && read_lo & 2) {
			read_buf = (read_buf << 16) | *(uint16_t *)read_lo;
			got_bits += 16;
			read_lo += 2;
			len -= 2;
		}

		if (got_bits >= 32) {
			got_bits -= 32;
			word = (uint32_t)((read_buf >> got_bits) & 0xffffffff);
			fill_level = fifo_fill_level_wait(sc);
			if (len == 0 && (i == (iovcnt - 1)) && (got_bits == 0))
				WR4_FIFO_MEM(sc,
				    A_ONCHIP_FIFO_MEM_CORE_METADATA,
				    htole32(A_ONCHIP_FIFO_MEM_CORE_EOP));
			WR4_FIFO_MEM(sc, A_ONCHIP_FIFO_MEM_CORE_DATA, word);
		}

		while (len >= 4) {
			read_buf = (read_buf << 32) |
			    (uint64_t)*(uint32_t *)read_lo;
			read_lo += 4;
			len -= 4;
			word = (uint32_t)((read_buf >> got_bits) & 0xffffffff);
			fill_level = fifo_fill_level_wait(sc);
			if (len == 0 && (i == (iovcnt - 1)) && (got_bits == 0))
				WR4_FIFO_MEM(sc,
				    A_ONCHIP_FIFO_MEM_CORE_METADATA,
				    htole32(A_ONCHIP_FIFO_MEM_CORE_EOP));
			WR4_FIFO_MEM(sc, A_ONCHIP_FIFO_MEM_CORE_DATA, word);
		}

		if (len & 2) {
			read_buf = (read_buf << 16) | *(uint16_t *)read_lo;
			got_bits += 16;
			read_lo += 2;
			len -= 2;
		}

		if (len & 1) {
			read_buf = (read_buf << 8) | *(uint8_t *)read_lo;
			got_bits += 8;
			read_lo += 1;
			len -= 1;
		}

		if (got_bits >= 32) {
			got_bits -= 32;
			word = (uint32_t)((read_buf >> got_bits) & 0xffffffff);
			fill_level = fifo_fill_level_wait(sc);
			if (len == 0 && (i == (iovcnt - 1)) && (got_bits == 0))
				WR4_FIFO_MEM(sc,
				    A_ONCHIP_FIFO_MEM_CORE_METADATA,
				    htole32(A_ONCHIP_FIFO_MEM_CORE_EOP));
			WR4_FIFO_MEM(sc, A_ONCHIP_FIFO_MEM_CORE_DATA, word);
		}
	}

	if (got_bits) {
		missing = 32 - got_bits;
		got_bits /= 8;
		fill_level = fifo_fill_level_wait(sc);
		
		reg = A_ONCHIP_FIFO_MEM_CORE_EOP |
		    ((4 - got_bits) << A_ONCHIP_FIFO_MEM_CORE_EMPTY_SHIFT);
		WR4_FIFO_MEM(sc, A_ONCHIP_FIFO_MEM_CORE_METADATA,
		    htole32(reg));

		word = (uint32_t)((read_buf << missing) & 0xFFFFFFFF);
		WR4_FIFO_MEM(sc, A_ONCHIP_FIFO_MEM_CORE_DATA, word);
	}

	return (1);
}

int
fifo_process_rx(struct altera_fifo_softc *sc,
    struct iovec *iov, int iovcnt, int strip_len)
{
	uint64_t write_lo;
	uint64_t read_buf;
	uint32_t word;
	uint32_t data;
	uint32_t transferred;
	uint32_t fill_level;
	uint32_t meta;
	int error;
	int sop_rcvd;
	int eop_rcvd;
	int empty;
	int got_bits;
	int data_bits;

	fill_level = fifo_fill_level(sc);
	if (fill_level == 0)
		return (0);

	dprintf("%s(%d): fill_level %d\n", __func__, sc->unit, fill_level);

	write_lo = (uint64_t)iov->iov_base;
	error = 0;
	sop_rcvd = 0;
	eop_rcvd = 0;
	empty = 0;
	transferred = 0;

	dprintf("%s: copy 0 -> %x\n", __func__, write_lo);

	read_buf = 0;
	got_bits = 0;
	while (fill_level) {
		data = RD4_FIFO_MEM_CACHED(sc, A_ONCHIP_FIFO_MEM_CORE_DATA);
		data_bits = 32;
		CACHE_FIFO_MEM_INV(sc, A_ONCHIP_FIFO_MEM_CORE_DATA);
		dprintf("%s1: data %x, data_bits %d\n",
		    __func__, data, data_bits);

		if (strip_len) {
			data_bits -= (strip_len * 8);
			strip_len = 0;
		}

		dprintf("%s2: data %x, data_bits %d\n",
		    __func__, data, data_bits);

		meta = RD4_FIFO_MEM(sc, A_ONCHIP_FIFO_MEM_CORE_METADATA);
		meta = le32toh(meta);
		if ((meta & A_ONCHIP_FIFO_MEM_CORE_ERROR_MASK) ||
		    (meta & A_ONCHIP_FIFO_MEM_CORE_CHANNEL_MASK)) {
			printf("%s: error: %x\n", __func__, meta);
			error = -1;
			break;
		}

		/* Check for the start of the packet */
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
			data_bits -= empty * 8;
		}

		dprintf("%s3: data %x, data_bits %d\n",
		    __func__, data, data_bits);

		read_buf = (read_buf << data_bits);

		switch (data_bits) {
		case 32:
			read_buf |= data;
			break;
		case 16:
			read_buf |= (data & 0xffff);
			break;
		case 8:
			read_buf |= (data & 0x00ff);
			break;
		}
		got_bits += data_bits;
		dprintf("%s4: read_buf %lx, got_bits %d\n",
		    __func__, read_buf, got_bits);

		if (got_bits >= 32) {
			got_bits -= 32;
			word = (uint32_t)((read_buf >> got_bits) & 0xffffffff);
			dprintf("%s5: writing word %x\n", __func__, word);
			*(uint32_t *)(write_lo) = word;
			write_lo += 4;
			transferred += 4;
		}

		if (meta & A_ONCHIP_FIFO_MEM_CORE_EOP)
			break;

		fill_level = fifo_fill_level(sc);
	}

	if (got_bits) {
		word = (uint32_t)(read_buf << (32 - got_bits)) & 0xffffffff;
		dprintf("%s6: writing word %x (finish)\n", __func__, word);
		*(uint32_t *)(write_lo) = word;
		transferred += got_bits * 8;
	}

	if (error != 0)
		return (error);

	dprintf("%s: pkt rcvd, %d bytes (sop %d eop %d fill_level %d)\n",
	    __func__, transferred, sop_rcvd, eop_rcvd, fill_level);

	return (transferred);
}
