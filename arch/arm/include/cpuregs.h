/*-
 * Copyright (c) 2019 Ruslan Bukin <br@bsdpad.com>
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

#ifndef	_MACHINE_CPUREGS_H_
#define	_MACHINE_CPUREGS_H_

#define	CONTROL_SFPA	(1 << 3) /* Secure floating-point active. */
#define	CONTROL_FPCA	(1 << 2) /* Floating-point context active. */
#define	CONTROL_SPSEL	(1 << 1) /* Stack-pointer select. */
#define	CONTROL_NPRIV	(1 << 0) /* Not privileged. Thread mode has unprivileged access only. */

/* Exception numbers */
#define	EXCP_RESET		1
#define	EXCP_NMI		2
#define	EXCP_HARD_FAULT		3
#define	EXCP_MEM_MANAGE		4
#define	EXCP_BUS_FAULT		5
#define	EXCP_USAGE_FAULT	6
#define	EXCP_SVCALL		11
#define	EXCP_DEBUG_MONITOR	12
#define	EXCP_PENDSV		14
#define	EXCP_SYSTICK		15

/* Execution PSR (EPSR) register */
#define	XPSR_T		(1 << 24) /* Thumb state bit (T-bit) */

/* Exception Return. */
#define	EXCP_RET		(0xff << 24)
#define	EXCP_RET_FTYPE		(1 << 4) /* The PE did not allocate space on the stack for FP context. */
#define	EXCP_RET_MODE_THREAD	(1 << 3) /* Indicates the mode that was stacked from. */
#define	EXCP_RET_SPSEL_PSP	(1 << 2) /* Indicates which stack contains the exception stack frame. */

/* Exception Return with Security Extensions. */
#define	EXCP_RET_S_SECURE	(1 << 6) /* Indicates whether registers have been pushed to a Secure or Non-secure stack. */
#define	EXCP_RET_DCRS		(1 << 5) /* Default rules for stacking the callee registers are followed. */
#define	EXCP_RET_ES_SECURE	(1 << 0) /* Indicates the Security state the exception was taken to. */

#define	EXCP_RET_RESERVED1	(1 << 1)
#define	EXCP_RET_RESERVED2	(0x1ffff << 7)

#endif /* !_MACHINE_CPUREGS_H_ */
