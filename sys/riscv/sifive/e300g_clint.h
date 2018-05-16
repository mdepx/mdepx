/*-
 * Copyright (c) 2017 Ruslan Bukin <br@bsdpad.com>
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
 * SiFive CLINT Memory Map.
 * SiFive E300 Platform Reference Manual, Version 1.0.1
 */

#ifndef _SYS_RISCV_SIFIVE_E300G_CLINT_H_
#define _SYS_RISCV_SIFIVE_E300G_CLINT_H_

/* MSIP Registers (16 KiB) */
#define	MSIP(hart)	(0x4 * (hart))

/* Timer Registers (32 KiB) */
#define	MTIMECMP(hart)	(0x4000 + 0x4 * (hart))
#define	MTIME		0xBFF8

struct clint_softc {
	uint32_t base;
};

int e300g_clint_init(struct clint_softc *sc, uint32_t base);
int clint_get_cpu_freq(struct clint_softc *sc, uint32_t osc_freq);
void clint_udelay(struct clint_softc *sc, uint32_t usec, uint32_t osc_freq);

#endif /* !_SYS_RISCV_SIFIVE_E300G_CLINT_H_ */
