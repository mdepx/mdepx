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

#ifndef _ARM_STM_STM32L4_RTC_H_
#define _ARM_STM_STM32L4_RTC_H_

#define	RTC_TR		0x00
#define	 TR_SU_S	0	/* Second units in BCD format */
#define	 TR_SU_M	(0xf << TR_SU_S)
#define	 TR_ST_S	4	/* Second tens in BCD format */
#define	 TR_ST_M	(0x7 << TR_ST_S)
#define	 TR_MNU_S	8	/* Minute units in BCD format */
#define	 TR_MNU_M	(0xf << TR_MNU_S)
#define	 TR_MNT_S	12	/* Minute tens in BCD format */
#define	 TR_MNT_M	(0x7 << TR_MNT_S)
#define	 TR_HU_S	16	/* Hour units in BCD format */
#define	 TR_HU_M	(0xf << TR_HU_S)
#define	 TR_HT_S	20	/* Hour tens in BCD format */
#define	 TR_HT_M	(0x3 << TR_HT_S)
#define	 TR_PM		(1 << 22)	/* AM/PM notation */
#define	RTC_DR		0x04
#define	 DR_DU_S	0	/* Date units in BCD format */
#define	 DR_DU_M	(0xf << DR_DU_S)
#define	 DR_DT_S	4	/* Date tens in BCD format */
#define	 DR_DT_M	(0x3 << DR_DT_S)
#define	 DR_MU_S	8	/* Month units in BCD format */
#define	 DR_MU_M	(0xf << DR_MU_S)
#define	 DR_MT		(1 << 12)
#define	 DR_WDU_S	13	/* Week day units */
#define	 DR_WDU_M	(0x7 << DR_WDU_S)
#define	 DR_WDU_MON	(1 << DR_WDU_S)
#define	 DR_WDU_TUE	(2 << DR_WDU_S)
#define	 DR_WDU_WED	(3 << DR_WDU_S)
#define	 DR_WDU_THU	(4 << DR_WDU_S)
#define	 DR_WDU_FRI	(5 << DR_WDU_S)
#define	 DR_WDU_SAT	(6 << DR_WDU_S)
#define	 DR_WDU_SUN	(7 << DR_WDU_S)
#define	 DR_YU_S	16	/* Year units in BCD format */
#define	 DR_YU_M	(0xf << DR_YU_S)
#define	 DR_YT_S	20	/* Year tens in BCD format */
#define	 DR_YT_M	(0xf << DR_YT_S)
#define	RTC_CR		0x08
#define	 CR_ITSE	(1 << 24)	/* timestamp on internal event enable */
#define	 CR_COE		(1 << 23)	/* Calibration output enable */
#define	 CR_OSEL_S	21	/* Output selection */
#define	 CR_OSEL_M	(0x3 << CR_OSEL_S)
#define	 CR_POL		(1 << 20)	/* Output polarity */
#define	 CR_COSEL	(1 << 19)	/* Calibration output selection */
#define	 CR_BKP		(1 << 18)	/* Backup */
#define	 CR_SUB1H	(1 << 17)	/* Subtract 1 hour (winter time change) */
#define	 CR_ADD1H	(1 << 16)	/* Add 1 hour (summer time change) */
#define	 CR_TSIE	(1 << 15)	/* Time-stamp interrupt enable */
#define	 CR_WUTIE	(1 << 14)	/* Wakeup timer interrupt enable */
#define	 CR_ALRBIE	(1 << 13)	/* Alarm B interrupt enable */
#define	 CR_ALRAIE	(1 << 12)	/* Alarm A interrupt enable */
#define	 CR_TSE		(1 << 11)	/* timestamp enable */
#define	 CR_WUTE	(1 << 10)	/* Wakeup timer enable */
#define	 CR_ALRBE	(1 << 9)	/* Alarm B enable */
#define	 CR_ALRAE	(1 << 8)	/* Alarm A enable */
#define	 CR_FMT		(1 << 6)	/* Hour format: AM/PM hour format */
#define	 CR_BYPSHAD	(1 << 5)	/* Bypass the shadow registers */
#define	 CR_REFCKON	(1 << 4)	/* RTC_REFIN reference clock detection enable (50 or 60 Hz) */
#define	 CR_TSEDGE	(1 << 3)	/* Time-stamp event active edge */
#define	 CR_WUCKSEL_S	0	/* Wakeup clock selection */
#define	 CR_WUCKSEL_M	(0x7 << CR_WUCKSEL_S)
#define	 CR_WUCKSEL_RTC16	(0 << CR_WUCKSEL_S)
#define	 CR_WUCKSEL_RTC8	(1 << CR_WUCKSEL_S)
#define	 CR_WUCKSEL_RTC4	(2 << CR_WUCKSEL_S)
#define	 CR_WUCKSEL_RTC2	(3 << CR_WUCKSEL_S)
#define	 CR_WUCKSEL_CK_SPRE	(4 << CR_WUCKSEL_S)
#define	 CR_WUCKSEL_CK_SPRE_WUT	(6 << CR_WUCKSEL_S)
#define	RTC_ISR		0x0C
#define	 ISR_ITSF	(1 << 17)	/* Internal tTime-stamp flag */
#define	 ISR_RECALPF	(1 << 16)	/* Recalibration pending Flag */
#define	 ISR_TAMP3F	(1 << 15)	/* RTC_TAMP3 detection flag */
#define	 ISR_TAMP2F	(1 << 14)	/* RTC_TAMP2 detection flag */
#define	 ISR_TAMP1F	(1 << 13)	/* RTC_TAMP1 detection flag */
#define	 ISR_TSOVF	(1 << 12)	/* Time-stamp overflow flag */
#define	 ISR_TSF	(1 << 11)	/* Time-stamp flag */
#define	 ISR_WUTF	(1 << 10)	/* Wakeup timer flag */
#define	 ISR_ALRBF	(1 << 9)	/* Alarm B flag */
#define	 ISR_ALRAF	(1 << 8)	/* Alarm A flag */
#define	 ISR_INIT	(1 << 7)	/* Initialization mode */
#define	 ISR_INITF	(1 << 6)	/* Initialization flag */
#define	 ISR_RSF	(1 << 5)	/* Registers synchronization flag */
#define	 ISR_INITS	(1 << 4)	/* Initialization status flag */
#define	 ISR_SHPF	(1 << 3)	/* Shift operation pending */
#define	 ISR_WUTWF	(1 << 2)	/* Wakeup timer write flag */
#define	 ISR_ALRBWF	(1 << 1)	/* Alarm B write flag */
#define	 ISR_ALRAWF	(1 << 0)	/* Alarm A write flag */
#define	RTC_PRER	0x10
#define	 PRER_PREDIV_A_S	16	/* Asynchronous prescaler factor */
#define	 PRER_PREDIV_A_M	(0x7f << PRER_PREDIV_A_S)
#define	 PRER_PREDIV_S_S	0	/* Synchronous prescaler factor */
#define	 PRER_PREDIV_S_M	(0x7fff << PRER_PREDIV_S_S)
#define	RTC_WUTR	0x14
#define	 WUTR_WUT_S	0	/* Wakeup auto-reload value bits */
#define	 WUTR_WUT_M	(0xffff << WUTR_WUT_S)
#define	RTC_ALRMAR	0x1C
#define	RTC_ALRMBR	0x20
#define	 ALRMAR_MSK4	(1 << 31)	/* Alarm A date mask */
#define	 ALRMAR_WDSEL	(1 << 30)	/* Week day selection */
#define	 ALRMAR_DT_S	28		/* Date tens in BCD format. */
#define	 ALRMAR_DT_M	(0x3 << ALRMAR_DT_S)
#define	 ALRMAR_DU_S	24		/* Date units or day in BCD format. */
#define	 ALRMAR_DU_M	(0xf << ALRMAR_DU_S)
#define	 ALRMAR_MSK3	(1 << 23)	/* Alarm A hours mask */
#define	 ALRMAR_PM	(1 << 22)	/* AM/PM notation */
#define	 ALRMAR_HT_S	20		/* Hour tens in BCD format. */
#define	 ALRMAR_HT_M	(0x3 << ALRMAR_HT_S)
#define	 ALRMAR_HU_S	16		/* Hour units in BCD format. */
#define	 ALRMAR_HU_M	(0xf << ALRMAR_HU_S)
#define	 ALRMAR_MSK2	(1 << 15)	/* Alarm A minutes mask */
#define	 ALRMAR_MNT_S	12		/* Minute tens in BCD format. */
#define	 ALRMAR_MNT_M	(0x7 << ALRMAR_MNT_S)
#define	 ALRMAR_MNU_S	8		/* Minute units in BCD format. */
#define	 ALRMAR_MNU_M	(0xf << ALRMAR_MNU_S)
#define	 ALRMAR_MSK1	(1 << 7)	/* Alarm A seconds mask */
#define	 ALRMAR_ST_S	4		/* Second tens in BCD format. */
#define	 ALRMAR_ST_M	(0x7 << ALRMAR_ST_S)
#define	 ALRMAR_SU_S	0		/* Second units in BCD format. */
#define	 ALRMAR_SU_M	(0xf << ALRMAR_SU_S)
#define	RTC_WPR		0x24
#define	 WPR_KEY_S	0	/* Write protection key */
#define	 WPR_KEY_M	(0xff << WPR_KEY_S)
#define	RTC_SSR		0x28
#define	 SSR_SS_S	0	/* Sub second value */
#define	 SSR_SS_M	(0xffff << SSR_SS_S)
#define	RTC_SHIFTR	0x2C
#define	 SHIFTR_ADD1S	(1 << 31)	/* Add one second */
#define	 SHIFTR_SUBFS_S	0	/* Subtract a fraction of a second */
#define	 SHIFTR_SUBFS_M	(0x7fff << SHIFTR_SUBFS_S)
#define	RTC_TSTR	0x30
#define	 TSTR_PM	(1 << 22)	/* AM/PM notation */
#define	 TSTR_HT_S	20	/* Hour tens in BCD format. */
#define	 TSTR_HT_M	(0x3 << TSTR_HT_S)
#define	 TSTR_HU_S	16	/* Hour units in BCD format. */
#define	 TSTR_HU_M	(0xf << TSTR_HU_S)
#define	 TSTR_MNT_S	12	/* Minute tens in BCD format. */
#define	 TSTR_MNT_M	(0x7 << TSTR_MNT_S)
#define	 TSTR_MNU_S	8	/* Minute units in BCD format. */
#define	 TSTR_MNU_M	(0xf << TSTR_MNU_S)
#define	 TSTR_ST_S	4	/* Second tens in BCD format. */
#define	 TSTR_ST_M	(0x7 << TSTR_ST_S)
#define	 TSTR_SU_S	0	/* Second units in BCD format. */
#define	 TSTR_SU_M	(0xf << TSTR_SU_S)
#define	RTC_TSDR	0x34
#define	 TSDR_WDU_S	13	/* Week day units */
#define	 TSDR_WDU_M	(0x7 << TSDR_WDU_S)
#define	 TSDR_MT	(1 << 12)	/* Month tens in BCD format */
#define	 TSDR_MU_S	8	/* Month units in BCD format */
#define	 TSDR_MU_M	(0xf << TSDR_MU_S)
#define	 TSDR_DT_S	4	/* Date tens in BCD format */
#define	 TSDR_DT_M	(0x3 << TSDR_DT_S)
#define	 TSDR_DU_S	0	/* Date units in BCD format */
#define	 TSDR_DU_M	(0xf << TSDR_DU_S)
#define	RTC_TSSSR	0x38
#define	 TSSSR_SS_S	0	/* Sub second value */
#define	 TSSSR_SS_M	(0xffff << TSSSR_SS_S)
#define	RTC_CALR	0x3C
#define	 CALR_CALP	(1 << 15)	/* Increase frequency of RTC by 488.5 ppm */
#define	 CALR_CALW8	(1 << 14)	/* Use an 8-second calibration cycle period */
#define	 CALR_CALW16	(1 << 13)	/* Use a 16-second calibration cycle period */
#define	 CALR_CALM_S	0	/* Calibration minus */
#define	 CALR_CALM_M	(0x1ff << CALR_CALM_S)
#define	RTC_TAMPCR	0x40
#define	 TAMPCR_TAMP3MF		(1 << 24)	/* Tamper 3 mask flag */
#define	 TAMPCR_TAMP3NOERASE	(1 << 23)	/* Tamper 3 no erase */
#define	 TAMPCR_TAMP3IE		(1 << 22)	/* Tamper 3 interrupt enable */
#define	 TAMPCR_TAMP2MF		(1 << 21)	/* Tamper 2 mask flag */
#define	 TAMPCR_TAMP2NOERASE	(1 << 20)	/* Tamper 2 no erase */
#define	 TAMPCR_TAMP2IE		(1 << 19)	/* Tamper 2 interrupt enable */
#define	 TAMPCR_TAMP1MF		(1 << 18)	/* Tamper 1 mask flag */
#define	 TAMPCR_TAMP1NOERASE	(1 << 17)	/* Tamper 1 no erase */
#define	 TAMPCR_TAMP1IE		(1 << 16)	/* Tamper 1 interrupt enable */
#define	 TAMPCR_TAMPPUDIS	(1 << 15)	/* RTC_TAMPx pull-up disable */
#define	 TAMPCR_TAMPPRCH_S	13	/* RTC_TAMPx precharge duration */
#define	 TAMPCR_TAMPPRCH_M	(0x3 << TAMPCR_TAMPPRCH_S)
#define	 TAMPCR_TAMPFLT_S	11	/* RTC_TAMPx filter count */
#define	 TAMPCR_TAMPFLT_M	(0x3 << TAMPCR_TAMPFLT_S)
#define	 TAMPCR_TAMPFREQ_S	8	/* Tamper sampling frequency */
#define	 TAMPCR_TAMPFREQ_M	(0x7 << TAMPCR_TAMPFREQ_S)
#define	 TAMPCR_TAMPTS		(1 << 7)	/* Activate timestamp on tamper detection event */
#define	 TAMPCR_TAMP3TRG	(1 << 6)	/* Active level for RTC_TAMP3 input */
#define	 TAMPCR_TAMP3E		(1 << 5)	/* RTC_TAMP3 detection enable */
#define	 TAMPCR_TAMP2TRG	(1 << 4)	/* Active level for RTC_TAMP2 input */
#define	 TAMPCR_TAMP2E		(1 << 3)	/* RTC_TAMP2 input detection enable */
#define	 TAMPCR_TAMPIE		(1 << 2)	/* Tamper interrupt enable */
#define	 TAMPCR_TAMP1TRG	(1 << 1)	/* Active level for RTC_TAMP1 input */
#define	 TAMPCR_TAMP1E		(1 << 0)	/* RTC_TAMP1 input detection enable */
#define	RTC_ALRMASSR		0x44
#define	RTC_ALRMBSSR		0x48
#define	 ALRMASSR_MASKSS_S	24		/* Mask the most-significant bits starting at this bit */
#define	 ALRMASSR_MASKSS_M	(0xf << ALRMASSR_MASKSS_S)
#define	 ALRMASSR_SS_S		0	/* Sub seconds value */
#define	 ALRMASSR_SS_M		(0x7fff << ALRMASSR_SS_S)
#define	RTC_OR			0x4C
#define	 OR_RTC_OUT_RMP		(1 << 1)	/* RTC_OUT remap */
#define	 OR_RTC_ALARM_TYPE	(1 << 0)	/* RTC_ALARM output type on PC13 */
#define	RTC_BKPR(n)		(0x50 + 0x4 * (n))
#define	 BKPR_BKP_S		0	/* application data */
#define	 BKPR_BKP_M		(0xffffffff << BKPR_BKP_S)

struct stm32l4_rtc_timedate {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t pm;
	uint8_t date;
	uint8_t month;
	uint8_t weekday;
	uint8_t year;
};

struct stm32l4_rtc_softc {
	uint32_t base;
};

void stm32l4_rtc_init(struct stm32l4_rtc_softc *sc, uint32_t base);
int stm32l4_rtc_set_timedate(struct stm32l4_rtc_softc *sc,
    struct stm32l4_rtc_timedate *td);
void stm32l4_rtc_get_timedate(struct stm32l4_rtc_softc *sc,
    struct stm32l4_rtc_timedate *td);

#endif /* !_ARM_STM_STM32L4_RTC_H_ */
