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
#include <sys/time.h>

static struct rtc_driver *l_rtc_driver;
static void *l_rtc_arg;

int
mdx_clock_gettime(clockid_t clk_id, struct timespec *tp)
{

	switch (clk_id) {
	case CLOCK_REALTIME:
		if (l_rtc_driver == NULL)
			return (-1);
		return (l_rtc_driver->gettime(tp, l_rtc_arg));
	default:
		return (-1);
	}

	return (0);
}

int
mdx_clock_settime(clockid_t clk_id, const struct timespec *tp)
{

	switch (clk_id) {
	case CLOCK_REALTIME:
		if (l_rtc_driver == NULL)
			return (-1);
		return (l_rtc_driver->settime(tp, l_rtc_arg));
	default:
		return (-1);
	}

	return (0);
}

int
mdx_clock_register(clockid_t clk_id, void *drv, void *arg)
{
	struct rtc_driver *rtc_drv;

	if (clk_id != CLOCK_REALTIME)
		return (-1);

	if (drv == NULL)
		return (-1);

	rtc_drv = drv;

	if (rtc_drv->settime == NULL || rtc_drv->gettime == NULL)
		return (-1);

	l_rtc_driver = rtc_drv;
	l_rtc_arg = arg;

	return (0);
}
