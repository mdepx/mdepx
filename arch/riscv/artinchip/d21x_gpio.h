/*-
 * Copyright (c) 2024 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _RISCV_ARTINCHIP_D21X_GPIO_H_
#define	_RISCV_ARTINCHIP_D21X_GPIO_H_

enum {
	GRP_PA,
	GRP_PB,
	GRP_PC,
	GRP_PD,
	GRP_PE,
	GRP_PF,
	GRP_PG,
	GRP_PO = 14,
	GRP_MAX,
};

#define	GPIO_BASE_PA	0x00
#define	GPIO_BASE_PB	0x20
#define	GPIO_BASE_PC	0x40
#define	GPIO_BASE_PD	0x60
#define	GPIO_BASE_PE	0x80
#define	GPIO_BASE_PF	0xa0
#define	GPIO_BASE_PG	0xc0
#define	GPIO_BASE_PH	0xe0
#define	GPIO_BASE_PI	0x100
#define	GPIO_BASE_PJ	0x120
#define	GPIO_BASE_PK	0x140
#define	GPIO_BASE_PL	0x160
#define	GPIO_BASE_PM	0x180
#define	GPIO_BASE_PN	0x1a0
#define	GPIO_BASE_PO	0x1c0

#define	GPIOA(n)	(PA_BASE + (n))
#define	GPIOB(n)	(PB_BASE + (n))
#define	GPIOC(n)	(PC_BASE + (n))
#define	GPIOD(n)	(PD_BASE + (n))
#define	GPIOE(n)	(PE_BASE + (n))
#define	GPIOF(n)	(PF_BASE + (n))
#define	GPIOG(n)	(PG_BASE + (n))
#define	GPIOH(n)	(PH_BASE + (n))
#define	GPIOI(n)	(PI_BASE + (n))
#define	GPIOJ(n)	(PJ_BASE + (n))
#define	GPIOK(n)	(PK_BASE + (n))
#define	GPIOL(n)	(PL_BASE + (n))
#define	GPIOM(n)	(PM_BASE + (n))
#define	GPION(n)	(PN_BASE + (n))
#define	GPIOO(n)	(PO_BASE + (n))

#endif /* !_RISCV_ARTINCHIP_D21X_GPIO_H_ */
