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

#ifndef	_DEV_SOLOMON_SSD2828_H_
#define	_DEV_SOLOMON_SSD2828_H_

#define	SSD2828_DIR	0xB0 /* Device Identification Register */
#define	SSD2828_VICR1	0xB1 /* RGB Interface Control Register 1 */
#define	SSD2828_VICR2	0xB2 /* RGB Interface Control Register 2 */
#define	SSD2828_VICR3	0xB3 /* RGB Interface Control Register 3 */
#define	SSD2828_VICR4	0xB4 /* RGB Interface Control Register 4 */
#define	SSD2828_VICR5	0xB5 /* RGB Interface Control Register 5 */
#define	SSD2828_VICR6	0xB6 /* RGB Interface Control Register 6 */
#define	SSD2828_CFGR	0xB7 /* Configuration Register */
#define	SSD2828_VCR	0xB8 /* VC Control Register */
#define	SSD2828_PCR	0xB9 /* PLL Control Register */
#define	SSD2828_PLCR	0xBA /* PLL Configuration Register */
#define	SSD2828_CCR	0xBB /* Clock Control Register */
#define	SSD2828_PSCR1	0xBC /* Packet Size Control Register 1 */
#define	SSD2828_PSCR2	0xBD /* Packet Size Control Register 2 */
#define	SSD2828_PSCR3	0xBE /* Packet Size Control Register 3 */
#define	SSD2828_PDR	0xBF /* Packet Drop Register */
#define	SSD2828_OCR	0xC0 /* Operation Control Register */
#define	SSD2828_MRSR	0xC1 /* Maximum Return Size Register */
#define	SSD2828_RDCR	0xC2 /* Return Data Count Register */
#define	SSD2828_ARSR	0xC3 /* ACK Response Register */
#define	SSD2828_LCR	0xC4 /* Line Control Register */
#define	SSD2828_ICR	0xC5 /* Interrupt Control Register */
#define	SSD2828_ISR	0xC6 /* Interrupt Status Register */
#define	SSD2828_ESR	0xC7 /* Error Status Register */
#define	SSD2828_DAR1	0xC9 /* Delay Adjustment Register 1 */
#define	SSD2828_DAR2	0xCA /* Delay Adjustment Register 2 */
#define	SSD2828_DAR3	0xCB /* Delay Adjustment Register 3 */
#define	SSD2828_DAR4	0xCC /* Delay Adjustment Register 4 */
#define	SSD2828_DAR5	0xCD /* Delay Adjustment Register 5 */
#define	SSD2828_DAR6	0xCE /* Delay Adjustment Register 6 */
#define	SSD2828_HTTR1	0xCF /* HS TX Timer Register 1 */
#define	SSD2828_HTTR2	0xD0 /* HS TX Timer Register 2 */
#define	SSD2828_LRTR1	0xD1 /* LP RX Timer Register 1 */
#define	SSD2828_LRTR2	0xD2 /* LP RX Timer Register 2 */
#define	SSD2828_TSR	0xD3 /* TE Status Register */
#define	SSD2828_LRR	0xD4 /* SPI Read Register */
#define	SSD2828_PLLR	0xD5 /* PLL Lock Register */
#define	SSD2828_TR	0xD6 /* Test Register */
#define	SSD2828_TECR	0xD7 /* TE Count Register */
#define	SSD2828_ACR1	0xD8 /* Analog Control Register 1 */
#define	SSD2828_ACR2	0xD9 /* Analog Control Register 2 */
#define	SSD2828_ACR3	0xDA /* Analog Control Register 3 */
#define	SSD2828_ACR4	0xDB /* Analog Control Register 4 */
#define	SSD2828_IOCR	0xDC /* Interrupt Output Control Register */
#define	SSD2828_VICR7	0xDD /* RGB Interface Control Register 7 */
#define	SSD2828_LCFR	0xDE /* Lane Configuration Register */
#define	SSD2828_DAR7	0xDF /* Delay Adjustment Register 7 */
#define	SSD2828_PUCR1	0xE0 /* Pull Control Register 1 */
#define	SSD2828_PUCR2	0xE1 /* Pull Control Register 2 */
#define	SSD2828_PUCR3	0xE2 /* Pull Control Register 3 */
#define	SSD2828_CBCR1	0xE9 /* CABC Brightness Control Register 1 */
#define	SSD2828_CBCR2	0xEA /* CABC Brightness Control Register 2 */
#define	SSD2828_CBSR	0xEB /* CABC Brightness Status Register */
#define	SSD2828_ECR	0xEC /* Encoder Control Register */
#define	SSD2828_VSDR	0xED /* Video Sync Delay Register */
#define	SSD2828_TMR	0xEE /* Trimming Register */
#define	SSD2828_GPIO1	0xEF /* GPIO Register 1 */
#define	SSD2828_GPIO2	0xF0 /* GPIO Register 2 */
#define	SSD2828_DLYA01	0xF1 /* DLYA01 Register */
#define	SSD2828_DLYA23	0xF2 /* DLYA23 Register */
#define	SSD2828_DLYB01	0xF3 /* DLYB01 Register */
#define	SSD2828_DLYB23	0xF4 /* DLYB23 Register */
#define	SSD2828_DLYC01	0xF5 /* DLYC01 Register */
#define	SSD2828_DLYC23	0xF6 /* DLYC23 Register */
#define	SSD2828_ACR5	0xF7 /* Analog Control Register 5 */
#define	SSD2828_RR	0xFF /* Read Register */

#endif /* !_DEV_SOLOMON_SSD2828_H_ */
