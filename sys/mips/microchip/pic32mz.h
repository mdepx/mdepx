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

#ifndef	_MIPS_MICROCHIP_PIC32MZ_H_
#define	_MIPS_MICROCHIP_PIC32MZ_H_

#define	PORTS_BASE	0xBF860000
#define	PPS_BASE	0xBF801400
#define	UART2_BASE	0xBF822200
#define	SYSCFG_BASE	0xBF800000

#define	DEVCFG0_EJTAGBEN		(1 << 30) /* EJTAG Boot Enable bit */
#define	DEVCFG0_POSCAGC			(1 << 27) /* Primary Oscillator Auto Gain Control bit */
#define	DEVCFG0_POSCTYPE_S		24 /* Primary Oscillator Type bits */
#define	DEVCFG0_POSCTYPE_M		(0x3 << DEVCFG0_POSCTYPE_S)
#define	DEVCFG0_POSCTYPE_12MHZ		(0x3 << DEVCFG0_POSCTYPE_S)
#define	DEVCFG0_POSCTYPE_24MHZ		(0x2 << DEVCFG0_POSCTYPE_S)
#define	DEVCFG0_POSCTYPE_RESONATOR	(0x1 << DEVCFG0_POSCTYPE_S)
#define	DEVCFG0_POSCTYPE_8MHZ		(0x0 << DEVCFG0_POSCTYPE_S)
#define	DEVCFG0_POSCBOOST		(1 << 21) /* Primary Oscillator Boost Kick Start Enable bit */
#define	DEVCFG0_POSCGAIN_S		19 /* Primary Oscillator Gain Control bits */
#define	DEVCFG0_POSCGAIN_M		(0x3 << DEVCFG0_POSCGAIN_S)
#define	DEVCFG0_POSCGAIN_GAIN_LEVEL_3	(0x3 << DEVCFG0_POSCGAIN_S) /* (highest) */
#define	DEVCFG0_POSCGAIN_GAIN_LEVEL_2	(0x2 << DEVCFG0_POSCGAIN_S)
#define	DEVCFG0_POSCGAIN_GAIN_LEVEL_1	(0x1 << DEVCFG0_POSCGAIN_S)
#define	DEVCFG0_POSCGAIN_GAIN_LEVEL_0	(0x0 << DEVCFG0_POSCGAIN_S) /* (lowest) */
#define	DEVCFG0_SOSCBOOST		(1 << 18) /* Secondary Oscillator Boost Kick Start Enable bit */
#define	DEVCFG0_SOSCGAIN_S		16 /* Secondary Oscillator Gain Control bits */
#define	DEVCFG0_SOSCGAIN_M		(0x3 << DEVCFG0_SOSCGAIN_S)
#define	DEVCFG0_SOSCGAIN_GAIN_LEVEL_3	(0x3 << DEVCFG0_SOSCGAIN_S) /* (highest) */
#define	DEVCFG0_SOSCGAIN_GAIN_LEVEL_2	(0x2 << DEVCFG0_SOSCGAIN_S)
#define	DEVCFG0_SOSCGAIN_GAIN_LEVEL_1	(0x1 << DEVCFG0_SOSCGAIN_S)
#define	DEVCFG0_SOSCGAIN_GAIN_LEVEL_0	(0x0 << DEVCFG0_SOSCGAIN_S) /* (lowest) */
#define	DEVCFG0_SMCLR			(1 << 15) /* Soft Master Clear Enable bit */
#define	DEVCFG0_DBGPER_S		12 /* Debug Mode CPU Access Permission bits */
#define	DEVCFG0_DBGPER_M		(0x7 << DEVCFG0_DBGPER_S)
#define	DEVCFG0_DBGPER_2		(4 << DEVCFG0_DBGPER_S) /* Allow CPU access to PG2 regions */
#define	DEVCFG0_DBGPER_1		(2 << DEVCFG0_DBGPER_S) /* Allow CPU access to PG1 regions */
#define	DEVCFG0_DBGPER_0		(1 << DEVCFG0_DBGPER_S) /* Allow CPU access to PG0 regions */
#define	DEVCFG0_FSLEEP			(1 << 10) /* Flash Sleep Mode bit */
#define	DEVCFG0_FECCCON_S		8 /* Dynamic Flash ECC Configuration bits */
#define	DEVCFG0_FECCCON_M		(0x3 << DEVCFG0_FECCCON_S)
#define	DEVCFG0_ECC_DIS_ECCCON_UNLOCKED	(0x3 << DEVCFG0_FECCCON_S)
#define	DEVCFG0_ECC_DIS			(0x2 << DEVCFG0_FECCCON_S)
#define	DEVCFG0_ECC_DYN_FLASH_ENABLED	(0x1 << DEVCFG0_FECCCON_S)
#define	DEVCFG0_ECC_ENABLED		(0x0 << DEVCFG0_FECCCON_S)
#define	DEVCFG0_BOOTISA_MIPS32		(1 << 6) /* Boot ISA Selection bit */
#define	DEVCFG0_TRCEN			(1 << 5) /* Trace Enable bit */
#define	DEVCFG0_ICESEL_S		3 /* In-Circuit Emulator/Debugger Communication Channel Select bits */
#define	DEVCFG0_ICESEL_M		(0x3 << DEVCFG0_ICESEL_S)
#define	DEVCFG0_ICESEL_PGEC1_PGED1	(0x3 << DEVCFG0_ICESEL_S)
#define	DEVCFG0_ICESEL_PGEC2_PGED2	(0x2 << DEVCFG0_ICESEL_S)
#define	DEVCFG0_JTAGEN			(1 << 2) /* JTAG Enable bit */
#define	DEVCFG0_DEBUG_S			0 /* Background Debugger (forced to `11' if code-protect is ON) */
#define	DEVCFG0_DEBUG_DISABLED		(2 << DEVCFG0_DEBUG_S)

