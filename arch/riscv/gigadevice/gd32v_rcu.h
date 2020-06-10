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

#ifndef _RISCV_GIGADEVICE_GD32V_RCU_H_
#define _RISCV_GIGADEVICE_GD32V_RCU_H_

#define	RCU_CTL		0x00	/* Control register */
#define	RCU_CFG0	0x04	/* Clock configuration register 0 */
#define	RCU_INT		0x08	/* Clock interrupt register */
#define	RCU_APB2RST	0x0C	/* APB2 reset register */
#define	RCU_APB1RST	0x10	/* APB1 reset register */
#define	RCU_AHBEN	0x14	/* AHB enable register */
#define	RCU_APB2EN	0x18	/* APB2 enable register */
#define	 APB2EN_USART0EN	(1 << 14)	/* USART0 clock enable */
#define	 APB2EN_TIMER0EN	(1 << 11)	/* TIMER0 clock enable */
#define	 APB2EN_PAEN		(1 << 2)	/* GPIO port A clock enable */
#define	RCU_APB1EN	0x1C	/* APB1 enable register */
#define	 APB1EN_I2C1EN		(1 << 22) /* I2C1 clock enable */
#define	 APB1EN_I2C0EN		(1 << 21) /* I2C0 clock enable */
#define	 APB1EN_TIMER1EN	(1 << 0) /* TIMER1 clock enable */
#define	RCU_BDCTL	0x20	/* Backup domain control register */
#define	RCU_RSTSCK	0x24	/* Reset source/clock register */
#define	RCU_AHBRST	0x28	/* AHB reset register */
#define	RCU_CFG1	0x2C	/* Clock configuration register 1 */
#define	RCU_DSV		0x34	/* Deep-sleep mode voltage register */

struct gd32v_rcu_softc {
	uint32_t base;
};

int gd32v_rcu_init(mdx_device_t dev, uint32_t base);
void gd32v_rcu_setup(mdx_device_t dev,
    uint32_t ahben, uint32_t apb1en, uint32_t apb2en);

#endif /* !_RISCV_GIGADEVICE_GD32V_RCU_H_ */
