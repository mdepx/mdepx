/*-
 * Copyright (c) 2018 Ruslan Bukin <br@bsdpad.com>
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
 * ("CTSRD"), as part of the DARPA CRASH research programme.
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

#include <mips/beri/beri_epw.h>

#define	EPW_DEBUG
#undef	EPW_DEBUG

#ifdef	EPW_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	RD8(_sc, _reg)		*(volatile uint64_t *)((_sc)->base + _reg)
#define	RD4(_sc, _reg)		*(volatile uint32_t *)((_sc)->base + _reg)
#define	RD2(_sc, _reg)		*(volatile uint16_t *)((_sc)->base + _reg)
#define	RD1(_sc, _reg)		*(volatile uint8_t *)((_sc)->base + _reg)

#define	WR8(_sc, _reg, _val)	*(volatile uint64_t *)((_sc)->base + _reg) = _val
#define	WR4(_sc, _reg, _val)	*(volatile uint32_t *)((_sc)->base + _reg) = _val
#define	WR2(_sc, _reg, _val)	*(volatile uint16_t *)((_sc)->base + _reg) = _val
#define	WR1(_sc, _reg, _val)	*(volatile uint8_t *)((_sc)->base + _reg) = _val

int
epw_request(struct epw_softc *sc, struct epw_request *req)
{
	uint64_t val;
	int i;

	val = RD1(sc, EPW_REQUEST_LEVEL_SEND_RESPONSE);
	if (val == 0)
		return (0);

	req->is_write = RD1(sc, EPW_REQUEST_IS_WRITE);
	if (req->is_write) {
		val = RD8(sc, EPW_WRITE_ADDRESS);
		req->addr = bswap64(val);
		dprintf("%s: (write) address %lx\n", __func__, req->addr);

		val = RD4(sc, EPW_WRITE_BYTE_ENABLE);
		req->byte_enable = bswap32(val);

		KASSERT(req->byte_enable != 0,
		    ("Unexpected byte_enable: %d", req->byte_enable));

		/* Fix the address */
		for (i = 0; i < sizeof(uint64_t); i++) {
			if ((req->byte_enable & (1 << i)) == 0)
				req->addr += 1;
			else
				break;
		}

		dprintf("%s: fixed address %lx\n", __func__, req->addr);

		req->data_len = 0;
		for (i = 0; i < sizeof(uint64_t); i++) {
			if (req->byte_enable & (1 << (7 - i)))
				req->data[req->data_len++] = RD1(sc, EPW_WRITE_DATA + (7 - i));
		}

		switch (req->data_len) {
		case 8:
			dprintf("%s: val %lx\n", __func__, *(uint64_t *)req->data);
			break;
		case 4:
			dprintf("%s: val %x\n", __func__, *(uint32_t *)req->data);
			break;
		case 2:
			dprintf("%s: val %x\n", __func__, *(uint16_t *)req->data);
			break;
		case 1:
			dprintf("%s: val %x\n", __func__, *(uint8_t *)req->data);
			break;
		default:
			break;
		};
	} else {
		val = RD8(sc, EPW_READ_ADDRESS);
		req->addr = bswap64(val);
		dprintf("%s: (read) address %lx\n", __func__, req->addr);

		val = RD8(sc, EPW_READ_FLIT_SIZE);
		req->flit_size = bswap64(val);
		val = RD4(sc, EPW_READ_BURST_COUNT);
		req->burst_count = bswap32(val);

		dprintf("%s: read %lx, flit_size %lx, burst count %x\n",
		    __func__, req->addr, req->flit_size, req->burst_count);
	}

	return (1);
}

void
epw_reply(struct epw_softc *sc, struct epw_request *req)
{
	int i;

	dprintf("%s\n", __func__);

	if (req->is_write == 0) {
		for (i = 0; i < 8; i++)
			WR1(sc, EPW_READ_RESPONSE_DATA + i, req->data[7 - i]);
	}

	WR1(sc, EPW_REQUEST_LEVEL_SEND_RESPONSE, 1);
}

void
epw_control(struct epw_softc *sc, uint8_t enable)
{

	if (enable)
		WR1(sc, EPW_ENABLE_DEVICE_EMULATION, 1);
	else
		WR1(sc, EPW_ENABLE_DEVICE_EMULATION, 0);
}

void
epw_init(struct epw_softc *sc, uint64_t base, uint64_t window)
{

	sc->base = base | MIPS_XKPHYS_UNCACHED_BASE;
	sc->window = window | MIPS_XKPHYS_UNCACHED_BASE;
}
