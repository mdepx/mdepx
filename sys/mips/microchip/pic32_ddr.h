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

/*
 * 38.0 DDR2 SDRAM Controller
 * PIC32MZ Graphics (DA) Family (Document DS60001361E)
 */

#ifndef	_MIPS_MICROCHIP_PIC32_DDR_H_
#define	_MIPS_MICROCHIP_PIC32_DDR_H_

#define	DDR_TSEL		0x00
#define	DDR_MINLIM		0x04
#define	DDR_RQPER		0x08
#define	DDR_MINCMD		0x0C
#define	DDR_MEMCON		0x10
#define	 STINIT			(1 << 0)	/* Memory Initialize Start bit */
#define	 INITDN			(1 << 1)	/* Memory Initialize Done bit */
#define	DDR_MEMCFG0		0x14
#define	 RWADDR_S		0		/* Row Address Select Shift bits */
#define	 RWADDR_M		(0xf << RWADDR_S)
#define	 BNKADDR_S		8		/* Bank Address Select Shift bits */
#define	 BNKADDR_M		(0x1f << BNKADDR_S)
#define	 CSADDR_S		16		/* Chip Select Shift bits */
#define	 CSADDR_M		(0x1f << CSADDR_S)
#define	 CLHADDR_S		24		/* Column Address Shift bits */
#define	 CLHADDR_M		(0x1f << CLHADDR_S)
#define	 APCHRGEN		(1 << 30)	/* Automatic Precharge Enable bit */
#define	DDR_MEMCFG1		0x18
#define	 RWADDRMSK_S		0		/* Row Address Mask bits */
#define	 RWADDRMSK_M		(0x1fff << RWADDRMSK_S)
#define	DDR_MEMCFG2		0x1C
#define	 CLADDRHMSK_S		0		/* Column Address High Mask bits */
#define	 CLADDRHMSK_M		(0x1fff << CLADDRHMSK_S)
#define	DDR_MEMCFG3		0x20
#define	 CLADDRLMSK_S		0		/* Column Address Low Mask bits */
#define	 CLADDRLMSK_M		(0x1fff << CLADDRLMSK_S)
#define	DDR_MEMCFG4		0x24
#define	 BNKADDRMSK_S		0		/* Bank Address Mask bits */
#define	 BNKADDRMSK_M		(0x7 << BNKADDRMSK_S)
#define	 CSADDRMSK_S		6		/* Chip Select Address Mask bits */
#define	 CSADDRMSK_M		(0x7 << CSADDRMSK_S)
#define	DDR_REFCFG		0x28
#define	 REFCNT_S		0		/* Refresh Count bits */
#define	 REFCNT_M		(0xffff << REFCNT_S)
#define	 REFDLY_S		16		/* Minimum Refresh-to-Refresh Delay bits */
#define	 REFDLY_M		(0xff << REFDLY_S)
#define	 MAXREFS_S		24		/* Maximum Pending Refreshes bits */
#define	 MAXREFS_M		(0x7 << MAXREFS_S)
#define	DDR_PWRCFG		0x2C
#define	 APWRDNEN		(1 << 2)	/* Automatic Power Down Enable bit */
#define	 ASLFREFEN		(1 << 3)	/* Automatic Self Refresh Enable bit */
#define	 PWDNDLY_S		4		/* Refresh Count bits */
#define	 PWDNDLY_M		(0xff << PWDNDLY_S)
#define	 SLFREFDLY_S		12		/* Self Refresh Delay bits */
#define	 SLFREFDLY_M		(0x3ff << SLFREFDLY_S)
#define	 PCHRGPWDN		(1 << 22)	/* Precharge Power Down Only bit */
#define	DDR_DLYCFG0		0x30
#define	 CFG0_W2RDLY_S		0		/* Write-to-Read Delay bits */
#define	 CFG0_W2RDLY_M		(0xf << CFG0_W2RDLY_S)
#define	 CFG0_W2RCSDLY_S	4		/* Write-to-Read Chip Select Delay bits */
#define	 CFG0_W2RCSDLY_M	(0xf << CFG0_W2RCSDLY_S)
#define	 R2RDLY_S		8		/* Read-to-Read Delay bits */
#define	 R2RDLY_M		(0xf << R2RDLY_S)
#define	 R2RCSDLY_S		12		/* Read-to-Read Chip Select Delay bits */
#define	 R2RCSDLY_M		(0xf << R2RCSDLY_S)
#define	 W2WDLY_S		16		/* Write-to-Write Delay bits */
#define	 W2WDLY_M		(0xf << W2WDLY_S)
#define	 W2WCSDLY_S		20		/* Write-to-Write Chip Select Delay bits */
#define	 W2WCSDLY_M		(0xf << W2WCSDLY_S)
#define	 R2WDLY_S		24		/* Read-to-Write Delay bits */
#define	 R2WDLY_M		(0xf << R2WDLY_S)
#define	 RMWDLY_S		28		/* Read-Modify-Write Delay bits */
#define	 RMWDLY_M		(0xf << RMWDLY_S)
#define	DDR_DLYCFG1		0x34
#define	 SLFREFMINDLY_S		0		/* Self Refresh Minimum Delay bits */
#define	 SLFREFMINDLY_M		(0xff << SLFREFMINDLY_S)
#define	 SLFREFEXDLY_S		8		/* Self Refresh Exit Delay bits */
#define	 SLFREFEXDLY_M		(0xff << SLFREFEXDLY_S)
#define	 PWRDNMINDLY_S		16		/* Power Down Minimum Delay bits */
#define	 PWRDNMINDLY_M		(0xf << PWRDNMINDLY_S)
#define	 PWRDNEXDLY_S		20		/* Power Down Exit Delay bits */
#define	 PWRDNEXDLY_M		(0x3f << PWRDNEXDLY_S)
#define	 CFG1_W2PCHRGDLY_S	26		/* Write to Precharge Delay bit */
#define	 CFG1_W2PCHRGDLY_M	(1 << CFG1_W2PCHRGDLY_S)
#define	 CFG1_W2RDLY_S		27		/* Write-to-Read Delay bit */
#define	 CFG1_W2RDLY_M		(0x1 << CFG1_W2RDLY_S)
#define	 CFG1_W2RCSDLY_S	28		/* Write-to-Read Chip Select Delay bit */
#define	 CFG1_W2RCSDLY_M	(0x1 << CFG1_W2RCSDLY_S)
#define	 CFG1_NXTDATAVDLY_S	29		/* Next Data Available Delay bit */
#define	 CFG1_NXTDATAVDLY_M	(0x1 << CFG1_NXTDATAVDLY_S)
#define	 SLFREFEXDLY9_S		30		/* Self Refresh Exit Delay bit */
#define	 SLFREFEXDLY9_M		(0x1 << SLFREFEXDLY9_S)
#define	DDR_DLYCFG2		0x38
#define	 PCHRGALLDLY_S		0		/* Precharge All Delay bits */
#define	 PCHRGALLDLY_M		(0xf << PCHRGALLDLY_S)
#define	 R2PCHRGDLY_S		8		/* Read-to-Precharge Delay bits */
#define	 R2PCHRGDLY_M		(0xf << R2PCHRGDLY_S)
#define	 CFG2_W2PCHRGDLY_S	12		/* Write-to-Precharge Delay bits */
#define	 CFG2_W2PCHRGDLY_M	(0xf << CFG2_W2PCHRGDLY_S)
#define	 RAS2RASDLY_S		16		/* Write-to-Read Delay bits */
#define	 RAS2RASDLY_M		(0xf << RAS2RASDLY_S)
#define	 RAS2CASDLY_S		20		/* RAS-to-CAS Delay bits */
#define	 RAS2CASDLY_M		(0xf << RAS2CASDLY_S)
#define	 PCHRG2RASDLY_S		24		/* Precharge-to-RAS Delay bits */
#define	 PCHRG2RASDLY_M		(0xf << PCHRG2RASDLY_S)
#define	 RBENDDLY_S		28		/* Read Burst End Delay bits */
#define	 RBENDDLY_M		(0xf << RBENDDLY_S)
#define	DDR_DLYCFG3		0x3C
#define	 RAS2PCHRGDLY_S		0		/* RAS-to-Precharge Delay bits */
#define	 RAS2PCHRGDLY_M		(0x1f << RAS2PCHRGDLY_S)
#define	 RAS2RASSBNKDLY_S	8		/* RAS-to-RAS Same Bank Delay bits */
#define	 RAS2RASSBNKDLY_M	(0x3f << RAS2RASSBNKDLY_S)
#define	 FAWTDLY_S		16		/* Four Activate Window Time Delay bits */
#define	 FAWTDLY_M		(0x3f << FAWTDLY_S)
#define	DDR_ODTCFG		0x40
#define	 ODTCSEN_S		0		/* On-Die Termination Chip Select Enable bits */
#define	 ODTCSEN_M		(0xff << ODTCSEN_S)
#define	 ODTRDLY_S		8		/* On-Die Termination Read Delay bits */
#define	 ODTRDLY_M		(0xf << ODTRDLY_S)
#define	 ODTWDLY_S		12		/* On-Die Termination Write Delay bits */
#define	 ODTWDLY_M		(0xf << ODTWDLY_S)
#define	 ODTRLEN_S		16		/* On-Die Termination Read Length bits */
#define	 ODTRLEN_M		(0x7 << ODTRLEN_S)
#define	 ODTWLEN_S		20		/* On-Die Termination Write Length bits */
#define	 ODTWLEN_M		(0x7 << ODTWLEN_S)
#define	DDR_XFERCFG		0x44
#define	 NXTDATRQDLY_S		0		/* Next Data Request Delay bits */
#define	 NXTDATRQDLY_M		(0xf << NXTDATRQDLY_S)
#define	 NXTDATAVDLY_S		4		/* Next Data Available Delay bits */
#define	 NXTDATAVDLY_M		(0xf << NXTDATAVDLY_S)
#define	 RDATENDLY_S		16		/* PHY Read Data Enable Delay bits */
#define	 RDATENDLY_M		(0xf << RDATENDLY_S)
#define	 MAXBURST_S		24		/* Maximum Command Burst Count bits */
#define	 MAXBURST_M		(0xf << MAXBURST_S)
#define	 BIGENDIAN		(1 << 31)	/* Big Endian bit */
#define	DDR_CMDISSUE		0x48
#define	 NUMHOSTCMDS_S		0		/* Number of Host Commands bits */
#define	 NUMHOSTCMDS_M		(0xf << NUMHOSTCMDS_S)
#define	 VALID			(1 << 4)	/* Host Command Valid bit */
#define	DDR_ODTENCFG		0x4C
#define	 ODTWEN			(1 << 16)	/* On-Die Termination Write Enable bit */
#define	 ODTREN			(1 << 0)	/* On-Die Termination Read Enable bit */
#define	DDR_MEMWIDTH		0x50
#define	 MEMWIDTH_HALFRATE	(1 << 3)	/* Half-rate Mode bit */
#define	DDR_CMD1(x)		(0x80 + x * 0x4)
#define	 CLKENCMD1		(1 << 0)	/* Clock Enable Command 1 bit */
#define	 CSCMD1_S		1		/* Chip Select Command 1 bit */
#define	 CSCMD1_M		(0xff << CSCMD1_S)
#define	 RASCMD1		(1 << 9)	/* Row Address Strobe Command 1 bit */
#define	 CASCMD1		(1 << 10)	/* Column Address Strobe Command 1 bit */
#define	 WENCMD1		(1 << 11)	/* Write Enable Command 1 bit */
#define	 CLKENCMD2		(1 << 12)	/* Clock Enable Command 2 bit */
#define	 CSCMD2_S		13		/* Chip Select Command 2 bits */
#define	 CSCMD2_M		(0xff << CSCMD2_S)
#define	 RASCMD2		(1 << 21)	/* Row Address Strobe Command 2 bit */
#define	 CASCMD2		(1 << 22)	/* Column Address Strobe Command 2 bit */
#define	 WENCMD2		(1 << 23)	/* Write Enable Command 2 bit */
#define	 MDALCMD_S		24		/* Mode Address Low Command bits */
#define	 MDALCMD_M		(0xff << MDALCMD_S)
#define	DDR_CMD2(x)		(0xC0 + x * 0x4)
#define	DDR_SCLSTART		0x1100
#define	 SCLLBPASS		(1 << 0)	/* Self Calibration Logic Lower Data Byte Status bit */
#define	 SCLUBPASS		(1 << 1)	/* Self Calibration Logic Upper Data Byte Status bit */
#define	 SCLEN			(1 << 26)	/* Self Calibration Logic Enable bit */
#define	 SCLSTART		(1 << 28)	/* Start Self Calibration Logic bit */
#define	DDR_SCLLAT		0x110C
#define	 DDRCLKDLY_S		4		/* DDR Clock Delay bit */
#define	 DDRCLKDLY_M		(0xf << DDRCLKDLY_S)
#define	 CAPCLKDLY_S		0		/* Capture Clock Delay bit */
#define	 CAPCLKDLY_M		(0xf << CAPCLKDLY_S)
#define	DDR_SCLCFG0		0x1118
#define	 ODTCSW			(1 << 24)	/* On-Die Termination Chip Select Write bit */
#define	 RCASLAT_S		4		/* Read CAS Latency bits */
#define	 RCASLAT_M		(0xf << RCASLAT_S)
#define	 DDR2			(1 << 1)	/* DDR2 bit */
#define	 BURST8			(1 << 0)	/* PHY Burst 8 bit */
#define	DDR_SCLCFG1		0x111C
#define	 DBLREFDLY		(1 << 12)	/* Double Reference Delay bit */
#define	 WCASLAT_S		8		/* Write CAS Latency bits */
#define	 WCASLAT_M		(0xf << WCASLAT_S)
#define	 SCLCSEN		(1 << 0)	/* SCL Chip Select Enable bit */
#define	DDR_PHYPADCON		0x1120
#define	 PREAMBDLY_S		29		/* Preamble Delay bits */
#define	 PREAMBDLY_M		(0x3 << PREAMBDLY_S)
#define	 RCVREN			(1 << 28)	/* Receiver Enable bit */
#define	 DRVSTRPFET_S		20		/* PFET Drive Strength bits */
#define	 DRVSTRPFET_M		(0xf << DRVSTRPFET_S)
#define	 DRVSTRNFET_S		16		/* NFET Drive Strength bits */
#define	 DRVSTRNFET_M		(0xf << DRVSTRNFET_S)
#define	 HALFRATE		(1 << 14)	/* Half Rate bit */
#define	 WRCMDDLY		(1 << 13)	/* Write Command Delay bit */
#define	 NOEXTDLL		(1 << 9)	/* No External DLL bit */
#define	 EOENCLKCYC		(1 << 8)	/* Extra Output Enable bit */
#define	 ODTPUCAL_S		6		/* On-Die Termination Pull-up Calibration bits */
#define	 ODTPUCAL_M		(0x3 << ODTPUCAL_S)
#define	 ODTPFDCAL_S		4		/* On-Die Termination Pull-down Calibration bits */
#define	 ODTPFDCAL_M		(0x3 << ODTPFDCAL_S)
#define	 ADDCDRVSEL		(1 << 3)	/* Address and Control Pads Drive Strength Select bit */
#define	 DATDRVSEL		(1 << 2)	/* Data Pad Drive Strength Select bit */
#define	 ODTEN			(1 << 1)	/* On-Die Termination Enable bit */
#define	 ODTSEL			(1 << 0)	/* On-Die Termination Select bit */
#define	DDR_PHYDLLR		0x1124
#define	 DISRECALIB		(1 << 26)	/* Disable Recalibration bit */
#define	 RECALIBCNT_S		8		/* Recalibration Count bits */
#define	 RECALIBCNT_M		(0x3ffff << RECALIBCNT_S)
#define	 DLYSTVAL_S		28		/* Delay Start Value bits */
#define	 DLYSTVAL_M		(0xf << DLYSTVAL_S)
#define	DDR_PHYCLKDLY		0x1140

struct pic32_ddr_softc {
	uint32_t base;
};

#endif	/* !_MIPS_MICROCHIP_PIC32_DDR_H_ */
