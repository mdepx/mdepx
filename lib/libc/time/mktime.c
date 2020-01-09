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

#include <sys/types.h>
#include <time.h>

#define	JDN_EPOCH_START		2440588	/* 1 Jan, 1970. */

static int
gregorian2jdn(int y, int m, int d)
{
	int jdn;

	/*
	 * 12.92 Converting between Gregorian Calendar Day and Julian
	 * Day Number.
	 * Page 604, Explanatory Supplement To The Astronomical Almanac,
	 * P. Kenneth Seidelmann, U.S. Naval Observatory.
	 */

	jdn = (1461 * (y + 4800 + (m - 14) / 12)) / 4 +
	    (367 * (m - 2 - 12 * ((m - 14) / 12))) / 12 -
	    (3 * ((y + 4900 + (m / 14) / 12) / 100)) / 4 +
	    d - 32075;

	return (jdn);
}

time_t
mktime(struct tm *tm)
{
	time_t t;
	int jdn;

	jdn = gregorian2jdn(tm->tm_year + 1900, tm->tm_mon, tm->tm_mday);
	jdn -= JDN_EPOCH_START;

	t = ((jdn * 24 + tm->tm_hour) * 60 + tm->tm_min) * 60 + tm->tm_sec;

	return (t);
}
