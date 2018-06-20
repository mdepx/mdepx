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

#ifndef	_ARM_STM_STM32F4_DMA_H_
#define	_ARM_STM_STM32F4_DMA_H_

#define	DMA_LISR	0x00 /* low interrupt status register */
#define	DMA_HISR	0x04 /* high interrupt status register */
#define	DMA_ISR(n)	(n > 3 ? DMA_HISR : DMA_LISR)
#define	DMA_LIFCR	0x08 /* low interrupt flag clear register */
#define	DMA_HIFCR	0x0C /* high interrupt flag clear register */
#define	DMA_IFCR(n)	(n > 3 ? DMA_HIFCR : DMA_LIFCR)
#define	 offN(n)	(n > 1 ? (16 + 6 * (n - 2)) : (0 + 6 * n))
#define	 offs(n)	(n > 3 ? offN((n - 4)) : offN(n))
#define	 TCIF(n)	(1 << (5 + offs(n))) /* Stream x transfer complete */
#define	 HTIF(n)	(1 << (4 + offs(n))) /* Stream x half transfer */
#define	 TEIF(n)	(1 << (3 + offs(n))) /* Stream x transfer error */
#define	 DMEIF(n)	(1 << (2 + offs(n))) /* Stream x direct mode error */
#define	 FEIF(n)	(1 << (0 + offs(n))) /* Stream x FIFO error */

#define	DMA_SCR(n)	(0x10 + n * 0x18) /* stream x configuration register */
#define	 SCR_CHSEL_S	25 /* Channel selection */
#define	 SCR_CHSEL_M	(7 << SCR_CHSEL_S) /* Channel selection Mask */
#define	 SCR_MBURST_S	23 /* Memory burst transfer configuration */
#define	 SCR_MBURST_M	(3 << SCR_MBURST_S)
#define	  MBURST_SNGL	0
#define	  MBURST_INCR4	1
#define	  MBURST_INCR8	2
#define	  MBURST_INCR16	3
#define	 SCR_PBURST_S	21 /* Peripheral burst transfer configuration */
#define	 SCR_PBURST_M	3
#define	  PBURST_SNGL	0
#define	  PBURST_INCR4	1
#define	  PBURST_INCR8	2
#define	  PBURST_INCR16	3
#define	 SCR_CT		(1 << 19) /* Current target (only in double buffer mode) */
#define	 SCR_DBM	(1 << 18) /* Double buffer mode */
#define	 SCR_MSIZE_S	13 /* Memory data size */
#define	 SCR_MSIZE_M	(3 << SCR_MSIZE_S)
#define	  MSIZE_8	0
#define	  MSIZE_16	1
#define	  MSIZE_32	2
#define	 SCR_PSIZE_S	11 /* Peripheral data size */
#define	 SCR_PSIZE_M	(3 << SCR_PSIZE_S)
#define	  PSIZE_8	0
#define	  PSIZE_16	1
#define	  PSIZE_32	2
#define	 SCR_CIRC	(1 << 8) /* Circular mode */
#define	 SCR_MINC	(1 << 10) /* Memory increment mode */
#define	 SCR_DIR_S	6 /* Data transfer direction */
#define	 SCR_DIR_M	(3 << SCR_DIR_S)
#define	 SCR_DIR_P2M	0
#define	 SCR_DIR_M2P	1
#define	 SCR_DIR_M2M	2
#define	 SCR_PFCTRL	(1 << 5) /* Peripheral flow controller */
#define	 SCR_TCIE	(1 << 4) /* Transfer complete interrupt enable */
#define	 SCR_HTIE	(1 << 3) /* Half transfer interrupt enable */
#define	 SCR_TEIE	(1 << 2) /* Transfer error interrupt enable */
#define	 SCR_DMEIE	(1 << 1) /* Direct mode error interrupt enable */
#define	 SCR_EN		(1 << 0) /* Stream enable / flag stream ready when read low */
#define	DMA_SNDTR(n)	(0x14 + n * 0x18) /* stream x number of data register */
#define	DMA_SPAR(n)	(0x18 + n * 0x18) /* stream x peripheral address register */
#define	DMA_SM0AR(n)	(0x1C + n * 0x18) /* stream x memory 0 address register */
#define	DMA_SM1AR(n)	(0x20 + n * 0x18) /* stream x memory 1 address register */
#define	DMA_SFCR(n)	(0x24 + n * 0x18) /* stream x FIFO control register */
#define	 SFCR_DMDIS	(1 << 2) /* Direct mode disable */
#define	 SFCR_FTH_S	0 /* FIFO threshold selection */
#define	 SFCR_FTH_M	(0x3 << SFCR_FTH_S)
#define	 SFCR_FTH_FULL	0x3 /* Full FIFO */

struct stm32f4_dma_softc {
	uint32_t base;
};

#endif	/* !_ARM_STM_STM32F4_DMA_H_ */
