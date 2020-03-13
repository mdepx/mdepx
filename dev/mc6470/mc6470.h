/*-
 * Copyright (c) 2020 Ruslan Bukin <br@bsdpad.com>
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

#ifndef	_DEV_MC6470_MC6470_H_
#define	_DEV_MC6470_MC6470_H_

#define	MC6470_SR		0x03
#define	MC6470_OPSTAT		0x04
#define	MC6470_INTEN		0x06
#define	MC6470_MODE		0x07
#define	MC6470_SRTFR		0x08
#define	MC6470_TAPEN		0x09
#define	MC6470_TTTRX		0x0A
#define	MC6470_TTTRY		0x0B
#define	MC6470_TTTRZ		0x0C
#define	MC6470_XOUT_EX_L	0x0D
#define	MC6470_XOUT_EX_H	0x0E
#define	MC6470_YOUT_EX_L	0x0F
#define	MC6470_YOUT_EX_H	0x10
#define	MC6470_ZOUT_EX_L	0x11
#define	MC6470_ZOUT_EX_H	0x12
#define	MC6470_OUTCFG		0x20
#define	MC6470_XOFFL		0x21
#define	MC6470_XOFFH		0x22
#define	MC6470_YOFFL		0x23
#define	MC6470_YOFFH		0x24
#define	MC6470_ZOFFL		0x25
#define	MC6470_ZOFFH		0x26
#define	MC6470_XGAIN		0x27
#define	MC6470_YGAIN		0x28
#define	MC6470_ZGAIN		0x29
#define	MC6470_PCODE		0x3B

#endif /* !_DEV_MC6470_MC6470_H_ */
