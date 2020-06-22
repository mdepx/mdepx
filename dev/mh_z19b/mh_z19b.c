/*-
 * Copyright (c) 2020 Ruslan Bukin <br@bsdpad.com>
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

#include <dev/uart/uart.h>

#include "mh_z19b.h"

uint8_t
mh_z19b_checksum(uint8_t *pkt)
{
	uint8_t csum;
	uint8_t i;

	csum = 0;

	for(i = 1; i < 8; i++)
		csum += pkt[i];

	csum = 0xff - csum;
	csum += 1;

	return (csum);
}

void
mh_z19b_set_range_req(uint8_t *req, int range)
{

	req[0] = 0xFF;
	req[1] = 0x01;
	req[2] = MH_Z19B_DETECTION_RANGE;
	req[3] = range / 256;
	req[4] = range % 256;
	req[5] = 0x00;
	req[6] = 0x00;
	req[7] = 0x00;
	req[8] = mh_z19b_checksum(req);
}

void
mh_z19b_read_co2_req(uint8_t *req)
{

	req[0] = 0xFF;
	req[1] = 0x01;
	req[2] = MH_Z19B_CO2_CONCENTRATION;
	req[3] = 0x00;
	req[4] = 0x00;
	req[5] = 0x00;
	req[6] = 0x00;
	req[7] = 0x00;
	req[8] = mh_z19b_checksum(req);
}

int
mh_z19b_read_co2_reply(uint8_t *reply, uint32_t *co2)
{
	uint8_t csum;

	csum = mh_z19b_checksum(reply);
	if (csum != reply[8])
		return (-1);

	*co2 = reply[2] * 256 + reply[3];

	return (0);
}

void
mh_z19b_cycle(mdx_device_t dev, uint8_t *req,
    uint8_t *reply, int reply_len)
{
	int i;

	for (i = 0; i < 9; i++)
		mdx_uart_putc(dev, req[i]);

	for (i = 0; i < reply_len; i++)
		reply[i] = mdx_uart_getc(dev);
}
