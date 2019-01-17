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

#ifndef _ARM_ARM_NVIC_H_
#define _ARM_ARM_NVIC_H_

/* System Timer */
#define	SYST_CSR	0x010	/* SysTick Control and Status Register */
#define	SYST_RVR	0x014	/* SysTick Reload Value Register */
#define	SYST_CVR	0x018	/* SysTick Current Value Register */
#define	SYST_CALIB	0x01C	/* SysTick Calibration Value Register */

/* Processor features */
#define	PF_CLIDR	0xD78	/* Cache Level ID Register */
#define	PF_CTR		0xD7C	/* Cache Type Register */
#define	PF_CCSIDR	0xD80	/* Cache Size ID Register */
#define	PF_CSSELR	0xD84	/* Cache Size Selection Register */

/* Memory Protection Unit */
#define	MPU_TYPE	0xD90	/* MPU Type Register */
#define	MPU_CTRL	0xD94	/* MPU Control Register */
#define	MPU_RNR		0xD98	/* MPU Region Number Register */
#define	MPU_RBAR	0xD9C	/* MPU Region Base Address Register */
#define	MPU_RASR	0xDA0	/* MPU Region Attribute and Size Register */
#define	MPU_RBAR_A1	0xDA4	/* Alias of RBAR */
#define	MPU_RASR_A1	0xDA8	/* Alias of RASR */
#define	MPU_RBAR_A2	0xDAC	/* Alias of RBAR */
#define	MPU_RASR_A2	0xDB0	/* Alias of RASR */
#define	MPU_RBAR_A3	0xDB4	/* Alias of RBAR */
#define	MPU_RASR_A3	0xDB8	/* Alias of RASR */

/* System Control Block */
#define	SCB_ACTLR	0x008	/* Auxiliary Control Register */
#define	SCB_CPUID	0xD00	/* CPUID Base Register */
#define	SCB_ICSR	0xD04	/* Interrupt Control State Register */
#define	 ICSR_PENDSWSET	(1 << 28) /* Set pending pendSV bit */
#define	SCB_VTOR	0xD08	/* Vector Table Offset Register */
#define	SCB_AIRCR	0xD0C	/* Application Interrupt and Reset Control */
#define	SCB_SCR		0xD10	/* System Control Register */
#define	SCB_CCR		0xD14	/* Configuration Control Register */
#define	SCB_SHPR1	0xD18	/* System Handler Priority Register 1 */
#define	SCB_SHPR2	0xD1C	/* System Handler Priority Register 2 */
#define	SCB_SHPR3	0xD20	/* System Handler Priority Register 3 */
#define	SCB_SHCRS	0xD24	/* System Handler Control and State Register */
#define	SCB_CFSR	0xD28	/* Configurable Fault Status Register */
#define	SCB_MMSR	0xD28	/* MemManage Fault Status Register */
#define	SCB_BFSR	0xD29	/* BusFault Status Register */
#define	SCB_UFSR	0xD2A	/* UsageFault Status Register */
#define	SCB_HFSR	0xD2C	/* HardFault Status Register */
#define	SCB_MMAR	0xD34	/* MemManage Fault Address Register */
#define	SCB_BFAR	0xD38	/* BusFault Address Register */
#define	SCB_AFSR	0xD3C	/* Auxiliary Fault Status Register */

/* Nested Vectored Interrupt Controller */
#define	NVIC_ISER(n)	(0x100 + 0x4 * n) /* Interrupt Set-enable Regs */
#define	NVIC_ICER(n)	(0x180 + 0x4 * n) /* Interrupt Clear-enable Regs */
#define	NVIC_ISPR(n)	(0x200 + 0x4 * n) /* Interrupt Set-pending Regs */
#define	NVIC_ICPR(n)	(0x280 + 0x4 * n) /* Interrupt Clear-pending Regs */
#define	NVIC_IABR(n)	(0x300 + 0x4 * n) /* Interrupt Active Bit Regs */
#define	NVIC_IPR(n)	(0x400 + 0x4 * n) /* Interrupt Priority Regs */
#define	NVIC_STIR	0xF00	/* Software Trigger Interrupt Register */

struct nvic_intr_entry {
	void (*handler) (void *arg, struct trapframe *frame, int irq);
	void *arg;
};

struct arm_nvic_softc {
	uint32_t base;
};

int arm_nvic_init(struct arm_nvic_softc *sc, uint32_t base);
void arm_nvic_enable_intr(struct arm_nvic_softc *sc, uint32_t intr);
void arm_nvic_disable_intr(struct arm_nvic_softc *sc, uint32_t n);
void arm_nvic_intr(uint32_t irq, struct trapframe *frame);
void arm_nvic_install_intr_map(struct arm_nvic_softc *sc,
    const struct nvic_intr_entry *m);

#endif /* !_ARM_ARM_NVIC_H_ */
