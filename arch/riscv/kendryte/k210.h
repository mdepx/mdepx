/*-
 * Copyright (c) 2020 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _RISCV_KENDRYTE_K210_H_
#define _RISCV_KENDRYTE_K210_H_

#define	BASE_GPIO	0x50200000

#define	BASE_UART1	0x50210000
#define	BASE_UART2	0x50220000
#define	BASE_UART3	0x50230000

#define	BASE_TIMER0	0x502D0000
#define	BASE_TIMER1	0x502E0000
#define	BASE_TIMER2	0x502F0000

#define	BASE_I2C0	0x50280000
#define	BASE_I2C1	0x50290000
#define	BASE_I2C2	0x502A0000

#define	BASE_I2S0	0x50250000
#define	BASE_I2S1	0x50260000
#define	BASE_I2S2	0x50270000

#define	BASE_SPI0	0x52000000
#define	BASE_SPI1	0x53000000
#define	BASE_SPI2	0x54000000

#define	BASE_WDT0	0x50400000
#define	BASE_WDT1	0x50410000

#define	BASE_OTP	0x50420000
#define	BASE_DVP	0x50430000
#define	BASE_SYSCTL	0x50440000
#define	BASE_AES	0x50450000
#define	BASE_RTC	0x50460000

#define	BASE_CLINT	0x02000000
#define	BASE_PLIC	0x0C000000

#define	BASE_UARTHS	0x38000000
#define	BASE_GPIOHS	0x38001000

#endif /* !_RISCV_KENDRYTE_K210_H_ */
