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

#ifndef _ARM_NORDICSEMI_NRF9160_KMU_H_
#define _ARM_NORDICSEMI_NRF9160_KMU_H_

/* Push a key slot over secure APB */
#define	KMU_TASKS_PUSH_KEYSLOT		0x000

/* Key successfully pushed over secure APB */
#define	KMU_EVENTS_KEYSLOT_PUSHED	0x100

/* Key has been revoked and cannot be tasked for selection */
#define	KMU_EVENTS_KEYSLOT_REVOKED	0x104

/*
 * No key slot selected, no destination address defined,
 * or error during push operation
 */
#define	KMU_EVENTS_KEYSLOT_ERROR	0x108

/* Enable or disable interrupt */
#define	KMU_INTEN			0x300
#define	KMU_INTENSET			0x304	/* Enable interrupt */
#define	KMU_INTENCLR			0x308	/* Disable interrupt */
#define	KMU_INTPEND			0x30C	/* Pending interrupts */

/* Status bits for KMU operation */
#define	KMU_STATUS			0x40C

/*
 * Select key slot ID to be read over AHB or
 * pushed over secure APB when TASKS_PUSH_KEYSLOT is started
 */
#define	KMU_SELECTKEYSLOT		0x500

struct kmu_softc {
	size_t base;
};

#endif /* !_ARM_NORDICSEMI_NRF9160_KMU_H_ */
