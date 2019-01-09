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

#ifndef _ARM_NORDICSEMI_NRF9160_DPPI_H_
#define _ARM_NORDICSEMI_NRF9160_DPPI_H_

/* Enable channel group n */
#define	DPPI_TASKS_CHG_EN(n)		(0x000 + 0x8 * n)

/* Disable channel group n */
#define	DPPI_TASKS_CHG_DIS(n)		(0x004 + 0x8 * n)

/* Subscribe configuration for task CHG[n].EN */
#define	DPPI_SUBSCRIBE_CHG_EN(n)	(0x080 + 0x8 * n)

/* Subscribe configuration for task CHG[n].DIS */
#define	DPPI_SUBSCRIBE_CHG_DIS(n)	(0x084 + 0x8 * n)

/* Channel enable register */
#define	DPPI_CHEN			0x500

/* Channel enable set register */
#define	DPPI_CHENSET			0x504

/* Channel enable clear register */
#define	DPPI_CHENCLR			0x508

/* Channel group n */
#define	DPPI_CHG(n)			(0x800 + 0x4 * n)

struct dppi_softc {
	size_t base;
};

#endif /* !_ARM_NORDICSEMI_NRF9160_DPPI_H_ */
