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

#ifndef _RISCV_GIGADEVICE_GD32V_GPIO_H_
#define _RISCV_GIGADEVICE_GD32V_GPIO_H_

#define	GPIO_CTL(pin)		(0x04 * ((pin) / 8))
#define	 CTL_MD_S(pin)		(((pin) % 8) * 4)
#define	 CTL_MD_M(pin)		(0x3 << CTL_MD_S(pin))
#define	 CTL_MD_INP(pin)	(0 << CTL_MD_S(pin))
#define	 CTL_MD_OUT_10MHZ(pin)	(1 << CTL_MD_S(pin))
#define	 CTL_MD_OUT_2MHZ(pin)	(2 << CTL_MD_S(pin))
#define	 CTL_MD_OUT_50MHZ(pin)	(3 << CTL_MD_S(pin))

#define	 CTL_CTL_S(pin)		(2 + ((pin) % 8) * 4)
#define	 CTL_CTL_M(pin)		(0x3 << CTL_CTL_S(pin))
/* Pin is output. */
#define	 CTL_CTL_GPIO_PUSHPULL(pin)	(0 << CTL_CTL_S(pin))
#define	 CTL_CTL_GPIO_OPENDRAIN(pin)	(1 << CTL_CTL_S(pin))
#define	 CTL_CTL_AFIO_PUSHPULL(pin)	(2 << CTL_CTL_S(pin))
#define	 CTL_CTL_AFIO_OPENDRAIN(pin)	(3 << CTL_CTL_S(pin))
/* Pin is input. */
#define	 CTL_CTL_ANALOG(pin)		(0 << CTL_CTL_S(pin))
#define	 CTL_CTL_INP_FLOATING(pin)	(1 << CTL_CTL_S(pin))
#define	 CTL_CTL_INP_PULLUPDOWN(pin)	(2 << CTL_CTL_S(pin))

#define	GPIO_BOP		0x10 /* Port bit operate register */
#define	 BOP_BOP_S		0	/* Port Set bit */
#define	 BOP_BOP(pin)		(1 << ((pin) + BOP_BOP_S))
#define	 BOP_CR_S		16	/* Port Clear bit */
#define	 BOP_CR(pin)		(1 << ((pin) + BOP_CR_S))

struct gd32v_gpio_softc {
	uint32_t base;
};

int gd32v_gpio_init(mdx_device_t dev, uint32_t base);

#endif /* !_RISCV_GIGADEVICE_GD32V_GPIO_H_ */
