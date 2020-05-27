/*-
 * Copyright (c) 2018-2020 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/console.h>
#include <sys/systm.h>
#include <sys/malloc.h>

#include <dev/display/panel.h>
#include <dev/display/dsi.h>
#include <dev/otm8009a/otm8009a.h>

#include <libfont/libfont.h>

#include "board.h"

static struct global_data {
	uint32_t ptr;
	struct font_info font;
} g_data;

static void
draw_pixel(void *arg, int x, int y, int pixel)
{
	uint32_t *addr;

	addr = (uint32_t *)(g_data.ptr + DISPLAY_WIDTH * y * 3 + x * 3);

	if (pixel)
		*addr = 0xffffff;
	else
		*addr = 0;
}

static void
draw_text_utf8(uint8_t *s)
{
	struct char_info ci;
	uint8_t *newptr;
	uint8_t *buf;
	int c;

	g_data.ptr = FB_BASE;

	buf = (uint8_t *)s;

	for (;;) {
		c = utf8_to_ucs2(buf, &newptr);
		if (c == -1)
			return;
		buf = newptr;
		get_char_info(&g_data.font, c, &ci);
		draw_char(&g_data.font, c);
		g_data.ptr += ci.xsize * 3;
	}
}

static void
display_clear(void)
{
	uint32_t p;
	uint8_t *b;

	b = (uint8_t *)FB_BASE;
	for (p = 0; p < (DISPLAY_WIDTH * DISPLAY_HEIGHT * 3); p++)
		*(b + p) = 0;
}

int
main(void)
{
	uint8_t text[64];
	int err;
	int i;

	printf("Hello world\n");

	g_data.font.draw_pixel = draw_pixel;

	display_clear();

	err = font_init(&g_data.font, (uint8_t *)FONT_ADDR);
	if (err != 0) {
		while (1) {
			printf("Error: font not found\n");
			usleep(1000000);
		}
	}

	i = 0;

	while (1) {
		printf(".");
		sprintf(text, "Hello World %d", i);
		draw_text_utf8(text);
		i += 1;
		usleep(1000000);
	}

	return (0);
}
