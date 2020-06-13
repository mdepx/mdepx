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

#ifndef	_DEV_CCS811_CCS811_H_
#define	_DEV_CCS811_CCS811_H_

#define	CCS811_STATUS		0x00
#define	CCS811_MEAS_MODE	0x01
#define	CCS811_ALG_RESULT_DATA	0x02
#define	CCS811_RAW_DATA		0x03
#define	CCS811_ENV_DATA		0x05
#define	CCS811_NTC		0x06
#define	CCS811_THRESHOLDS	0x10
#define	CCS811_BASELINE		0x11
#define	CCS811_HW_ID		0x20
#define	CCS811_HW_VERSION	0x21
#define	CCS811_FW_BOOT_VERSION	0x23
#define	CCS811_FW_APP_VERSION	0x24
#define	CCS811_ERROR_ID		0xE0
#define	CCS811_APP_ERASE	0xF1
#define	CCS811_APP_DATA		0xF2
#define	CCS811_APP_VERIFY	0xF3
#define	CCS811_APP_START	0xF4
#define	CCS811_SW_RESET		0xFF

#endif /* !_DEV_CCS811_CCS811_H_ */
