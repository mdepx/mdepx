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

#ifndef _ARM_NXP_LPC43XX_SCU_H_
#define _ARM_NXP_LPC43XX_SCU_H_

#define	SCU_SFSP(p, n)	(0x80 * (p) + 0x4 * (n))	/* Pin configuration */
#define	SCU_SFSCLK0	0xC00	/* Pin configuration register for pin CLK0 */
#define	SCU_SFSCLK1	0xC04	/* Pin configuration register for pin CLK1 */
#define	SCU_SFSCLK2	0xC08	/* Pin configuration register for pin CLK2 */
#define	SCU_SFSCLK3	0xC0C	/* Pin configuration register for pin CLK3 */
#define	SCU_SFSUSB	0xC80	/* Pin configuration for USB1_DM and USB1_DP */
#define	SCU_SFSI2C0	0xC84	/* Pin configuration register for I2C0-bus */
#define	SCU_ENAIO0	0xC88	/* ADC0 function select register */
#define	SCU_ENAIO1	0xC8C	/* ADC1 function select register */
#define	SCU_ENAIO2	0xC90	/* Analog function select register */
#define	SCU_EMCDELAYCLK	0xD00	/* EMC clock delay register */
#define	SCU_SDDELAY	0xD80	/* SD/MMC sample and drive delay register */
#define	SCU_PINTSEL0	0xE00	/* Pin int. select for pin interrupts 0 to 3.*/
#define	SCU_PINTSEL1	0xE04	/* Pin int. select for pin interrupts 4 to 7.*/

struct scu_softc {
	size_t base;
};

#endif /* !_ARM_NXP_LPC43XX_SCU_H_ */
