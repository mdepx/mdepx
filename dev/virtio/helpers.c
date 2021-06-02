/*-
 * Copyright (c) 2021 Ruslan Bukin <br@bsdpad.com>
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

#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"

#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/tick.h>

void *
SLOF_alloc_mem_aligned(size_t size, size_t alignment)
{
	uint8_t *address;
	uint32_t offset;
	void **ptr;

	offset = alignment - 1 + sizeof(void *);

	address = malloc(size + offset);

	ptr = (void **)((uintptr_t)(address + offset) & ~(alignment - 1));
	ptr[-1] = address;

	return (ptr);
}

void *
SLOF_alloc_mem(size_t size)
{
	void *ret;

	ret = SLOF_alloc_mem_aligned(size, sizeof(uintptr_t));

	return (ret);
}

void
SLOF_free_mem(void *addr, long size)
{

	free(((void**)addr)[-1]);
}

long
SLOF_dma_map_in(void *virt, long size, int cacheable)
{

	/*
	 * TODO:
	 *  Empty as only used if IOMMU and VIRTIO_VERSION1 are supported.
	 */
	(void) size;
	(void) cacheable;

	return (long)virt;
}

void
SLOF_dma_map_out(long phys, void *virt, long size)
{
	/*
	 * TODO:
	 *  Empty as only used if IOMMU and VIRTIO_VERSION1 are supported.
	 */
	(void) size;
	(void) phys;
	(void) virt;
}

/**
 * get msec-timer value
 * access to HW register
 * overrun will occur if boot exceeds 1.193 hours (49 days)
 *
 * @param   -
 * @return  actual timer value in ms as 32bit
 */
uint32_t
SLOF_GetTimer(void)
{
	uint64_t usec;

	usec = mdx_uptime();

	return (usec / 1000);
}

void
SLOF_msleep(uint32_t time)
{

	mdx_usleep(time * 1000);
}

void
SLOF_usleep(uint32_t time)
{

	mdx_usleep(time);
}
