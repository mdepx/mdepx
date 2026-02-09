/*-
 * Copyright (c) 2018-2023 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _ARM_STM_STM32F4_TIM_H_
#define _ARM_STM_STM32F4_TIM_H_

#define	TIM_CR1		0x00 /* Control register 1 */
#define	 CR1_DIR	(1 << 4) /* Counter used as downcounter */
#define	 CR1_OPM	(1 << 3) /* One pulse mode */
#define	 CR1_CEN	(1 << 0) /* Counter enable */
#define	TIM_CR2		0x04 /* Control register 2 */
#define	TIM_SMCR	0x08 /* Slave mode control register */
#define	TIM_DIER	0x0C /* DMA/interrupt enable register */
#define	 DIER_UIE	(1 << 0) /* Update interrupt enable */
#define	 DIER_CC1IE	(1 << 1) /* Capture/Compare 1 interrupt enable */
#define	TIM_SR		0x10 /* Status register */
#define	 SR_UIF		(1 << 0) /* Update interrupt flag */
#define	 SR_CC1IF	(1 << 1) /* Capture/Compare 1 interrupt flag */
#define	TIM_EGR		0x14 /* Event generation register */
#define	 EGR_UG		(1 << 0) /* Update generation */
#define	TIM_CCMR1	0x18 /* Capture/compare mode register 1 */
#define	 CCMR1_OC1M_S		4
#define	 CCMR1_OC1M_PWM1	(0x6 << CCMR1_OC1M_S)
#define	 CCMR1_OC1M_PWM2	(0x7 << CCMR1_OC1M_S)
#define	 CCMR1_OC1M_PWM1_ASYM	(0xe << CCMR1_OC1M_S)
#define	 CCMR1_OC1M_PWM2_ASYM	(0xf << CCMR1_OC1M_S)
#define	 CCMR1_OC1PE		(1 << 3) /* Output compare 1 preload enable */
#define	 CCMR1_OC2M_S		12
#define	 CCMR1_OC2M_PWM1	(0x6 << CCMR1_OC2M_S)
#define	 CCMR1_OC2M_PWM2	(0x7 << CCMR1_OC2M_S)
#define	 CCMR1_OC2PE		(1 << 11) /* Output compare 2 preload enable */
#define	TIM_CCMR2	0x1C /* Capture/compare mode register 2 */
#define	 CCMR2_OC3M_S		4
#define	 CCMR2_OC3M_PWM1	(0x6 << CCMR2_OC3M_S)
#define	 CCMR2_OC3M_PWM2	(0x7 << CCMR2_OC3M_S)
#define	 CCMR2_OC3PE		(1 << 3) /* Output compare 3 preload enable */
#define	 CCMR2_OC4M_S		12
#define	 CCMR2_OC4M_PWM1	(0x6 << CCMR2_OC4M_S)
#define	 CCMR2_OC4M_PWM2	(0x7 << CCMR2_OC4M_S)
#define	 CCMR2_OC4PE		(1 << 11) /* Output compare 4 preload enable */
#define	TIM_CCER	0x20 /* Capture/compare enable register */
#define	 CCER_CC4E	(1 << 12) /* Capture/Compare 4 output enable */
#define	 CCER_CC3NP	(1 << 11) /* Capture/Compare 3 complementary out pol */
#define	 CCER_CC3NE	(1 << 10) /* Capture/Compare 3 complementary out en */
#define	 CCER_CC3E	(1 << 8) /* Capture/Compare 3 output enable */
#define	 CCER_CC2NP	(1 << 7) /* Capture/Compare 2 complementary out pol */
#define	 CCER_CC2NE	(1 << 6) /* Capture/Compare 2 complementary out en */
#define	 CCER_CC2E	(1 << 4) /* Capture/Compare 2 output enable */
#define	 CCER_CC1NP	(1 << 3) /* Capture/Compare 1 complementary out pol */
#define	 CCER_CC1NE	(1 << 2) /* Capture/Compare 1 complementary out en */
#define	 CCER_CC1E	(1 << 0) /* Capture/Compare 1 output enable */
#define	TIM_CNT		0x24 /* Counter */
#define	TIM_PSC		0x28 /* Prescaler */
#define	 PSC_SHIFT	(0)
#define	 PSC_MASK	(0xffff << PSC_SHIFT)
#define	TIM_ARR		0x2C /* Auto-reload register */
#define	TIM_RCR		0x30 /* Repetition counter register */
#define	TIM_CCR1	0x34 /* Capture/compare register 1 */
#define	TIM_CCR2	0x38 /* Capture/compare register 2 */
#define	TIM_CCR3	0x3C /* Capture/compare register 3 */
#define	TIM_CCR4	0x40 /* Capture/compare register 4 */
#define	TIM_BDTR	0x44 /* Break and dead-time register */
#define	TIM_DCR		0x48 /* DMA control register */
#define	TIM_DMAR	0x4C /* DMA address for full transfer */

#endif /* !_ARM_STM_STM32F4_TIM_H_ */
