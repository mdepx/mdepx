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

#ifndef _RISCV_GIGADEVICE_GD32V_FMC_H_
#define _RISCV_GIGADEVICE_GD32V_FMC_H_

#define	FMC_WS		0x000	/* Wait state register */
#define	FMC_KEY		0x004	/* Unlock key register */
#define	FMC_OBKEY	0x008	/* Option byte unlock key register */
#define	FMC_STAT	0x00C	/* Status register */
#define	FMC_CTL		0x010	/* Control register */
#define	FMC_ADDR	0x014	/* Address register */
#define	FMC_OBSTAT	0x01C	/* Option byte status register */
#define	FMC_WP		0x020	/* Erase/Program Protection register */
#define	FMC_PID		0x100	/* Product ID register */

struct gd32v_fmc_softc {
	uint32_t base;
};

int gd32v_fmc_init(mdx_device_t dev, uint32_t base);

#endif /* !_RISCV_GIGADEVICE_GD32V_FMC_H_ */