#define	DEVCFG1_FDMTEN			(1 << 31) /* Deadman Timer enable bit */
#define	DEVCFG1_DMTCNT(n)		((n) << 26) /* Deadman Timer Count Select bits */
#define	DEVCFG1_FWDTWINSZ_S		24 /* Watchdog Timer Window Size bits */
#define	DEVCFG1_FWDTWINSZ_M		(0x3 << DEVCFG1_FWDTWINSZ_S)
#define	DEVCFG1_FWDTWINSZ_25		(0x3 << DEVCFG1_FWDTWINSZ_S)
#define	DEVCFG1_FWDTWINSZ_37_5		(0x2 << DEVCFG1_FWDTWINSZ_S)
#define	DEVCFG1_FWDTWINSZ_50		(0x1 << DEVCFG1_FWDTWINSZ_S)
#define	DEVCFG1_FWDTWINSZ_75		(0x0 << DEVCFG1_FWDTWINSZ_S)
#define	DEVCFG1_FWDTEN			(1 << 23) /* Watchdog Timer Enable bit */
#define	DEVCFG1_WINDIS			(1 << 22) /* Watchdog Timer Window Enable bit */
#define	DEVCFG1_WDTSPGM			(1 << 21) /* Watchdog Timer Stop During Flash Programming bit */
#define	DEVCFG1_WDTPS_S			16 /* Watchdog Timer Postscale Select bits */
#define	DEVCFG1_WDTPS_M			(0x1f << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_1048576		(20 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_524288		(19 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_262144		(18 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_131072		(17 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_64536		(16 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_32768		(15 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_16384		(14 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_8192		(13 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_4096		(12 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_2048		(11 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_1024		(10 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_512		(9 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_256		(8 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_128		(7 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_64		(6 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_32		(5 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_16		(4 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_8			(3 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_4			(2 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_2			(1 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_WDTPS_1			(0 << DEVCFG1_WDTPS_S)
#define	DEVCFG1_FCKSM_S			14 /* Clock Switching and Monitoring Selection Configuration bits */
#define	DEVCFG1_FCKSM_M			(0x3 << DEVCFG1_FCKSM_S)
#define	DEVCFG1_FCKSM_CKS_EN_M_EN	(0x3 << DEVCFG1_FCKSM_S)
#define	DEVCFG1_FCKSM_CKS_DIS_M_EN	(0x2 << DEVCFG1_FCKSM_S)
#define	DEVCFG1_FCKSM_CKS_EN_M_DIS	(0x1 << DEVCFG1_FCKSM_S)
#define	DEVCFG1_FCKSM_CKS_DIS_M_DIS	(0x0 << DEVCFG1_FCKSM_S)
#define	DEVCFG1_OSCIOFNC		(1 << 10) /* CLKO Enable Configuration bit */
#define	DEVCFG1_POSCMOD_S		8 /* Primary Oscillator Configuration bits */
#define	DEVCFG1_POSCMOD_M		(0x3 << DEVCFG1_POSCMOD_S)
#define	DEVCFG1_POSCMOD_DIS		(0x3 << DEVCFG1_POSCMOD_S)
#define	DEVCFG1_POSCMOD_HS_OSC		(0x2 << DEVCFG1_POSCMOD_S)
#define	DEVCFG1_POSCMOD_EC		(0x0 << DEVCFG1_POSCMOD_S)
#define	DEVCFG1_IESO			(1 << 7) /* Internal External Switchover bit */
#define	DEVCFG1_FSOSCEN			(1 << 6) /* Secondary Oscillator Enable bit */
#define	DEVCFG1_DMTINV_S		3 /* Deadman Timer Count Window Interval bits */
#define	DEVCFG1_DMTINV_M		(0x7 << DEVCFG1_DMTINV_S)
#define	DEVCFG1_DMTINV_127_128		(0x7 << DEVCFG1_DMTINV_S)
#define	DEVCFG1_DMTINV_63_64		(0x6 << DEVCFG1_DMTINV_S)
#define	DEVCFG1_DMTINV_31_32		(0x5 << DEVCFG1_DMTINV_S)
#define	DEVCFG1_DMTINV_15_16		(0x4 << DEVCFG1_DMTINV_S)
#define	DEVCFG1_DMTINV_7_8		(0x3 << DEVCFG1_DMTINV_S)
#define	DEVCFG1_DMTINV_3_4		(0x2 << DEVCFG1_DMTINV_S)
#define	DEVCFG1_DMTINV_1_2		(0x1 << DEVCFG1_DMTINV_S)
#define	DEVCFG1_DMTINV_0		(0x0 << DEVCFG1_DMTINV_S)
#define	DEVCFG1_FNOSC_S			0 /* Oscillator Selection bits */
#define	DEVCFG1_FNOSC_M			(0x7 << DEVCFG1_FNOSC_S)
#define	DEVCFG1_FNOSC_SPLL		(0x7 << DEVCFG1_FNOSC_S)
#define	DEVCFG1_FNOSC_LPRC		(0x5 << DEVCFG1_FNOSC_S)
#define	DEVCFG1_FNOSC_SOSC		(0x4 << DEVCFG1_FNOSC_S)
#define	DEVCFG1_FNOSC_POSC_HS_EC	(0x2 << DEVCFG1_FNOSC_S)
#define	DEVCFG1_FNOSC_SPLL		(0x1 << DEVCFG1_FNOSC_S)
#define	DEVCFG1_FNOSC_FRC		(0x0 << DEVCFG1_FNOSC_S)

