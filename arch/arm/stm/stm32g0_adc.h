/*-
 * Copyright (c) 2026 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _ARM_STM_STM32G0_ADC_H_
#define _ARM_STM_STM32G0_ADC_H_

#define	ADC_ISR		0x00 /* ADC interrupt and status register */
#define	 ISR_AWD3	(1 << 9) /* Analog watchdog 3 flag */
#define	 ISR_AWD2	(1 << 8) /* Analog watchdog 2 flag */
#define	 ISR_AWD1	(1 << 7) /* Analog watchdog 1 flag */
#define	 ISR_OVR	(1 << 4) /* ADC overrun */
#define	 ISR_EOS	(1 << 3) /* End of regular sequence flag */
#define	 ISR_EOC	(1 << 2) /* End of conversion flag */
#define	 ISR_EOSMP	(1 << 1) /* End of sampling flag */
#define	 ISR_ADRDY	(1 << 0) /* ADC ready */
#define	ADC_IER		0x04 /* ADC interrupt enable register */
#define	 IER_AWD3IE	(1 << 9) /* Analog watchdog 3 interrupt enable */
#define	 IER_AWD2IE	(1 << 8) /* Analog watchdog 2 interrupt enable */
#define	 IER_AWD1IE	(1 << 7) /* Analog watchdog 1 interrupt enable */
#define	 IER_OVRIE	(1 << 4) /* Overrun interrupt enable */
#define	 IER_EOSIE	(1 << 3) /* End of regular sequence of conversions IE */
#define	 IER_EOCIE	(1 << 2) /* End of regular conversion IE */
#define	 IER_EOSMPIE	(1 << 1) /* End of sampling flag IE for regular conv */
#define	 IER_ADRDYIE	(1 << 0) /* ADC ready interrupt enable */
#define	ADC_CR		0x08 /* ADC control register */
#define	 CR_ADCAL	(1 << 31) /* ADC calibration */
#define	 CR_ADVREGEN	(1 << 28) /* ADC voltage regulator enable */
#define	 CR_ADSTP	(1 << 4) /* ADC stop of regular conversion command */
#define	 CR_ADSTART	(1 << 2) /* ADC start of regular conversion */
#define	 CR_ADDIS	(1 << 1) /* ADC disable command */
#define	 CR_ADEN	(1 << 0) /* ADC enable control */
#define	ADC_CFGR	0x0C /* ADC configuration register */
#define	 CFGR_AWD1CH_S	26	/* Analog watchdog 1 channel selection */
#define	 CFGR_AWD1CH_M	(0x1f << CFGR_AWD1CH_S)
#define	 CFGR_AWD1EN	(1 << 23)/* Analog watchdog 1 enable on regular chans */
#define	 CFGR_AWD1SGL	(1 << 22) /* En the wdog 1 on a single or all chans */
#define	 CFGR_DISCEN	(1 << 16) /* Discontinuous mode for regular channels */
#define	 CFGR_CONT	(1 << 13) /* Single / continuous conv mode */
#define	 CFGR_OVRMOD	(1 << 12) /* Overrun mode */
#define	 CFGR_EXTEN_S	10 /* External trigger en and polarity sel for reg ch*/
#define	 CFGR_EXTEN_M	(0x3 << CFGR_EXTEN_S)
#define	 CFGR_EXTSEL_S	6 /* External trigger selection for regular group */
#define	 CFGR_EXTSEL_M	(0xf << CFGR_EXTSEL_S)
#define	 CFGR_ALIGN	(1 << 5)	/* Data alignment */
#define	 CFGR_RES_S	3 /* Data resolution */
#define	 CFGR_RES_M	(0x3 << CFGR_RES_S)
#define	 CFGR_RES_12	(0 << CFGR_RES_S)
#define	 CFGR_RES_10	(1 << CFGR_RES_S)
#define	 CFGR_RES_8	(2 << CFGR_RES_S)
#define	 CFGR_RES_6	(3 << CFGR_RES_S)
#define	 CFGR_DMACFG	(1 << 1) /* Direct memory access configuration */
#define	 CFGR_DMAEN	(1 << 0) /* Direct memory access enable */
#define	ADC_CFGR2	0x10 /* ADC configuration register 2 */
#define	 CFGR2_TROVS	(1 << 9) /* Triggered Regular Oversampling */
#define	 CFGR2_OVSS_S	5	/* Oversampling shift */
#define	 CFGR2_OVSS_M	(0xf << CFGR2_OVSS_S)
#define	 CFGR2_OVSR_S	2	/* Oversampling ratio */
#define	 CFGR2_OVSR_M	(0x7 << CFGR2_OVSR_S)
#define	 CFGR2_OVSE	(1 << 0) /* Oversampling Enable */
#define	ADC_SMPR	0x14 /* ADC sample time register */
#define	ADC_AWD1TR		0x20 /* ADC watchdog threshold register 1 */
#define	 TR1_HT1_S	16	/* Analog watchdog 1 higher threshold */
#define	 TR1_HT1_M	(0xfff << TR1_HT1_S)
#define	 TR1_LT1_S	0	/* Analog watchdog 1 lower threshold */
#define	 TR1_LT1_M	(0xfff << TR1_LT1_S)
#define	ADC_AWD2TR	0x24 /* ADC watchdog threshold register 2 */
#define	 TR2_HT2_S	16	/* Analog watchdog 2 higher threshold */
#define	 TR2_HT2_M	(0xff << TR2_HT2_S)
#define	 TR2_LT2_S	0	/* Analog watchdog 2 lower threshold */
#define	 TR2_LT2_M	(0xff << TR2_LT2_S)
#define	ADC_CHSELR	0x28
#define	ADC_DR		0x40 /* ADC regular Data Register */
#define	 DR_RDATA_S	0	/* Regular Data converted */
#define	 DR_RDATA_M	(0xffff << DR_RDATA_S)

struct stm32g0_adc_softc {
	uint32_t base;
};

void stm32g0_adc_init(struct stm32g0_adc_softc *sc, uint32_t base);
int stm32g0_adc_conv(struct stm32g0_adc_softc *sc, int chan, int *data);
int stm32g0_adc_enable(struct stm32g0_adc_softc *sc);

#endif /* !_ARM_STM_STM32G0_ADC_H_ */
