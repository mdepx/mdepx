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
#include <sys/systm.h>
#include <dev/dsi/dsi.h>
#include <dev/otm8009a/otm8009a.h>

#define	OTM8009A_DEBUG
#undef	OTM8009A_DEBUG

#ifdef	OTM8009A_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define dcs_write_seq(dev, seq...)			\
({							\
	static const uint8_t data[] = { seq };		\
	dsi_cmd(dev, data, ARRAY_SIZE(data));		\
})

#define dcs_write_cmd(dev, cmd, seq...)		\
({							\
	dcs_write_seq(dev, MCS_ADRSFT, (cmd) & 0xff);	\
	dcs_write_seq(dev, (cmd) >> 8, seq);		\
})

static void
dsi_cmd(dsi_device_t *dev, const uint8_t *params, uint32_t nparams)
{

	if (nparams < 2)
		dev->dsi_short(dev, dev->vchid, DSI_DCS_SHORT_PKT_WRITE_P1,
		    params[0], params[1]); 
	else
		dev->dsi_long(dev, dev->vchid, DSI_DCS_LONG_PKT_WRITE,
		    params, nparams); 
}

void
otm8009a_init(dsi_device_t *dev, enum otm_format fmt,
    enum otm_orientation orientation)
{

	/* Enter CMD2 */
	dcs_write_cmd(dev, MCS_CMD2_ENA1, 0x80, 0x09, 0x01);

	/* Enter ORISE CMD2 */
	dcs_write_cmd(dev, MCS_CMD2_ENA2, 0x80, 0x09);

	dcs_write_cmd(dev, MCS_SD_PCH_CTRL, 0x30);
	udelay(10000);

	dcs_write_cmd(dev, MCS_NO_DOC1, 0x40);
	udelay(10000);

	dcs_write_cmd(dev, MCS_PWR_CTRL4 + 1, 0xA9);
	dcs_write_cmd(dev, MCS_PWR_CTRL2 + 1, 0x34);
	dcs_write_cmd(dev, MCS_P_DRV_M, 0x50);
	dcs_write_cmd(dev, MCS_VCOMDC, 0x4E);
	dcs_write_cmd(dev, MCS_OSC_ADJ, 0x66); /* 65Hz */
	dcs_write_cmd(dev, MCS_PWR_CTRL2 + 2, 0x01);
	dcs_write_cmd(dev, MCS_PWR_CTRL2 + 5, 0x34);
	dcs_write_cmd(dev, MCS_PWR_CTRL2 + 4, 0x33);
	dcs_write_cmd(dev, MCS_GVDDSET, 0x79, 0x79);
	dcs_write_cmd(dev, MCS_SD_CTRL + 1, 0x1B);
	dcs_write_cmd(dev, MCS_PWR_CTRL1 + 2, 0x83);
	dcs_write_cmd(dev, MCS_SD_PCH_CTRL + 1, 0x83);
	dcs_write_cmd(dev, MCS_RGB_VIDEO_SET, 0x0E);
	dcs_write_cmd(dev, MCS_PANSET, 0x00, 0x01);

	dcs_write_cmd(dev, MCS_GOAVST, 0x85, 0x01, 0x00, 0x84, 0x01, 0x00);
	dcs_write_cmd(dev, MCS_GOACLKA1, 0x18, 0x04, 0x03, 0x39, 0x00, 0x00,
	    0x00, 0x18, 0x03, 0x03, 0x3A, 0x00, 0x00, 0x00);
	dcs_write_cmd(dev, MCS_GOACLKA3, 0x18, 0x02, 0x03, 0x3B, 0x00, 0x00,
	    0x00, 0x18, 0x01, 0x03, 0x3C, 0x00, 0x00, 0x00);
	dcs_write_cmd(dev, MCS_GOAECLK, 0x01, 0x01, 0x20, 0x20, 0x00, 0x00,
	    0x01, 0x02, 0x00, 0x00);

	dcs_write_cmd(dev, MCS_NO_DOC2, 0x00);