#define	DEVCFG2_UPLLFSEL_24MHZ		(1 << 30) /* USB PLL Input Frequency Select bit */
#define	DEVCFG2_UPLLFSEL_12MHZ		(0 << 30) /* USB PLL Input Frequency Select bit */
#define	DEVCFG2_FDSEN			(1 << 28) /* Deep Sleep mode is entered on a WAIT instruction */
#define	DEVCFG2_DSWDTEN			(1 << 27) /* Deep Sleep Watchdog Timer Enable bit */
#define	DEVCFG2_DSWDTOSC		(1 << 26) /* Deep Sleep Watchdog Timer Reference Clock Select bit */
#define	DEVCFG2_DSWDTPS_S		21 /* Deep Sleep Watchdog Timer Postscale Select bits */
#define	DEVCFG2_DSWDTPS_M		(0x1f << DEVCFG2_DSWDTPS_S)
#define	DEVCFG2_DSWDTPS(n)		((n) << DEVCFG2_DSWDTPS_S)
#define	DEVCFG2_DSBOREN			(1 << 20) /* Deep Sleep BOR Enable bit */
#define	DEVCFG2_VBATBOREN		(1 << 19) /* VBAT BOR Enable bit */
#define	DEVCFG2_FPLLODIV_S		16 /* Default System PLL Output Divisor bits */
#define	DEVCFG2_FPLLODIV_M		(0x7 << DEVCFG2_FPLLODIV_S)
#define	DEVCFG2_FPLLODIV_32		(0x7 << DEVCFG2_FPLLODIV_S)
#define	DEVCFG2_FPLLODIV_16		(0x4 << DEVCFG2_FPLLODIV_S)
#define	DEVCFG2_FPLLODIV_8		(0x3 << DEVCFG2_FPLLODIV_S)
#define	DEVCFG2_FPLLODIV_4		(0x2 << DEVCFG2_FPLLODIV_S)
#define	DEVCFG2_FPLLODIV_2		(0x1 << DEVCFG2_FPLLODIV_S)
#define	DEVCFG2_FPLLMULT_S		8 /* System PLL Feedback Divider bits */
#define	DEVCFG2_FPLLMULT_M		(0x7f << DEVCFG2_FPLLMULT_S)
#define	DEVCFG2_FPLLMULT(n)		(((n) - 1) << DEVCFG2_FPLLMULT_S)
#define	DEVCFG2_FPLLICLK_FRC		(1 << 7) /* System PLL Input Clock Select bit */
#define	DEVCFG2_FPLLICLK_POSC		(0 << 7) /* System PLL Input Clock Select bit */
#define	DEVCFG2_FPLLRNG_S		4 /* System PLL Divided Input Clock Frequency Range bits */
#define	DEVCFG2_FPLLRNG_M		(0x7 << DEVCFG2_FPLLRNG_S)
#define	DEVCFG2_FPLLRNG_34_64		(0x5 << DEVCFG2_FPLLRNG_S)
#define	DEVCFG2_FPLLRNG_21_42		(0x4 << DEVCFG2_FPLLRNG_S)
#define	DEVCFG2_FPLLRNG_13_26		(0x3 << DEVCFG2_FPLLRNG_S)
#define	DEVCFG2_FPLLRNG_8_16		(0x2 << DEVCFG2_FPLLRNG_S)
#define	DEVCFG2_FPLLRNG_5_10		(0x1 << DEVCFG2_FPLLRNG_S)
#define	DEVCFG2_FPLLRNG_BYPASS		(0x0 << DEVCFG2_FPLLRNG_S)
#define	DEVCFG2_FPLLIDIV_S		0 /* PLL Input Divider bits */
#define	DEVCFG2_FPLLIDIV_M		(0x7 << DEVCFG2_FPLLIDIV_S)
#define	DEVCFG2_FPLLIDIV(n)		(((n) - 1) << DEVCFG2_FPLLIDIV_S)

