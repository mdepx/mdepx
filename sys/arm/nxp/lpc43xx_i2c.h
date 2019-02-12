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

#ifndef _ARM_NXP_LPC43XX_I2C_H_
#define _ARM_NXP_LPC43XX_I2C_H_

#define	I2C_CONSET	0x000 /* I2C Control Set Register */
#define	 CONSET_AA	(1 << 2) /* Assert acknowledge flag */
#define	 CONSET_SI	(1 << 3) /* I2C interrupt flag */
#define	 CONSET_STO	(1 << 4) /* STOP flag */
#define	 CONSET_STA	(1 << 5) /* START flag */
#define	 CONSET_I2EN	(1 << 6) /* I2C interface enable */
#define	I2C_STAT	0x004 /* I2C Status Register */
#define	I2C_DAT		0x008 /* I2C Data Register */
#define	I2C_ADR0	0x00C /* I2C Slave Address Register 0 */
#define	 ADR0_ADDRESS_S	1 /* The I2C device address for slave mode */
#define	 ADR0_ADDRESS_M	(0x7f << I2C_ADDRESS_S)
#define	I2C_SCLH	0x010 /* SCH Duty Cycle Register High Half Word */
#define	I2C_SCLL	0x014 /* SCL Duty Cycle Register Low Half Word */
#define	I2C_CONCLR	0x018 /* I2C Control Clear Register */
#define	I2C_MMCTRL	0x01C /* Monitor mode control register */
#define	I2C_ADR1	0x020 /* I2C Slave Address Register 1 */
#define	I2C_ADR2	0x024 /* I2C Slave Address Register 2 */
#define	I2C_ADR3	0x028 /* I2C Slave Address Register 3 */
#define	I2C_DATA_BUFFER	0x02C /* Data buffer register */
#define	I2C_MASK0	0x030 /* I2C Slave address mask register 0 */
#define	I2C_MASK1	0x034 /* I2C Slave address mask register 1 */
#define	I2C_MASK2	0x038 /* I2C Slave address mask register 2 */
#define	I2C_MASK3	0x03C /* I2C Slave address mask register 3 */

#endif /* !_ARM_NXP_LPC43XX_I2C_H_ */
