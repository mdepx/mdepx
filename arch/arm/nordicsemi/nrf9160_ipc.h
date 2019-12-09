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

#ifndef _ARM_NORDICSEMI_NRF9160_IPC_H_
#define _ARM_NORDICSEMI_NRF9160_IPC_H_

#define	IPC_TASKS_SEND(n)	(0x000 + (n) * 0x4)	/* Trigger events on channel enabled in SEND_CNF(n). */
#define	IPC_SUBSCRIBE_SEND(n)	(0x080 + (n) * 0x4)	/* Subscribe configuration for task SEND(n) */
#define	IPC_EVENTS_RECEIVE(n)	(0x100 + (n) * 0x4)	/* Event received on one or more of the enabled channels in RECEIVE_C */
#define	IPC_PUBLISH_RECEIVE(n)	(0x180 + (n) * 0x4)	/* Publish configuration for event RECEIVE(n) */
#define	IPC_INTEN		0x300			/* Enable or disable interrupt */
#define	IPC_INTENSET		0x304			/* Enable interrupt */
#define	IPC_INTENCLR		0x308			/* Disable interrupt */
#define	IPC_INTPEND		0x30C			/* Pending interrupts */
#define	IPC_SEND_CNF(n)		(0x510 + (n) * 0x4)	/* Send event configuration for TASKS_SEND(n). */
#define	IPC_RECEIVE_CNF(n)	(0x590 + (n) * 0x4)	/* Receive event configuration for EVENTS_RECEIVE(n). */
#define	IPC_GPMEM(n)		(0x610 + (n) * 0x4)	/* General purpose memory. */

struct nrf_ipc_softc {
	size_t base;
};

#endif /* !_ARM_NORDICSEMI_NRF9160_IPC_H_ */
