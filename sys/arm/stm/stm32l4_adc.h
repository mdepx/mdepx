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

#ifndef _ARM_STM_STM32L4_ADC_H_
#define _ARM_STM_STM32L4_ADC_H_

#define	ADC_ISR		0x00 /* ADC interrupt and status register */
#define	 ISR_JQOVF	(1 << 10) /* Injected context queue overflow */
#define	 ISR_AWD3	(1 << 9) /* Analog watchdog 3 flag */
#define	 ISR_AWD2	(1 << 8) /* Analog watchdog 2 flag */
#define	 ISR_AWD1	(1 << 7) /* Analog watchdog 1 flag */
#define	 ISR_JEOS	(1 << 6) /* Injected channel end of sequence flag */
#define	 ISR_JEOC	(1 << 5) /* Injected channel end of conversion flag */
#define	 ISR_OVR	(1 << 4) /* ADC overrun */
#define	 ISR_EOS	(1 << 3) /* End of regular sequence flag */
#define	 ISR_EOC	(1 << 2) /* End of conversion flag */
#define	 ISR_EOSMP	(1 << 1) /* End of sampling flag */
#define	 ISR_ADRDY	(1 << 0) /* ADC ready */
#define	ADC_IER		0x04 /* ADC interrupt enable register */
#define	 IER_JQOVFIE	(1 << 10) /* Injected context queue overflow IE */
#define	 IER_AWD3IE	(1 << 9) /* Analog watchdog 3 interrupt enable */
#define	 IER_AWD2IE	(1 << 8) /* Analog watchdog 2 interrupt enable */
#define	 IER_AWD1IE	(1 << 7) /* Analog watchdog 1 interrupt enable */
#define	 IER_JEOSIE	(1 << 6) /* End of injected sequence of conversions IE */
#define	 IER_JEOCIE	(1 << 5) /* End of injected conversion interrupt enable */
#define	 IER_OVRIE	(1 << 4) /* Overrun interrupt enable */
#define	 IER_EOSIE	(1 << 3) /* End of regular sequence of conversions IE */
#define	 IER_EOCIE	(1 << 2) /* End of regular conversion interrupt enable */
#define	 IER_EOSMPIE	(1 << 1) /* End of sampling flag interrupt enable for regular conversions */
#define	 IER_ADRDYIE	(1 << 0) /* ADC ready interrupt enable */
#define	ADC_CR		0x08 /* ADC control register */
#define	 CR_ADCAL	(1 << 31) /* ADC calibration */
#define	 CR_ADCALDIF	(1 << 30) /* Differential mode for calibration */
#define	 CR_DEEPPWD	(1 << 29) /* Deep-power-down enable */
#define	 CR_ADVREGEN	(1 << 28) /* ADC voltage regulator enable */
#define	 CR_JADSTP	(1 << 5) /* ADC stop of injected conversion command */
#define	 CR_ADSTP	(1 << 4) /* ADC stop of regular conversion command */
#define	 CR_JADSTART	(1 << 3) /* ADC start of injected conversion */
#define	 CR_ADSTART	(1 << 2) /* ADC start of regular conversion */
#define	 CR_ADDIS	(1 << 1) /* ADC disable command */
#define	 CR_ADEN	(1 << 0) /* ADC enable control */
#define	ADC_CFGR	0x0C /* ADC configuration register */
#define	 CFGR_JQDIS	(1 << 31) /* Injected Queue disable */
#define	 CFGR_AWD1CH_S	26	/* Analog watchdog 1 channel selection */
#define	 CFGR_AWD1CH_M	(0x1f << CFGR_AWD1CH_S)
#define	 CFGR_JAUTO	(1 << 25) /* Automatic injected group conversion */
#define	 CFGR_JAWD1EN	(1 << 24) /* Analog watchdog 1 enable on injected channels */
#define	 CFGR_AWD1EN	(1 << 23)/* Analog watchdog 1 enable on regular channels */
#define	 CFGR_AWD1SGL	(1 << 22) /* Enable the watchdog 1 on a single channel or on all channels */
#define	 CFGR_JQM	(1 << 21) /* JSQR queue mode */
#define	 CFGR_JDISCEN	(1 << 20) /* Discontinuous mode on injected channels */
#define	 CFGR_DISCNUM_S	17	/* Discontinuous mode channel count */
#define	 CFGR_DISCNUM_M	(0x7 << CFGR_DISCNUM_S)
#define	 CFGR_DISCEN	(1 << 16) /* Discontinuous mode for regular channels */
#define	 CFGR_AUTDLY	(1 << 14) /* Delayed conversion mode */
#define	 CFGR_CONT	(1 << 13) /* Single / continuous conversion mode for regular conversions */
#define	 CFGR_OVRMOD	(1 << 12) /* Overrun mode */
#define	 CFGR_EXTEN_S	10 /* External trigger enable and polarity selection for regular channels */
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
#define	 CFGR_DFSDMCFG	(1 << 2) /* DFSDM mode configuration */
#define	 CFGR_DMACFG	(1 << 1) /* Direct memory access configuration */
#define	 CFGR_DMAEN	(1 << 0) /* Direct memory access enable */
#define	ADC_CFGR2	0x10 /* ADC configuration register 2 */
#define	 CFGR2_ROVSM	(1 << 10) /* Regular Oversampling mode */
#define	 CFGR2_TROVS	(1 << 9) /* Triggered Regular Oversampling */
#define	 CFGR2_OVSS_S	5	/* Oversampling shift */
#define	 CFGR2_OVSS_M	(0xf << CFGR2_OVSS_S)
#define	 CFGR2_OVSR_S	2	/* Oversampling ratio */
#define	 CFGR2_OVSR_M	(0x7 << CFGR2_OVSR_S)
#define	 CFGR2_JOVSE	(1 << 1) /* Injected Oversampling Enable */
#define	 CFGR2_ROVSE	(1 << 0) /* Regular Oversampling Enable */
#define	ADC_SMPR1	0x14 /* ADC sample time register 1 */
#define	ADC_SMPR2	0x18 /* ADC sample time register 2 */
#define	ADC_SMPR(n)	((n) > 9 ? ADC_SMPR2 : ADC_SMPR1)
				/* ADC sample time register */