#define	DEVCFG3_IOL1WAY			(1 << 29) /* Peripheral Pin Select Configuration bit */
#define	DEVCFG3_PMDL1WAY		(1 << 28) /* Peripheral Module Disable Configuration bit */
#define	DEVCFG3_PGL1WAY			(1 << 27) /* Permission Group Lock One Way Configuration bit */
#define	DEVCFG3_FETHIO			(1 << 25) /* Ethernet I/O Pin Selection Configuration bit */
#define	DEVCFG3_FMIIEN			(1 << 24) /* Ethernet MII Enable Configuration bit */
#define	DEVCFG3_EXTDDRSIZE_S		16 /* External DDR2 SDRAM Size bits */
#define	DEVCFG3_EXTDDRSIZE_M		(0xf << DEVCFG3_EXTDDRSIZE_S)
#define	DEVCFG3_EXTDDRSIZE_128MB	(0xf << DEVCFG3_EXTDDRSIZE_S)
#define	DEVCFG3_EXTDDRSIZE_64MB		(0x6 << DEVCFG3_EXTDDRSIZE_S)
#define	DEVCFG3_EXTDDRSIZE_32MB		(0x5 << DEVCFG3_EXTDDRSIZE_S)
#define	DEVCFG3_EXTDDRSIZE_16MB		(0x4 << DEVCFG3_EXTDDRSIZE_S)
#define	DEVCFG3_EXTDDRSIZE_8MB		(0x3 << DEVCFG3_EXTDDRSIZE_S)
#define	DEVCFG3_EXTDDRSIZE_4MB		(0x2 << DEVCFG3_EXTDDRSIZE_S)
#define	DEVCFG3_EXTDDRSIZE_2MB		(0x1 << DEVCFG3_EXTDDRSIZE_S)
#define	DEVCFG3_EXTDDRSIZE_1MB		(0x0 << DEVCFG3_EXTDDRSIZE_S)
#define	DEVCFG3_USERID_S		0
#define	DEVCFG3_USERID_M		(0xffff << DEVCFG3_USERID_S)

#endif /* !_MIPS_MICROCHIP_PIC32MZ_H_ */
