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
#define	SEC_IN_DAY		(60 * 60 * 24)

static void
jdn2gregorian(int jdn, int *year, int *mon, int *mday)
{
	long l, n, i, j, d, m, y;

	/*
	 * 12.92 Converting between Gregorian Calendar Day and Julian
	 * Day Number.
	 * Page 604, Explanatory Supplement To The Astronomical Almanac,
	 * P. Kenneth Seidelmann, U.S. Naval Observatory.
	 */

	l = jdn + 68569;
	n = (4 * l) / 146097;
	l = l - (146097 * n + 3) / 4;
	i = (4000 * (l + 1)) / 1461001;
	l = l - (1461 * i) / 4 + 31;
	j = (80 * l) / 2447;
	d = l - (2447 * j) / 80;
	l = j / 11;
	m = j + 2 - 12 * l;
	y = 100 * (n - 49) + i + l;

	*year = (int)y;
	*mon = (int)m;
	*mday = (int)d;
}

struct tm *
gmtime_r(const time_t *tp, struct tm *result)
{
	int year, mon, mday;
	int sec, min, hour;
	int jdn;

	jdn = *tp / SEC_IN_DAY;
	jdn += JDN_EPOCH_START;

	jdn2gregorian(jdn, &year, &mon, &mday);

	result->tm_year = year - 1900;
	result->tm_mon = mon - 1;
	result->tm_mday = mday;

	hour = (*tp / (60 * 60)) % 24;
	min = (*tp / 60) % 60;
	sec = *tp % 60;

	result->tm_hour = hour;
	result->tm_min = min;
	result->tm_sec = sec;

	return (result);
}
