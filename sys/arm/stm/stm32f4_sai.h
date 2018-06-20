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

#ifndef	_ARM_STM_STM32F4_SAI_H_
#define	_ARM_STM_STM32F4_SAI_H_

#define	SAI_GCR		0x00 /* Global configuration register */
#define	SAI_CR1(n)	(0x04 + 0x20 * n) /* Configuration register 1 */
#define	 CR1_MCKDIV_S	20 /* Master clock divider */
#define	 CR1_DMAEN	(1 << 17) /* DMA enable */
#define	 CR1_SAIEN	(1 << 16) /* Audio block enable */
#define	 CR1_MONO	(1 << 12) /* Mono mode */
#define	 CR1_CKSTR	(1 << 9) /* Clock strobing edge */
#define	 CR1_LSBFIRST	(1 << 8) /* Least significant bit first */
#define	 CR1_DS_S	5 /* Data size in bits */
#define	 CR1_DS_M	(0x3 << CR1_DS_S) /* Data size Mask */
#define	 CR1_DS_8	2
#define	 CR1_DS_10	3
#define	 CR1_DS_16	4
#define	 CR1_DS_20	5
#define	 CR1_DS_24	6
#define	 CR1_DS_32	7
#define	 CR1_MODE_S	0 /* SAIx audio block mode */
#define	 CR1_MODE_M	(2 << CR1_MODE_S)
#define	 CR1_MODE_MT	0 /* Master transmitter */
#define	 CR1_MODE_MR	1 /* Master receiver */
#define	 CR1_MODE_ST	2 /* Slave transmitter */
#define	 CR1_MODE_SR	3 /* Slave receiver */
#define	SAI_CR2(n)	(0x08 + 0x20 * n) /* Configuration register 2 */
#define	 CR2_MUTE	(1 << 5) /* Mute */
#define	 CR2_FFLUSH	(1 << 3) /* FIFO flush */
#define	 CR2_FTH_S	0 /* FIFO threshold */
#define	 CR2_FTH_M	(7 << CR2_FTH_S)
#define	 CR2_FTH_0	0 /* Empty */
#define	 CR2_FTH_25	1 /* 1/4 empty */
#define	 CR2_FTH_50	2 /* half empty */
#define	 CR2_FTH_75	3 /* 3/4 empty */
#define	 CR2_FTH_100	4 /* Full */
#define	SAI_FRCR(n)	(0x0C + 0x20 * n) /* Frame configuration register */
#define	 FRCR_FRL_S	0 /* Frame length. */
#define	 FRCR_FSALL_S	8 /* Frame synchronization active level length. */
#define	 FRCR_FSDEF	(1 << 16) /* Frame synchronization definition */
#define	 FRCR_FSPOL	(1 << 17) /* Frame synchronization polarity. */
#define	 FRCR_FSOFF	(1 << 18) /* Frame synchronization offset. */
#define	SAI_SLOTR(n)	(0x10 + 0x20 * n) /* Slot register */
#define	 SLOTR_SLOTEN_S	16 /* Slot enable */
#define	 SLOTR_NBSLOT_S	8 /* Number of slots in an audio frame */
#define	 SLOTR_SLOTSZ_S	6 /* Slot size */
#define	 SLOTR_FBOFF_S	0 /* First bit offset */
#define	SAI_IM(n)	(0x14 + 0x20 * n) /* Interrupt mask register 2 */
#define	 IM_AFSDETIE	(1 << 5) /* Anticipated frame synchronization detection interrupt enable */
#define	 IM_FREQIE	(1 << 3) /* FIFO request interrupt enable */
#define	 IM_WCKCFGIE	(1 << 2) /* Wrong clock configuration */
#define	 IM_OVRUDRIE	(1 << 0) /* Overrun/underrun interrupt enable */
#define	SAI_SR(n)	(0x18 + 0x20 * n) /* Status register */
#define	 SR_WCKCFG	(1 << 2) /* Wrong clock configuration flag. */
#define	 SR_FREQ	(1 << 3) /* FIFO request */
#define	SAI_CLRFR(n)	(0x1C + 0x20 * n) /* Clear flag register */
#define	SAI_DR(n)	(0x20 + 0x20 * n) /* Data register */

struct stm32f4_sai_softc {
	uint32_t base;
};

#endif	/* !_ARM_STM_STM32F4_SAI_H_ */
