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
	int n;

	val = RD1(sc, EPW_REQUEST_LEVEL_SEND_RESPONSE);
	if (val == 0)
		return (0);

	dprintf("%s\n", __func__);

	req->is_write = RD1(sc, EPW_REQUEST_IS_WRITE);
	dprintf("%s: req->is_write %d\n", __func__, req->is_write);

	/* Read/Write */
	val = RD4(sc, EPW_WRITE_BYTE_ENABLE);
	req->byte_enable = bswap32(val);

	KASSERT(req->byte_enable != 0,
	    ("Unexpected byte_enable: %d", req->byte_enable));

	if (req->is_write) {
		val = RD8(sc, EPW_WRITE_ADDRESS);
		req->addr = bswap64(val);
	} else {
		val = RD8(sc, EPW_READ_ADDRESS);
		req->addr = bswap64(val);
	}

	/* Fix the address */
	for (i = 0; i < sizeof(uint64_t); i++) {
		if ((req->byte_enable & (1 << i)) == 0)
			req->addr += 1;
		else
			break;
	}

	/* Get access width */
	req->data_len = 0;
	for (i = 0; i < sizeof(uint64_t); i++) {
		if (req->byte_enable & (1 << (7 - i)))
			req->data_len += 1;
	}

	dprintf("%s: addr %lx\n", __func__, req->addr);

	if (req->is_write) {
		n = 0;
		for (i = 0; i < sizeof(uint64_t); i++) {
			if (req->byte_enable & (1 << (7 - i)))
				req->data[n++] = RD1(sc, EPW_WRITE_DATA + (7 - i));
		}

		switch (req->data_len) {
		case 8:
			printf("%s: val %lx\n", __func__, *(uint64_t *)req->data);
			break;
		case 4:
			printf("%s: val %x\n", __func__, *(uint32_t *)req->data);
			break;
		case 2:
			printf("%s: val %x\n", __func__, *(uint16_t *)req->data);
			break;
		case 1:
			printf("%s: val %x\n", __func__, *(uint8_t *)req->data);
			break;
		default:
			break;
		};
	} else {
		val = RD8(sc, EPW_READ_FLIT_SIZE);
		req->flit_size = bswap64(val);
		val = RD4(sc, EPW_READ_BURST_COUNT);
		req->burst_count = bswap32(val);

		printf("%s: read, byte enable %x\n", __func__, req->byte_enable);
		printf("%s: read %lx %lx %x\n", __func__, req->addr, req->flit_size, req->burst_count);
	}

	return (1);
}

void
epw_reply(struct epw_softc *sc, struct epw_request *req)
{
	uint64_t val;
	uint32_t offs;
	int i;

	dprintf("%s\n", __func__);

	if (req->is_write == 0) {
		dprintf("flit_size %d\n", req->flit_size);

		for (i = 0; i < (req->burst_count + 1); i++) {
			switch (req->flit_size) {
			case 1:
				val = (uint8_t)req->data[i * 1];
				offs = EPW_READ_RESPONSE_DATA + i * 1;
				printf("write1 to %x, val %x\n", offs, val);
				WR1(sc, offs, val);
				break;
			case 2:
				val = (uint16_t)req->data[i * 2];
				offs = EPW_READ_RESPONSE_DATA + i * 2;
				printf("write2 to %x, val %x\n", offs, val);
				WR2(sc, offs, val);
				break;
			case 4:
				val = (uint32_t)req->data[i * 4];
				offs = (EPW_READ_RESPONSE_DATA + i * 4);
				printf("write4 to %x, val %x\n", offs, val);
				WR4(sc, offs, val);
			case 8:
				val = (uint64_t)req->data[i * 8];
				offs = (EPW_READ_RESPONSE_DATA + i * 8);
				printf("write8 to %x, val %x\n", offs, val);
				WR8(sc, offs, val);
			default:
				break;
			};
		}
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
