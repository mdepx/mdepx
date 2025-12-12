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
#include <sys/systm.h>

#include <arm/stm/stm32n6_xspi.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val
#define	WR2(_sc, _reg, _val)	\
	*(volatile uint16_t *)((_sc)->base + _reg) = _val

int
stm32n6_xspi_transfer(struct stm32n6_xspi_softc *sc, int addr, int val, int len)
{

	WR4(sc, XSPI_AR, addr);
	WR4(sc, XSPI_DLR, len - 1);

	/* TODO */
	WR2(sc, XSPI_DR, val);

	printf("XSPI_SR after write %x\n", RD4(sc, XSPI_SR));

	return (0);
}

int
stm32n6_xspi_setup(struct stm32n6_xspi_softc *sc, struct xspi_config *conf)
{
	uint32_t reg;
	uint32_t cr;
	uint32_t ccr;

	WR4(sc, XSPI_CR, 0);
	WR4(sc, XSPI_FCR, 0xff);

	cr = 7 << CR_FTHRES_S;
	cr |= CR_TCEN;
	if (conf->mode == XSPI_MODE_INDIRECT_WRITE)
		cr |= CR_FMODE_IW;
	else if (conf->mode == XSPI_MODE_MEMORY_MAPPED)
		cr |= CR_FMODE_MM;
	else
		panic("implement me");
	cr |= CR_EN;
	WR4(sc, XSPI_CR, cr);

	if (conf->mode == XSPI_MODE_MEMORY_MAPPED) {
		WR4(sc, XSPI_WIR, conf->instruction_write);
		WR4(sc, XSPI_IR, conf->instruction_read);
	}

	reg = conf->mem_type;
	reg |= conf->dev_size;
	reg |= (conf->cs_cycles - 1) << DCR1_CSHT_S;
	WR4(sc, XSPI_DCR1, reg);

	WR4(sc, XSPI_DCR2, conf->prescaler);
	WR4(sc, XSPI_TCR, conf->dummy_cycles);
	if (conf->mode == XSPI_MODE_MEMORY_MAPPED)
		WR4(sc, XSPI_WTCR, conf->dummy_cycles);

	ccr = conf->data_dtr ? CCR_DDTR : 0;
	ccr |= conf->address_dtr ? CCR_ADDTR : 0;
	ccr |= conf->instruction_dtr ? CCR_IDTR : 0;
	ccr |= conf->dqs_en ? CCR_DQSE : 0;

	switch (conf->data_lines) {
	case 16: ccr |= CCR_DMODE_16; break;
	case  8: ccr |= CCR_DMODE_8;  break;
	case  4: ccr |= CCR_DMODE_4;  break;
	case  2: ccr |= CCR_DMODE_2;  break;
	case  1: ccr |= CCR_DMODE_1;  break;
	};
	switch (conf->address_size) {
	case 32: ccr |= CCR_ADSIZE_32; break;
	case 24: ccr |= CCR_ADSIZE_24; break;
	case 16: ccr |= CCR_ADSIZE_16; break;
	case  8: ccr |= CCR_ADSIZE_8;  break;
	};
	switch (conf->address_lines) {
	case 8: ccr |= CCR_ADMODE_8; break;
	case 4: ccr |= CCR_ADMODE_4; break;
	case 2: ccr |= CCR_ADMODE_2; break;
	case 1: ccr |= CCR_ADMODE_1; break;
	};
	switch (conf->instruction_size) {
	case 32: ccr |= CCR_ISIZE_32; break;
	case 24: ccr |= CCR_ISIZE_24; break;
	case 16: ccr |= CCR_ISIZE_16; break;
	case  8: ccr |= CCR_ISIZE_8;  break;
	};
	switch (conf->instruction_lines) {
	case 8: ccr |= CCR_IMODE_8; break;
	case 4: ccr |= CCR_IMODE_4; break;
	case 2: ccr |= CCR_IMODE_2; break;
	case 1: ccr |= CCR_IMODE_1; break;
	};
	WR4(sc, XSPI_CCR, ccr);
	if (conf->mode == XSPI_MODE_MEMORY_MAPPED)
		WR4(sc, XSPI_WCCR, ccr);

	/* These apply to indirect mode. */
	WR4(sc, XSPI_IR, conf->instruction);

	return (0);
}

void
stm32n6_xspi_init(struct stm32n6_xspi_softc *sc, uint32_t base)
{

	sc->base = base;
}
