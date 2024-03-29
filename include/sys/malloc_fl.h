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

#ifndef _SYS_MALLOC_FL_
#define _SYS_MALLOC_FL_

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

struct mdx_fl_zone {
	struct node_s nodelist[32];
#ifdef __CHERI_PURE_CAPABILITY__
	void *datacap;
#endif
};

void mdx_fl_init(struct mdx_fl_zone *fl);
void mdx_fl_add_region(struct mdx_fl_zone *fl, void *base, int size);
void mdx_fl_dump(struct mdx_fl_zone *fl);
void mdx_fl_free(struct mdx_fl_zone *fl, void *ptr);
void * mdx_fl_malloc(struct mdx_fl_zone *fl, size_t size);
void * mdx_fl_realloc(struct mdx_fl_zone *fl, void *ptr, size_t size);
void * mdx_fl_calloc(struct mdx_fl_zone *fl, size_t number, size_t size);
uint32_t mdx_fl_count(struct mdx_fl_zone *fl);

extern struct mdx_fl_zone mdx_fl_defaultzone;

#endif /* !_SYS_MALLOC_FL_*/
