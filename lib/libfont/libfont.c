/*-
 * Copyright (c) 2015-2018 Ruslan Bukin <br@bsdpad.com>
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
#include <sys/endian.h>

#include <libfont/libfont.h>

#define	LIBFONT_DEBUG
#undef	LIBFONT_DEBUG

#ifdef	LIBFONT_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define	PCF_PROPERTIES		(1 << 0)
#define	PCF_ACCELERATORS	(1 << 1)
#define	PCF_METRICS		(1 << 2)
#define	PCF_BITMAPS		(1 << 3)
#define	PCF_INK_METRICS		(1 << 4)
#define	PCF_BDF_ENCODINGS	(1 << 5)
#define	PCF_SWIDTHS		(1 << 6)
#define	PCF_GLYPH_NAMES		(1 << 7)
#define	PCF_BDF_ACCELERATORS	(1 << 8)

struct toc_entry {
	uint32_t type;		/* See below, indicates which table */
	uint32_t format;	/* See below, indicates how the data are formatted in the table */
	uint32_t size;		/* In bytes */
	uint32_t offset;	/* from start of file */
};

int
utf8_to_ucs2(uint8_t *input, uint8_t **end_ptr)
{

	*end_ptr = input;

	if (input[0] == 0)
		return (-1);

	if (input[0] < 0x80) {
		*end_ptr = (input + 1);
		return (input[0]);
	}

	if ((input[0] & 0xE0) == 0xE0) {
		if (input[1] == 0 || input[2] == 0)
			return (-1);

		*end_ptr = (input + 3);
		return ((input[0] & 0x0F) << 12 |
			(input[1] & 0x3F) <<  6 |
			(input[2] & 0x3F) <<  0);
	}

	if ((input[0] & 0xC0) == 0xC0) {
		if (input[1] == 0)
			return (-1);

		*end_ptr = (input + 2);
		return ((input[0] & 0x1F) << 6 |
			(input[1] & 0x3F) << 0);
	}

	return (-1);
}

void
get_char_info(struct font_info *font, uint16_t c,
    struct char_info *ci)
{
	uint8_t *ptr;
	uint32_t i;

	i = font->charmap[c];
	ptr = (font->metrics_data + i * 5);

	ci->left = *ptr++ - 128;
	ci->right = *ptr++ - 128;
	ci->width = *ptr++ - 128;
	ci->ascent = *ptr++ - 128;
	ci->descent = *ptr++ - 128;
	ci->xsize = (ci->right - ci->left);
	ci->ysize = (ci->ascent + ci->descent);

	dprintf("xsize %d ysize %d box %d\n", ci->xsize, ci->ysize, (ci->xsize * ci->ysize));
	dprintf("left %d right %d width %d ascent %d descent %d\n",
	    ci->left, ci->right, ci->width, ci->ascent, ci->descent);
}

void
draw_char(struct font_info *font, uint16_t c)
{
	uint32_t *glyph;
	uint32_t offs;
	struct char_info ci;
	uint32_t i;
	uint32_t d;
	uint32_t bit;
	int p;
	uint32_t pixel;

	i = font->charmap[c];

	offs = be32toh(*(uint32_t *)(font->offsets_data + i));
	dprintf("offset is 0x%x\n", offs);
	dprintf("next offset is 0x%x\n",
	    be32toh(*(uint32_t *)(font->offsets_data + i + 1)));

	glyph = (uint32_t *)(font->bitmap_data + offs);
	dprintf("glyph addr 0x%x\n", (uint8_t *)glyph);

	get_char_info(font, c, &ci);

	for (p = 0; p < ci.ysize; p++) {
		d = be16toh(*glyph);
		glyph++;
		dprintf("row data 0x%08x\n", d);
		for (bit = 0; bit < ci.xsize; bit++) {
			if (d & (1 << (16 - bit)))
				pixel = 1; /* foreground */
			else
				pixel = 0; /* background */
			font->draw_pixel(font->draw_pixel_arg, bit, p, pixel);
		}
	}
}

static void
encodings(struct font_info *font, uint8_t *ptr, uint32_t size)
{
	uint16_t first_col, last_col;
	uint16_t first_row, last_row;
	uint16_t offs;
	uint32_t nencoding;
	int i;

	/* format = *(volatile uint32_t *)(ptr); */
	ptr += 4;

	first_col = be16toh(*(volatile uint16_t *)(ptr));
	ptr += 2;
	last_col = be16toh(*(volatile uint16_t *)(ptr));
	ptr += 2;
	first_row = be16toh(*(volatile uint16_t *)(ptr));
	ptr += 2;
	last_row = be16toh(*(volatile uint16_t *)(ptr));
	ptr += 2;
	/* default_char = be16toh(*(volatile uint16_t *)(ptr)); */
	ptr += 2;

	font->encodings_data = (uint16_t *)(ptr);
	
	nencoding = (last_col - first_col + 1) * (last_row - first_row + 1);
	dprintf("nencoding %d first_col %d\n", nencoding, first_col);

	for (i = 0; i < nencoding; i++) {
		if (i >= MAXCHAR)
			break;
		offs = be16toh(*(volatile uint16_t *)(ptr));
		ptr += 2;
		if (offs != 0xffff) {
			font->charmap[(i + first_col)] = offs;
			dprintf("i %d offs %d\n", i, offs);
		}
	}
}

