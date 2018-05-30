/*-
 * Copyright (c) 2018 Ruslan Bukin <br@bsdpad.com>
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
 * ("CTSRD"), as part of the DARPA CRASH research programme.
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

#include <sys/types.h>
#include <sys/endian.h>

#include <stdio.h>

#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#include <mips/beri/beripic.h>

#define	WR_CFG(_sc, _reg, _val)		\
    *(volatile uint64_t *)((_sc)->res->cfg + _reg) = _val
#define	WR_IP_SET(_sc, _reg, _val)	\
    *(volatile uint64_t *)((_sc)->res->ip_set + _reg) = _val
#define	WR_IP_CLEAR(_sc, _reg, _val)	\
    *(volatile uint64_t *)((_sc)->res->ip_clear + _reg) = _val

#define	BERIPIC_NIRQS	32

void
beripic_enable(struct beripic_softc *sc,
    uint32_t beripic_irq, uint32_t hard_irq)
{
	uint64_t reg;
	int tid;

	tid = 0;

	printf("%s: enabling IRQ %d\n", __func__, beripic_irq);

#if 0
	printf("beripic_enable\n");
	printf("beripic_enable: excp vector[0] %x\n",
	    *(uint32_t *)(0x80000180 | MIPS_XKPHYS_UNCACHED_BASE));
#endif

	WR_IP_CLEAR(sc, 0, (1 << beripic_irq));

	reg = BP_CFG_ENABLE;
	reg |= (hard_irq << BP_CFG_IRQ_S);
	reg |= (tid << BP_CFG_TID_S);
	WR_CFG(sc, (beripic_irq + tid) * 8, reg);
}

int
beripic_init(struct beripic_softc *sc,
    struct beripic_resource *res)
{
	int i;

	sc->res = res;

	res->cfg |= MIPS_XKPHYS_UNCACHED_BASE;
	res->ip_set |= MIPS_XKPHYS_UNCACHED_BASE;
	res->ip_clear |= MIPS_XKPHYS_UNCACHED_BASE;

	for (i = 0; i < BERIPIC_NIRQS; i++)
		WR_CFG(sc, i * 8, 0);

	return (0);
}
