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

#include <sys/cdefs.h>
#include <sys/malloc.h>

#include <string.h>
#include <stdio.h>

#define	MALLOC_DEBUG
//#undef	MALLOC_DEBUG

#ifdef	MALLOC_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	MAX_REGIONS	10

#define	DIV_ROUND_UP(n, d)	(((n) + (d) - 1) / (d))

struct mem_info {
	uintptr_t start;
	uintptr_t size;
};

struct node_s {
	struct node_s *next;
	struct node_s *prev;
	uint32_t size;
	uint16_t index;
	uint16_t flags;
#define	FLAG_ALLOCATED	(1 << 0)
};

struct node_s nodelist[32];
static uint32_t nregions;
struct mem_info regions[MAX_REGIONS];

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
malloc_addfree(struct node_s *node)
{
	struct node_s *next;
	struct node_s *prev;
	int i;

	printf("\n%s: %p\n", __func__, node);

	i = size2i(node->size);

	dprintf("%s: node %p size %d\n", __func__, node, node->size);

	for (prev = &nodelist[i], next = nodelist[i].next;
	    (next && next->size && (next->size < node->size));
	     prev = next, next = next->next) {
		dprintf("prev %p\n", prev);
	}

	dprintf("%s: prev is %p next is %p node is %p\n",
	    __func__, prev, next, node);

	prev->next = node;
	node->prev = prev;
	node->next = next;
	if (next)
		next->prev = node;
}


static void
region_init(struct mem_info *region)
{
	struct node_s *node;

	node = (struct node_s *)region->start;
	node->size = (region->size - sizeof(struct node_s));
	node->next = 0;
	node->prev = 0;

	malloc_addfree(node);
}

void
malloc_add_region(uintptr_t base, int size)
{

	bzero((void *)base, size);

	regions[nregions].start = base;
	regions[nregions].size = size;
	region_init(&regions[nregions]);
	nregions++;
}

void
malloc_init(void)
{
	int i;

	memset(&nodelist, 0, sizeof(struct node_s) * 32);
	for (i = 1; i < 32; i++) {
		nodelist[i-1].next = &nodelist[i];
		nodelist[i].prev = &nodelist[i-1];
	}
	for (i = 0; i < 32; i++)
		nodelist[i].index = i + 1;

	nregions = 0;
}

void
dump_mem(void)
{
	struct node_s *node;

	printf("\n -- %s -- \n", __func__);

	for (node = &nodelist[0]; node != NULL; node = node->next)
		printf("node %p next %p idx %d size %d\n",
		    node, node->next, node->index, node->size);
	printf(" -- %s completed -- \n", __func__);
}

void *
kern_malloc(size_t size)
{
	struct node_s *node;
	struct node_s *next;
	struct node_s *prev;
	struct node_s *new;
	void *res;
	int avail;
	int i;

	printf("\n%s: %d\n", __func__, size);

	if (size == 0)
		return (NULL);

	while (size & 0x3)
		size += 1;

	i = size2i(size);

	for (node = nodelist[i].next;
	    (node && (node->size < size));
	     node = node->next) {
		dprintf("node %p idx %d size %d request size %d\n",
		    node, node->index, node->size, size);
	}

	if (node) {
		/* Node found. Remove it from list */
		dprintf("node found %p size %d\n",
		    node, node->size);

		prev = node->prev;
		prev->next = node->next;
		if (node->next) {
			next = node->next;
			next->prev = prev;
		}

		/* Exact match ? */
		if (node->size == size)
			goto finish;

		/* Create new free node */
		avail = (node->size - size - sizeof(struct node_s));
		if (avail > 0) {
			new = (struct node_s *)((uint8_t *)node +
			    sizeof(struct node_s) + size);
			new->size = avail;
			new->next = NULL;
			new->prev = NULL;
			new->index = 0;
			dprintf("add new free %p\n", new);
			malloc_addfree(new);

		}

		/* Adjust the size */
		node->size = size;
	} else
		return (NULL);

finish:
	res = (void *)((char *)node + sizeof(struct node_s));
	node->flags = FLAG_ALLOCATED;

	//printf("node 0x%08x bzero 0x%08x, %d\n", (uint32_t)node,
	//  (uint32_t)res, node->size);
	bzero(res, node->size);

	return (res);
}

void
kern_free(void *ptr)
{
	struct node_s *node;
	struct node_s *prev;
	struct node_s *next;
	struct node_s *n;

	if (ptr == NULL)
		return;

	printf("\n%s: %p\n", __func__, ptr);

	node = (struct node_s *)((char *)ptr - sizeof(struct node_s));
	node->flags &= ~FLAG_ALLOCATED;

	next = ptr + node->size;
	printf("%s: next is %p, next->flags %d, next->index %d\n",
	    __func__, next, next->flags, next->index);

	//if (node->next != next)
	//	printf("node->next %p, next %p\n", node->next, next);
	//	while (1);

	if ((next->flags & FLAG_ALLOCATED) == 0 && (next->index == 0)) {
		n = next + next->size + sizeof(struct node_s);

		/* Remove node */
		printf("%s: removing next node: %p\n", __func__, next);
		next->prev->next = next->next;
		if (next->next)
			next->next->prev = next->prev;

		/* Merge */
		node->size += next->size + sizeof(struct node_s);
		next = n;
	}

	prev = node->prev;
	if ((prev->flags & FLAG_ALLOCATED) == 0 && (prev->index == 0)) {
		/* Remove node */
		printf("%s: removing prev node: %p\n", __func__, prev);
		prev->prev->next = prev->next;
		if (prev->next)
			prev->next->prev = prev->prev;

		/* Merge */
		prev->size += node->size + sizeof(struct node_s);
		node = prev;
	}

	node->next = 0;
	node->prev = 0;
	malloc_addfree(node);
}
