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

#ifndef	_MIPS_BERI_BERIPIC_H_
#define	_MIPS_BERI_BERIPIC_H_

#define	BP_CFG_IRQ_S		0
#define	BP_CFG_IRQ_M		(0xf << BP_CFG_IRQ_S)
#define	BP_CFG_TID_S		8
#define	BP_CFG_TID_M		(0x7FFFFF << BP_CFG_TID_S)
#define	BP_CFG_ENABLE		(1 << 31)

#define	BERIPIC_NIRQS	32

struct beripic_resource {
	capability cfg;
	capability ip_read;
	capability ip_set;
	capability ip_clear;
};

struct beripic_intr_entry {
	void (*handler) (void *arg);
	void *arg;
};

struct beripic_softc {
	struct beripic_resource *res;
	const struct beripic_intr_entry *map;
};

int beripic_init(mdx_device_t dev, struct beripic_resource *res);
void beripic_enable(mdx_device_t dev, uint32_t beripic_irq, uint32_t hard_irq);
void beripic_disable(mdx_device_t dev, uint32_t beripic_irq);
void beripic_intr(void *arg, struct trapframe *frame, int i);
void beripic_install_intr_map(mdx_device_t dev,
    const struct beripic_intr_entry *map);
void beripic_ip_set(mdx_device_t dev, uint32_t irq);
void beripic_ip_clear(mdx_device_t dev, uint32_t irq);

#endif /* !_MIPS_BERI_BERIPIC_H_ */
