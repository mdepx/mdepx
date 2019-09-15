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
#include <sys/systm.h>
#include <arm/stm/stm32l4_rtc.h>

#include <machine/frame.h>

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

static int
stm32l4_rtc_init_enable(struct stm32l4_rtc_softc *sc)
{
	int timeout;
	int reg;

	/* Remove write protection */
	WR4(sc, RTC_WPR, 0xca);
	WR4(sc, RTC_WPR, 0x53);

	WR4(sc, RTC_ISR, ISR_INIT);

	timeout = 1000;
	do {
		reg = RD4(sc, RTC_ISR);
		if (reg & ISR_INITF)
			break;
	} while (--timeout);

	if (timeout == 0) {
		printf("%s: Failed to enter INIT mode\n", __func__);
		return (-1);
	}

	return (0);
}

static void
stm32l4_rtc_init_disable(struct stm32l4_rtc_softc *sc)
{
	uint32_t reg;

	reg = RD4(sc, RTC_ISR);
	reg &= ~ISR_INIT;
	WR4(sc, RTC_ISR, reg);
}

int
stm32l4_rtc_set_timedate(struct stm32l4_rtc_softc *sc,
    struct stm32l4_rtc_timedate *td)
{
	uint32_t reg;
	int err;

	err = stm32l4_rtc_init_enable(sc);
	if (err != 0)
		return (err);

	reg = (td->second / 10) << TR_ST_S;
	reg |= (td->second % 10) << TR_SU_S;
	reg |= (td->minute / 10) << TR_MNT_S;
	reg |= (td->minute % 10) << TR_MNU_S;
	reg |= (td->hour / 10) << TR_HT_S;
	reg |= (td->hour % 10) << TR_HU_S;
	WR4(sc, RTC_TR, reg);

	reg = (td->date / 10) << DR_DT_S;
	reg |= (td->date % 10) << DR_DU_S;
	reg |= (td->month / 10) ? DR_MT : 0;
	reg |= (td->month % 10) << DR_MU_S;
	reg |= td->weekday << DR_WDU_S;
	reg |= (td->year / 10) << DR_YT_S;
	reg |= (td->year % 10) << DR_YU_S;
	WR4(sc, RTC_DR, reg);

	stm32l4_rtc_init_disable(sc);

	return (0);
}

void
stm32l4_rtc_get_timedate(struct stm32l4_rtc_softc *sc,
    struct stm32l4_rtc_timedate *td)
{
	uint32_t reg;

	reg = RD4(sc, RTC_TR);
	td->second = ((reg & TR_ST_M) >> TR_ST_S) * 10;
	td->second += (reg & TR_SU_M) >> TR_SU_S;
	td->minute = ((reg & TR_MNT_M) >> TR_MNT_S) * 10;
	td->minute += (reg & TR_MNU_M) >> TR_MNU_S;
	td->hour = ((reg & TR_HT_M) >> TR_HT_S) * 10;
	td->hour += (reg & TR_HU_M) >> TR_HU_S;

	reg = RD4(sc, RTC_DR);
	td->date = ((reg & DR_DT_M) >> DR_DT_S) * 10;
	td->date += (reg & DR_DU_M) >> DR_DU_S;
	td->month = (reg & DR_MT) ? 10 : 0;
	td->month += (reg & DR_MU_M) >> DR_MU_S;
	td->weekday = (reg & DR_WDU_M) >> DR_WDU_S;
	td->year = ((reg & DR_YT_M) >> DR_YT_S) * 10;
	td->year += (reg & DR_YU_M) >> DR_YU_S;
}

void
stm32l4_rtc_init(struct stm32l4_rtc_softc *sc, uint32_t base)
{

	sc->base = base;
}
