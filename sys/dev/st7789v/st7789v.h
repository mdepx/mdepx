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

#ifndef	_DEV_ST7789V_ST7789V_H_
#define	_DEV_ST7789V_ST7789V_H_

/* System Function Command Table 1 */
#define	ST7789V_NOP		0x00	/* No operation */
#define	ST7789V_SWRESET		0x01	/* Software reset */
#define	ST7789V_RDDID		0x04	/* Read display ID */
#define	ST7789V_RDDST		0x09	/* Read display status */
#define	ST7789V_RDDPM		0x0A	/* Read display power */
#define	ST7789V_RDDMADCTL	0x0B	/* Read display */
#define	ST7789V_RDDCOLMOD	0x0C	/* Read display pixel */
#define	ST7789V_RDDIM		0x0D	/* Read display image */
#define	ST7789V_RDDSM		0x0E	/* Read display signal */
#define	ST7789V_RDDSDR		0x0F	/* Read display self-diagnostic result */
#define	ST7789V_SLPIN		0x10	/* Sleep in */
#define	ST7789V_SLPOUT		0x11	/* Sleep out */
#define	ST7789V_PTLON		0x12	/* Partial mode on */
#define	ST7789V_NORON		0x13	/* Partial off (Normal) */
#define	ST7789V_INVOFF		0x20	/* Display inversion off */
#define	ST7789V_INVON		0x21	/* Display inversion on */
#define	ST7789V_GAMSET		0x26	/* Display inversion on */
#define	ST7789V_DISPOFF		0x28	/* Display off */
#define	ST7789V_DISPON		0x29	/* Display on */
#define	ST7789V_CASET		0x2A	/* Column address set */
#define	ST7789V_RASET		0x2B	/* Row address set */
#define	ST7789V_RAMWR		0x2C	/* Memory write */
#define	ST7789V_RAMRD		0x2E	/* Memory read */
#define	ST7789V_PTLAR		0x30	/* Partial sart/end address set */
#define	ST7789V_VSCRDEF		0x33	/* Vertical scrolling definition */
#define	ST7789V_TEOFF		0x34	/* Tearing effect line off */
#define	ST7789V_TEON		0x35	/* Tearing effect line on */
#define	ST7789V_MADCTL		0x36	/* Memory data access control */
#define	ST7789V_VSCRSADD	0x37	/* Vertical scrolling start address */
#define	ST7789V_IDMOFF		0x38	/* Idle mode off */
#define	ST7789V_IDMON		0x39	/* Idle mode on */
#define	ST7789V_COLMOD		0x3A	/* Interface pixel format */
#define	ST7789V_RAMWRC		0x3C	/* Memory write continue */
#define	ST7789V_RAMRDC		0x3E	/* Memory read continue */
#define	ST7789V_TESCAN		0x44	/* Set tear scanline */
#define	ST7789V_RDTESCAN	0x45	/* Get scanline */
#define	ST7789V_WRDISBV		0x51	/* Write display brightness */
#define	ST7789V_RDDISBV		0x52	/* Read display brightness value */
#define	ST7789V_WRCTRLD		0x53	/* Write CTRL display */
#define	ST7789V_RDCTRLD		0x54	/* Read CTRL value dsiplay */
#define	ST7789V_WRCACE		0x55	/* Write content adaptive brightness control and Color enhancemnet */
#define	ST7789V_RDCABC		0x56	/* Read content adaptive brightness control */
#define	ST7789V_WRCABCMB	0x5E	/* Write CABC minimum brightness */
#define	ST7789V_RDCABCMB	0x5F	/* Read CABC minimum brightness */
#define	ST7789V_RDABCSDR	0x68	/* Read Automatic Brightness Control Self-Diagnostic Result */
#define	ST7789V_RDID1		0xDA	/* Read ID1 */
#define	ST7789V_RDID2		0xDB	/* Read ID2 */
#define	ST7789V_RDID3		0xDC	/* Read ID3 */

/* System Function Command Table 2 */
#define	ST7789V_RAMCTRL		0xB0	/* RAM Control */
#define	ST7789V_RGBCTRL		0xB1	/* RGB Control */
#define	ST7789V_PORCTRL		0xB2	/* Porch control */
#define	ST7789V_FRCTRL1		0xB3	/* Frame Rate Control 1 */
#define	ST7789V_PARCTRL		0xB5	/* Partial mode Control */
#define	ST7789V_GCTRL		0xB7	/* Gate control */
#define	ST7789V_GTADJ		0xB8	/* Gate on timing adjustment */
#define	ST7789V_DGMEN		0xBA	/* Digital Gamma Enable */
#define	ST7789V_VCOMS		0xBB	/* VCOMS Setting */
#define	ST7789V_LCMCTRL		0xC0	/* LCM Control */
#define	ST7789V_IDSET		0xC1	/* ID Setting */
#define	ST7789V_VDVVRHEN	0xC2	/* VDV and VRH Command Enable */
#define	ST7789V_VRHS		0xC3	/* VRH Set */
#define	ST7789V_VDVSET		0xC4	/* VDV Setting */
#define	ST7789V_VCMOFSET	0xC5	/* VCOMS Offset Set */
#define	ST7789V_FRCTR2		0xC6	/* FR Control 2 */
#define	ST7789V_CABCCTRL	0xC7	/* CABC Control */
#define	ST7789V_REGSEL1		0xC8	/* Register value selection1 */
#define	ST7789V_REGSEL2		0xCA	/* Register value selection2 */
#define	ST7789V_PWMFRSEL	0xCC	/* PWM Frequency Selection */
#define	ST7789V_PWCTRL1		0xD0	/* Power Control 1 */
#define	ST7789V_VAPVANEN	0xD2	/* Enable VAP/VAN signal output */
#define	ST7789V_CMD2EN		0xDF	/* Command 2 Enable */
#define	ST7789V_PVGAMCTRL	0xE0	/* Positive Voltage Gamma Control */
#define	ST7789V_NVGAMCTRL	0xE1	/* Negative Voltage Gamma Control */
#define	ST7789V_DGMLUTR		0xE2	/* Digital Gamma Look-up Table for Red */
#define	ST7789V_DGMLUTB		0xE3	/* Digital Gamma Look-up Table for Blue */
#define	ST7789V_GATECTRL	0xE4	/* Gate control */
#define	ST7789V_SPI2EN		0xE7	/* SPI2 */
#define	ST7789V_PWCTRL2		0xE8	/* Power Control 2 */
#define	ST7789V_EQCTRL		0xE9	/* Equalize Time Control */
#define	ST7789V_PROMCTRL	0xEC	/* Program Control */
#define	ST7789V_PROMEN		0xFA	/* Program Mode Enable */
#define	ST7789V_NVMSET		0xFC	/* NVM Setting */
#define	ST7789V_PROMACT		0xFE	/* Program Action */

#endif	/* _DEV_ST7789V_ST7789V_H_ */
