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

#include <sys/cdefs.h>
#include <sys/thread.h>
#include <sys/sem.h>

#include "mqtt.h"

static void
mqtt_ping_req(void *arg)
{
	struct mqtt_client *c;

	c = arg;

	mdx_sem_post(&c->sem_ping_req);
}

static void
mqtt_resched_ping(struct mqtt_client *c)
{

	critical_enter();
	mdx_callout_cancel(&c->c_ping_req);
	mdx_callout_set(&c->c_ping_req, 5000000, mqtt_ping_req, c);
	critical_exit();
}

static int
mqtt_send(struct mqtt_network *net, uint8_t *buf, int len)
{
	uint32_t sent;
	int ret;

	sent = 0;

	while (sent != len) {
		ret = net->write(net, &buf[sent], (len - sent));
		if (ret < 0)
			break;
		sent += ret;
	}

	if (sent == len) {
		printf("%s: data sent\n", __func__);
		return (0);
	}

	printf("%s: failed to send data\n", __func__);

	return (-1);
}

static int
mqtt_recv(struct mqtt_network *net, uint8_t *buf, int len)
{
	int ret;

	ret = net->read(net, buf, len);

	return (ret);
}

static int
handle_pingresp(struct mqtt_client *c, uint8_t *buf, uint32_t len)
{
	struct mqtt_network *net;
	int err;

	net = &c->net;

	printf("pingresp received\n");

	/* Read remaining */
	err = mqtt_recv(net, &buf[1], 1);
	if (err != 1) {
		printf("%s: rem is not received\n", __func__);
		return (-1);
	}

	mdx_sem_post(&c->sem_ping_ack);

	return (0);
}

static int
handle_connack(struct mqtt_client *c, uint8_t *buf, uint32_t len)
{
	struct mqtt_network *net;
	int ret, rem, flags, error;
	int err;

	net = &c->net;

	printf("conn ack received\n");

	/* Read remaining */
	err = mqtt_recv(net, &buf[1], 1);
	if (err != 1) {
		printf("%s: rem is not received\n", __func__);
		return (-1);
	}

	/* Verify the size */
	rem = buf[1];
	if (rem != 2) {
		printf("%s: invalid conn ack received\n", __func__);
		return (-1);
	}

	/* Read flags */
	ret = net->read(net, &buf[2], 2);
	if (ret != 2) {
		printf("%s: invalid read\n", __func__);
		return (-1);
	}

	flags = buf[2];
	error = buf[3];

	printf("flags %d error %d\n", flags, error);

	switch (error) {
	case ERR_ACCEPTED:
		mdx_sem_post(&c->sem_connect);
		KASSERT(c->connected == 0, ("Already connected\n"));
		c->connected = 1;
		break;
	case ERR_REFUSED_PROTOCOL_VERSION:
	case ERR_REFUSED_ID_REJECTED:
	case ERR_REFUSED_SERVER_UNAVAILABLE:
	case ERR_REFUSED_AUTHENTICATION:
	case ERR_REFUSED_AUTHORIZATION:
		c->connected = 0;
		break;
	default:
		/* Invalid error code */
		break;
	}

	return (0);
}

static int
handle_suback(struct mqtt_client *c, uint8_t *data, uint32_t len)
{
	int packet_id;
	int n_topics;
	int status, rem_size_expected;
	int rem;
	int err;

	printf("sub ack received\n");

	/* Read remaining */
	err = mqtt_recv(&c->net, &data[1], 1);
	if (err != 1) {
		printf("%s: rem is not received\n", __func__);
		return (-1);
	}

	rem = data[1];

	n_topics = 1;
	rem_size_expected = 2 + n_topics;

	if (rem_size_expected != rem) {
		printf("%s: invalid rem\n", __func__);
		return (-1);
	}

	err = mqtt_recv(&c->net, &data[2], rem);
	if (err != rem) {
		printf("%s: invalid read\n", __func__);
		return (-1);
	}

	packet_id = data[2] << 8;
	packet_id |= data[3];
	printf("%s: packet_id %d\n", __func__, packet_id);

	status = data[4]; /* Status for the 1st topic */

	switch (status) {
	case 0x00:
	case 0x01:
	case 0x02:
		printf("Subscribed successfully, status %x\n", status);
		mdx_sem_post(&c->sem_subscribe);
		return (0);
	case 0x80:
		return (-1);
	default:
		printf("invalid status: 0x%x\n", status);
		return (-1);
	}

	return (0);
}

static void
handle_recv(struct mqtt_client *c, uint8_t *data, uint32_t len)
{
	uint8_t ctl;

	ctl = data[0] & MQTT_CONTROL_M;
	/* TODO: verify lsb */

	switch (ctl) {
	case CONTROL_CONNACK:
		handle_connack(c, data, len);
		break;
	case CONTROL_PINGRESP:
		handle_pingresp(c, data, len);
		break;
	case CONTROL_SUBACK:
		handle_suback(c, data, len);
		break;
	default:
		printf("Unknown packet received: %d\n", ctl);
		break;
	}
}

