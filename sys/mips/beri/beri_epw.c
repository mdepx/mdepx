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
#define	RD1(_sc, _reg)		*(volatile uint8_t *)((_sc)->base + _reg)

#define	WR8(_sc, _reg, _val)	*(volatile uint64_t *)((_sc)->base + _reg) = _val
#define	WR4(_sc, _reg, _val)	*(volatile uint32_t *)((_sc)->base + _reg) = _val
#define	WR1(_sc, _reg, _val)	*(volatile uint8_t *)((_sc)->base + _reg) = _val

int
epw_request(struct epw_softc *sc, struct epw_request *req)
{
	uint8_t val;
	int i;

	dprintf("%s: issuing a byte-width read to %lx\n",
	    __func__, (sc->base + EPW_REQUEST_LEVEL_SEND_RESPONSE));

	val = RD1(sc, EPW_REQUEST_LEVEL_SEND_RESPONSE);
	if (val == 0)
		return (0);

	dprintf("%s: val %d\n", __func__, val);

	req->is_write = RD1(sc, EPW_REQUEST_IS_WRITE);
	if (req->is_write) {
		req->addr = RD8(sc, EPW_WRITE_ADDRESS);
		req->byte_enable = RD4(sc, EPW_WRITE_BYTE_ENABLE);
		KASSERT(req->byte_enable <= 8,
		    ("Unexpected byte_enable: %d", req->byte_enable));
		for (i = 0; i < req->byte_enable; i++)
			req->data[i] = RD1(sc, EPW_WRITE_DATA + i);
	} else {
		req->addr = RD8(sc, EPW_READ_ADDRESS);
		req->flit_size = RD4(sc, EPW_READ_FLIT_SIZE);
		req->burst_count = RD4(sc, EPW_READ_BURST_COUNT);
	}

	return (1);
}

void
epw_reply(struct epw_softc *sc, struct epw_request *req)
{
	int i;

	if (req->is_write == 0)
		for (i = 0; i < req->burst_count; i++)
			WR1(sc, EPW_READ_RESPONSE_DATA + i,
			    req->data[i]);

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
