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

#ifndef _RISCV_GIGADEVICE_GD32V_WDGT_H_
#define _RISCV_GIGADEVICE_GD32V_WDGT_H_

#define	WWDGT_CTL	0x00 /* Control register */
#define	 CTL_WDGTEN	(1 << 7) /* Start the window watchdog timer. */
#define	 CTL_CNT_S	0 /* The value of the watchdog timer counter. */
#define	 CTL_CNT_M	(0x7f << CTL_CNT_S)
#define	WWDGT_CFG	0x04 /* Configuration register */
#define	 CFG_EWIE	(1 << 9) /* Early wakeup interrupt enable. */
#define	 CFG_PSC_S	7 /* Prescaler. */
#define	 CFG_PSC_M	(0x3 << CFG_PSC_S)
#define	 CFG_PSC_DIV_1	(0x0 << CFG_PSC_S)
#define	 CFG_PSC_DIV_2	(0x1 << CFG_PSC_S)
#define	 CFG_PSC_DIV_4	(0x2 << CFG_PSC_S)
#define	 CFG_PSC_DIV_8	(0x3 << CFG_PSC_S)
#define	 CFG_WIN_S	0 /* The Window value. */
#define	 CFG_WIN_M	(0x7f << CFG_WIN_S)
#define	WWDGT_STAT	0x08 /* Status register */
#define	 STAT_EWIF	(1 << 0) /* Early wakeup interrupt flag. */

struct gd32v_wdgt_softc {
	uint32_t base;
};

int gd32v_wdgt_init(mdx_device_t dev, uint32_t base);

#endif /* !_RISCV_GIGADEVICE_GD32V_WDGT_H_ */
