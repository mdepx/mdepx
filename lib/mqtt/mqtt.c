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

static struct mqtt_message *
msg_first(struct mqtt_client *c)
{
	struct mqtt_message *m;

	if (list_empty(&c->msg_list))
		return (NULL);

	m = CONTAINER_OF(c->msg_list.next, struct mqtt_message, node);

	return (m);
}

static struct mqtt_message *
msg_next(struct mqtt_client *c, struct mqtt_message *m0)
{
	struct mqtt_message *m;

	if (m0->node.next == &c->msg_list)
		return (NULL);

	m = CONTAINER_OF(m0->node.next, struct mqtt_message, node);

	return (m);
}

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
	mdx_callout_set(&c->c_ping_req, 20000000, mqtt_ping_req, c);
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
read_msg(struct mqtt_client *c, uint8_t *buf, uint32_t len,
    uint32_t size_expected)
{
	struct mqtt_network *net;
	int ret, rem;
	int err;

	net = &c->net;

	/* Read remaining */
	err = mqtt_recv(net, &buf[1], 1);
	if (err != 1) {
		printf("%s: rem is not received\n", __func__);
		return (-1);
	}

	rem = buf[1];
	if (rem != size_expected) {
		printf("%s: invalid msg received\n", __func__);
		return (-1);
	}

	/* Read the rest */
	ret = net->read(net, &buf[2], rem);
	if (ret != rem) {
		printf("%s: invalid read\n", __func__);
		return (-1);
	}

	return (0);
}

static int
handle_puback(struct mqtt_client *c, uint8_t *buf, uint32_t len)
{
	struct mqtt_message *m;
	int packet_id;
	int err;

	printf("pub ack received\n");

	err = read_msg(c, buf, len, 2);
	if (err)
		return (-1);

	packet_id = buf[2] << 8 | buf[3];

	mdx_mutex_lock(&c->msg_mtx);
	for (m = msg_first(c); m != NULL; m = msg_next(c, m)) {
		if (m->packet_id == packet_id) {
			/* Found */
			if (m->state == MSG_STATE_PUBLISH && m->qos == 1) {
				m->state = MSG_STATE_PUBACK;
				mdx_sem_post(&m->complete);
				break;
			}
		}
	}
	mdx_mutex_unlock(&c->msg_mtx);

	return (0);
}

static int
mqtt_send_pubrel(struct mqtt_client *c, struct mqtt_message *m)
{
	uint8_t data[128];
	int err;

	printf("%s\n", __func__);

	data[0] = CONTROL_PUBREL | 2;
	data[1] = 2;		/* Remaining Length */

	/* Variable header */
	data[2] = m->packet_id >> 8;
	data[3] = m->packet_id & 0xff;

	mdx_sem_wait(&c->sem_sendrecv);
	err = mqtt_send(&c->net, data, 4);
	mdx_sem_post(&c->sem_sendrecv);

	if (err)
		return (-1);

	return (0);
}

static int
handle_pubrec(struct mqtt_client *c, uint8_t *buf, uint32_t len)
{
	struct mqtt_message *m;
	int packet_id;
	int err;

	printf("pub rec received\n");

	err = read_msg(c, buf, len, 2);
	if (err)
		return (-1);

	packet_id = buf[2] << 8 | buf[3];
	mdx_mutex_lock(&c->msg_mtx);
	for (m = msg_first(c); m != NULL; m = msg_next(c, m)) {
		if (m->packet_id == packet_id) {
			/* Found */
			if (m->state == MSG_STATE_PUBLISH && m->qos == 2) {
				m->state = MSG_STATE_PUBREC;
				mqtt_send_pubrel(c, m);
				break;
			}
		}
	}
	mdx_mutex_unlock(&c->msg_mtx);

	return (0);
}

static int
handle_pubcomp(struct mqtt_client *c, uint8_t *buf, uint32_t len)
{
	struct mqtt_message *m;
	int packet_id;
	int err;

	printf("pub comp received\n");

	err = read_msg(c, buf, len, 2);
	if (err)
		return (-1);

	packet_id = buf[2] << 8 | buf[3];
	mdx_mutex_lock(&c->msg_mtx);
	for (m = msg_first(c); m != NULL; m = msg_next(c, m)) {
		if (m->packet_id == packet_id) {
			/* Found */
			if (m->state == MSG_STATE_PUBREC && m->qos == 2) {
				m->state = MSG_STATE_PUBREL;
				mdx_sem_post(&m->complete);
				break;
			}
		}
	}
	mdx_mutex_unlock(&c->msg_mtx);

	return (0);
}

