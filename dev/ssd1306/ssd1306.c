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

#include <sys/types.h>
#include <dev/spi/spi.h>
#include <dev/ssd1306/ssd1306.h>

#define	SSD1306_DEBUG
#undef	SSD1306_DEBUG

#ifdef	SSD1306_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

static const uint8_t init_seq_128x64[] = {
	0xAE,			/* Display off */
	0xD4, 0x80,		/* Display clock div ratio */
	0xA8, 0x3F,		/* Multiplex ratio */
	0xD3, 0x00,		/* Display offset */
	0x40,			/* Start line */
	0x20, 0x00,		/* Memory mode */
	SSD1306_SEGREMAP | 0x1,
	SSD1306_COMSCANDEC,
	0x8D, 0x14,		/* Charge pump (internal) */
	0xA1,			/* Segment re-map */
	0xC8,			/* Com output scan dir */
	0xDA, 0x12,		/* Com hardware configuration */
	0x81, 0xCF,		/* Contrast */
	0xD9, 0xF1,		/* Pre-charge period */
	0xDB, 0x40,		/* VCOMH Deselect Level */
	0xA4,			/* All pixels off */
	0xA6,			/* Display not inverted */
	0xAF,			/* Display on */
};

void
ssd1306_draw_pixel(uint8_t *addr, int x, int y, int pixel)
{

	dprintf("%s: %d %d = %d\n", __func__, x, y, pixel);

	if (pixel)
		addr[x + (y / 8) * (128 * 8 / 8)] |= (1 << (y & 0x7));
	else
		addr[x + (y / 8) * (128 * 8 / 8)] &= ~(1 << (y & 0x7));
}

void
ssd1306_init(mdx_device_t dev)
{
	uint8_t cmd;
	int i;

	for (i = 0; i < sizeof(init_seq_128x64); i++) {
		cmd = init_seq_128x64[i];
		mdx_spi_transfer(dev, &cmd, NULL, 1);
	}
}