#define	 SMPR_SMP_S(n)	((n) > 9 ? ((n) - 10) * 3 : (n) * 3)
				/* Channel x sampling time selection */
#define	 SMPR_SMP_M(n)	(0x7 << SMPR_SMP_S(n))
#define	ADC_TR1		0x20 /* ADC watchdog threshold register 1 */
#define	 TR1_HT1_S	16	/* Analog watchdog 1 higher threshold */
#define	 TR1_HT1_M	(0xfff << TR1_HT1_S)
#define	 TR1_LT1_S	0	/* Analog watchdog 1 lower threshold */
#define	 TR1_LT1_M	(0xfff << TR1_LT1_S)
#define	ADC_TR2		0x24 /* ADC watchdog threshold register 2 */
#define	 TR2_HT2_S	16	/* Analog watchdog 2 higher threshold */
#define	 TR2_HT2_M	(0xff << TR2_HT2_S)
#define	 TR2_LT2_S	0	/* Analog watchdog 2 lower threshold */
#define	 TR2_LT2_M	(0xff << TR2_LT2_S)
#define	ADC_TR3		0x28 /* ADC watchdog threshold register 3 */
#define	 TR3_HT3_S	16	/* Analog watchdog 3 higher threshold */
#define	 TR3_HT3_M	(0xff << TR3_HT3_S)
#define	 TR3_LT3_S	0	/* Analog watchdog 3 lower threshold */
#define	 TR3_LT3_M	(0xff << TR3_LT3_S)
#define	ADC_SQR1	0x30 /* ADC regular sequence register 1 */
#define	 SQR1_SQ4_S	24	/* 4th conversion in regular sequence */
#define	 SQR1_SQ4_M	(0x1f << SQR1_SQ4_S)
#define	 SQR1_SQ3_S	18	/* 3rd conversion in regular sequence */
#define	 SQR1_SQ3_M	(0x1f << SQR1_SQ3_S)
#define	 SQR1_SQ2_S	12	/* 2nd conversion in regular sequence */
#define	 SQR1_SQ2_M	(0x1f << SQR1_SQ2_S)
#define	 SQR1_SQ1_S	6	/* 1st conversion in regular sequence */
#define	 SQR1_SQ1_M	(0x1f << SQR1_SQ1_S)
#define	 SQR1_L_S	0	/* Regular channel sequence length */
#define	 SQR1_L_M	(0xf << SQR1_L_S)
#define	ADC_SQR2	0x34 /* ADC regular sequence register 2 */
#define	 SQR2_SQ9_S	24	/* 9th conversion in regular sequence */
#define	 SQR2_SQ9_M	(0x1f << SQR2_SQ9_S)
#define	 SQR2_SQ8_S	18	/* 8th conversion in regular sequence */
#define	 SQR2_SQ8_M	(0x1f << SQR2_SQ8_S)
#define	 SQR2_SQ7_S	12	/* 7th conversion in regular sequence */
#define	 SQR2_SQ7_M	(0x1f << SQR2_SQ7_S)
#define	 SQR2_SQ6_S	6	/* 6th conversion in regular sequence */
#define	 SQR2_SQ6_M	(0x1f << SQR2_SQ6_S)
#define	 SQR2_SQ5_S	0	/* 5th conversion in regular sequence */
#define	 SQR2_SQ5_M	(0x1f << SQR2_SQ5_S)
#define	ADC_SQR3	0x38 /* ADC regular sequence register 3 */
#define	 SQR3_SQ14_S	24	/* 14th conversion in regular sequence */
#define	 SQR3_SQ14_M	(0x1f << SQR3_SQ14_S)
#define	 SQR3_SQ13_S	18	/* 13th conversion in regular sequence */
#define	 SQR3_SQ13_M	(0x1f << SQR3_SQ13_S)
#define	 SQR3_SQ12_S	12	/* 12th conversion in regular sequence */
#define	 SQR3_SQ12_M	(0x1f << SQR3_SQ12_S)
#define	 SQR3_SQ11_S	6	/* 11th conversion in regular sequence */
#define	 SQR3_SQ11_M	(0x1f << SQR3_SQ11_S)
#define	 SQR3_SQ10_S	0	/* 10th conversion in regular sequence */
#define	 SQR3_SQ10_M	(0x1f << SQR3_SQ10_S)
#define	ADC_SQR4	0x3C /* ADC regular sequence register 4 */
#define	 SQR4_SQ16_S	6	/* 16th conversion in regular sequence */
#define	 SQR4_SQ16_M	(0x1f << SQR4_SQ16_S)
#define	 SQR4_SQ15_S	0	/* 15th conversion in regular sequence */
#define	 SQR4_SQ15_M	(0x1f << SQR4_SQ15_S)
#define	ADC_DR		0x40 /* ADC regular Data Register */
#define	 DR_RDATA_S	0	/* Regular Data converted */
#define	 DR_RDATA_M	(0xffff << DR_RDATA_S)
#define	ADC_JSQR	0x4C /* ADC injected sequence register */
#define	 JSQR_JSQ4_S	26	/* 4th conversion in the injected sequence */
#define	 JSQR_JSQ4_M	(0x1f << JSQR_JSQ4_S)
#define	 JSQR_JSQ3_S	20	/* 3rd conversion in the injected sequence */
#define	 JSQR_JSQ3_M	(0x1f << JSQR_JSQ3_S)
#define	 JSQR_JSQ2_S	14	/* 2nd conversion in the injected sequence */
#define	 JSQR_JSQ2_M	(0x1f << JSQR_JSQ2_S)
#define	 JSQR_JSQ1_S	8	/* 1st conversion in the injected sequence */
#define	 JSQR_JSQ1_M	(0x1f << JSQR_JSQ1_S)
#define	 JSQR_JEXTEN_S	6	/* External Trigger Enable and Polarity Selection for injected channels */
#define	 JSQR_JEXTEN_M	(0x3 << JSQR_JEXTEN_S)
#define	 JSQR_JEXTSEL_S	2	/* External Trigger Selection for injected group */
#define	 JSQR_JEXTSEL_M	(0xf << JSQR_JEXTSEL_S)
#define	 JSQR_JL_S	0	/* Injected channel sequence length */
#define	 JSQR_JL_M	(0x3 << JSQR_JL_S)
#define	ADC_OFR(n)	(0x60 + 0x4 * ((n) - 1)) /* ADC offset register 1,2,3,4 */
#define	 OFR_OFFSETy_EN	(1 << 31)	/* Offset y Enable */
#define	 OFR_OFFSETy_CH_S	26	/* Channel selection for the Data offset y */
#define	 OFR_OFFSETy_CH_M	(0x1f << OFR_OFFSETy_CH_S)
#define	ADC_JDR(n)	(0x80 + 0x4 * ((n) - 1)) /* ADC injected data register */
#define	 JDR_JDATA_S	0	/* Injected data */
#define	 JDR_JDATA_M	(0xffff << JDR_JDATA_S)
#define	ADC_AWD2CR	0xA0 /* ADC Analog Watchdog 2 Configuration Register */
#define	 AWD2CR_AWD2CH_S	0	/* Analog watchdog 2 channel selection */
#define	 AWD2CR_AWD2CH_M	(0x7ffff << AWD2CR_AWD2CH_S)
#define	 AWD2CR_AWD2CH(n)	((n) << AWD2CR_AWD2CH_S)
#define	ADC_AWD3CR	0xA4 /* ADC Analog Watchdog 3 Configuration Register */
#define	 AWD3CR_AWD2CH_S	0	/* Analog watchdog 2 channel selection */
#define	 AWD3CR_AWD2CH_M	(0x7ffff << AWD3CR_AWD2CH_S)
#define	ADC_DIFSEL	0xB0 /* ADC Differential mode Selection Register */
#define	 DIFSEL_DIFSEL_S	16	/* Differential mode for channels 18 to 16. */
#define	 DIFSEL_DIFSEL_M	(0x7 << DIFSEL_DIFSEL_S)
#define	 DIFSEL_DIFSEL_S	1	/* Differential mode for channels 15 to 1 */
#define	 DIFSEL_DIFSEL_M	(0x7fff << DIFSEL_DIFSEL_S)
#define	ADC_CALFACT	0xB4 /* ADC Calibration Factors */
#define	 CALFACT_CALFACT_D_S	16 /* Calibration Factors in differential mode */
#define	 CALFACT_CALFACT_D_M	(0x7f << CALFACT_CALFACT_D_S)
#define	 CALFACT_CALFACT_S_S	0 /* Calibration Factors In single-ended mode */
#define	 CALFACT_CALFACT_S_M	(0x7f << CALFACT_CALFACT_S_S)

