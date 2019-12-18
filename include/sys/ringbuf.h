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

#ifndef	_SYS_RINGBUF_H_
#define	_SYS_RINGBUF_H_

struct mdx_ringbuf {
	void *buf;
	int bufsize;
	int fill;
	int flags;
#define	MDX_RINGBUF_OWN		(1 << 1)
	struct mdx_ringbuf *next;
	uint32_t user;
};

struct mdx_ringbuf_softc {
	struct mdx_ringbuf *head;
	int nentries;
	uint8_t role;
#define	MDX_RINGBUF_ROLE_RECVER	(0 << 0)
#define	MDX_RINGBUF_ROLE_SENDER	(1 << 0)
};

void mdx_ringbuf_init(struct mdx_ringbuf_softc *sc,
    void *base, uint32_t basesize, void *buf, uint32_t bufsize);
void mdx_ringbuf_join(struct mdx_ringbuf_softc *sc, void *base);
int mdx_ringbuf_head(struct mdx_ringbuf_softc *sc, struct mdx_ringbuf **);
void mdx_ringbuf_submit(struct mdx_ringbuf_softc *sc);

#endif	/* !_SYS_RINGBUF_H_ */
