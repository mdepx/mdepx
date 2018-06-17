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

/* Free list memory allocator */

#include <sys/cdefs.h>

/* For local build */
#include <sys/types.h>
#include <string.h>
#include <stdio.h>

#include "fl_alloc.h"

struct node_s {
	struct node_s *next;
	struct node_s *prev;
	uint32_t size;
	uint32_t flags;
#define	FLAG_ALLOCATED		(1 << 31)
#define	FLAG_PREV_SIZE_S	0
#define	FLAG_PREV_SIZE_M	(0x7fffffff << FLAG_PREV_SIZE_S)
};

#define	NODE_S	sizeof(struct node_s)

struct node_s nodelist[32];

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
fl_add_node(struct node_s *node)
{
	struct node_s *next;
	struct node_s *prev;
	int i;

	i = size2i(node->size);

	for (prev = &nodelist[i], next = nodelist[i].next;
	    (next && next->size && (next->size < node->size));
	     prev = next, next = next->next);

	prev->next = node;
	node->prev = prev;
	node->next = next;
	if (next)
		next->prev = node;
}

void
fl_add_region(uintptr_t base, int size)
{
	struct node_s *node;

	node = (struct node_s *)base;
	node->size = NODE_S;
	node->flags = FLAG_ALLOCATED;

	node = (struct node_s *)(base + NODE_S);
	node->size = size - 2 * NODE_S;
	node->flags = NODE_S;
	fl_add_node(node);

	node = (struct node_s *)(base + size - NODE_S);
	node->size = NODE_S;
	node->flags = (size - 2 * NODE_S) | FLAG_ALLOCATED;
}

void
fl_init(void)
{
	int i;

	memset(&nodelist, 0, NODE_S * 32);
	for (i = 1; i < 32; i++) {
		nodelist[i - 1].next = &nodelist[i];
		nodelist[i].prev = &nodelist[i - 1];
	}
}

void
fl_dump(void)
{
	struct node_s *node;

	printf("\n -- %s -- \n", __func__);

	for (node = &nodelist[0]; node != NULL; node = node->next) {
		if (node->size != 0)
			printf("    ");
		printf("node %p, next %p, prev %p, size %d, flags %x\n",
		    node, node->next, node->prev, node->size, node->flags);
	}
	printf(" -- %s completed -- \n", __func__);
}

void *
fl_alloc(size_t size)
{
	struct node_s *node;
	struct node_s *next;
	struct node_s *new;
	int avail;
	int i;

	if (size <= 0)
		return (NULL);

	size += NODE_S;

	i = size2i(size);

	for (node = nodelist[i].next;
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
			next = (struct node_s *)((uint8_t *)node + node->size);
			next->flags &= FLAG_ALLOCATED;
			next->flags |= avail;
			node->size = size;

			/* Create new free node */
			new = (struct node_s *)((uint8_t *)node + size);
			new->size = avail;
			new->flags = size;
			fl_add_node(new);
		}

		node->flags |= FLAG_ALLOCATED;
		node->next = 0;
		node->prev = 0;

		return ((void *)((uint8_t *)node + NODE_S));
	}

	return (NULL);
}

void
fl_free(void *ptr)
{
	struct node_s *node;
	struct node_s *prev;
	struct node_s *next;
	struct node_s *subseq;

	if (ptr == NULL)
		return;

	node = (struct node_s *)((char *)ptr - NODE_S);
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

	fl_add_node(node);
}
