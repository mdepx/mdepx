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

#ifndef	_RISCV_KENDRYTE_K210_FPIOA_H_
#define	_RISCV_KENDRYTE_K210_FPIOA_H_

struct fpioa_io_config { 
	uint32_t ch_sel	:8;
	uint32_t ds	:4;	/* Drive */
	uint32_t oe_en	:1;	/* Output Enable */
	uint32_t oe_inv	:1;
	uint32_t do_sel	:1;
	uint32_t do_inv	:1;
	uint32_t pu	:1;
	uint32_t pd	:1;
	uint32_t resv0	:1;
	uint32_t sl	:1;
	uint32_t ie_en	:1;	/* Input Enable */
	uint32_t ie_inv	:1;
	uint32_t di_inv	:1;
	uint32_t st	:1;
	uint32_t resv1	:7;
	uint32_t pad_di	:1;
};

struct k210_fpioa_softc {
	size_t base;
};

void k210_fpioa_init(struct k210_fpioa_softc *sc, uint32_t base);
void k210_fpioa_set_config(struct k210_fpioa_softc *sc,
    uint32_t number, struct fpioa_io_config *cfg);

#endif /* !_RISCV_KENDRYTE_K210_FPIOA_H_ */
