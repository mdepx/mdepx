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
#define	GT911_SPACE_TOP			0x805B
#define	GT911_SPACE_LEFT		0x805C
#define	GT911_MINI_FILTER		0x805D
#define	GT911_STRETCH_R0		0x805E
#define	GT911_STRETCH_R1		0x805F
#define	GT911_STRETCH_R2		0x8060
#define	GT911_STRETCH_RM		0x8061
#define	GT911_DRV_GROUPA_NUM		0x8062
#define	GT911_DRV_GROUPB_NUM		0x8063
#define	GT911_SENSOR_NUM		0x8064
#define	GT911_FREQA_FACTOR		0x8065
#define	GT911_FREQB_FACTOR		0x8066
#define	GT911_PANNEL_BITFREQL		0x8067
#define	GT911_PANNEL_BITFREQH		0x8068
#define	GT911_PANNEL_TX_GAIN		0x806B
#define	GT911_PANNEL_RX_GAIN		0x806C
#define	GT911_PANNEL_DUMP_SHIFT		0x806D
#define	GT911_DRV_FRAME_CONTROL		0x806E
#define	GT911_CHARGING_LEVEL_UP		0x806F
#define	GT911_MODULE_SWITCH3		0x8070
#define	GT911_GESTURE_DIS		0x8071
#define	GT911_GESTURE_LONG_PRESS_TIME	0x8072
#define	GT911_X_Y_SLOPE_ADJUST		0x8073
#define	GT911_GESTURE_CONTROL		0x8074
#define	GT911_GESTURE_SWITCH1		0x8075
#define	GT911_GESTURE_SWITCH2		0x8076
#define	GT911_GESTURE_REFRESH_RATE	0x8077
#define	GT911_GESTURE_TOUCH_LEVEL	0x8078
#define	GT911_NEWGREENWAKEUPLEVEL	0x8079
#define	GT911_FREQ_HOPPING_START	0x807A
#define	GT911_FREQ_HOPPING_END		0x807B
#define	GT911_NOISE_DETECT_TIMES	0x807C
#define	GT911_HOPPING_FLAG		0x807D
#define	GT911_HOPPING_THRESHOLD		0x807E
#define	GT911_NOISE_THRESHOLD		0x807F
#define	GT911_NOISE_MIN_THRESHOLD	0x8080
#define	GT911_HOPPING_SENSOR_GROUP	0x8082
#define	GT911_HOPPING_SEG1_NORMALIZE	0x8083
#define	GT911_HOPPING_SEG1_FACTOR	0x8084
#define	GT911_MAIN_CLOCK_AJDUST		0x8085
#define	GT911_HOPPING_SEG2_NORMALIZE	0x8086
#define	GT911_HOPPING_SEG2_FACTOR	0x8087
#define	GT911_HOPPING_SEG3_NORMALIZE	0x8089
#define	GT911_HOPPING_SEG3_FACTOR	0x808A
#define	GT911_HOPPING_SEG4_NORMALIZE	0x808C
#define	GT911_HOPPING_SEG4_FACTOR	0x808D
#define	GT911_HOPPING_SEG5_NORMALIZE	0x808F
#define	GT911_HOPPING_SEG5_FACTOR	0x8090
#define	GT911_HOPPING_SEG6_NORMALIZE	0x8092
#define	GT911_KEY_1			0x8093
#define	GT911_KEY_2			0x8094
#define	GT911_KEY_3			0x8095
#define	GT911_KEY_4			0x8096
#define	GT911_KEY_AREA			0x8097
#define	GT911_KEY_TOUCH_LEVEL		0x8098
#define	GT911_KEY_LEAVE_LEVEL		0x8099
#define	GT911_KEY_SENS			0x809A
#define	GT911_KEY_SENS			0x809B
#define	GT911_KEY_RESTRAIN		0x809C
#define	GT911_KEY_RESTRAIN_TIME		0x809D
#define	GT911_GESTURE_LARGE_TOUCH	0x809E
#define	GT911_HOTKNOT_NOISE_MAP		0x80A1
#define	GT911_LINK_THRESHOLD		0x80A2
#define	GT911_PXY_THRESHOLD		0x80A3
#define	GT911_GHOT_DUMP_SHIFT		0x80A4
#define	GT911_GHOT_RX_GAIN		0x80A5
#define	GT911_FREQ_GAIN0		0x80A6
#define	GT911_FREQ_GAIN1		0x80A7
#define	GT911_FREQ_GAIN2		0x80A8
#define	GT911_FREQ_GAIN3		0x80A9
#define	GT911_COMBINE_DIS		0x80B3
#define	GT911_SPLIT_SET			0x80B4
#define	GT911_SENSOR_CH0		0x80B7
#define	GT911_DRIVER_CH0		0x80D5
#define	GT911_CONFIG_CHKSUM		0x80FF
#define	GT911_CONFIG_FRESH		0x8100

#endif /* !_DEV_GT911_GT911_H_ */
