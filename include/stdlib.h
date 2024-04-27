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

#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <sys/types.h>

void *malloc(size_t size);
void *calloc(size_t number, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
long strtol(const char * restrict nptr, char ** restrict endptr, int base);
unsigned long strtoul(const char * restrict nptr, char ** restrict endptr, int base);
void qsort(void *base, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *));
int abs(int j);
int atoi(const char *nptr);
void * bsearch(const void *key, const void *base, size_t nmemb, size_t size,
    int (*compar) (const void *, const void *));

float strtof(const char *nptr, char **endptr);
double strtod(const char *nptr, char **endptr);
quad_t strtoq(const char *nptr, char **endptr, int base);
u_quad_t strtouq(const char *nptr, char **endptr, int base);

#endif /* !_STDLIB_H_ */
