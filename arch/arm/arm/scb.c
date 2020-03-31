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

#include <sys/cdefs.h>

#include <arm/arm/scb.h>

#include <arm/include/scs.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
arm_scb_exceptions_prio_config(struct arm_scb_softc *sc,
    int prio_secure)
{
	uint32_t reg;

	reg = RD4(sc, SCB_AIRCR);
	reg &= ~(AIRCR_VECTKEY_M);
	reg |= AIRCR_VECTKEY;

	if (prio_secure)
		/* Non-secure exceptions are de-prioritized. */
		reg |= AIRCR_PRIS;
	else
		/*
		 * Priority ranges of Secure and Non-secure
		 * exceptions are identical.
		 */
		reg &= ~AIRCR_PRIS;

	WR4(sc, SCB_AIRCR, reg);
}

void
arm_scb_exceptions_target_config(struct arm_scb_softc *sc,
    int secure)
{
	uint32_t reg;

	reg = RD4(sc, SCB_AIRCR);
	reg &= ~(AIRCR_VECTKEY_M);
	reg |= AIRCR_VECTKEY;

	if (secure)
		/* BusFault, HardFault, and NMI are Secure. */
		reg &= ~AIRCR_BFHFNMINS;
	else
		/*
		 * BusFault and NMI are Non-secure and exceptions
		 * can target Non-secure HardFault.
		 */
		reg |= AIRCR_BFHFNMINS;

	WR4(sc, SCB_AIRCR, reg);
}

void
arm_scb_sysreset_secure(struct arm_scb_softc *sc,
    int secure_only)
{
	uint32_t reg;

	reg = RD4(sc, SCB_AIRCR);
	reg &= ~(AIRCR_VECTKEY_M);
	reg |= AIRCR_VECTKEY;

	if (secure_only)
		/*
		 * SYSRESETREQ functionality is only available
		 * to Secure state.
		 */
		reg |= AIRCR_SYSRESETREQS;
	else
		/*
		 * SYSRESETREQ functionality is available to both
		 * Security states.
		 */
		reg &= ~AIRCR_SYSRESETREQS;
	WR4(sc, SCB_AIRCR, reg);
}

void
arm_scb_set_vector(struct arm_scb_softc *sc, uint32_t vtor)
{

	WR4(sc, SCB_VTOR, vtor);
}

void
arm_fpu_non_secure(struct arm_scb_softc *sc, int enable)
{
	uint32_t reg;

	reg = RD4(sc, SCB_NSACR);

	if (enable)
		reg |= (NSACR_CP10 | NSACR_CP11);
	else
		reg &= ~(NSACR_CP10 | NSACR_CP11);

	WR4(sc, SCB_NSACR, reg);
}

/* SAU Control Register routines */

void
arm_sau_configure(struct arm_scb_softc *sc, int enable, int allns)
{
	uint32_t reg;

	reg = RD4(sc, SAU_CTRL);
	if (enable)
		reg |= CTRL_ENABLE;
	else
		reg &= ~CTRL_ENABLE;

	if (allns)
		reg |= CTRL_ALLNS;
	else
		reg &= ~CTRL_ALLNS;

	WR4(sc, SAU_CTRL, reg);
}

int
arm_sau_nregions(struct arm_scb_softc *sc)
{
	int nreg;

	nreg = RD4(sc, SAU_TYPE);

	return (nreg);
}

int
arm_scb_init(struct arm_scb_softc *sc, uint32_t base)
{

	sc->base = base;

	return (0);
}
