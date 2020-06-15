/*-
 * Copyright (c) 2015 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _LIB_LIBFONT_LIBFONT_H_
#define _LIB_LIBFONT_LIBFONT_H_

#define	MAXCHAR	2048 /* Limit, max possible 0xffff */

struct font_info {
	uint16_t	charmap[MAXCHAR];
	uint8_t		*metrics_data;
	uint16_t	metrics_count;
	uint8_t		*bitmap_data;
	uint32_t	*offsets_data;
	uint16_t	*encodings_data;
	void		(*draw_pixel)(void *arg, int x, int y, int bit);
	void		*draw_pixel_arg;
};

struct char_info {
	uint8_t		left;
	uint8_t		right;
	uint8_t		width;
	uint8_t		ascent;
	uint8_t		descent;
	uint32_t	xsize;
	uint32_t	ysize;
};

int font_init(struct font_info *font, uint8_t *header);
void draw_char(struct font_info *font, uint16_t c);
void get_char_info(struct font_info *font, uint16_t c,
    struct char_info *ci);
int utf8_to_ucs2(uint8_t *input, uint8_t **end_ptr);

#endif /* !_LIB_LIBFONT_LIBFONT_H_ */
