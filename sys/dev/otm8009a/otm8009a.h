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

#ifndef	_DEV_OTM8009A_OTM8009A_H_
#define	_DEV_OTM8009A_OTM8009A_H_

#define	MCS_ADRSFT		0x0000 /* W Address Shift Function (1) */
#define	MCS_CMD2_ENA1		0xFF00 /* W Enable Access Command2 "CMD2" (3) */
#define	MCS_CMD2_ENA2		0xFF80 /* W Enable Access Orise Command2 (2) */
#define	MCS_OTPSEL		0xA000 /* W/R OTP Select Region (1) */
#define	MCS_MIPISET1		0xB080 /* W/R MIPI Setting1 (4) */
#define	MCS_MIPISET2		0xB0A1 /* W/R MIPI Setting2 (5) */
#define	MCS_IF_PARA1		0xB280 /* W/R IF Parameter 1 (1) */
#define	MCS_IF_PARA2		0xB282 /* W/R IF Parameter 2 (1) */
#define	MCS_PAD_PARA		0xB390 /* W/R IOPAD Parameter (3) */
#define	MCS_RAMPWRSET		0xB3C0 /* W/R SRAM Power Setting (2) */
#define	MCS_TSP1		0xC080 /* W/R TCON Setting Parameter1 (9) */
#define	MCS_PTSP1		0xC092 /* W/R Panel Timing Setting Parameter1 (2) */
#define	MCS_PTSP2		0xC094 /* W/R Panel Timing Setting Parameter2 (1) */
#define	MCS_SD_CTRL		0xC0A2 /* W/R Source Driver Timing Setting (6) */
#define	MCS_P_DRV_M		0xC0B4 /* W/R Panel Driving Mode (1) */
#define	MCS_OSC_ADJ		0xC181 /* W/R Oscillator Adjustment for Idle/Normal Mode (1) */
#define	MCS_RGB_VIDEO_SET	0xC1A1 /* W/R RGB Video Mode Setting (1) */
#define	MCS_SD_PCH_CTRL		0xC480 /* W/R Source Driver Precharge Control (9) */
#define	MCS_PWR_CTRL1		0xC580 /* W/R Power Control Setting 1 (4) */
#define	MCS_PWR_CTRL2		0xC590 /* W/R Power Control Setting 2 for Normal Mode (7) */
#define	MCS_PWR_CTRL3		0xC5A0 /* W/R Power Control Setting 3 for Idle Mode (7) */
#define	MCS_PWR_CTRL4		0xC5B0 /* W/R Power Control Setting 4 for DC Voltage Settings (2) */
#define	MCS_PWM_PARA1		0xC680 /* W/R PWM_PARA1 (C680H) PWM Parameter 1 (1) */
#define	MCS_PWM_PARA2		0xC6B0 /* W/R PWM_PARA2 (C6B0H) PWM Parameter 2 (1) */
#define	MCS_PWM_PARA3		0xC6B1 /* W/R PWM_PARA3 (C6B1H) PWM Parameter 3 (1) */
#define	MCS_PWM_PARA4		0xC6B2 /* W/R PWM_PARA4 (C6B3H) PWM Parameter 4 (1) */
#define	MCS_PWM_PARA5		0xC6B3 /* W/R PWM_PARA5 (C6B4H) PWM Parameter 5 (1) */
#define	MCS_PWM_PARA6		0xC6B4 /* W/R PWM_PARA6 (C6B5H) PWM Parameter 6 (1) */
#define	MCS_CABCSET1		0xC700 /* W/R CABC Setting (1) */
#define	MCS_CABCSET2		0xC800 /* W/R CABC Gamma Curve Setting (18) */
#define	MCS_AIESET		0xC900 /* W/R AIE Setting (18) */
#define	MCS_PANCTRLSET1		0xCB80 /* W/R Panel Control Setting 1 (10) */
#define	MCS_PANCTRLSET2		0xCB90 /* W/R Panel Control Setting 2 (15) */
#define	MCS_PANCTRLSET3		0xCBA0 /* W/R Panel Control Setting 3 (15) */
#define	MCS_PANCTRLSET4		0xCBB0 /* W/R Panel Control Setting 4 (10) */
#define	MCS_PANCTRLSET5		0xCBC0 /* W/R Panel Control Setting 5 (15) */
#define	MCS_PANCTRLSET6		0xCBD0 /* W/R Panel Control Setting 6 (15) */
#define	MCS_PANCTRLSET7		0xCBE0 /* W/R Panel Control Setting 7 (10) */
#define	MCS_PANCTRLSET8		0xCBF0 /* W/R Panel Control Setting 8 (10) */
#define	MCS_PANU2D1		0xCC80 /* W/R Panel U2D Setting 1 (10) */
#define	MCS_PANU2D2		0xCC90 /* W/R Panel U2D Setting 2 (15) */
#define	MCS_PANU2D3		0xCCA0 /* W/R Panel U2D Setting 3 (15) */
#define	MCS_PAND2U1		0xCCB0 /* W/R Panel D2U Setting 1 (10) */
#define	MCS_PAND2U2		0xCCC0 /* W/R Panel D2U Setting 2 (15) */
#define	MCS_PAND2U3		0xCCD0 /* W/R Panel D2U Setting 3 (15) */
#define	MCS_GOAVST		0xCE80 /* W/R GOA VST Setting (12) */
#define	MCS_GOAVEND		0xCE90 /* W/R GOA VEND Setting (12) */
#define	MCS_GOAGPSET		0xCE9C /* W/R GOA Group Setting (2) */
#define	MCS_GOACLKA1		0xCEA0 /* W/R GOA CLKA1 Setting (7) */
#define	MCS_GOACLKA2		0xCEA7 /* W/R GOA CLKA2 Setting (7) */
#define	MCS_GOACLKA3		0xCEB0 /* W/R GOA CLKA3 Setting (7) */
#define	MCS_GOACLKA4		0xCEB7 /* W/R GOA CLKA4 Setting (7) */
#define	MCS_GOACLKB1		0xCEC0 /* W/R GOA CLKB1 Setting (7) */
#define	MCS_GOACLKB2		0xCEC7 /* W/R GOA CLKB2 Setting (7) */
#define	MCS_GOACLKB3		0xCED0 /* W/R GOA CLKB3 Setting (7) */
#define	MCS_GOACLKB4		0xCED7 /* W/R GOA CLKB4 Setting (7) */
#define	MCS_GOACLKC1		0xCF80 /* W/R GOA CLKC1 Setting (7) */
#define	MCS_GOACLKC2		0xCF87 /* W/R GOA CLKC2 Setting (7) */
#define	MCS_GOACLKC3		0xCF90 /* W/R GOA CLKC3 Setting (7) */
#define	MCS_GOACLKC4		0xCF97 /* W/R GOA CLKC4 Setting (7) */
#define	MCS_GOACLKD1		0xCFA0 /* W/R GOA CLKD1 Setting (7) */
#define	MCS_GOACLKD2		0xCFA7 /* W/R GOA CLKD2 Setting (7) */
#define	MCS_GOACLKD3		0xCFB0 /* W/R GOA CLKD3 Setting (7) */
#define	MCS_GOACLKD4		0xCFB7 /* W/R GOA CLKD4 Setting (7) */
#define	MCS_GOAECLK		0xCFC0 /* W/R GOA ECLK Setting (6) */
#define	MCS_GOAOPT1		0xCFC6 /* W/R GOA Other Options 1 (1) */
#define	MCS_GOATGOPT		0xCFC7 /* W/R GOA Signal Toggle Option Setting (3) */
#define	MCS_WRID1		0xD000 /* W/R ID1 Setting (1) */
#define	MCS_WRID2		0xD100 /* W/R ID2/ID3 Setting (2) */
#define	MCS_WRDDB		0xD200 /* W/R DDB Setting (4) */
#define	MCS_EXTCCHK		0xD300 /* R EXTC Check (1) */
#define	MCS_CESET1		0xD400 /* W/R CE Correction Characteristics Setting1 (360) */
#define	MCS_CESET2		0xD500 /* W/R CE Correction Characteristics Setting2 (360) */
#define	MCS_CEEN		0xD680 /* W/R CE Enable (1) */
#define	MCS_AIEEN		0xD700 /* W/R AIE Enable (1) */
#define	MCS_GVDDSET		0xD800 /* W/R GVDD/NGVDD Setting (2) */
#define	MCS_VCOMDC		0xD900 /* W/R VCOM Voltage Setting (1) */
#define	MCS_GMCT22P		0xE100 /* W/R Gamma Correction Characteristics Setting (2.2 + ) (16) */
#define	MCS_GMCT22N		0xE200 /* W/R Gamma Correction Characteristics Setting (2.2 - ) (16) */
#define	MCS_GMCT18P		0xE300 /* W/R Gamma Correction Characteristics Setting (1.8 + ) (16) */
#define	MCS_GMCT18N		0xE400 /* W/R Gamma Correction Characteristics Setting (1.8 - ) (16) */
#define	MCS_GMCT25P		0xE500 /* W/R Gamma Correction Characteristics Setting (2.5 + ) (16) */
#define	MCS_GMCT25N		0xE600 /* W/R Gamma Correction Characteristics Setting (2.5 - ) (16) */
#define	MCS_GMCT10P		0xE700 /* W/R Gamma Correction Characteristics Setting (1.0 + ) (16) */
#define	MCS_GMCT10N		0xE800 /* W/R Gamma Correction Characteristics Setting (1.0 - ) (16) */
#define	MCS_NVMIN		0xEB00 /* W NV Memory Write Mode (1) */
#define	MCS_DGAMR		0xEC00 /* W/R Digital Gamma Correction Characteristics Setting (Red) (17) */
#define	MCS_DGAMG		0xED00 /* W/R Digital Gamma Correction Characteristics Setting (Green) (17) */
#define	MCS_DGAMB		0xEE00 /* W/R Digital Gamma Correction Characteristics Setting (Blue) (17) */
#define	MCS_PRG_FLAG		0xF101 /* R OTP Program flag Check (3) */

