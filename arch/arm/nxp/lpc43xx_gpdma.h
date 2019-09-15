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

#ifndef _ARM_NXP_LPC43XX_GPDMA_H_
#define _ARM_NXP_LPC43XX_GPDMA_H_

#define	GPDMA_INTSTAT		0x000 /* Int Status Register */
#define	GPDMA_INTTCSTAT		0x004 /* Int Terminal Count Request Status */
#define	GPDMA_INTTCCLEAR	0x008 /* Int Terminal Count Request Clear */
#define	GPDMA_INTERRSTAT	0x00C /* Int Error Status Register */
#define	GPDMA_INTERRCLR		0x010 /* Int Error Clear Register */
#define	GPDMA_RAWINTTCSTAT	0x014 /* Raw Interrupt Terminal Count Status */
#define	GPDMA_RAWINTERRSTAT	0x018 /* Raw Error Interrupt Status Register */
#define	GPDMA_ENBLDCHNS		0x01C /* Enabled Channel Register */
#define	GPDMA_SOFTBREQ		0x020 /* Software Burst Request Register */
#define	GPDMA_SOFTSREQ		0x024 /* Software Single Request Register */
#define	GPDMA_SOFTLBREQ		0x028 /* Software Last Burst Request Reg */
#define	GPDMA_SOFTLSREQ		0x02C /* Software Last Single Request Reg */
#define	GPDMA_CONFIG		0x030 /* Configuration Register */
#define	GPDMA_SYNC		0x034 /* Synchronization Register */
#define	GPDMA_SRCADDR(n)	(0x100 + 0x20 * (n)) /* Source Address */
#define	GPDMA_DESTADDR(n)	(0x104 + 0x20 * (n)) /* Destination Address */
#define	GPDMA_LLI(n)		(0x108 + 0x20 * (n)) /* Linked List Item */
#define	GPDMA_CONTROL(n)	(0x10C + 0x20 * (n)) /* Control Register */
#define	 CONTROL_TRANSFER_SIZE_S 0 /* Number of transfers */
#define	 CONTROL_SBSIZE_S	12 /* Source burst size */
#define	 CONTROL_SBSIZE_M	(0x7 << CONTROL_SBSIZE_S)
#define	 CONTROL_SBSIZE_1	(0x0 << CONTROL_SBSIZE_S)
#define	 CONTROL_SBSIZE_4	(0x1 << CONTROL_SBSIZE_S)
#define	 CONTROL_SBSIZE_8	(0x2 << CONTROL_SBSIZE_S)
#define	 CONTROL_SBSIZE_16	(0x3 << CONTROL_SBSIZE_S)
#define	 CONTROL_SBSIZE_32	(0x4 << CONTROL_SBSIZE_S)
#define	 CONTROL_SBSIZE_64	(0x5 << CONTROL_SBSIZE_S)
#define	 CONTROL_SBSIZE_128	(0x6 << CONTROL_SBSIZE_S)
#define	 CONTROL_SBSIZE_256	(0x7 << CONTROL_SBSIZE_S)
#define	 CONTROL_DBSIZE_S	15 /* Destination burst size */
#define	 CONTROL_DBSIZE_M	(0x7 << CONTROL_DBSIZE_S)
#define	 CONTROL_DBSIZE_1	(0x0 << CONTROL_DBSIZE_S)
#define	 CONTROL_DBSIZE_4	(0x1 << CONTROL_DBSIZE_S)
#define	 CONTROL_DBSIZE_8	(0x2 << CONTROL_DBSIZE_S)
#define	 CONTROL_DBSIZE_16	(0x3 << CONTROL_DBSIZE_S)
#define	 CONTROL_DBSIZE_32	(0x4 << CONTROL_DBSIZE_S)
#define	 CONTROL_DBSIZE_64	(0x5 << CONTROL_DBSIZE_S)
#define	 CONTROL_DBSIZE_128	(0x6 << CONTROL_DBSIZE_S)
#define	 CONTROL_DBSIZE_256	(0x7 << CONTROL_DBSIZE_S)
#define	 CONTROL_SWIDTH_S	18 /* Source transfer width */
#define	 CONTROL_SWIDTH_M	(0x7 << CONTROL_SWIDTH_S)
#define	 CONTROL_SWIDTH_1	(0x0 << CONTROL_SWIDTH_S)
#define	 CONTROL_SWIDTH_2	(0x1 << CONTROL_SWIDTH_S)
#define	 CONTROL_SWIDTH_4	(0x2 << CONTROL_SWIDTH_S)
#define	 CONTROL_DWIDTH_S	21 /* Destination transfer width */
#define	 CONTROL_DWIDTH_M	(0x7 << CONTROL_DWIDTH_S)
#define	 CONTROL_DWIDTH_1	(0x0 << CONTROL_DWIDTH_S)
#define	 CONTROL_DWIDTH_2	(0x1 << CONTROL_DWIDTH_S)
#define	 CONTROL_DWIDTH_4	(0x2 << CONTROL_DWIDTH_S)
#define	 CONTROL_S		(1 << 24) /* Source is AHB Master 1 */
#define	 CONTROL_D		(1 << 25) /* Dest is AHB Master 1 */
#define	 CONTROL_SI		(1 << 26) /* Source increment */
#define	 CONTROL_DI		(1 << 27) /* Destination increment */
#define	 CONTROL_I		(1 << 31) /* Terminal count interrupt */
#define	GPDMA_CONFIG(n)		(0x110 + 0x20 * (n))
#define	 CONFIG_E		(1 << 0) /* Channel enable */
#define	 CONFIG_SRCPERIPHERAL_S	1 /* Source peripheral */
#define	 CONFIG_DESTPERIPHERAL_S 6 /* Destination peripheral */
#define	 CONFIG_IE		(1 << 14) /* Interrupt error mask */
#define	 CONFIG_ITC		(1 << 15) /* Terminal count interrupt */
#define	 CONFIGFLOWCNTRL_S	11 /* Flow control and transfer type */

#endif /* !_ARM_NXP_LPC43XX_GPDMA_H_ */