static void
accelerators(struct font_info *font, uint8_t *ptr, uint32_t size)
{
#if 0
	uint32_t format;
	uint8_t noOverlap;
	uint8_t constantMetrics;
	uint8_t terminalFont;
	uint8_t constantWidth;
	uint8_t inkInside;
	uint8_t inkMetrics;
	uint8_t drawDirection;
	uint8_t padding;

	format = *(volatile uint32_t *)(ptr);
	ptr += 4;

	dprintf("accelerators format %x\n", format);

	noOverlap = *(volatile uint8_t *)(ptr);
	ptr += 1;
	constantMetrics = *(volatile uint8_t *)(ptr);
	ptr += 1;
	terminalFont = *(volatile uint8_t *)(ptr);
	ptr += 1;
	constantWidth = *(volatile uint8_t *)(ptr);
	ptr += 1;
	inkInside = *(volatile uint8_t *)(ptr);
	ptr += 1;
	inkMetrics = *(volatile uint8_t *)(ptr);
	ptr += 1;
	drawDirection = *(volatile uint8_t *)(ptr);
	ptr += 1;
	padding = *(volatile uint8_t *)(ptr);
#endif
}

static void
metrics(struct font_info *font, uint8_t *ptr, uint32_t size)
{
	uint16_t metrics_count;
	uint32_t format; 		/* Always stored with least significant byte first! */

	format = *(volatile uint32_t *)(ptr);
	ptr += 4;

	dprintf("metrics format %x\n", format);

	if ((format & 0xff00) == 0x100) { /* Compressed metrics */
		metrics_count = be16toh(*(uint16_t *)ptr);
		dprintf("nmetrics %d\n", metrics_count);
		font->metrics_count = metrics_count;
	}

	ptr += 2;
	font->metrics_data = (uint8_t *)(ptr);
}

static void
bitmaps(struct font_info *font, uint8_t *ptr, uint32_t size)
{
	uint32_t *bitmapSizes;
	uint32_t glyph_count;	/* byte ordering depends on format, should be the same as the metrics count */
	uint32_t format; 	/* Always stored with least significant byte first! */
	uint32_t tmp;

	uint32_t padindex;

	format = *(volatile uint32_t *)(ptr);

	dprintf("ptr %x\n", (uint32_t)ptr);
	ptr += 4;
	dprintf("ptr %x\n", (uint32_t)ptr);

	tmp = *(volatile uint32_t *)(ptr);
	if (format & 4) /* Big endian */
		glyph_count = be32toh(tmp);
	else
		glyph_count = tmp;

	dprintf("format %d glyph_count %d\n",
	    format, glyph_count);

	ptr += 4;
	font->offsets_data = (uint32_t *)(ptr);

	ptr += 4 * glyph_count;
	bitmapSizes = (uint32_t *)(ptr);

	padindex = (format & 3);
	dprintf("%s: padindex %x\n", __func__, padindex);

	bitmapSizes += padindex;
	dprintf("sizes %x\n", be32toh(*bitmapSizes));

	ptr += 16;
	font->bitmap_data = (uint8_t *)(ptr);

	dprintf("sizes %x\n", be32toh(*bitmapSizes));
}

int
font_init(struct font_info *font, uint8_t *header)
{
	struct toc_entry *entry;
	uint32_t table_count;
	uint8_t *addr;
	int i;

	if (header[0] != '\1' || header[1] != 'f' ||
	    header[2] != 'c' || header[3] != 'p')
		return (-1);

	dprintf("%s\n", __func__);

	table_count = *(volatile uint32_t *)(header + 4);

	entry = (struct toc_entry *)(header + 8);

	dprintf("pcf header %s table_count %x\n", header, table_count);

	for (i = 0; i < table_count; i++) {
		entry = (struct toc_entry *)(header + 8 + i * 16);

		switch (entry->type) {
		case (PCF_PROPERTIES):
			break;
		case (PCF_ACCELERATORS):
			addr = (uint8_t *)(header + entry->offset);
			accelerators(font, addr, entry->size);
			break;
		case (PCF_METRICS):
			addr = (uint8_t *)(header + entry->offset);
			metrics(font, addr, entry->size);
			break;
		case (PCF_BITMAPS):
			dprintf("bitmaps header %x entry->offset %x\n",
			    (uint32_t)header, entry->offset);
			addr = (uint8_t *)(header + entry->offset);
			bitmaps(font, addr, entry->size);
			break;
		case (PCF_INK_METRICS):
			break;
		case (PCF_BDF_ENCODINGS):
			addr = (uint8_t *)(header + entry->offset);
			encodings(font, addr, entry->size);
			break;
		case (PCF_SWIDTHS):
			break;
		case (PCF_GLYPH_NAMES):
			break;
		case (PCF_BDF_ACCELERATORS):
			addr = (uint8_t *)(header + entry->offset);
			accelerators(font, addr, entry->size);
			break;
		default:
			break;
		};

		dprintf("entry %d type %d\n", i, entry->type);
		dprintf("entry %d format %d\n", i, entry->format);
		dprintf("entry %d size %d\n", i, entry->size);
		dprintf("entry %d offset %d\n", i, entry->offset);
	}

	return (0);
}