static int
handle_connack(struct mqtt_client *c, uint8_t *buf, uint32_t len)
{
	int flags, error;
	int err;

	printf("conn ack received\n");

	err = read_msg(c, buf, len, 2);
	if (err)
		return (-1);

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
handle_publish(struct mqtt_client *c, uint8_t *data, uint32_t len)
{
	int err;
	int rem;

	printf("publish received, data %x\n", data[0]);

	/* Read remaining len */
	err = mqtt_recv(&c->net, &data[1], 1);
	if (err != 1) {
		printf("%s: rem is not received\n", __func__);
		return (-1);
	}

	rem = data[1];
	printf("publish rem %d\n", rem);

	/* Read remaining */
	err = mqtt_recv(&c->net, &data[1], rem);
	if (err != rem) {
		printf("%s: rem is not received\n", __func__);
		return (-1);
	}

#if 0
	int i;
	for (i = 0; i < rem + 2; i++)
		printf("%s: data[%d] == %x\n", __func__, i, data[i]);
#endif

	return (0);
}

static int
handle_suback(struct mqtt_client *c, uint8_t *buf, uint32_t len)
{
	int packet_id;
	int n_topics;
	int status, rem_size_expected;
	int err;

	printf("sub ack received\n");

	n_topics = 1;
	rem_size_expected = 2 + n_topics;

	err = read_msg(c, buf, len, rem_size_expected);
	if (err)
		return (-1);

	packet_id = buf[2] << 8;
	packet_id |= buf[3];
	printf("%s: packet_id %d\n", __func__, packet_id);

	status = buf[4]; /* Status for the 1st topic */

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
	case CONTROL_PUBLISH:
		handle_publish(c, data, len);
		break;
	case CONTROL_PUBACK:
		handle_puback(c, data, len);
		break;
	case CONTROL_PUBREC:
		handle_pubrec(c, data, len);
		break;
	case CONTROL_PUBCOMP:
		handle_pubcomp(c, data, len);
		break;
	default:
		printf("Unknown packet received: 0x%x\n", ctl);
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

int
mqtt_publish(struct mqtt_client *c, struct mqtt_message *m)
{
	uint8_t data[128];
	int retval;
	int err;

	mdx_sem_init(&m->complete, 0);

	/* TODO: verify QoS */

	/* Fixed header */
	data[0] = CONTROL_PUBLISH | FLAGS_PUBLISH_QOS(m->qos);
	data[1] = 12;		/* Remaining Length */

	/* Variable header: Topic Name */
	data[2] = 0;
	data[3] = 3;
	data[4] = 'a';
	data[5] = '/';
	data[6] = 'b';

	/* Variable header: Packet Identifier */
	switch (m->qos) {
	case 0:
		data[7] = 0;
		data[8] = 0; /* must not be used if qos == 0 */
		break;
	case 1:
	case 2:
		m->packet_id = c->next_id++;
		data[7] = m->packet_id >> 8;
		data[8] = m->packet_id & 0xff;
		break;
	default:
		return (-1);
	};

	/* Payload */
	data[9] = 'h';
	data[10] = 'e';
	data[11] = 'l';
	data[12] = 'l';
	data[13] = 'o';

	if (m->qos == 1 || m->qos == 2) {
		m->state = MSG_STATE_PUBLISH;
		mdx_mutex_lock(&c->msg_mtx);
		list_append(&c->msg_list, &m->node);
		mdx_mutex_unlock(&c->msg_mtx);
	};

	mdx_sem_wait(&c->sem_sendrecv);
	err = mqtt_send(&c->net, data, 14);
	mdx_sem_post(&c->sem_sendrecv);
	if (err) {
		/* Could not send packet */
		if (m->qos == 1 || m->qos == 2) {
			mdx_mutex_lock(&c->msg_mtx);
			list_remove(&m->node);
			mdx_mutex_unlock(&c->msg_mtx);
		}
		return (-1);
	}

	/* Packet sent */
	if (m->qos == 0)
		return (0);

	err = mdx_sem_timedwait(&m->complete, 5000000);
	if (err) {
		/* Message sent successfully. */
		retval = 0;
	} else {
		/* Timeout. */
		retval = -1;
	}

	mdx_mutex_lock(&c->msg_mtx);
	list_remove(&m->node);
	mdx_mutex_unlock(&c->msg_mtx);

	return (retval);
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
		mdx_sem_post(&c->sem_sendrecv);
		if (err == -2) {
			/* Connection closed. */
			c->connected = 0;
			break;
		}
		if (err == 1) {
			handle_recv(c, data, 128);
			mqtt_resched_ping(c);
		}
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

	mdx_mutex_init(&c->msg_mtx);
	list_init(&c->msg_list);

	c->next_id = 1;
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
