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

#ifndef _ARM_ARM_NVIC_H_
#define _ARM_ARM_NVIC_H_

#define	NVIC_ICSR	0xD04	/* Interrupt Control State Register */
#define	 ICSR_PENDSWSET	(1 << 28) /* Set pending pendSV bit */
#define	NVIC_VTOR	0xD08	/* Vector Table Offset Register */
#define	NVIC_CCR	0xD14	/* Configuration Control Register */
#define	NVIC_ISER(n)	(0x100 + 0x4 * n)
#define	NVIC_ICER(n)	(0x180 + 0x4 * n)
#define	NVIC_ICPR(n)	(0x280 + 0x4 * n)

struct nvic_intr_entry {
	void (*handler) (void *arg, struct trapframe *frame, int irq);
	void *arg;
};

struct arm_nvic_softc {
	uint32_t base;
};

int arm_nvic_init(struct arm_nvic_softc *sc, uint32_t base);
void arm_nvic_enable_intr(struct arm_nvic_softc *sc, uint32_t intr);
void arm_nvic_disable_intr(struct arm_nvic_softc *sc, uint32_t n);
void arm_nvic_intr(uint32_t irq, struct trapframe *frame);
void arm_nvic_install_intr_map(struct arm_nvic_softc *sc,
    const struct nvic_intr_entry *m);

#endif /* !_ARM_ARM_NVIC_H_ */
