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

/* Fixed header */
#define	MQTT_CONTROL_S		4
#define	MQTT_CONTROL_M		(0xf << MQTT_CONTROL_S)
#define	 CONTROL_RESERVED0	(0 << MQTT_CONTROL_S)
#define	 CONTROL_CONNECT	(1 << MQTT_CONTROL_S)
#define	 CONTROL_CONNACK	(2 << MQTT_CONTROL_S)
#define	 CONTROL_PUBLISH	(3 << MQTT_CONTROL_S)
#define	 CONTROL_PUBACK		(4 << MQTT_CONTROL_S)
#define	 CONTROL_PUBREC		(5 << MQTT_CONTROL_S)
#define	 CONTROL_PUBREL		(6 << MQTT_CONTROL_S)
#define	 CONTROL_PUBCOMP	(7 << MQTT_CONTROL_S)
#define	 CONTROL_SUBSCRIBE	(8 << MQTT_CONTROL_S)
#define	 CONTROL_SUBACK		(9 << MQTT_CONTROL_S)
#define	 CONTROL_UNSUBSCRIBE	(10 << MQTT_CONTROL_S)
#define	 CONTROL_UNSUBACK	(11 << MQTT_CONTROL_S)
#define	 CONTROL_PINGREQ	(12 << MQTT_CONTROL_S)
#define	 CONTROL_PINGRESP	(13 << MQTT_CONTROL_S)
#define	 CONTROL_DISCONNECT	(14 << MQTT_CONTROL_S)
#define	 CONTROL_RESERVED1	(15 << MQTT_CONTROL_S)

/* Variable header */
#define	MQTT_VERSION_3_1_1	4

/* Connect Flags */
#define	FLAG_CLEAN_SESSION	(1 << 1)
#define	FLAG_WILL		(1 << 2)
#define	FLAG_WILL_QOS0		(1 << 3)
#define	FLAG_WILL_QOS1		(1 << 4)
#define	FLAG_WILL_RETAIN	(1 << 5)
#define	FLAG_PASSWORD		(1 << 6)
#define	FLAG_USER		(1 << 7)

/* Connect return byte 1 */
#define	CONNECT_ACK_FLAG_SP	(1 << 0) /* Session Present */

/* Connect return byte 2 */
enum connect_return_errno {
	ERR_ACCEPTED,			/* Connection accepted */
	ERR_REFUSED_PROTOCOL_VERSION,	/* Unsupported MQTT protocol level */
	ERR_REFUSED_ID_REJECTED,	/* The Client ID is not allowed */
	ERR_REFUSED_SERVER_UNAVAILABLE,	/* MQTT service is unavailable */
	ERR_REFUSED_AUTHENTICATION,	/* Incorrect user name or password */
	ERR_REFUSED_AUTHORIZATION,	/* Client is not authorized to connect*/
};

struct mqtt_network {
	int fd;
	int (*read) (struct mqtt_network *, uint8_t *, int);
	int (*write) (struct mqtt_network *, uint8_t *, int);
};

int mqtt_connect(struct mqtt_network *net);
