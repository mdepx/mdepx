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

#include <mips/microchip/pic32_ddr.h>

#define	PIC32_DDR_DEBUG
#undef	PIC32_DDR_DEBUG

#ifdef	PIC32_DDR_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	RD4(_sc, _reg)		*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	*(volatile uint32_t *)((_sc)->base + _reg) = _val

#define	DIV_ROUND_UP(n, d)	(((n) + (d) - 1) / (d))
#define	MAX(a, b)		((a) > (b) ? (a) : (b))

static void
host_load_cmd(struct pic32_ddr_softc *sc, const struct ddr2_sdram *sdram,
    uint32_t cmd_idx, uint32_t hostcmd2, uint32_t hostcmd1,
    uint32_t delay)
{
	uint32_t hc_delay;
	uint32_t reg;

	hc_delay = MAX(DIV_ROUND_UP(delay, sdram->t_ck), 2) - 2;
	WR4(sc, DDR_CMD1(cmd_idx), hostcmd1);

	reg = (hostcmd2 & 0x7ff) | (hc_delay << 11);
	WR4(sc, DDR_CMD2(cmd_idx), reg);
}

static void
pic32_ddr_phy_init(struct pic32_ddr_softc *sc,
    const struct ddr2_sdram *sdram)
{
	uint32_t reg;

	/*
	 * Start value of the digital DLL master delay line.
	 * Recommended value is 0b0011.
	 */
	reg = (3 << DLYSTVAL_S);
	reg |= (16 << RECALIBCNT_S);
	WR4(sc, DDR_PHYDLLR, reg);

	reg = ODTSEL | ODTEN;
	reg |= (2 << ODTPFDCAL_S);
	reg |= (2 << ODTPUCAL_S);
	reg |= NOEXTDLL | WRCMDDLY | HALFRATE;
	reg |= (0xe << DRVSTRPFET_S);
	reg |= (0xe << DRVSTRNFET_S);
	reg |= RCVREN;
	reg |= (2 << PREAMBDLY_S);
	WR4(sc, DDR_PHYPADCON, reg);

	reg = BURST8 | DDR2;
	reg |= (sdram->rcas << RCASLAT_S);
	reg |= ODTCSW;
	WR4(sc, DDR_SCLCFG0, reg);

	reg = SCLCSEN;
	reg |= (sdram->wcas << WCASLAT_S);
	WR4(sc, DDR_SCLCFG1, reg);

	reg = (3 << CAPCLKDLY_S);
	reg |= (4 << DDRCLKDLY_S);
	WR4(sc, DDR_SCLLAT, reg);
}

