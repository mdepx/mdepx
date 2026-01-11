/*-
 * Copyright (c) 2018-2025 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _ARM_STM_STM32N6_LTDC_H_
#define _ARM_STM_STM32N6_LTDC_H_

#include <dev/display/panel.h>
#include <dev/display/dsi.h>

#define	LTDC_SSCR	0x0008 /* Synchronization size CR. */
#define	 SSCR_HSW_S	16 /* Horizontal synchronization width */
#define	 SSCR_VSW_S	0 /* Vertical synchronization height */
#define	LTDC_BPCR	0x000C /* Back porch configuration register. */
#define	 BPCR_AHBP_S	16
#define	 BPCR_AVBP_S	0
#define	LTDC_AWCR	0x0010 /*   */
#define	 AWCR_AAW_S	16
#define	 AWCR_AAH_S	0
#define	LTDC_TWCR	0x0014 /*   */
#define	 TWCR_TOTALW_S	16
#define	 TWCR_TOTALH_S	0
#define	LTDC_GCR	0x0018 /*   */
#define	 GCR_HSPOL	(1 << 31) /* Horizontal Synchronization Polarity */
#define	 GCR_VSPOL	(1 << 30) /* Vertical Synchronization Polarity */
#define	 GCR_DEPOL	(1 << 29) /* Data Enable Polarity */
#define	 GCR_PCPOL	(1 << 28) /* Pixel Clock Polarity */
#define	 GCR_LTDCEN	(1 << 0) /* LCD-TFT controller enable bit */
#define	LTDC_SRCR	0x0024 /*   */
#define	 SRCR_IMR	(1 << 0) /* Immediate Reload */
#define	LTDC_BCCR	0x002C /*   */
#define	LTDC_IER	0x0034 /*   */
#define	LTDC_ISR	0x0038 /*   */
#define	LTDC_ICR	0x003C /*   */
#define	LTDC_LIPCR	0x0040 /*   */
#define	LTDC_CPSR	0x0044 /*   */
#define	LTDC_CDSR	0x0048 /*   */

#define	LTDC_ISR2	0x68

#define	LTDC_LC0R(x)	(0x100 + 0x100 * (x))
#define	LTDC_LC1R(x)	(0x104 + 0x100 * (x))
#define	LTDC_LRCR(x)	(0x108 + 0x100 * (x))
#define	 LRCR_IMR	(1 << 0) /* Immediate reload trigger */
#define	LTDC_LxCR(x)	(0x10C + 0x100 * (x))
#define	 LCR_LEN	(1 << 0) /* Layer enable */
#define	 LCR_CKEN	(1 << 1) /* Color keying enable */
#define	 LCR_CLUTEN	(1 << 4) /* Color look-up table enable */
#define	 LCR_HMEN	(1 << 8) /* Horizontal mirroring enable */
#define	 LCR_DCBEN	(1 << 9) /* Default color blending enable */
#define	 LCR_SCEN	(1 << 10) /* Scaler enable */
#define	LTDC_LWHPCR(x)	(0x110 + 0x100 * (x))
#define	LTDC_LWVPCR(x)	(0x114 + 0x100 * (x))
#define	LTDC_LCKCR(x)	(0x118 + 0x100 * (x))
#define	LTDC_LPFCR(x)	(0x11C + 0x100 * (x))
#define	 LPFCR_PF_S		0
#define	 LPFCR_PF_ARGB8888	(0 << LPFCR_PF_S)
#define	 LPFCR_PF_ABGR8888	(1 << LPFCR_PF_S)
#define	 LPFCR_PF_RGBA8888	(2 << LPFCR_PF_S)
#define	 LPFCR_PF_BGRA8888	(3 << LPFCR_PF_S)
#define	 LPFCR_PF_RGB565	(4 << LPFCR_PF_S)
#define	 LPFCR_PF_BGR565	(5 << LPFCR_PF_S)
#define	 LPFCR_PF_RGB888	(6 << LPFCR_PF_S)
#define	 LPFCR_PF_FLEXIBLE	(7 << LPFCR_PF_S)
#define	LTDC_LCACR(x)	(0x120 + 0x100 * (x))
#define	LTDC_LDCCR(x)	(0x124 + 0x100 * (x))
#define	LTDC_LBFCR(x)	(0x128 + 0x100 * (x))
#define	LTDC_LBLCR(x)	(0x12C + 0x100 * (x))
#define	LTDC_LPCR(x)	(0x130 + 0x100 * (x))
#define	LTDC_LCFBAR(x)	(0x134 + 0x100 * (x))
#define	LTDC_LCFBLR(x)	(0x138 + 0x100 * (x))
#define	LTDC_LCFBLNR(x)	(0x13C + 0x100 * (x))
#define	LTDC_LAFBA0R(x)	(0x140 + 0x100 * (x))
#define	LTDC_LAFBA1R(x)	(0x144 + 0x100 * (x))
#define	LTDC_LAFBLR(x)	(0x148 + 0x100 * (x))
#define	LTDC_LAFBLNR(x)	(0x14C + 0x100 * (x))
#define	LTDC_LCLUTWR(x)	(0x150 + 0x100 * (x))
#define	LTDC_LSISR(x)	(0x154 + 0x100 * (x))
#define	LTDC_LSOSR(x)	(0x158 + 0x100 * (x))
#define	LTDC_LSVSFR(x)	(0x15C + 0x100 * (x))
#define	LTDC_LSVSPR(x)	(0x160 + 0x100 * (x))
#define	LTDC_LSHSFR(x)	(0x164 + 0x100 * (x))
#define	LTDC_LSHSPR(x)	(0x168 + 0x100 * (x))
#define	LTDC_LCYR0R(x)	(0x16C + 0x100 * (x))
#define	LTDC_LCYR1R(x)	(0x170 + 0x100 * (x))
#define	LTDC_LFPF0R(x)	(0x174 + 0x100 * (x))
#define	LTDC_LFPF1R(x)	(0x178 + 0x100 * (x))

struct stm32n6_ltdc_softc {
	uint32_t base;
};

int stm32n6_ltdc_init(struct stm32n6_ltdc_softc *sc, uint32_t base);
void stm32n6_ltdc_setup(struct stm32n6_ltdc_softc *sc,
    const struct layer_info *info, uint8_t nlayers);

#endif /* !_ARM_STM_STM32N6_LTDC_H_ */
