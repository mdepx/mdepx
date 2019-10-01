/*-
 * Copyright (c) 2014 Robert N. M. Watson
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract (FA8750-10-C-0237)
 * ("CTSRD"), as part of the DARPA CRASH research programme.
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

#include <string.h>

struct capreloc {
	uint64_t capability_location;
	uint64_t object;
	uint64_t offset;
	uint64_t size;
	uint64_t permissions;
};

static const uint64_t function_reloc_flag = 1ULL<<63;
static const uint64_t function_pointer_permissions =
	~0 &
	~__CHERI_CAP_PERMISSION_PERMIT_STORE_CAPABILITY__ &
	~__CHERI_CAP_PERMISSION_PERMIT_STORE__;
static const uint64_t global_pointer_permissions =
	~0 & ~__CHERI_CAP_PERMISSION_PERMIT_EXECUTE__;

__attribute__((weak))
extern struct capreloc __start___cap_relocs;
__attribute__((weak))
extern struct capreloc __stop___cap_relocs;

void crt_init_globals(void);

void
crt_init_globals(void)
{
	struct capreloc *start_relocs;
	struct capreloc *stop_relocs;

#ifndef __CHERI_CAPABILITY_TABLE__
	/*
	 * If we are not using the CHERI capability table we can
	 * just synthesize the capabilities for these using the
	 * GOT and $ddc
	 */
	start_relocs = &__start___cap_relocs;
	stop_relocs = &__stop___cap_relocs;
#else
	long start_addr, end_addr;

	__asm__ (".option pic0\n\t"
		 "dla %0, __start___cap_relocs\n\t"
		 "dla %1, __stop___cap_relocs\n\t"
		 :"=r"(start_addr), "=r"(end_addr));

	long relocs_size = end_addr - start_addr;
	/* void *ddc = __builtin_cheri_global_data_get(); */
	start_relocs = __builtin_cheri_offset_set(
			__builtin_cheri_global_data_get(), start_addr);
	start_relocs = __builtin_cheri_bounds_set(start_relocs, relocs_size);
	stop_relocs = __builtin_cheri_offset_set(start_relocs, relocs_size);
#endif
	void *gdc = __builtin_cheri_global_data_get();
	void *pcc = __builtin_cheri_program_counter_get();

	gdc = __builtin_cheri_perms_and(gdc, global_pointer_permissions);
	pcc = __builtin_cheri_perms_and(pcc, function_pointer_permissions);

	for (struct capreloc *reloc = start_relocs;
	    reloc < stop_relocs;
	    reloc++) {
		_Bool isFunction = (reloc->permissions & function_reloc_flag)
					== function_reloc_flag;
		void **dest = __builtin_cheri_offset_set(gdc,
						reloc->capability_location);
		void *base = isFunction ? pcc : gdc;
		void *src = __builtin_cheri_offset_set(base, reloc->object);
		if (!isFunction && (reloc->size != 0))
			src = __builtin_cheri_bounds_set(src, reloc->size);
		src = __builtin_cheri_offset_increment(src, reloc->offset);
		*dest = src;
	}
}

/* Also provide a simple purecap strlen function */
__SIZE_TYPE__ strlen(const char* str)
{
	const char* p = str;

	while (*p != '\0')
		p++;

	return p - str;
}
