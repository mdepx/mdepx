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

#include <sys/stdarg.h>

#ifndef	_STDIO_H_
#define	_STDIO_H_

int printf(const char * restrict format, ...);
int vsprintf(char * restrict str, const char * restrict format, va_list ap);
int vprintf(const char * restrict format, va_list ap);
int snprintf(char * restrict str, size_t size, const char * restrict format,
    ...);
int vsnprintf(char * restrict str, size_t size, const char * restrict format,
    va_list ap);
int kvprintf(char const *fmt, void (*func)(int, void*), void *arg, int radix,
    va_list ap);
int sprintf(char *buf, const char *cfmt, ...);
int puts(const char *str);

#define	stdin	0
#define	stdout	1
#define	EOF	(-1)

typedef void * FILE;

FILE * fopen(const char * restrict path, const char * restrict mode);
int fclose(FILE *stream);
size_t fread(void * restrict ptr, size_t size, size_t nmemb,
    FILE * restrict stream);
size_t fwrite(const void * restrict ptr, size_t size, size_t nmemb,
    FILE * restrict stream);
int feof(FILE *stream);
int ferror(FILE *stream);
int fseeko(FILE *stream, off_t offset, int whence);
int fseek(FILE *stream, long offset, int whence);
off_t ftello(FILE *stream);
long ftell(FILE *stream);
int fileno(FILE *stream);
int fflush(FILE *stream);
int fputs(const char *str, FILE *stream);
int fputc(int c, FILE *stream);
int fprintf(FILE * restrict stream, const char * restrict format, ...);

#endif /* !_STDIO_H_ */
