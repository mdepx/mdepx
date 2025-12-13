/*-
 * Copyright (c) 2025 Ruslan Bukin <br@bsdpad.com>
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
#include <arm/stm/stm32n6_risaf.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
stm32n6_risaf_setup(struct stm32n6_risaf_softc *sc, uint32_t reg_id,
    struct risaf_config *conf)
{
	uint32_t reg;

	/* Base region */
	WR4(sc, RISAF_REG_STARTR(reg_id), conf->base_start);
	WR4(sc, RISAF_REG_ENDR(reg_id), conf->base_end);

	reg = (conf->base_cid_read << 0);
	reg |= (conf->base_cid_write << 16);
	WR4(sc, RISAF_REG_CIDCFGR(reg_id), reg);

	reg = CFGR_BREN;
	if (conf->base_sec)
		reg |= CFGR_SEC;
	WR4(sc, RISAF_REG_CFGR(reg_id), reg);

	/* Subregion */
	WR4(sc, RISAF_REG_ASTARTR(reg_id), conf->suba_start);
	WR4(sc, RISAF_REG_AENDR(reg_id), conf->suba_end);

	reg = ACFGR_SREN;
	if (conf->suba_rd)
		reg |= ACFGR_RDEN;
	if (conf->suba_wr)
		reg |= ACFGR_WREN;
	reg |= (conf->suba_cid << ACFGR_SRCID_S);
	if (conf->suba_sec)
		reg |= ACFGR_SEC;
	WR4(sc, RISAF_REG_ACFGR(reg_id), reg);
}

void
stm32n6_risaf_init(struct stm32n6_risaf_softc *sc, uint32_t base)
{

	sc->base = base;
}
