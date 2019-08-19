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

#ifndef	_MIPS_BERI_BERI_EPW_
#define	_MIPS_BERI_BERI_EPW_

#define	EPW_READ_ADDRESS		0x0000	/* read only */
#define	EPW_READ_FLIT_SIZE		0x0008	/* read only */
#define	EPW_READ_BURST_COUNT		0x000C	/* read only */
#define	EPW_READ_RESPONSE_DATA		0x0040	/* read/write */
#define	EPW_WRITE_ADDRESS		0x1000	/* read only */
#define	EPW_WRITE_BYTE_ENABLE		0x1008	/* read only */
#define	EPW_WRITE_DATA			0x1040	/* read only */
#define	EPW_TIME_STAMP			0x2000	/* read only */
#define	EPW_REQUEST_ID			0x2004	/* read only */
#define	EPW_REQUEST_IS_WRITE		0x2006	/* read only */
#define	EPW_REQUEST_LEVEL_SEND_RESPONSE	0x2007	/* read/write */
#define	EPW_ENABLE_DEVICE_EMULATION	0x2008	/* read/write */

struct epw_request {
	uint64_t addr;
	uint8_t is_write;
	uint64_t flit_size;
	uint32_t burst_count;
	uint32_t byte_enable;
	uint8_t data[32];
	uint8_t data_len;
};

struct epw_softc {
	uint64_t base;
	uint64_t window;
};

void epw_init(struct epw_softc *sc, uint64_t base, uint64_t window);
void epw_control(struct epw_softc *sc, uint8_t enable);
int epw_request(struct epw_softc *sc, struct epw_request *req);
void epw_reply(struct epw_softc *sc, struct epw_request *req);

#endif /* !_MIPS_BERI_BERI_EPW_ */
