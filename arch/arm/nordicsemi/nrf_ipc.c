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
#include <sys/systm.h>

#include "nrf_ipc.h"

#define	NRF_IPC_DEBUG
#undef	NRF_IPC_DEBUG

#ifdef	NRF_IPC_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
nrf_ipc_trigger(struct nrf_ipc_softc *sc, int ev)
{

	WR4(sc, IPC_TASKS_SEND(ev), 1);
}

void
nrf_ipc_configure_send(struct nrf_ipc_softc *sc,
    int ev, int chanmask)
{

	WR4(sc, IPC_SEND_CNF(ev), chanmask);
}

void
nrf_ipc_configure_recv(struct nrf_ipc_softc *sc,
    int ev, int chanmask, void (*cb)(void *arg), void *user)
{

	sc->ev[ev].cb = cb;
	sc->ev[ev].user = user;

	WR4(sc, IPC_RECEIVE_CNF(ev), chanmask);
}

void
nrf_ipc_inten(struct nrf_ipc_softc *sc, int ev, bool set)
{

	if (set)
		WR4(sc, IPC_INTENSET, (1 << ev));
	else
		WR4(sc, IPC_INTENCLR, (1 << ev));
}

void
nrf_ipc_intr(void *arg, struct trapframe *tf, int irq)
{
	struct nrf_ipc_softc *sc;
	struct ipc_event *ev;
	int pending;
	int i;

	sc = arg;

	pending = RD4(sc, IPC_INTPEND);
	for (i = 0; i < NRF_IPC_MAX_EVENTS; i++) {
		if (pending & (1 << i)) {
			ev = &sc->ev[i];
			WR4(sc, IPC_EVENTS_RECEIVE(i), 0);
			dprintf("%s: ev %d\n", __func__, i);
			if (ev->cb != NULL)
				ev->cb(ev->user);
		}
	}
}

void
nrf_ipc_init(struct nrf_ipc_softc *sc, uint32_t base)
{

	sc->base = base;
}
