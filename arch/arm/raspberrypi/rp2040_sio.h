/*-
 * Copyright (c) 2021 Ruslan Bukin <br@bsdpad.com>
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

#ifndef	_RP2040_SIO_H_
#define	_RP2040_SIO_H_

#define	RP2040_SIO_CPUID		0x00
#define	RP2040_SIO_GPIO_IN		0x04
#define	RP2040_SIO_GPIO_HI_IN		0x08
#define	RP2040_SIO_GPIO_OUT		0x10
#define	RP2040_SIO_GPIO_OUT_SET		0x14
#define	RP2040_SIO_GPIO_OUT_CLR		0x18
#define	RP2040_SIO_GPIO_OUT_XOR		0x1c
#define	RP2040_SIO_GPIO_OE		0x20
#define	RP2040_SIO_GPIO_OE_SET		0x24
#define	RP2040_SIO_GPIO_OE_CLR		0x28
#define	RP2040_SIO_GPIO_OE_XOR		0x2c
#define	RP2040_SIO_GPIO_HI_OUT		0x30
#define	RP2040_SIO_GPIO_HI_OUT_SET	0x34
#define	RP2040_SIO_GPIO_HI_OUT_CLR	0x38
#define	RP2040_SIO_GPIO_HI_OUT_XOR	0x3c
#define	RP2040_SIO_GPIO_HI_OE		0x40
#define	RP2040_SIO_GPIO_HI_OE_SET	0x44
#define	RP2040_SIO_GPIO_HI_OE_CLR	0x48
#define	RP2040_SIO_GPIO_HI_OE_XOR	0x4c
#define	RP2040_SIO_FIFO_ST		0x50
#define	RP2040_SIO_FIFO_WR		0x54
#define	RP2040_SIO_FIFO_RD		0x58
#define	RP2040_SIO_SPINLOCK_ST		0x5c
#define	RP2040_SIO_DIV_UDIVIDEND	0x60
#define	RP2040_SIO_DIV_UDIVISOR		0x64
#define	RP2040_SIO_DIV_SDIVIDEND	0x68
#define	RP2040_SIO_DIV_SDIVISOR		0x6c
#define	RP2040_SIO_DIV_QUOTIENT		0x70
#define	RP2040_SIO_DIV_REMAINDER	0x74
#define	RP2040_SIO_DIV_CSR		0x78
#define	RP2040_SIO_INTERP0_ACCUM0	0x80
#define	RP2040_SIO_INTERP0_ACCUM1	0x84
#define	RP2040_SIO_INTERP0_BASE0	0x88
#define	RP2040_SIO_INTERP0_BASE1	0x8c
#define	RP2040_SIO_INTERP0_BASE2	0x90
#define	RP2040_SIO_INTERP0_POP_LANE0	0x94
#define	RP2040_SIO_INTERP0_POP_LANE1	0x98
#define	RP2040_SIO_INTERP0_POP_FULL	0x9c
#define	RP2040_SIO_INTERP0_PEEK_LANE0	0xa0
#define	RP2040_SIO_INTERP0_PEEK_LANE1	0xa4
#define	RP2040_SIO_INTERP0_PEEK_FULL	0xa8
#define	RP2040_SIO_INTERP0_CTRL_LANE0	0xac
#define	RP2040_SIO_INTERP0_CTRL_LANE1	0xb0
#define	RP2040_SIO_INTERP0_ACCUM0_ADD	0xb4
#define	RP2040_SIO_INTERP0_ACCUM1_ADD	0xb8
#define	RP2040_SIO_INTERP0_BASE_1AND0	0xbc
#define	RP2040_SIO_INTERP1_ACCUM0	0xc0
#define	RP2040_SIO_INTERP1_ACCUM1	0xc4
#define	RP2040_SIO_INTERP1_BASE0	0xc8
#define	RP2040_SIO_INTERP1_BASE1	0xcc
#define	RP2040_SIO_INTERP1_BASE2	0xd0
#define	RP2040_SIO_INTERP1_POP_LANE0	0xd4
#define	RP2040_SIO_INTERP1_POP_LANE1	0xd8
#define	RP2040_SIO_INTERP1_POP_FULL	0xdc
#define	RP2040_SIO_INTERP1_PEEK_LANE0	0xe0
#define	RP2040_SIO_INTERP1_PEEK_LANE1	0xe4
#define	RP2040_SIO_INTERP1_PEEK_FULL	0xe8
#define	RP2040_SIO_INTERP1_CTRL_LANE0	0xec
#define	RP2040_SIO_INTERP1_CTRL_LANE1	0xf0
#define	RP2040_SIO_INTERP1_ACCUM0_ADD	0xf4
#define	RP2040_SIO_INTERP1_ACCUM1_ADD	0xf8
#define	RP2040_SIO_INTERP1_BASE_1AND0	0xfc
#define	RP2040_SIO_SPINLOCK(n)		(0x100 + 0x4 * (n))

#endif /* !_RP2040_SIO_H_ */
