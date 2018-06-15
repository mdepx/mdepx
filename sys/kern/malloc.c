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

#define	MALLOC_DEBUG
#undef	MALLOC_DEBUG

#ifdef	MALLOC_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	MAX_REGIONS	10

#define	DIV_ROUND_UP(n, d)	(((n) + (d) - 1) / (d))

struct mem_info {
	uint32_t start;
	uint32_t size;
};

struct node_s {
	struct node_s *next;
	struct node_s *prev;
	uint32_t size;
	uint32_t index;
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

	i = size2i(node->size);

	dprintf("%s: node %p size %d\n", __func__, node, node->size);

	for (prev = &nodelist[i], next = nodelist[i].next;
	    (next && next->size && (next->size < node->size));
	     prev = next, next = next->next) {
		printf("prev %p\n", prev);
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
malloc_add_region(int base, int size)
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
		nodelist[i].index = i;

	nregions = 0;
}

void *
malloc(size_t size)
{

	return (NULL);
}

void
free(void *ptr)
{
	struct node_s *node;

	if (ptr == NULL)
		return;

	node = (struct node_s *)((char *)ptr - sizeof(struct node_s));

	/* TODO: merge free */
	//prev = node->prev;
	//next = node->next;
	//prev->next = next;
	//next->prev = prev;

	node->next = 0;
	node->prev = 0;
	malloc_addfree(node);
}
