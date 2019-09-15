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

#ifndef	_ARM_STM_STM32F4_SDMMC_H_
#define	_ARM_STM_STM32F4_SDMMC_H_

#define	SDMMC_POWER	0x00 /*   */
#define	 POWER_PWRCTRL	(0x3 << 0)
#define	SDMMC_CLKCR	0x04 /*   */
#define	 CLKCR_HWFC_EN	(1 << 14) /* HW Flow Control enable */
#define	 CLKCR_WIDBUS_S	11 /* Wide bus mode enable */
#define	 CLKCR_WIDBUS_1	0
#define	 CLKCR_WIDBUS_4	1
#define	 CLKCR_WIDBUS_8	2
#define	 CLKCR_CLKEN	(1 << 8)
#define	 CLKCR_CLKDIV_S	0
#define	SDMMC_ARG	0x08 /*   */
#define	SDMMC_CMD	0x0C /*   */
#define	 CMD_CPSMEN	(1 << 10)
#define	 CMD_WAITPEND	(1 << 9)
#define	 CMD_WAITRESP_S	6
#define	 WAITRESP_SHORT	1
#define	 WAITRESP_LONG	3
#define	SDMMC_RESPCMD	0x10 /*   */
#define	SDMMC_RESP1	0x14 /*   */
#define	SDMMC_RESP2	0x18 /*   */
#define	SDMMC_RESP3	0x1C /*   */
#define	SDMMC_RESP4	0x20 /*   */
#define	SDMMC_DTIMER	0x24 /*   */
#define	SDMMC_DLEN	0x28 /*   */
#define	SDMMC_DCTRL	0x2C /*   */
#define	 DCTRL_DBLOCKSIZE_S	4
#define	 DCTRL_DMAEN	(1 << 3) /* DMA enable */
#define	 DCTRL_DTDIR	(1 << 1)	/* READ */
#define	 DCTRL_DTEN	(1 << 0)
#define	SDMMC_DCOUNT	0x30 /*   */
#define	SDMMC_STA	0x34 /*   */
#define	 STA_RXDAVL	(1 << 21)
#define	 STA_RXFIFOE	(1 << 19)
#define	 STA_RXFIFOF	(1 << 17)
#define	 STA_RXFIFOHF	(1 << 15)
#define	 STA_RXACT	(1 << 13)
#define	 STA_DBCKEND	(1 << 10) /* Data block sent/received (CRC check passed) */
#define	 STA_DATAEND	(1 << 8)
#define	 STA_CMDSENT	(1 << 7)
#define	 STA_CMDREND	(1 << 6)
#define	 STA_RXOVERRIE	(1 << 5)
#define	 STA_TXUNDERR	(1 << 4)
#define	 STA_DTIMEOUT	(1 << 3)
#define	 STA_CTIMEOUT	(1 << 2)
#define	 STA_DCRCFAIL	(1 << 1)
#define	 STA_CCRCFAIL	(1 << 0)
#define	SDMMC_ICR	0x38 /*   */
#define	SDMMC_MASK	0x3C /*   */
#define	SDMMC_FIFOCNT	0x48 /*   */
#define	SDMMC_FIFO	0x80 /*   */

struct stm32f4_sdmmc_softc {
	uint32_t base;
};

#endif	/* !_ARM_STM_STM32F4_SDMMC_H_ */
