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
#define	ADC_IER		0x04 /* ADC interrupt enable register */
#define	ADC_CR		0x08 /* ADC control register */
#define	ADC_CFGR	0x0C /* ADC configuration register */
#define	ADC_CFGR2	0x10 /* ADC configuration register 2 */
#define	ADC_SMPR1	0x14 /* ADC sample time register 1 */
#define	ADC_SMPR2	0x18 /* ADC sample time register 2 */
#define	ADC_TR1		0x20 /* ADC watchdog threshold register 1 */
#define	ADC_TR2		0x24 /* ADC watchdog threshold register 2 */
#define	ADC_TR3		0x28 /* ADC watchdog threshold register 3 */
#define	ADC_SQR1	0x30 /* ADC regular sequence register 1 */
#define	ADC_SQR2	0x34 /* ADC regular sequence register 2 */
#define	ADC_SQR3	0x38 /* ADC regular sequence register 3 */
#define	ADC_SQR4	0x3C /* ADC regular sequence register 4 */
#define	ADC_DR		0x40 /* ADC regular Data Register */
#define	ADC_JSQR	0x4C /* ADC injected sequence register */
#define	ADC_OFR(n)	(0x60 + 0x4 * ((n) - 1)) /* ADC offset register 1,2,3,4 */
#define	ADC_JDR(n)	(0x80 + 0x4 * ((n) - 1)) /* ADC injected data register */
#define	ADC_AWD2CR	0xA0 /* ADC Analog Watchdog 2 Configuration Register */
#define	ADC_AWD3CR	0xA4 /* ADC Analog Watchdog 3 Configuration Register */
#define	ADC_DIFSEL	0xB0 /* ADC Differential mode Selection Register */
#define	ADC_CALFACT	0xB4 /* ADC Calibration Factors */

#define	CREGS_OFFS	0x300
#define	ADC_CSR		(0x00 + CREGS_OFFS) /* ADC Common status register */
#define	ADC_CCR		(0x08 + CREGS_OFFS) /* ADC common control register */

#endif /* !_ARM_STM_STM32L4_ADC_H_ */
