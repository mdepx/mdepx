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

#include <sys/cdefs.h>
#include <sys/systm.h>

/* Symbols defined in linker script. */
extern uint32_t _sbss;	/* Start of the BSS section */
extern uint32_t _ebss;	/* End of the BSS section */
extern uint32_t _smem;	/* Start of the data section on ROM */
extern uint32_t _sdata;	/* Start of the data section in RAM */
extern uint32_t _edata;	/* End of the data section in RAM */

/*
 * Relocate data section from ROM to RAM.
 */
void
relocate_data(void)
{
	uint8_t *dst;
	uint8_t *src;

	if (&_smem == &_sdata)
		return;

	for (src = (uint8_t *)&_smem, dst = (uint8_t *)&_sdata;
	    dst < (uint8_t *)&_edata; )
		*dst++ = *src++;
}

void
zero_bss(void)
{
	uint8_t *sbss;
	uint8_t *ebss;

	sbss = (uint8_t *)&_sbss;
	ebss = (uint8_t *)&_ebss;

	while (sbss < ebss)
		*sbss++ = 0;
}