static void
pic32_ddr_ctrl_init(struct pic32_ddr_softc *sc,
    const struct ddr2_sdram *sdram)
{
	uint32_t r2pchrgdly, ras2rasdly, ras2casdly, pchrg2rasdly;
	uint32_t w2rdly, w2rcsdly, w2pchrgdly;
	uint32_t reg;
	uint32_t a;
	uint32_t b;

	reg = MEMWIDTH_HALFRATE;
	WR4(sc, DDR_MEMWIDTH, reg);

	reg = (sdram->col_bits + sdram->ba_bits) << RWADDR_S;
	reg |= sdram->col_bits << BNKADDR_S;
	reg |= 0 << CSADDR_S;
	reg |= 0 << CLHADDR_S;
	reg |= APCHRGEN;
	WR4(sc, DDR_MEMCFG0, reg);

	reg = ((1 << sdram->row_bits) - 1) << RWADDRMSK_S;
	WR4(sc, DDR_MEMCFG1, reg);

	reg = 0 << CLADDRHMSK_S;
	WR4(sc, DDR_MEMCFG2, reg);

	reg = ((1 << sdram->col_bits) - 1);
	WR4(sc, DDR_MEMCFG3, reg);

	reg = ((1 << sdram->ba_bits) - 1) << BNKADDRMSK_S;
	reg |= 0 << CSADDRMSK_S;
	WR4(sc, DDR_MEMCFG4, reg);

	/* Refresh configuration */
	reg = (DIV_ROUND_UP(sdram->t_refi, sdram->t_ck_ctrl) - 2)
	    << REFCNT_S;
	reg |= (DIV_ROUND_UP(sdram->t_rfc_min, sdram->t_ck_ctrl) - 2)
	    << REFDLY_S;
	reg |= (7 << MAXREFS_S);
	WR4(sc, DDR_REFCFG, reg);

	/* Power configuration */
	reg = 0;
	reg |= ASLFREFEN;
	/*
	 * Power down enable?
	 * reg |= APWRDNEN;
	 * reg |= PCHRGPWDN;
	 */
	reg |= (8 << PWDNDLY_S);
	reg |= (17 << SLFREFDLY_S);
	WR4(sc, DDR_PWRCFG, reg);

	/* Delay configuration */
	a = DIV_ROUND_UP(sdram->t_wtr, sdram->t_ck_ctrl);
	b = DIV_ROUND_UP(sdram->t_wtr_tck, 2);
	w2rdly = MAX(a, b) + sdram->wcas + sdram->bl;
	w2rcsdly = MAX(w2rdly - 1, 3);
	w2pchrgdly = DIV_ROUND_UP(sdram->t_wr, sdram->t_ck_ctrl)
	    + sdram->wcas + sdram->bl;
	a = DIV_ROUND_UP(sdram->t_rtp, sdram->t_ck_ctrl);
	b = DIV_ROUND_UP(sdram->t_rtp_tck, 2);
	r2pchrgdly = MAX(a, b) + sdram->bl - 2;
	a = DIV_ROUND_UP(sdram->t_rrd, sdram->t_ck_ctrl);
	b = DIV_ROUND_UP(sdram->t_rrd_tck, 2);
	ras2rasdly = MAX(a, b) - 1;
	ras2casdly = DIV_ROUND_UP(sdram->t_rcd, sdram->t_ck_ctrl) - 1;
	pchrg2rasdly = DIV_ROUND_UP(sdram->t_rp, sdram->t_ck_ctrl) - 1;

	reg = (w2rdly << CFG0_W2RDLY_S) & CFG0_W2RDLY_M;
	reg |= (w2rcsdly << CFG0_W2RCSDLY_S) & CFG0_W2RCSDLY_M;
	reg |= (sdram->bl - 1) << R2RDLY_S;
	reg |= (sdram->bl) << R2RCSDLY_S;
	reg |= (sdram->bl - 1) << W2WDLY_S;
	reg |= (sdram->bl - 1) << W2WCSDLY_S;
	reg |= (sdram->bl + 2) << R2WDLY_S;
	reg |= (sdram->rcas - sdram->wcas + 3) << RMWDLY_S;
	WR4(sc, DDR_DLYCFG0, reg);

	reg = (sdram->t_cke_tck - 1) << SLFREFMINDLY_S;
	reg |= ((DIV_ROUND_UP(sdram->t_dllk, 2) - 2) << SLFREFEXDLY_S)
	    & SLFREFEXDLY_M;
	reg |= (sdram->t_cke_tck - 1) << PWRDNMINDLY_S;
	reg |= (MAX(sdram->t_xp_tck, sdram->t_cke_tck) - 1) << PWRDNEXDLY_S;
	reg |= (w2pchrgdly >> 4) << CFG1_W2PCHRGDLY_S;
	reg |= (w2rdly >> 4) << CFG1_W2RDLY_S;
	reg |= (w2rcsdly >> 4) << CFG1_W2RCSDLY_S;
	reg |= ((sdram->rcas + 5) >> 4) << CFG1_NXTDATAVDLY_S;
	reg |= (DIV_ROUND_UP(sdram->t_dllk, 2) >> 8) << SLFREFEXDLY9_S;
	WR4(sc, DDR_DLYCFG1, reg);

	reg = DIV_ROUND_UP(sdram->t_rp, sdram->t_ck_ctrl) << PCHRGALLDLY_S;
	reg |= r2pchrgdly << R2PCHRGDLY_S;
	reg |= (w2pchrgdly << CFG2_W2PCHRGDLY_S) & CFG2_W2PCHRGDLY_M;
	reg |= ras2rasdly << RAS2RASDLY_S;
	reg |= ras2casdly << RAS2CASDLY_S;
	reg |= pchrg2rasdly << PCHRG2RASDLY_S;
	reg |= (sdram->rcas + 3) << RBENDDLY_S;
	WR4(sc, DDR_DLYCFG2, reg);

	reg = (DIV_ROUND_UP(sdram->t_ras_min, sdram->t_ck_ctrl) - 1)
	    << RAS2PCHRGDLY_S;
	reg |= (DIV_ROUND_UP(sdram->t_rc, sdram->t_ck_ctrl) - 1)
	    << RAS2RASSBNKDLY_S;
	reg |= (DIV_ROUND_UP(sdram->t_faw, sdram->t_ck_ctrl) - 1)
	    << FAWTDLY_S;
	WR4(sc, DDR_DLYCFG3, reg);

	/* ODT configuration */
	WR4(sc, DDR_ODTCFG, 0);
	WR4(sc, DDR_ODTENCFG, ODTWEN);

	reg = (sdram->rcas - 3) << ODTRDLY_S;
	reg |= (sdram->wcas - 3) << ODTWDLY_S;
	reg |= 2 << ODTRLEN_S;
	reg |= 3 << ODTWLEN_S;
	WR4(sc, DDR_ODTCFG, reg);

	/* Transfer configuration */
	reg = 2 << NXTDATRQDLY_S;
	reg |= 4 << NXTDATAVDLY_S;
	reg |= 2 << RDATENDLY_S;
	reg |= 3 << MAXBURST_S;
	WR4(sc, DDR_XFERCFG, reg);
}

