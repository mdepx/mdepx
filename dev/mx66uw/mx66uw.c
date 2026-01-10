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

#include <arm/stm/stm32n6.h>

#include <dev/mx66uw/mx66uw.h>
#include <dev/mx66uw/mx66uw_reg.h>

#define DUMMY_CYCLES_READ            8U
#define DUMMY_CYCLES_READ_OCTAL      6U
#define DUMMY_CYCLES_READ_OCTAL_DTR  6U
#define DUMMY_CYCLES_REG_OCTAL       4U
#define DUMMY_CYCLES_REG_OCTAL_DTR   5U

void
xspi_check_ready(struct mx66uw_softc *sc, uint32_t match_value,
    uint32_t match_mask)
{
	struct stm32n6_xspi_poll_cfg poll_cfg;
	struct xspi_config conf;

	bzero(&conf, sizeof(struct xspi_config));
	conf.mode = XSPI_MODE_AUTO_POLLING;
	conf.prescaler = 2; /* 50 MHz */
	conf.mem_type = DCR1_MTYP_MACRONIX;
	conf.dev_size = DCR1_DEVSIZE_128M;
	conf.cs_cycles = 2;
	conf.dummy_cycles = sc->dopi ? DUMMY_CYCLES_REG_OCTAL_DTR :
	    DUMMY_CYCLES_READ;
	conf.dqs_en = sc->dopi ? 1 : 0;
	conf.data_dtr = sc->dopi ? 1 : 0;
	conf.data_lines = sc->dopi ? 8 : 1;
	conf.address_dtr = sc->dopi ? 1 : 0;
	conf.address_lines = 0;
	conf.address_size = 32;
	conf.instruction_lines = sc->dopi ? 8 : 1;
	conf.instruction_size = sc->dopi ? 16 : 8;
	conf.instruction_dtr = sc->dopi ? 1 : 0;
	conf.instruction = sc->dopi ? MX66UW1G45G_OCTA_READ_STATUS_REG_CMD :
	    MX66UW1G45G_READ_STATUS_REG_CMD;
	stm32n6_xspi_setup(sc->xsc, &conf);

	bzero(&poll_cfg, sizeof(struct stm32n6_xspi_poll_cfg));
	poll_cfg.instruction = sc->dopi ? MX66UW1G45G_OCTA_READ_STATUS_REG_CMD :
	    MX66UW1G45G_READ_STATUS_REG_CMD;
	poll_cfg.match_value = match_value;
	poll_cfg.match_mask = match_mask;
	poll_cfg.match_mode = CR_PMM_AND;
	poll_cfg.interval = MX66UW1G45G_AUTOPOLLING_INTERVAL_TIME;
	poll_cfg.auto_stop = CR_APMS;
	stm32n6_xspi_autopoll(sc->xsc, &poll_cfg);
}

static int
xspi_read_reg(struct mx66uw_softc *sc, uint32_t reg, uint8_t *val)
{
	struct xspi_config conf;
	int error;
	int len;

	bzero(&conf, sizeof(struct xspi_config));
	conf.mode = XSPI_MODE_INDIRECT_READ;
	conf.prescaler = 2; /* 50 MHz */
	conf.mem_type = DCR1_MTYP_MACRONIX;
	conf.dev_size = DCR1_DEVSIZE_128M;
	conf.cs_cycles = 2;
	conf.dummy_cycles = sc->dopi ? DUMMY_CYCLES_READ_OCTAL_DTR :
	    DUMMY_CYCLES_READ;
	conf.data_dtr = sc->dopi ? 1 : 0;
	conf.data_lines = sc->dopi ? 8 : 1;
	conf.address_dtr = sc->dopi ? 1 : 0;
	conf.address_lines = sc->dopi ? 8 : 1;
	conf.address_size = 32;
	conf.dqs_en = sc->dopi ? 1 : 0;
	conf.instruction_lines = sc->dopi ? 8 : 1;
	conf.instruction_size = sc->dopi ? 16 : 8;
	conf.instruction_dtr = sc->dopi ? 1 : 0;
	conf.instruction = sc->dopi ? MX66UW1G45G_OCTA_READ_CFG_REG2_CMD :
	    MX66UW1G45G_READ_CFG_REG2_CMD;
	len = sc->dopi ? 2 : 1;

	stm32n6_xspi_setup(sc->xsc, &conf);
	error = stm32n6_xspi_receive(sc->xsc, reg, val, len);

	return (error);
}