	dcs_write_cmd(dev, MCS_PANCTRLSET1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	dcs_write_cmd(dev, MCS_PANCTRLSET2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0);
	dcs_write_cmd(dev, MCS_PANCTRLSET3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0);
	dcs_write_cmd(dev, MCS_PANCTRLSET4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	dcs_write_cmd(dev, MCS_PANCTRLSET5, 0, 4, 4, 4, 4, 4, 0, 0, 0, 0,
	    0, 0, 0, 0, 0);
	dcs_write_cmd(dev, MCS_PANCTRLSET6, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4,
	    4, 0, 0, 0, 0);
	dcs_write_cmd(dev, MCS_PANCTRLSET7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	dcs_write_cmd(dev, MCS_PANCTRLSET8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	    0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

	dcs_write_cmd(dev, MCS_PANU2D1, 0x00, 0x26, 0x09, 0x0B, 0x01, 0x25,
	    0x00, 0x00, 0x00, 0x00);
	dcs_write_cmd(dev, MCS_PANU2D2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x0A, 0x0C, 0x02);
	dcs_write_cmd(dev, MCS_PANU2D3, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
	dcs_write_cmd(dev, MCS_PAND2U1, 0x00, 0x25, 0x0C, 0x0A, 0x02, 0x26,
	    0x00, 0x00, 0x00, 0x00);
	dcs_write_cmd(dev, MCS_PAND2U2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x0B, 0x09, 0x01);
	dcs_write_cmd(dev, MCS_PAND2U3, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00,
	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

	dcs_write_cmd(dev, MCS_PWR_CTRL1 + 1, 0x66);

	dcs_write_cmd(dev, MCS_NO_DOC3, 0x06);

	dcs_write_cmd(dev, MCS_GMCT22P, 0x00, 0x09, 0x0F, 0x0E, 0x07, 0x10,
	    0x0B, 0x0A, 0x04, 0x07, 0x0B, 0x08, 0x0F, 0x10, 0x0A, 0x01);
	dcs_write_cmd(dev, MCS_GMCT22N, 0x00, 0x09, 0x0F, 0x0E, 0x07, 0x10,
	    0x0B, 0x0A, 0x04, 0x07, 0x0B, 0x08, 0x0F, 0x10, 0x0A, 0x01);

	/* Exit CMD2 */
	dcs_write_cmd(dev, MCS_CMD2_ENA1, 0xFF, 0xFF, 0xFF);

	dcs_write_seq(dev, OTM_CMD_NOP, 0x00);

	dcs_write_seq(dev, OTM_CMD_SLPOUT);
  
	/* Wait for sleep out */
	udelay(120000);

	switch (fmt) {
	case OTM_FMT_RGB888:
		dcs_write_seq(dev, OTM_CMD_COLMOD,
		    (COLMOD_VIPF_24_1 | COLMOD_IFPF_24));
		break;
	case OTM_FMT_RGB565:
		dcs_write_seq(dev, OTM_CMD_COLMOD,
		    (COLMOD_VIPF_16 | COLMOD_IFPF_16));
	default:
		break;
	}

	/*
	 * Default orientation is portrait,
	 * so configure landspace if required
	 */
	if (orientation == OTM_ORIENTATION_LANDSCAPE) {
		dcs_write_seq(dev, OTM_CMD_MADCTL, (MADCTL_MV | MADCTL_MX));
		dcs_write_seq(dev, OTM_CMD_CASET, 0x00, 0x00, 0x03, 0x1F);
		dcs_write_seq(dev, OTM_CMD_PASET, 0x00, 0x00, 0x01, 0xDF);
	}

	/* Content Adaptive Backlight Control */
	dcs_write_seq(dev, OTM_CMD_WRDISBV, 0x7F);
	dcs_write_seq(dev, OTM_CMD_WRCTRLD, 0x2C);
	dcs_write_seq(dev, OTM_CMD_WRCABC, 0x02);
	dcs_write_seq(dev, OTM_CMD_WRCABCMB, 0xFF);

	dcs_write_seq(dev, OTM_CMD_DISPON, 0x00);
	dcs_write_seq(dev, OTM_CMD_NOP, 0x00);
	dcs_write_seq(dev, OTM_CMD_RAMWR, 0x00);
}
