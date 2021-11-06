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
#include <sys/ringbuf.h>

void
mdx_ringbuf_init(struct mdx_ringbuf_softc *sc, void *base, uint32_t basesize,
    void *buf, uint32_t bufsize)
{
	struct mdx_ringbuf *rb;
	int entry_bufsize;
	int i;

	KASSERT(basesize > 0, ("Invalid arguments"));

	sc->nentries = basesize / sizeof(struct mdx_ringbuf);
	entry_bufsize = bufsize / sc->nentries;

	printf("%s: nentries %d, entry_bufsize %d\n", __func__,
	    sc->nentries, entry_bufsize);

	rb = (struct mdx_ringbuf *)base;

	for (i = 0; i < sc->nentries; i++) {
		rb[i].buf = (void *)((uintptr_t)buf + entry_bufsize * i);
		rb[i].bufsize = entry_bufsize;
		rb[i].fill = 0;
		rb[i].flags = 0;
		if (i == (sc->nentries - 1))
			rb[i].next = &rb[0];
		else
			rb[i].next = &rb[i + 1];
	}

	sc->role = MDX_RINGBUF_ROLE_SENDER;
	sc->head = &rb[0];
}

void
mdx_ringbuf_join(struct mdx_ringbuf_softc *sc, void *base)
{
	struct mdx_ringbuf *rb;

	rb = (struct mdx_ringbuf *)base;

	sc->role = MDX_RINGBUF_ROLE_RECVER;
	sc->head = &rb[0];
}

int
mdx_ringbuf_head(struct mdx_ringbuf_softc *sc, struct mdx_ringbuf **rb0)
{
	struct mdx_ringbuf *rb;

	rb = sc->head;

	if (sc->role == MDX_RINGBUF_ROLE_SENDER &&
	    rb->flags & MDX_RINGBUF_OWN)
		return (MDX_ERROR);

	if (sc->role == MDX_RINGBUF_ROLE_RECVER &&
	    (rb->flags & MDX_RINGBUF_OWN) == 0)
		return (MDX_ERROR);

	*rb0 = sc->head;

	return (0);
}

void
mdx_ringbuf_submit(struct mdx_ringbuf_softc *sc)
{
	struct mdx_ringbuf *rb;

	rb = sc->head;

	if (sc->role == MDX_RINGBUF_ROLE_SENDER)
		rb->flags |= MDX_RINGBUF_OWN;
	else 
		rb->flags &= ~MDX_RINGBUF_OWN;

	sc->head = rb->next;
}