void
xspi_write_reg(struct mx66uw_softc *sc, uint32_t reg, uint32_t val)
{
	struct xspi_config conf;
	int len;

	bzero(&conf, sizeof(struct xspi_config));
	conf.dummy_cycles = 0;
	conf.mode = XSPI_MODE_INDIRECT_WRITE;
	conf.prescaler = 2; /* 50 MHz */
	conf.mem_type = DCR1_MTYP_MACRONIX;
	conf.dev_size = DCR1_DEVSIZE_128M;
	conf.cs_cycles = 2;
	conf.data_dtr = sc->dopi ? 1 : 0;
	conf.data_lines = sc->dopi ? 8 : 1;
	conf.address_dtr = sc->dopi ? 1 : 0;
	conf.address_lines = sc->dopi ? 8 : 1;
	conf.address_size = 32;
	conf.dqs_en = sc->dopi ? 1 : 0;
	conf.instruction_lines = sc->dopi ? 8 : 1;
	conf.instruction_size = sc->dopi ? 16 : 8;
	conf.instruction_dtr = sc->dopi ? 1 : 0;
	conf.instruction = sc->dopi ? MX66UW1G45G_OCTA_WRITE_CFG_REG2_CMD :
	    MX66UW1G45G_WRITE_CFG_REG2_CMD;

	len = sc->dopi ? 2 : 1;

	stm32n6_xspi_setup(sc->xsc, &conf);
	stm32n6_xspi_transmit(sc->xsc, reg, (uint8_t *)&val, len);
}

void
xspi_write_enable(struct mx66uw_softc *sc)
{
	struct xspi_config conf;

	printf("%s: sending write enable\n", __func__);

	bzero(&conf, sizeof(struct xspi_config));
	conf.dummy_cycles = 0;
	conf.mode = XSPI_MODE_INDIRECT_WRITE;
	conf.prescaler = 2; /* 50 MHz */
	conf.mem_type = DCR1_MTYP_MACRONIX;
	conf.dev_size = DCR1_DEVSIZE_128M;
	conf.cs_cycles = 2;
	conf.data_dtr = 0;
	conf.data_lines = 0;
	conf.address_dtr = 0;
	conf.address_lines = 0;
	conf.address_size = 0;
	conf.dqs_en = sc->dopi ? 1 : 0;
	conf.instruction_lines = sc->dopi ? 8 : 1;
	conf.instruction_size = sc->dopi ? 16 : 8;
	conf.instruction_dtr = sc->dopi ? 1 : 0;
	conf.instruction = sc->dopi ? MX66UW1G45G_OCTA_WRITE_ENABLE_CMD :
	    MX66UW1G45G_WRITE_ENABLE_CMD;
	stm32n6_xspi_setup(sc->xsc, &conf);

	xspi_check_ready(sc, 2, 2);
}

static int
xspi_switch_to_dopi(struct mx66uw_softc *sc)
{
	uint8_t data[2];
	int error;

	xspi_write_enable(sc);
	xspi_write_reg(sc, MX66UW1G45G_CR2_REG3_ADDR,
	    MX66UW1G45G_CR2_DC_20_CYCLES);

	xspi_write_enable(sc);
	xspi_write_reg(sc, MX66UW1G45G_CR2_REG1_ADDR, MX66UW1G45G_CR2_DOPI);

	sc->dopi = 1;

	/* Verify. */
	error = xspi_read_reg(sc, MX66UW1G45G_CR2_REG1_ADDR, data);
	if (error || data[0] != MX66UW1G45G_CR2_DOPI)
		return (-1);

	return (0);
}

