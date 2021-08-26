/*-
 * Copyright (c) 2018-2021 Ruslan Bukin <br@bsdpad.com>
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

/* Free list memory allocator */

#include <sys/cdefs.h>
#include <sys/cheri.h>
#include <sys/types.h>
#include <sys/malloc.h>

#include <string.h>
#include <stdio.h>

static int
size2i(uint32_t size)
{
	int i;

	i = 0;

	size >>= 4;

	while (size > 1) {
		size >>= 1;
		i++;
	}

	return (i);
}

static void
mdx_fl_add_node(struct mdx_fl_zone *fl, struct node_s *node)
{
	struct node_s *next;
	struct node_s *prev;
	int i;

	i = size2i(node->size);

	for (prev = &fl->nodelist[i], next = fl->nodelist[i].next;
	    (next && next->size && (next->size < node->size));
	     prev = next, next = next->next);

	prev->next = node;
	node->prev = prev;
	node->next = next;
	if (next)
		next->prev = node;
}

void
mdx_fl_add_region(struct mdx_fl_zone *fl, void *base, int size)
{
	struct node_s *node;

	node = base;
	node->size = NODE_S;
	node->flags = FLAG_ALLOCATED;
	mdx_setbounds(node, NODE_S);

	node = mdx_incoffset(base, NODE_S);
	node->size = size - 2 * NODE_S;
	node->flags = NODE_S;
	mdx_setbounds(node, node->size);
	mdx_fl_add_node(fl, node);

	node = mdx_incoffset(base, size - NODE_S);
	node->size = NODE_S;
	node->flags = (size - 2 * NODE_S) | FLAG_ALLOCATED;
	mdx_setbounds(node, node->size);
}

void
mdx_fl_init(struct mdx_fl_zone *fl)
{
	int i;

	memset(&fl->nodelist, 0, NODE_S * 32);
	for (i = 1; i < 32; i++) {
		fl->nodelist[i - 1].next = &fl->nodelist[i];
		fl->nodelist[i].prev = &fl->nodelist[i - 1];
	}
}

void
mdx_fl_dump(struct mdx_fl_zone *fl)
{
	struct node_s *node;

	printf("\n -- %s -- \n", __func__);

	for (node = &fl->nodelist[0]; node != NULL; node = node->next) {
		if (node->size != 0)
			printf("    ");
		printf("node %p, next %p, prev %p, size %d, flags %x\n",
		    node, node->next, node->prev, node->size, node->flags);
	}
	printf(" -- %s completed -- \n", __func__);
}

uint32_t
mdx_fl_count(struct mdx_fl_zone *fl)
{
	struct node_s *node;
	uint32_t size;

	size = 0;

	for (node = &fl->nodelist[0];
	    node != NULL;
	    node = node->next)
		size += node->size;

	return (size);
}

void *
mdx_fl_malloc(struct mdx_fl_zone *fl, size_t size)
{
	struct node_s *node;
	struct node_s *next;
	struct node_s *new;
	int avail;
	int i;

	if (size <= 0)
		return (NULL);

	size += NODE_S;

	while (size & 0xf)
		size += 1;

	i = size2i(size);

	for (node = fl->nodelist[i].next;
	    (node && (node->size < size));
	     node = node->next);

	if (node) {
		/* Node found. Remove it from list */
		node->prev->next = node->next;
		if (node->next)
			node->next->prev = node->prev;

		/* Split the node if space allows */
		avail = (node->size - size);
		if (avail > NODE_S) {
			/* Adjust the size of current node */
			next = mdx_incoffset(node, node->size);
			next->flags &= FLAG_ALLOCATED;
			next->flags |= avail;
			node->size = size;

			/* Create new free node */
			new = mdx_incoffset(node, size);
			new->size = avail;
			new->flags = size;
			mdx_fl_add_node(fl, new);
		}

		node->flags |= FLAG_ALLOCATED;
		node->next = 0;
		node->prev = 0;

		node = mdx_incoffset(node, NODE_S);
		node = mdx_setbounds(node, size - NODE_S);
		return ((void *)node);
	}

	return (NULL);
}

