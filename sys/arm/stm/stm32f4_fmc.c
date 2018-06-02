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
#include <arm/stm/stm32f4_fmc.h>

#define	RD4(_sc, _reg)		*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static void
fmc_sdram_cmd(struct stm32f4_fmc_softc *sc, uint32_t bank, uint32_t cmd)
{

	if (bank == 1)
		cmd |= SDCMR_CTB1;
	else
		cmd |= SDCMR_CTB2;

	while (RD4(sc, FMC_SDSR) & SDSR_BUSY)
		;

	WR4(sc, FMC_SDCMR, cmd);

	while (RD4(sc, FMC_SDSR) & SDSR_BUSY)
		;
}

static void
configure_bank(struct stm32f4_fmc_softc *sc, const struct sdram *entry, int b)
{
	const struct sdram_bank *bank;
	uint32_t mrd;
	uint32_t reg;
	int i;

	if (b == 1)
		bank = &entry->bank1;
	else
		bank = &entry->bank2;

	reg =   ( bank->sdcr.rpipe	<< RPIPE_S)	|
		( bank->sdcr.rburst	<< RBURST_S)	|
		( bank->sdcr.sdclk	<< SDCLK_S)	|
		( bank->sdcr.cas	<< CAS_S)	|
		( bank->sdcr.nb		<< NB_S )	|
		( bank->sdcr.mwid	<< MWID_S)	|
		( bank->sdcr.nr		<< NR_S )	|
		( bank->sdcr.nc		<< NC_S );

	if (b == 1)
		WR4(sc, FMC_SDCR_1, reg);
	else
		WR4(sc, FMC_SDCR_2, reg);

	reg =   ( bank->sdtr.trcd	<< TRCD_S )	|
		( bank->sdtr.trp	<< TRP_S )	|
		( bank->sdtr.twr	<< TWR_S )	|
		( bank->sdtr.trc	<< TRC_S )	|
		( bank->sdtr.tras	<< TRAS_S )	|
		( bank->sdtr.txsr	<< TXSR_S )	|
		( bank->sdtr.tmrd	<< TMRD_S );

	if (b == 1)
		WR4(sc, FMC_SDTR1, reg);
	else
		WR4(sc, FMC_SDTR2, reg);

	reg = (SDCMR_MODE_CLKEN << SDCMR_MODE_S);
	fmc_sdram_cmd(sc, b, reg);

	for (i = 0; i < 10000; i++);

	reg = (SDCMR_MODE_PALL << SDCMR_MODE_S);
	fmc_sdram_cmd(sc, b, reg);

	reg = (SDCMR_MODE_AUTO_REFRESH << SDCMR_MODE_S) |
	    (entry->nrfs << SDCMR_NRFS_S);
	fmc_sdram_cmd(sc, b, reg);

	mrd =   ( bank->sdcmr.burstlen ) |
		( SDCMR_MRD_BURST_TYP_SEQ ) |
		( bank->sdcr.cas << SDCMR_MRD_CAS_S ) |
		( SDCMR_MRD_WBURST_SINGLE );

	reg = (SDCMR_MODE_LOAD << SDCMR_MODE_S) |
	      (mrd << SDCMR_MRD_S);

	fmc_sdram_cmd(sc, b, reg);
}

void
stm32f4_fmc_setup(struct stm32f4_fmc_softc *sc,
    const struct sdram *entry)
{
	uint32_t reg;

	configure_bank(sc, entry, 1);
	configure_bank(sc, entry, 2);

	reg = (entry->sdrtr << 1);
	WR4(sc, FMC_SDRTR, reg);
}

int
stm32f4_fmc_init(struct stm32f4_fmc_softc *sc, uint32_t base)
{

	sc->base = base;

	return (0);
}
