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

#include <sys/cdefs.h>
#include <sys/endian.h>

#include <dev/spi/spi.h>
#include <dev/max6675/max6675.h>

#define	MAX6675_DEBUG
#undef	MAX6675_DEBUG

#ifdef	MAX6675_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

static uint16_t
max6675_read(mdx_device_t dev)
{
	uint16_t val;
	uint16_t wr;

	wr = 0xffff;

	mdx_spi_transfer(dev, (uint8_t *)&wr, (uint8_t *)&val, 2);

	val = bswap16(val);

	dprintf("%s: read 0x%x, val %d\n", __func__, val,
	    ((val >> 3) & 0x3ff) >> 2);

	return (val);
}

uint32_t
max6675_read_celsius(mdx_device_t dev)
{
	uint32_t data;

	data = max6675_read(dev);

	data >>= 3;
	data >>= 2;

	return (data);
}