static uint8_t *
mqtt_variable_header(uint8_t *ptr, uint8_t flags,
    uint16_t keepalive)
{

	/* Variable header */
	*ptr++ = 0x00;
	*ptr++ = 0x04;
	*ptr++ = 'M';
	*ptr++ = 'Q';
	*ptr++ = 'T';
	*ptr++ = 'T';
	*ptr++ = MQTT_VERSION_3_1_1;	/* Protocol Level */
	*ptr++ = flags;			/* Connect Flags */
	*ptr++ = keepalive >> 8;	/* Keep Alive MSB */
	*ptr++ = keepalive & 0xff;	/* Keep Alive LSB */

	return (ptr);
}

int
mqtt_connect(struct mqtt_client *c)
{
	struct mqtt_network *net;
	uint8_t data[128];
	uint8_t *ptr;
	uint8_t flags;
	uint16_t keepalive;
	int err;

	net = &c->net;

	/* Fixed header */
	data[0] = CONTROL_CONNECT;
	data[1] = 15;			/* Remaining Length */

	flags = FLAG_CLEAN_SESSION;
	keepalive = 20;
	ptr = mqtt_variable_header(&data[2], flags, keepalive);

	/* Payload */
	*ptr++ = 0;			/* Payload len MSB */
	*ptr++ = 3;			/* Payload len LSB */
	*ptr++ = 'a';			/* Client ID ... */
	*ptr++ = 'b';
	*ptr++ = 'c';

	mdx_sem_wait(&c->sem_sendrecv);
	err = mqtt_send(net, data, 17);
	mdx_sem_post(&c->sem_sendrecv);
	if (err)
		return (-1);

	/*
	 * Connect message sent.
	 * Now wait for the connection status change.
	 */

	err = mdx_sem_timedwait(&c->sem_connect, 5000000);
	if (err) {
		/* Connack received. Let's check status. */
		if (c->connected) {
			printf("Connection established\n");
			return (0);	/* Connected */
		} else {
			printf("Failed to connect\n");
			return (-2); 	/* Connection failed */
		}
	}

	printf("%s: Timeout\n", __func__);

	return (-3);
}

int
mqtt_subscribe(struct mqtt_client *c)
{
	struct mqtt_network *net;
	uint8_t data[128];
	int err;

	net = &c->net;

	/* Fixed header */
	data[0] = CONTROL_SUBSCRIBE | FLAGS_SUBSCRIBE;
	data[1] = 8;		/* Remaining Length */

	/* Variable header */
	data[2] = 0;
	data[3] = 10;

	/* Payload */
	data[4] = 0;
	data[5] = 3;
	data[6] = 'a';
	data[7] = '/';
	data[8] = 'b';
	data[9] = 0;	/* QoS */

	mdx_sem_wait(&c->sem_sendrecv);
	err = mqtt_send(net, data, 10);
	mdx_sem_post(&c->sem_sendrecv);

	if (err)
		return (-1);

	err = mdx_sem_timedwait(&c->sem_subscribe, 5000000);
	if (err) {
		printf("%s: subscribed\n", __func__);
		return (0);
	}

	return (-1);
}

static void
mqtt_thread_ping(void *arg)
{
	struct mqtt_network *net;
	struct mqtt_client *c;
	uint8_t data[128];
	int err;

	c = arg;
	net = &c->net;

	data[0] = CONTROL_PINGREQ;
	data[1] = 0;		/* Remaining Length */

	while (1) {
		mdx_sem_wait(&c->sem_ping_req);

		mdx_sem_wait(&c->sem_sendrecv);
		err = mqtt_send(net, data, 2);
		mdx_sem_post(&c->sem_sendrecv);

		if (err) {
			printf("can't send ping packet\n");
			c->connected = 0;
			break;
		}

		/* Wait reply. */
		err = mdx_sem_timedwait(&c->sem_ping_ack, 5000000);
		if (err == 0) {
			printf("no ping reply received\n");
			c->connected = 0;
			break;
		}
	}
}

static void
mqtt_thread_recv(void *arg)
{
	uint8_t data[128];
	struct mqtt_network *net;
	struct mqtt_client *c;
	int err;

	c = arg;
	net = &c->net;

	while (1) {
		mdx_sem_wait(&c->sem_sendrecv);
		err = mqtt_recv(net, data, 1);
		if (err == -2) {
			/* Connection closed. */
			c->connected = 0;
			break;
		}
		if (err == 1) {
			handle_recv(c, data, 128);
			mqtt_resched_ping(c);
		}
		mdx_sem_post(&c->sem_sendrecv);
		mdx_usleep(1000000);
	}
}

int
mqtt_init(struct mqtt_client *c)
{

	mdx_sem_init(&c->sem_sendrecv, 1);
	mdx_sem_init(&c->sem_connect, 0);
	mdx_sem_init(&c->sem_subscribe, 0);
	mdx_sem_init(&c->sem_ping_req, 0);
	mdx_sem_init(&c->sem_ping_ack, 0);

	mdx_callout_init(&c->c_ping_req);

	c->connected = 0;
	c->td_recv = mdx_thread_create("mqtt recv", 1, 0, 8192,
	    mqtt_thread_recv, c);
	if (c->td_recv == NULL)
		return (-1);

	c->td_ping = mdx_thread_create("mqtt ping", 1, 0, 8192,
	    mqtt_thread_ping, c);
	if (c->td_ping == NULL)
		return (-1);

	mdx_sched_add(c->td_recv);
	mdx_sched_add(c->td_ping);

	return (0);
}
