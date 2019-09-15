/*-
 * Copyright (c) 2019 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _ARM_ARM_SCB_H_
#define _ARM_ARM_SCB_H_

#include <machine/frame.h>
#include <machine/scs.h>

/* System Control Block. */

struct scb_intr_entry {
	void (*handler) (void *arg, struct trapframe *frame, int irq);
	void *arg;
};

struct arm_scb_softc {
	uint32_t base;
};

int arm_scb_init(struct arm_scb_softc *sc, uint32_t base);
void arm_scb_set_vector(struct arm_scb_softc *sc, uint32_t vtor);
void arm_scb_exceptions_prio_config(struct arm_scb_softc *sc,
    int prio_secure);
void arm_scb_exceptions_target_config(struct arm_scb_softc *sc,
    int secure);
void arm_scb_sysreset_secure(struct arm_scb_softc *sc,
    int secure_only);
void arm_sau_configure(struct arm_scb_softc *sc, int enable, int allns);
void arm_fpu_non_secure(struct arm_scb_softc *sc, int enable);

#endif /* !_ARM_ARM_SCB_H_ */
