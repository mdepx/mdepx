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
#include <sys/endian.h>

#include <stdio.h>

#include <dev/spi/spi.h>
#include <dev/max31855/max31855.h>

#define	MAX31855_DEBUG
#undef	MAX31855_DEBUG

#ifdef	MAX31855_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

uint32_t
max31855_read(spi_device_t *dev)
{
	uint32_t val1;
	uint32_t val;
	uint32_t wr;

	wr = 0xffffffff;

	dev->transfer(dev, (uint8_t *)&wr, (uint8_t *)&val1, 4);

	val = bswap32(val1);

	if (val & 0x1)
		printf("Error: OC\n");
	if (val & 0x2)
		printf("Error: SCG\n");
	if (val & 0x4)
		printf("Error: SCV\n");

	if (val & (1 << 16))
		printf("Fault\n");

	dprintf("val1 %x, val %x, val %d\n", val1, val, val >> 18);

	return (val);
}

int
max31855_read_celsius(spi_device_t *dev)
{
	int v;

	v = max31855_read(dev);

	if (v & 0x80000000) {
		/* Negative value, drop the lower 18 bits and sign-extend. */
		v = 0xFFFFC000 | ((v >> 18) & 0x00003FFFF);
	} else {
		/* Positive value, drop the lower 18 bits. */
		v >>= 18;
	}

	v >>= 2;

	dprintf("v %d\n", v);

	return (v);
}
