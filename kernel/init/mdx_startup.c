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

#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/device.h>
#include <sys/list.h>

void
mi_startup(void)
{
	struct mdx_sysinit *si, *xi, *si_start, *si_end;
	struct mdx_sysinit save;
	int i;

	si_start = &__start_set_sysinit_set;
	si_end = &__stop_set_sysinit_set;

	/* Sort by subsystem, order. */
	for (si = si_start; si < si_end; si++) {
		for (xi = si + 1; xi < si_end; xi++) {
			if (si->subsystem < xi->subsystem ||
			    (si->subsystem == xi->subsystem &&
			    si->order <= xi->order))
				continue;
			save = *si;
			*si = *xi;
			*xi = save;
		}
	}

	printf("%s: (Re)ordered sysinit list:\n", __func__);
	for (si = si_start, i = 0; si < si_end; si++, i++)
		printf("%s: #%d subsystem %d order %d\n",
		    __func__, i, si->subsystem, si->order);

	for (si = si_start, i = 0; si < si_end; si++, i++) {
		printf("%s: Initializing sysinit #%d\n", __func__, i);
		si->func(si->arg);
	}

	printf("%s: %d sysinits initialized\n", __func__, i);
}