void
mdx_fl_free(struct mdx_fl_zone *fl, void *ptr)
{
	struct node_s *node;
	struct node_s *prev;
	struct node_s *next;
	struct node_s *subseq;

	if (ptr == NULL)
		return;

#ifdef __CHERI_PURE_CAPABILITY__
	node = cheri_setoffset(fl->datacap, cheri_getaddress(ptr) - NODE_S);
#else
	node = (struct node_s *)((char *)ptr - NODE_S);
#endif

	node->flags &= ~FLAG_ALLOCATED;

	next = (struct node_s *)((uint8_t *)node + node->size);
	if ((next->flags & FLAG_ALLOCATED) == 0) {
		subseq = (struct node_s *)((uint8_t *)next + next->size);

		/* Remove node */
		next->prev->next = next->next;
		if (next->next)
			next->next->prev = next->prev;

		/* Merge with next */
		node->size += next->size;
		subseq->flags &= FLAG_ALLOCATED;
		subseq->flags |= node->size;
		next = subseq;
	}

	prev = (struct node_s *)((uint8_t *)node - node->flags);
	if ((prev->flags & FLAG_ALLOCATED) == 0) {

		/* Remove node */
		prev->prev->next = prev->next;
		if (prev->next)
			prev->next->prev = prev->prev;

		/* Merge with prev */
		prev->size += node->size;
		next->flags &= FLAG_ALLOCATED;
		next->flags |= prev->size;
		node = prev;
	}

	mdx_fl_add_node(fl, node);
}

void *
mdx_fl_calloc(struct mdx_fl_zone *fl, size_t number, size_t size)
{
	void *res;
	size_t sz;

	sz = number * size;

	res = mdx_fl_malloc(fl, sz);
	if (res)
		bzero(res, sz);

	return (res);
}

void *
mdx_fl_realloc(struct mdx_fl_zone *fl, void *ptr, size_t size)
{
	struct node_s *node;
	struct node_s *next;
	struct node_s *subs;
	struct node_s *new;
	void *newptr;

	if (ptr == NULL)
		return (mdx_fl_malloc(fl, size));

	if (size == 0) {
		mdx_fl_free(fl, ptr);
		return (NULL);
	}

	size += NODE_S;

	while (size & 0xf)
		size += 1;

	node = mdx_decoffset(ptr, NODE_S);
	next = mdx_incoffset(node, node->size);

	if (size <= node->size) {
		if ((next->flags & FLAG_ALLOCATED) == 0) {
			subs = mdx_incoffset(next, next->size);

			/* Remove node */
			next->prev->next = next->next;
			if (next->next)
				next->next->prev = next->prev;

			/* Merge with next */
			new = mdx_incoffset(node, size);
			new->size = next->size + node->size - size;
			new->flags = size;
			node->size = size;
			subs->flags &= FLAG_ALLOCATED;
			subs->flags |= new->size;
			mdx_fl_add_node(fl, new);
		} else if (node->size > (size + NODE_S)) {
			new = mdx_incoffset(node, size);
			new->size = node->size - size;
			new->flags = size;
			node->size = size;
			next->flags &= FLAG_ALLOCATED;
			next->flags |= new->size;
			mdx_fl_add_node(fl, new);
		}
	} else {
		if (((next->flags & FLAG_ALLOCATED) == 0) &&
		    (node->size + next->size) > (size + NODE_S)) {
			subs = mdx_incoffset(next, next->size);

			/* Remove node */
			next->prev->next = next->next;
			if (next->next)
				next->next->prev = next->prev;

			/* Merge with next */
			new = (struct node_s *)((uint8_t *)node + size);
			new->size = next->size + node->size - size;
			new->flags = size;
			node->size = size;
			subs->flags &= FLAG_ALLOCATED;
			subs->flags |= new->size;
			mdx_fl_add_node(fl, new);

		} else if (((next->flags & FLAG_ALLOCATED) == 0) &&
		    (node->size + next->size) == size) {
			subs = mdx_incoffset(next, next->size);

			/* Remove node */
			next->prev->next = next->next;
			if (next->next)
				next->next->prev = next->prev;

			/* Merge with next */
			node->size = size;
			subs->flags &= FLAG_ALLOCATED;
			subs->flags |= node->size;
		} else {
			newptr = mdx_fl_malloc(fl, size);
			if (newptr) {
				memcpy(newptr, ptr, node->size);
				mdx_fl_free(&mdx_fl_defaultzone, ptr);
			}

			return (newptr);
		}
	}

	return (ptr);
}
