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

#ifndef _RISCV_GIGADEVICE_GD32V_TIMER_H_
#define _RISCV_GIGADEVICE_GD32V_TIMER_H_

#define	TIMER_CTL0	0x00	/* Control register 0 */
#define	 CTL0_CEN	(1 << 0) /* Counter enable */
#define	TIMER_CTL1	0x04	/* Control register 1 */
#define	TIMER_SMCFG	0x08	/* Slave mode configuration register */
#define	TIMER_DMAINTEN	0x0C	/* DMA and interrupt enable register */
#define	 DMAINTEN_CH0IE	(1 << 1) /* Channel 0 capture/compare interrupt enable*/
#define	 DMAINTEN_UPIE	(1 << 0) /* Update interrupt enable */
#define	TIMER_INTF	0x10	/* Interrupt flag register */
#define	 INTF_CH0IF	(1 << 1) /* Channel 0 capture/compare interrupt flag */
#define	TIMER_SWEVG	0x14	/* Software event generation register */
#define	TIMER_CHCTL0	0x18	/* Channel control register 0 */
#define	TIMER_CHCTL1	0x1C	/* Channel control register 1 */
#define	TIMER_CHCTL2	0x20	/* Channel control register 2 */
#define	 CHCTL2_CH0EN	(1 << 0) /* Channel 0 capture/compare function enable*/
#define	TIMER_CNT	0x24	/* Counter register */
#define	TIMER_PSC	0x28	/* Prescaler register */
#define	TIMER_CAR	0x2C	/* Counter auto reload register */
#define	TIMER_CREP	0x30	/* Counter repetition register */
#define	TIMER_CH0CV	0x34	/* Channel 0 capture/compare value register */
#define	TIMER_CH1CV	0x38	/* Channel 1 capture/compare value register */
#define	TIMER_CH2CV	0x3C	/* Channel 2 capture/compare value register */
#define	TIMER_CH3CV	0x40	/* Channel 3 capture/compare value register */
#define	TIMER_CCHP	0x44	/* Complementary channel protection register */
#define	TIMER_DMACFG	0x48	/* DMA configuration register */
#define	TIMER_DMATB	0x4C	/* DMA transfer buffer register */

struct gd32v_timer_softc {
	uint32_t base;
	struct mi_timer mt;
};

int gd32v_timer_init(mdx_device_t dev, uint32_t base, int freq);
void gd32v_timer_intr(void *arg, int irq);

#endif /* !_RISCV_GIGADEVICE_GD32V_TIMER_H_ */