/* Common registers */
#define	COMMON_REGS_OFFSET	0x300
#define	ADC_CSR		(0x00 + COMMON_REGS_OFFSET) /* ADC Common status register */
#define	 CSR_JQOVF_MST	(1 << 10)
		/* Injected Context Queue Overflow flag of the master ADC */
#define	 CSR_AWD3_MST	(1 << 9) /* Analog watchdog 3 flag of the master ADC */
#define	 CSR_AWD2_MST	(1 << 8) /* Analog watchdog 2 flag of the master ADC */
#define	 CSR_AWD1_MST	(1 << 7) /* Analog watchdog 1 flag of the master ADC */
#define	 CSR_JEOS_MST	(1 << 6) /* End of injected sequence flag of the master ADC */
#define	 CSR_JEOC_MST	(1 << 5)
		/* End of injected conversion flag of the master ADC */
#define	 CSR_OVR_MST	(1 << 4) /* Overrun flag of the master ADC */
#define	 CSR_EOS_MST	(1 << 3) /* End of regular sequence flag of the master ADC */
#define	 CSR_EOC_MST	(1 << 2) /* End of regular conversion of the master ADC */
#define	 CSR_EOSMP_MST	(1 << 1) /* End of Sampling phase flag of the master ADC */
#define	 CSR_ADRDY_MST	(1 << 0) /* Master ADC ready */
#define	ADC_CCR		(0x08 + COMMON_REGS_OFFSET) /* ADC common control register */
#define	 CCR_CH18SEL	(1 << 24) /* CH18 selection */
#define	 CCR_CH17SEL	(1 << 23) /* CH17 selection */
#define	 CCR_VREFEN	(1 << 22) /* VREFINT enable */
#define	 CCR_PRESC_S	18	/* ADC prescaler */
#define	 CCR_PRESC_M	(0xf << CCR_PRESC_S)
#define	 CCR_CKMODE_S	16	/* ADC clock mode */
#define	 CCR_CKMODE_M	(0x3 << CCR_CKMODE_S)

struct stm32l4_adc_softc {
	uint32_t base;
};

void stm32l4_adc_init(struct stm32l4_adc_softc *sc, uint32_t base);

#endif /* !_ARM_STM_STM32L4_ADC_H_ */
