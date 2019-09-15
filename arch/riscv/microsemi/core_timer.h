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

#ifndef	_SYS_RISCV_MICROSEMI_CORE_TIMER_H_
#define	_SYS_RISCV_MICROSEMI_CORE_TIMER_H_

#define	TIMER_LOAD		0x00
#define	TIMER_VALUE		0x04
#define	TIMER_CONTROL		0x08
#define	 CONTROL_ENABLE		(1 << 0)
#define	 CONTROL_IE		(1 << 1)
#define	 CONTROL_MODE_ONE_SHOT	(1 << 2)
#define	TIMER_PRESCALER		0x0C
#define	 PRESCALER_DIV_2	0
#define	 PRESCALER_DIV_4	1
#define	 PRESCALER_DIV_8	2
#define	 PRESCALER_DIV_16	3
#define	 PRESCALER_DIV_32	4
#define	 PRESCALER_DIV_64	5
#define	 PRESCALER_DIV_128	6
#define	 PRESCALER_DIV_256	7
#define	 PRESCALER_DIV_512	8
#define	 PRESCALER_DIV_1024	9
#define	TIMER_INT_CLR		0x10
#define	TIMER_RIS		0x14
#define	 RAW_TIMER_INT		(1 << 0)
#define	TIMER_MIS		0x18
#define	 MIS_TIMER_INT		(1 << 0)

struct core_timer_softc {
	uint32_t base;
	uint32_t load_val;
};

void core_timer_udelay(struct core_timer_softc *sc, uint32_t usec,
    uint32_t osc_freq);
uint32_t core_timer_value(struct core_timer_softc *sc);
int core_timer_init(struct core_timer_softc *sc, uint32_t base,
    uint32_t load_val, uint8_t prescaler);

#endif /* !_SYS_RISCV_MICROSEMI_CORE_TIMER_H_ */
