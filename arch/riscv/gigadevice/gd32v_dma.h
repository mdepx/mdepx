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

#ifndef _RISCV_GIGADEVICE_GD32V_DMA_H_
#define _RISCV_GIGADEVICE_GD32V_DMA_H_

#define	DMA_INTF	0x00	/* Interrupt flag register */
#define	 INTF_GIF(n)	(1 << (0x4 * (n)) /* Global interrupt flag of chan x*/
#define	 INTF_FTFIF(n)	(2 << (0x4 * (n)) /* Full Transfer finish flag */
#define	 INTF_HTFIF(n)	(4 << (0x4 * (n)) /* Half transfer finish flag */
#define	 INTF_ERRIF(n)	(8 << (0x4 * (n)) /* Error flag of channel */
#define	DMA_INTC	0x04	/* Interrupt flag clear register */
#define	 INTF_GIFC(n)	(1 << (0x4 * (n)) /* Clear Global interrupt flag */
#define	 INTF_FTFIFC(n)	(2 << (0x4 * (n)) /* Clear Full Transfer finish flag */
#define	 INTF_HTFIFC(n)	(4 << (0x4 * (n)) /* Clear Half transfer finish flag */
#define	 INTF_ERRIFC(n)	(8 << (0x4 * (n)) /* Clear Error flag of channel */
#define	DMA_CHCTL(n)	(0x08 + (0x14 * (n))) /* Channel x control register */
#define	 CHCTL_M2M	(1 << 14) /* Memory to Memory Mode */
#define	 CHCTL_PRIO_S	12	/* Priority level */
#define	 CHCTL_PRIO_M	(0x3 << CHCTL_PRIO_S)
#define	 CHCTL_PRIO_LOW		(0x00 << CHCTL_PRIO_S)
#define	 CHCTL_PRIO_MEDIUM	(0x01 << CHCTL_PRIO_S)
#define	 CHCTL_PRIO_HIGH	(0x02 << CHCTL_PRIO_S)
#define	 CHCTL_PRIO_ULTRAHIGH	(0x03 << CHCTL_PRIO_S)
#define	 CHCTL_MWIDTH_S		10	/* Transfer data size of memory */
#define	 CHCTL_MWIDTH_M		(0x3 << CHCTL_MWIDTH_S)
#define	 CHCTL_MWIDTH_8BIT	(0x00 << CHCTL_MWIDTH_S)
#define	 CHCTL_MWIDTH_16BIT	(0x01 << CHCTL_MWIDTH_S)
#define	 CHCTL_MWIDTH_32BIT	(0x02 << CHCTL_MWIDTH_S)
#define	 CHCTL_PWIDTH_S		8	/* Transfer data size of peripheral */
#define	 CHCTL_PWIDTH_M		(0x3 << CHCTL_PWIDTH_S)
#define	 CHCTL_PWIDTH_8BIT	(0x00 << CHCTL_PWIDTH_S)
#define	 CHCTL_PWIDTH_16BIT	(0x01 << CHCTL_PWIDTH_S)
#define	 CHCTL_PWIDTH_32BIT	(0x02 << CHCTL_PWIDTH_S)
#define	 CHCTL_MNAGA		(1 << 7) /* Memory next address generation */
#define	 CHCTL_PNAGA		(1 << 6) /* Peripheral next address generation*/
#define	 CHCTL_CMEN		(1 << 5) /*  Circular mode enable */
#define	 CHCTL_DIR		(1 << 4) /* Transfer direction */
#define	 CHCTL_ERRIE		(1 << 3) /* Channel error interrupt enable */
#define	 CHCTL_HTFIE		(1 << 2) /* Channel half transfer finish intr*/
#define	 CHCTL_FTFIE		(1 << 1) /* Channel full transfer finish intr*/
#define	 CHCTL_CHEN		(1 << 0) /* Channel enable. */
#define	DMA_CHCNT(n)	(0x0C + (0x14 * (n))) /* Channel x counter register */
#define	 CHCNT_CNT_S	0
#define	 CHCNT_CNT_M	(0xffff << CHCNT_CNT_S) /* Transfer counter */
#define	DMA_CHPADDR(n)	(0x10 + (0x14 * (n))) /* Chan x peripheral base addr*/
#define	 CHPADDR_PADDR_S	0 /* Peripheral base address */
#define	DMA_CHMADDR(n)	(0x14 + (0x14 * (n))) /* Chan x memory base address */
#define	 CHPADDR_MADDR_S	0 /* Memory base address. */

struct gd32v_dma_softc {
	uint32_t base;
};

void gd32v_dma_intr(void *arg);
void gd32v_dma_init(mdx_device_t dev, uint32_t base);

#endif /* !_RISCV_GIGADEVICE_GD32V_DMA_H_ */
