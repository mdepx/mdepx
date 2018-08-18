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

#ifndef _ARM_STM_STM32F7_ETH_H_
#define _ARM_STM_STM32F7_ETH_H_

#define	ETH_MACCR	0x00 /* Ethernet MAC configuration register */
#define	 MACCR_WD	(1 << 23) /* Watchdog disable */
#define	 MACCR_JD	(1 << 22) /* Jabber disable */
#define	 MACCR_FES	(1 << 14) /* Fast Ethernet speed */
#define	 MACCR_DM	(1 << 11) /* Duplex mode */
#define	 MACCR_APCS	(1 << 7) /* Automatic pad/CRC stripping */
#define	 MACCR_TE	(1 << 3) /* Transmitter enable */
#define	 MACCR_RE	(1 << 2) /* Receiver enable */
#define	ETH_MACFFR	0x04 /* Ethernet MAC frame filter register */
#define	 MACFFR_RA	(1 << 31) /* Receive all */
#define	 MACFFR_HPF	(1 << 10) /* Hash or perfect filter */
#define	 MACFFR_SAF	(1 << 9) /* Source address filter */
#define	 MACFFR_PAM	(1 << 4) /* Pass all multicast */
#define	 MACFFR_PM	(1 << 0) /* Promiscuous mode */
#define	ETH_MACHTHR	0x08 /* Ethernet MAC hash table high register */
#define	ETH_MACHTLR	0x0C /* Ethernet MAC hash table low register */
#define	ETH_MACMIIAR	0x10 /* Ethernet MAC MII address register */
#define	 MACMIIAR_PA_S	11 /* PHY address */
#define	 MACMIIAR_PA_M	(0x1f << MACMIIAR_PA_S)
#define	 MACMIIAR_MR_S	6 /* MII register */
#define	 MACMIIAR_MR_M	(0x1f << MACMIIAR_MR_S)
#define	 MACMIIAR_CR_S	2 /* Clock range */
#define	 MACMIIAR_CR_M	(0x7 << MACMIIAR_CR_S)
#define	 MACMIIAR_MW	(1 << 1) /* MII write */
#define	 MACMIIAR_MB	(1 << 0) /* MII busy */
#define	ETH_MACMIIDR	0x14 /* Ethernet MAC MII data register */
#define ETH_MACFCR	0x18 /* Ethernet MAC flow control register */
#define ETH_MACVLANTR	0x1C /* Ethernet MAC VLAN tag register */
#define ETH_MACRWUFFR	0x28 /* Ethernet MAC remote wakeup frame filter register */
#define ETH_MACPMTCSR	0x2C /* Ethernet MAC PMT control and status register */
#define	ETH_MACDBGR	0x34 /* Ethernet MAC debug register */
#define ETH_MACSR	0x38 /* Ethernet MAC interrupt status register */
#define	ETH_MACIMR	0x3C /* Ethernet MAC interrupt mask register */
#define	ETH_MACAHR(n)	(0x40 + 0x8 * n) /* Ethernet MAC address 0 high register */
#define	ETH_MACALR(n)	(0x44 + 0x8 * n) /* Ethernet MAC address 0 low register */
#define	ETH_DMABMR	0x1000 /* Ethernet DMA bus mode register */
#define	 DMABMR_SR	(1 << 0) /* Software reset */
#define	 DMABMR_PBL_S	8 /* Programmable burst length */
#define	 DMABMR_PBL_M	(0x3f << DMABMR_PBL_S)
#define	 DMABMR_EDFE	(1 << 7) /* Enhanced descriptor format enable */
#define	 DMABMR_FPM	(1 << 24) /* 4xPBL mode */
#define	 DMABMR_AAB	(1 << 25) /* Address-aligned beats */
#define	ETH_DMATPDR	0x1004 /* Ethernet DMA transmit poll demand register */
#define ETH_DMARPDR	0x1008 /* Ethernet DMA receive poll demand register */
#define ETH_DMARDLAR	0x100C /* Ethernet DMA receive descriptor list address */
#define	ETH_DMATDLAR	0x1010 /* Ethernet DMA transmit descriptor list address */
#define ETH_DMASR	0x1014 /* Ethernet DMA status register */
#define	 DMASR_NIS	(1 << 16) /* Normal interrupt summary */
#define	 DMASR_AIS	(1 << 15) /* Abnormal interrupt summary */
#define	 DMASR_FBES	(1 << 13) /* Fatal bus error status */
#define	 DMASR_RS	(1 << 6) /* Receive status */
#define	 DMASR_TS	(1 << 0) /* Transmit status */
#define	ETH_DMAOMR	0x1018 /* Ethernet DMA operation mode register */
#define	 DMAOMR_DTCEFD	(1 << 26) /* Dropping of TCP/IP checksum error frames disable */
#define	 DMAOMR_RSF	(1 << 25) /* Receive store and forward */
#define	 DMAOMR_DFRF	(1 << 24) /* Disable flushing of received frames */
#define	 DMAOMR_TSF	(1 << 21) /* Transmit store and forward */
#define	 DMAOMR_FTF	(1 << 20) /* Flush transmit FIFO */
#define	 DMAOMR_ST	(1 << 13) /* Start/stop transmission */
#define  DMAOMR_FEF	(1 << 7) /* Forward error frames */
#define	 DMAOMR_FUGF	(1 << 6) /*  Forward undersized good frames */
#define	 DMAOMR_RTC_S	3 /*  Receive threshold control */
#define	 DMAOMR_RTC_M	(0x3 << DMAOMR_RTC_S)
#define	 DMAOMR_RTC_32	0x1
#define	 DMAOMR_OSF	(1 << 2) /* Operate on second frame */
#define	 DMAOMR_SR	(1 << 1) /* Start/stop receive */
#define	ETH_DMAIER	0x101C /* Ethernet DMA interrupt enable register */
#define	ETH_DMAMFBOCR	0x1020 /* Ethernet DMA missed frame & buffer overflow counter */
#define	ETH_DMARSWTR	0x1024 /* Ethernet DMA receive status watchdog timer */
#define	ETH_DMACHTDR	0x1048 /* Ethernet DMA current host transmit descriptor */
#define	ETH_DMACHRDR	0x104C /* Ethernet DMA current host receive descriptor */
#define	ETH_DMACHTBAR	0x1050 /* Ethernet DMA current host transmit buffer address */
#define	ETH_DMACHRBAR	0x1054 /* Ethernet DMA current host receive buffer address */

#endif /* !_ARM_STM_STM32F7_ETH_H_ */