#define	OTM_CMD_NOP		0x00 /* C No Operation (0) */
#define	OTM_CMD_SWRESET		0x01 /* C Software reset (0) */
#define	OTM_CMD_RDNUMED		0x05 /* R Read Number of the Errors on DSI (1) */
#define	OTM_CMD_RDDPM		0x0A /* R Read Display Power Mode (1) */
#define	OTM_CMD_RDDMADCTL	0x0B /* R Read Display MADCTL (1) */
#define	OTM_CMD_RDDCOLMOD	0x0C /* R Read Display Pixel Format (1) */
#define	OTM_CMD_RDDIM		0x0D /* R Read Display Image Mode (1) */
#define	OTM_CMD_RDDSM		0x0E /* R Read Display Signal Mode (1) */
#define	OTM_CMD_RDDSDR		0x0F /* R Read Display Self Diagnostic Result (1) */
#define	OTM_CMD_SLPIN		0x10 /* C Sleep in (0) */
#define	OTM_CMD_SLPOUT		0x11 /* C Sleep out (0) */
#define	OTM_CMD_PTLON		0x12 /* C Partial Mode On (0) */
#define	OTM_CMD_NORON		0x13 /* C Normal Display Mode On (0) */
#define	OTM_CMD_INVOFF		0x20 /* C Display Inversion Off (0) */
#define	OTM_CMD_INVON		0x21 /* C Display Inversion On (0) */
#define	OTM_CMD_ALLPOFF		0x22 /* C All Pixels Off (0) */
#define	OTM_CMD_ALLPON		0x23 /* C All Pixels On (0) */
#define	OTM_CMD_GAMSET		0x26 /* W Gamma Set (1) */
#define	OTM_CMD_DISPOFF		0x28 /* C Display off (0) */
#define	OTM_CMD_DISPON		0x29 /* C Display on (0) */
#define	OTM_CMD_CASET		0x2A /* W Column Address Set (4) */
#define	OTM_CMD_PASET		0x2B /* W Page Address Set (4) */
#define	OTM_CMD_RAMWR		0x2C /* W Memory Write (Any) */
#define	OTM_CMD_RAMRD		0x2E /* R Memory Read (Any) */
#define	OTM_CMD_PLTAR		0x30 /* W Partial area (4) */
#define	OTM_CMD_TEOFF		0x34 /* C Tearing Effect Line Off (0) */
#define	OTM_CMD_TEEON		0x35 /* W Tearing Effect Line On (1) */
#define	OTM_CMD_MADCTL		0x36 /* W Memory Access Control (1) */
#define	 MADCTL_RGB		(1 << 3) /* BGR order */
#define	 MADCTL_ML		(1 << 4)
#define	 MADCTL_MV		(1 << 5)
#define	 MADCTL_MX		(1 << 6)
#define	 MADCTL_MY		(1 << 7)
#define	OTM_CMD_IDMOFF		0x38 /* C Idle Mode off (0) */
#define	OTM_CMD_IDMON		0x39 /* C Idle Mode on (0) */
#define	OTM_CMD_COLMOD		0x3A /* C Interface Pixel Format (1) */
#define	 COLMOD_IFPF_S		0 /* MCU Interface Color Format */
#define	 COLMOD_IFPF_12		(0x3 << COLMOD_IFPF_S)
#define	 COLMOD_IFPF_16		(0x5 << COLMOD_IFPF_S)
#define	 COLMOD_IFPF_18		(0x6 << COLMOD_IFPF_S)
#define	 COLMOD_IFPF_24		(0x7 << COLMOD_IFPF_S)
#define	 COLMOD_VIPF_S		4 /* RGB Interface Color Format */
#define	 COLMOD_VIPF_16		(0x5 << COLMOD_VIPF_S)
#define	 COLMOD_VIPF_18		(0x6 << COLMOD_VIPF_S)
#define	 COLMOD_VIPF_24_1	(0x7 << COLMOD_VIPF_S) /* (1-times data transfer) */
#define	 COLMOD_VIPF_24_3	(0xe << COLMOD_VIPF_S) /* (3-times data transfer) */
#define	OTM_CMD_RAMWRC		0x3C /* W Memory Write Continue (Any) */
#define	OTM_CMD_RAMRDC		0x3E /* R Memory Read Continue (Any) */
#define	OTM_CMD_WRTESCN		0x44 /* W Write TE Scan Line (2) */
#define	OTM_CMD_RDSCNL		0x45 /* R Read Scan Line (2) */
#define	OTM_CMD_WRDISBV		0x51 /* W Write Display Brightness (1) */
#define	OTM_CMD_RDDISBV		0x52 /* R Read Display Brightness Value (1) */
#define	OTM_CMD_WRCTRLD		0x53 /* W Write CTRL Display (1) */
#define	OTM_CMD_RDCTRLD		0x54 /* R Read CTRL Display (1) */
#define	OTM_CMD_WRCABC		0x55 /* W Write Content Adaptive Brightness Control (1) */
#define	OTM_CMD_RDCABC		0x56 /* R Read Content Adaptive Brightness Control (1) */
#define	OTM_CMD_WRCABCMB	0x5E /* W Write CABC Minimum Brightness (1) */
#define	OTM_CMD_RDCABCMB	0x5F /* R Read CABC Minimum Brightness (1) */
#define	OTM_CMD_RDABCSDR	0x68 /* R Read Automatic Brightness Control Self-diagnostics Result (1) */
#define	OTM_CMD_RDBWLB		0x70 /* R Read Black/White Low Bits (1) */
#define	OTM_CMD_RDBKx		0x71 /* R Read Bkx (1) */
#define	OTM_CMD_RDBKy		0x72 /* R Read Bky (1) */
#define	OTM_CMD_RDWx		0x73 /* R Read Wx (1) */
#define	OTM_CMD_RDWy		0x74 /* R Read Wy (1) */
#define	OTM_CMD_RDRGLB		0x75 /* R Read Red/Green Low Bits (1) */
#define	OTM_CMD_RDRx		0x76 /* R Read Rx (1) */
#define	OTM_CMD_RDRy		0x77 /* R Read Ry (1) */
#define	OTM_CMD_RDGx		0x78 /* R Read Gx (1) */
#define	OTM_CMD_RDGy		0x79 /* R Read Gy (1) */
#define	OTM_CMD_RDBALB		0x7A /* R Read Blue/AColour Low Bits (1) */
#define	OTM_CMD_RDBx		0x7B /* R Read Bx (1) */
#define	OTM_CMD_RDBy		0x7C /* R Read By (1) */
#define	OTM_CMD_RDAx		0x7D /* R Read Ax (1) */
#define	OTM_CMD_RDAy		0x7E /* R Read Ay (1) */
#define	OTM_CMD_RDDDBS		0xA1 /* R Read DDB Start (5) */
#define	OTM_CMD_RDDDBC		0xA8 /* R Read DDB Continue (Any) */
#define	OTM_CMD_RDFCS		0xAA /* R Read First Checksum (1) */
#define	OTM_CMD_RDCCS		0xAF /* R Read Continue Checksum (1) */
#define	OTM_CMD_RDID1		0xDA /* R Read ID1 (1) */
#define	OTM_CMD_RDID2		0xDB /* R Read ID2 (1) */
#define	OTM_CMD_RDID3		0xDC /* R Read ID3 (1) */

/* Undocumented commands */
#define	MCS_PANSET	0xB3A6 /* Panel Type Setting */
#define	MCS_NO_DOC1	0xC48A
#define	MCS_NO_DOC2	0xCFD0
#define	MCS_NO_DOC3	0xF5B6

enum otm_format {
	OTM_FMT_RGB565,
	OTM_FMT_RGB888,
};

enum otm_orientation {
	OTM_ORIENTATION_PORTRAIT,
	OTM_ORIENTATION_LANDSCAPE,
};

void otm8009a_init(dsi_device_t *dev, enum otm_format fmt,
    enum otm_orientation orientation);

#endif /* !_DEV_OTM8009A_OTM8009A_H_ */