static void
pic32_ddr_dram_init(struct pic32_ddr_softc *sc,
    const struct ddr2_sdram *sdram)
{
	uint32_t cmd_idx;
	uint32_t reg;
	uint32_t tmp;
	int i;

	tmp = ((DIV_ROUND_UP(sdram->t_wr, sdram->t_ck) - 1) << 1) | 1;

	dprintf("DDR initialization\n");

	cmd_idx = 0;

	/*
	 * a) Bring CKE high after Reset, and then wait 400 ns
	 * using a NOP or DESELECT command.
	 */
	host_load_cmd(sc, sdram, cmd_idx++, 0,
	    IDLE_NOP, 400000);

	/* b) Issue a Precharge All Banks command. */
	host_load_cmd(sc, sdram, cmd_idx++, 0x004,
	    PRECH_ALL_CMD, sdram->t_rp + sdram->t_ck);

	/* c) Initialize Extended Mode Register 2 (EMR2). */
	host_load_cmd(sc, sdram, cmd_idx++, 0x200,
	    LOAD_MODE_CMD, sdram->t_mrd_tck * sdram->t_ck);

	/* d) Initialize Extended Mode Register 3 (EMR3). */
	host_load_cmd(sc, sdram, cmd_idx++, 0x300,
	    LOAD_MODE_CMD, sdram->t_mrd_tck * sdram->t_ck);

	/* e) Enable the DLL by writing to the Extended Mode Register (EMR). */
	host_load_cmd(sc, sdram, cmd_idx++, 0x100,
	    LOAD_MODE_CMD | (0x40 << 24), sdram->t_mrd_tck * sdram->t_ck);

	/* f) Reset the DLL by writing to the Mode Register (MR). */
	host_load_cmd(sc, sdram, cmd_idx++, tmp,
	    LOAD_MODE_CMD | (sdram->rcas << 28) | (2 << 24),
	    sdram->t_mrd_tck * sdram->t_ck);

	/* g) Issue a Precharge All Banks command. */
	host_load_cmd(sc, sdram, cmd_idx++, 0x004,
	    PRECH_ALL_CMD, sdram->t_rp + sdram->t_ck);

	/* h) Issue two Auto-refresh commands. */
	for (i = 0; i < 2; i++)
		host_load_cmd(sc, sdram, cmd_idx++, 0x000,
		    REF_CMD, sdram->t_rfc_min);

	/* i) Reprogram the MR without resetting the DLL. */
	host_load_cmd(sc, sdram, cmd_idx++, tmp,
	    LOAD_MODE_CMD | (sdram->rcas << 28) | (3 << 24),
	    sdram->t_mrd_tck * sdram->t_ck);

	/* j) Reprogram the EMR with the OCD default. */
	host_load_cmd(sc, sdram, cmd_idx++, 0x103,
	    LOAD_MODE_CMD | (0xc << 24), sdram->t_mrd_tck * sdram->t_ck);

	/* k) Reprogram the EMR with a OCD exit. */
	host_load_cmd(sc, sdram, cmd_idx++, 0x100,
	    LOAD_MODE_CMD | (0x4 << 28), 140 * sdram->t_ck);

	reg = cmd_idx << NUMHOSTCMDS_S;
	reg |= VALID;
	WR4(sc, DDR_CMDISSUE, reg);
	WR4(sc, DDR_MEMCON, STINIT);

	dprintf("Wait for the commands to issue\n");

	i = 100;

	do {
		if ((RD4(sc, DDR_CMDISSUE) & VALID) == 0)
			break;
	} while (i--);

	if (i == 0)
		printf("Error during DDR commands issue\n");

	WR4(sc, DDR_MEMCON, STINIT | INITDN);

	/* Calibrate */
	WR4(sc, DDR_SCLSTART, SCLSTART | SCLEN);

	i = 100;

	do {
		if (RD4(sc, DDR_SCLSTART) & SCLUBPASS)
			break;
	} while (i--);

	if (i == 0)
		printf("Error during DDR calibration\n");
}

void
pic32_ddr_init(struct pic32_ddr_softc *sc, uint32_t base,
    const struct ddr2_sdram *sdram)
{

	sc->base = base;

	pic32_ddr_phy_init(sc, sdram);
	pic32_ddr_ctrl_init(sc, sdram);
	pic32_ddr_dram_init(sc, sdram);
}
