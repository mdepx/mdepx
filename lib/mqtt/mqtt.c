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

static struct mqtt_request *
msg_first(struct mqtt_client *c)
{
	struct mqtt_request *m;

	if (list_empty(&c->msg_list))
		return (NULL);

	m = CONTAINER_OF(c->msg_list.next, struct mqtt_request, node);

	return (m);
}

static struct mqtt_request *
msg_next(struct mqtt_client *c, struct mqtt_request *m0)
{
	struct mqtt_request *m;

	if (m0->node.next == &c->msg_list)
		return (NULL);

	m = CONTAINER_OF(m0->node.next, struct mqtt_request, node);

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
	struct mqtt_request *m;
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
mqtt_send_pubrel(struct mqtt_client *c, struct mqtt_request *m)
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
mqtt_send_puback(struct mqtt_client *c, struct mqtt_request *m)
{
	uint8_t data[128];
	int err;

	printf("%s\n", __func__);

	data[0] = CONTROL_PUBACK;
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
	struct mqtt_request *m;
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
	struct mqtt_request *m;
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
	struct mqtt_request m;
	int flags;
	int pos;
	int qos;
	int err;
	int rem;

	flags = data[0] & 0xf;
	if (flags & FLAGS_PUBLISH_DUP)
		printf("DUP flag set\n");
	if (flags & FLAGS_PUBLISH_RETAIN)
		printf("RETAIN flag set\n");

	qos = (flags & FLAGS_PUBLISH_QOS_M) >> FLAGS_PUBLISH_QOS_S;

	printf("publish received, qos %d\n", qos);

	/* Read remaining len */
	err = mqtt_recv(&c->net, &data[1], 1);
	if (err != 1) {
		printf("%s: rem is not received\n", __func__);
		return (-1);
	}

	rem = data[1];
	printf("publish rem %d\n", rem);

	/* Read remaining */
	err = mqtt_recv(&c->net, &data[2], rem);
	if (err != rem) {
		printf("%s: rem is not received\n", __func__);
		return (-1);
	}

	m.topic_len = data[2] << 8 | data[3];
	m.topic = &data[4];
	pos = 4 + m.topic_len;
	if (qos == 1 || qos == 2) {
		m.packet_id = data[pos++] << 8;
		m.packet_id |= data[pos++];
	} else
		m.packet_id = 0;
	m.data = &data[pos];
	m.data_len = rem - m.topic_len - 4;

	if (c->cb)
		c->cb(c, &m);

	if (qos == 1)
		mqtt_send_puback(c, &m);

	if (qos == 2) {
		/* No support */
	}

	return (0);
}

static int
handle_suback(struct mqtt_client *c, uint8_t *buf, uint32_t len)
{
	struct mqtt_request *m;
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

	status = buf[4]; /* Subscription result of the 1st topic. */

	mdx_mutex_lock(&c->msg_mtx);
	for (m = msg_first(c); m != NULL; m = msg_next(c, m)) {
		if (m->packet_id == packet_id) {
			/* Found */
			if (m->state == MSG_STATE_SUBSCRIBE) {
				m->state = MSG_STATE_SUBACK;
				m->error = status;
				mdx_sem_post(&m->complete);
				break;
			}
		}
	}
	mdx_mutex_unlock(&c->msg_mtx);

	return (0);
}

static int
handle_recv(struct mqtt_client *c, uint8_t *data, uint32_t len)
{
	uint8_t ctl;
	int err;

	ctl = data[0] & MQTT_CONTROL_M;
	/* TODO: verify lsb */

	err = 0;

	switch (ctl) {
	case CONTROL_CONNACK:
		err = handle_connack(c, data, len);
		break;
	case CONTROL_PINGRESP:
		err = handle_pingresp(c, data, len);
		break;
	case CONTROL_SUBACK:
		err = handle_suback(c, data, len);
		break;
	case CONTROL_PUBLISH:
		err = handle_publish(c, data, len);
		break;
	case CONTROL_PUBACK:
		err = handle_puback(c, data, len);
		break;
	case CONTROL_PUBREC:
		err = handle_pubrec(c, data, len);
		break;
	case CONTROL_PUBCOMP:
		err = handle_pubcomp(c, data, len);
		break;
	default:
		printf("Unknown packet received: 0x%x\n", ctl);
		break;
	}

	return (err);
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
mqtt_subscribe(struct mqtt_client *c, struct mqtt_request *s)
{
	struct mqtt_network *net;
	uint8_t data[128];
	int retval;
	int pos;
	int err;

	net = &c->net;

	if (s->qos != 0 && s->qos != 1)
		return (-1);

	s->packet_id = c->next_id++; /* Use atomics ? */

	/* Variable header */
	data[2] = s->packet_id >> 8;
	data[3] = s->packet_id & 0xff;

	/* Payload */
	data[4] = s->topic_len >> 8;
	data[5] = s->topic_len & 0xff;
	memcpy(&data[6], s->topic, s->topic_len);
	pos = 6 + s->topic_len;
	data[pos++] = s->qos;

	/* Fixed header */
	data[0] = CONTROL_SUBSCRIBE | FLAGS_SUBSCRIBE;
	data[1] = (pos - 2);	/* Remaining Length */

	mdx_sem_init(&s->complete, 0);

	s->state = MSG_STATE_SUBSCRIBE;
	mdx_mutex_lock(&c->msg_mtx);
	list_append(&c->msg_list, &s->node);
	mdx_mutex_unlock(&c->msg_mtx);

	mdx_sem_wait(&c->sem_sendrecv);
	err = mqtt_send(net, data, pos);
	mdx_sem_post(&c->sem_sendrecv);

	if (err) {
		/* TODO: close connection */
		mdx_mutex_lock(&c->msg_mtx);
		list_remove(&s->node);
		mdx_mutex_unlock(&c->msg_mtx);
		return (-1);
	}

	err = mdx_sem_timedwait(&s->complete, 5000000);
	if (err) {
		switch (s->error) {
		case 0x00:
		case 0x01:
		case 0x02:
			printf("Subscribed successfully\n");
			retval = 0;
			break;
		case 0x80:
			retval = -2;
			break;
		default:
			retval = -1;
		};
	} else {
		/* Timeout */
		/* TODO: close connection */
		retval = -1;
	}

	mdx_mutex_lock(&c->msg_mtx);
	list_remove(&s->node);
	mdx_mutex_unlock(&c->msg_mtx);

	return (retval);
}

int
mqtt_publish(struct mqtt_client *c, struct mqtt_request *m)
{
	uint8_t data[128];
	int retval;
	int err;
	int pos;

	if (m->topic == NULL || m->topic_len == 0 ||
	    m->data == NULL || m->data_len == 0)
		return (-1);

	/* Variable header: Topic details */
	data[2] = m->topic_len >> 8;
	data[3] = m->topic_len & 0xff;
	memcpy(&data[4], m->topic, m->topic_len);
	pos = 4 + m->topic_len;

	/* Variable header: Packet Identifier */
	switch (m->qos) {
	case 0:
		data[pos++] = 0;
		data[pos++] = 0; /* must not be used if qos == 0 */
		break;
	case 1:
	case 2:
		m->packet_id = c->next_id++;
		data[pos++] = m->packet_id >> 8;
		data[pos++] = m->packet_id & 0xff;
		break;
	default:
		return (-1);
	};

	/* Payload */
	memcpy(&data[pos], m->data, m->data_len);
	pos += m->data_len;

	/* Fixed header */
	data[0] = CONTROL_PUBLISH | FLAGS_PUBLISH_QOS(m->qos);
	data[1] = (pos - 2);	/* Remaining Length */

	if (m->qos == 1 || m->qos == 2) {
		mdx_sem_init(&m->complete, 0);
		m->state = MSG_STATE_PUBLISH;
		mdx_mutex_lock(&c->msg_mtx);
		list_append(&c->msg_list, &m->node);
		mdx_mutex_unlock(&c->msg_mtx);
	};

	mdx_sem_wait(&c->sem_sendrecv);
	err = mqtt_send(&c->net, data, pos);
	mdx_sem_post(&c->sem_sendrecv);
	if (err) {
		/* Could not send packet */
		/* TODO: close connection */
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
		/* TODO: close connection */
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
			err = handle_recv(c, data, 128);
			if (err)
				break;
			mqtt_resched_ping(c);
		}
		mdx_usleep(1000000);
	}

	/* TODO: close connection here */
}

int
mqtt_init(struct mqtt_client *c)
{

	mdx_sem_init(&c->sem_sendrecv, 1);
	mdx_sem_init(&c->sem_connect, 0);
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
