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

#endif /* !_MIPS_MICROCHIP_PIC32MZ_H_ */
