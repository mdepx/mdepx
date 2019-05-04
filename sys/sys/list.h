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

#ifndef _SYS_LIST_H_
#define	_SYS_LIST_H_

struct entry {
	struct entry *next;
	struct entry *prev;
};

#define	LIST_INIT(l) 	{ l, l }

static inline void
list_init(struct entry *e)
{

	e->next = e;
	e->prev = e;
}

/*
 * Put node e to the head of list l.
 */
static inline void
list_prepend(struct entry *l, struct entry *e)
{

	e->next = l->next;
	e->prev = l;

	e->next->prev = e;
	e->prev->next = e;
}

/*
 * Put node e before node l.
 */
static inline void
list_insert(struct entry *l, struct entry *e)
{

	e->next = l;
	e->prev = l->prev;

	e->next->prev = e;
	e->prev->next = e;
}

/*
 * Put node e to the tail of list l.
 */
static inline void
list_append(struct entry *l, struct entry *e)
{

	e->next = l;
	e->prev = l->prev;

	e->next->prev = e;
	e->prev->next = e;
}

/*
 * Remove node e from the list.
 */
static inline void
list_remove(struct entry *e)
{

	e->prev->next = e->next;
	e->next->prev = e->prev;

	list_init(e);
}

static inline bool
list_empty(struct entry *l)
{

	return (l->next == l);
}

#endif /* !_SYS_LIST_H_*/