static void
xspi_enable_memory_mapped(struct mx66uw_softc *sc)
{
	struct xspi_config conf;

	bzero(&conf, sizeof(struct xspi_config));
	conf.mem_type = DCR1_MTYP_MACRONIX;
	conf.dev_size = DCR1_DEVSIZE_128M;
	conf.cs_cycles = 2;
	conf.prescaler = sc->dopi ? 0 : 2;
	conf.mode = XSPI_MODE_MEMORY_MAPPED;
	conf.dummy_cycles = sc->dopi ? DUMMY_CYCLES_READ_OCTAL_DTR : 8;
	conf.wdummy_cycles = 0;
	conf.dqs_en = sc->dopi ? 1 : 0;
	conf.wdqs_en = 0;
	conf.data_dtr = sc->dopi ? 1 : 0;
	conf.data_lines = sc->dopi ? 8 : 1;
	conf.address_dtr = sc->dopi ? 1 : 0;
	conf.address_lines = sc->dopi ? 8 : 1;
	conf.address_size = 32;
	conf.instruction = 0;
	conf.instruction_dtr = sc->dopi ? 1 : 0;
	conf.instruction_lines = sc->dopi ? 8 : 1;
	conf.instruction_size = sc->dopi ? 16 : 8;
	conf.instruction_read = sc->dopi ? MX66UW1G45G_OCTA_READ_DTR_CMD :
	    MX66UW1G45G_4_BYTE_ADDR_FAST_READ_CMD;
	conf.instruction_write = sc->dopi ? MX66UW1G45G_OCTA_PAGE_PROG_CMD :
	    MX66UW1G45G_4_BYTE_PAGE_PROG_CMD;
	stm32n6_xspi_setup(sc->xsc, &conf);
}

void
mx66uw_init(struct mx66uw_softc *sc, struct stm32n6_xspi_softc *xspi_sc)
{
	struct xspi_config conf;
	int error;

	sc->xsc = xspi_sc;

	bzero(&conf, sizeof(struct xspi_config));
	conf.dummy_cycles = 0;
	conf.wdummy_cycles = 0;
	conf.mode = XSPI_MODE_INDIRECT_WRITE;
	conf.dqs_en = 0;
	conf.prescaler = 2; /* 50 MHz */
	conf.mem_type = DCR1_MTYP_MACRONIX;
	conf.dev_size = DCR1_DEVSIZE_128M;
	conf.cs_cycles = 2;
	conf.data_dtr = 0;
	conf.data_lines = 0;
	conf.address_dtr = 0;
	conf.address_lines = 0;
	conf.address_size = 0;
	conf.instruction_dtr = 0;
	conf.instruction_lines = 1;
	conf.instruction_size = 8;

	printf("NOR Reset\n");
	conf.instruction = MX66UW1G45G_RESET_ENABLE_CMD;
	stm32n6_xspi_setup(sc->xsc, &conf);
	conf.instruction = MX66UW1G45G_RESET_MEMORY_CMD;
	stm32n6_xspi_setup(sc->xsc, &conf);

	printf("NOR reset OPI STR\n");
	conf.instruction_lines = 8;
	conf.instruction_size = 16;
	conf.instruction_dtr = 0;
	conf.instruction = MX66UW1G45G_OCTA_RESET_ENABLE_CMD;
	stm32n6_xspi_setup(sc->xsc, &conf);
	conf.instruction = MX66UW1G45G_OCTA_RESET_MEMORY_CMD;
	stm32n6_xspi_setup(sc->xsc, &conf);

	printf("NOR reset OPI DTR (DOPI)\n");
	conf.instruction_dtr = 1;
	conf.instruction = MX66UW1G45G_OCTA_RESET_ENABLE_CMD;
	stm32n6_xspi_setup(sc->xsc, &conf);
	conf.instruction = MX66UW1G45G_OCTA_RESET_MEMORY_CMD;
	stm32n6_xspi_setup(sc->xsc, &conf);

	xspi_check_ready(sc, 0, MX66UW1G45G_SR_WIP);

	error = xspi_switch_to_dopi(sc);
	if (error)
		printf("could not switch to dual octa protocol\n");

	xspi_check_ready(sc, 0, MX66UW1G45G_SR_WIP);

	xspi_enable_memory_mapped(sc);
}
