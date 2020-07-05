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

#ifndef	_DEV_GT911_GT911_H_
#define	_DEV_GT911_GT911_H_

#define	GT911_CMD			0x8040
#define	GT911_ESD_CHECK			0x8041
#define	GT911_CMD_CHECK			0x8046
#define	GT911_CONFIG_VERSION		0x8047
#define	GT911_X_OUTPUT_MAX_LO		0x8048 /* Resolution of X axis */
#define	GT911_X_OUTPUT_MAX_HI		0x8049
#define	GT911_Y_OUTPUT_MAX_LO		0x804A /* Resolution of Y axis */
#define	GT911_Y_OUTPUT_MAX_HI		0x804B
#define	GT911_TOUCH_NUMBER		0x804C
#define	GT911_MODULE_SWITCH1		0x804D
#define	GT911_MODULE_SWITCH2		0x804E
#define	GT911_SNAKE_COUNT		0x804F
#define	GT911_FILTER			0x8050
#define	GT911_LARGE_TOUCH		0x8051
#define	GT911_NOISE_REDUCTION		0x8052
#define	GT911_SCREEN_TOUCH_LEVEL	0x8053
#define	GT911_SCREEN_LEAVE_LEVEL	0x8054
#define	GT911_LOW_POWER_CONTROL		0x8055
#define	GT911_REFRESH_RATE		0x8056
#define	GT911_X_THRESHOLD		0x8057
#define	GT911_Y_THRESHOLD		0x8058
#define	GT911_X_SPEED_LIMIT		0x8059
#define	GT911_Y_SPEED_LIMIT		0x805A

#endif /* !_DEV_GT911_GT911_H_ */
