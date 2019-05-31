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

#ifndef _SYS_SYSTM_H_
#define _SYS_SYSTM_H_

void udelay(uint32_t usec);	/* microseconds polling delay */
void usleep(uint32_t usec);	/* microseconds sleep */
void msleep(uint32_t msec);	/* milliseconds sleep */
void raw_sleep(uint32_t ticks);	/* raw timer ticks sleep */

void critical_enter(void);
void critical_exit(void);

void panic(const char *fmt, ...);

#ifdef CONFIG_PRODUCTION_BUILD
#define	KASSERT(e, m) do {} while (0)
#else
#define	KASSERT(e, m) do {		\
	if (__predict_false(!(e)))	\
		panic m;		\
} while (0)
#endif

#define	CTASSERT(x)	_Static_assert(x, "compile-time assertion failed")

void zero_bss(void);
void relocate_data(void);

#endif /* !_SYS_SYSTM_H_ */
