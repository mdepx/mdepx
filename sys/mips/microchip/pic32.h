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

#ifndef	_MIPS_MICROCHIP_PIC32_H_
#define	_MIPS_MICROCHIP_PIC32_H_

#define	PIC32_DEVCFG(cfg0, cfg1, cfg2, cfg3) \
	unsigned __DEVCFG0 __attribute__ ((section (".config0"))) = (cfg0); \
	unsigned __DEVCFG1 __attribute__ ((section (".config1"))) = (cfg1); \
	unsigned __DEVCFG2 __attribute__ ((section (".config2"))) = (cfg2); \
	unsigned __DEVCFG3 __attribute__ ((section (".config3"))) = (cfg3)

#define	PIC32MM_CFG0	0xa4210582
#define	PIC32MM_CFG1	0x80000012
#define	PIC32MM_CFG2	0x00237068
#define	PIC32MM_CFG3	0x00000001

#define	PIC32MM_DEVCFG	PIC32_DEVCFG(PIC32MM_CFG0, PIC32MM_CFG1, \
				     PIC32MM_CFG2, PIC32MM_CFG3)

#endif /* !_MIPS_MICROCHIP_PIC32_H_ */
