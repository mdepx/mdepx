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
#include <sys/mutex.h>

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

static int
mqtt_send(struct mqtt_client *c, uint8_t *buf, int len)
{
	struct mqtt_network *net;
	uint32_t sent;
	int ret;

	sent = 0;

	net = &c->net;

	mdx_sem_wait(&c->sem_send);

	while (sent != len) {
		ret = net->write(net, &buf[sent], (len - sent));
		if (ret < 0)
			break;
		sent += ret;
	}

	mdx_sem_post(&c->sem_send);

	if (sent == len) {
		printf("%s: data sent\n", __func__);
		return (0);
	}

	printf("%s: failed to send data\n", __func__);

	return (MQTT_ERR_CONN);
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
	int err;

	printf("pingresp received\n");

	/* Read remaining */
	err = mqtt_recv(&c->net, &buf[1], 1);
	if (err != 1) {
		printf("%s: rem is not received\n", __func__);
		return (MQTT_ERR_CONN);
	}

	return (0);
}

static int
read_msg(struct mqtt_client *c, uint8_t *buf, uint32_t len,
    uint32_t size_expected)
{
	int rem;
	int err;

	/* Read remaining */
	err = mqtt_recv(&c->net, &buf[1], 1);
	if (err != 1) {
		printf("%s: rem is not received\n", __func__);
		return (MQTT_ERR_CONN);
	}

	rem = buf[1];
	if (rem != size_expected) {
		printf("%s: invalid msg received\n", __func__);
		return (MQTT_ERR_UNKNOWN);
	}

	/* Read the rest */
	err = mqtt_recv(&c->net, &buf[2], rem);
	if (err != rem) {
		printf("%s: invalid read\n", __func__);
		return (MQTT_ERR_CONN);
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
		return (err);

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

	err = mqtt_send(c, data, 4);
	if (err)
		return (MQTT_ERR_CONN);

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

	err = mqtt_send(c, data, 4);
	if (err)
		return (MQTT_ERR_CONN);

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
		return (err);

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
		return (err);

	packet_id = buf[2] << 8 | buf[3];
	mdx_mutex_lock(&c->msg_mtx);
	for (m = msg_first(c); m != NULL; m = msg_next(c, m)) {
		if (m->packet_id == packet_id) {
			/* Found */
			if (m->state == MSG_STATE_PUBREC && m->qos == 2) {
				m->state = MSG_STATE_PUBCOMP;
				mdx_sem_post(&m->complete);
				break;
			}
		}
	}
	mdx_mutex_unlock(&c->msg_mtx);

	return (0);
}

static void
mqtt_event(struct mqtt_client *c, enum mqtt_connection_event ev)
{

	mdx_mutex_lock(&c->mtx);
	if (ev == MQTT_EVENT_CONNECTED) {
		if (c->connected == 1) {
			mdx_mutex_unlock(&c->mtx);
			return;
		}
		c->connected = 1;
	}

	if (ev == MQTT_EVENT_DISCONNECTED) {
		if (c->connected == 0) {
			mdx_mutex_unlock(&c->mtx);
			return;
		}
		c->connected = 0;
	}
	mdx_mutex_unlock(&c->mtx);

	if (c->event)
		c->event(c, ev);
}

static int
handle_connack(struct mqtt_client *c, uint8_t *buf, uint32_t len)
{
	struct mqtt_request *m;
	int flags, error;
	int err;

	printf("conn ack received\n");

	err = read_msg(c, buf, len, 2);
	if (err)
		return (err);

	flags = buf[2];
	error = buf[3];

	printf("%s: flags %d error %d\n", __func__, flags, error);

	/* Find our connection request in the queue. */

	mdx_mutex_lock(&c->msg_mtx);
	for (m = msg_first(c); m != NULL; m = msg_next(c, m)) {
		if (m->state == MSG_STATE_CONNECT) {
			m->state = MSG_STATE_CONNACK;
			m->status = error;
			mdx_sem_post(&m->complete);
			break;
		}
	}
	mdx_mutex_unlock(&c->msg_mtx);

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
		mqtt_event(c, MQTT_EVENT_DISCONNECTED);
		return (MQTT_ERR_CONN);
	}

	rem = data[1];
	printf("publish rem %d\n", rem);

	/* Read remaining */
	err = mqtt_recv(&c->net, &data[2], rem);
	if (err != rem) {
		printf("%s: rem is not received\n", __func__);
		mqtt_event(c, MQTT_EVENT_DISCONNECTED);
		return (MQTT_ERR_CONN);
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

	if (c->msgcb)
		c->msgcb(c, &m);

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
		return (err);

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

int
mqtt_disconnect(struct mqtt_client *c)
{
	uint8_t data[2];
	int err;

	/* Fixed header */
	data[0] = CONTROL_DISCONNECT;
	data[1] = 0;

	err = mqtt_send(c, data, 2);
	if (err)
		return (MQTT_ERR_CONN);

	mqtt_event(c, MQTT_EVENT_DISCONNECTED);

	return (err);
}

int
mqtt_connect(struct mqtt_client *c)
{
	struct mqtt_request req;
	uint8_t data[128];
	uint8_t flags;
	uint16_t keepalive;
	int timeout;
	int retval;
	int err;

	/* Fixed header */
	data[0] = CONTROL_CONNECT;
	data[1] = 17;			/* Remaining Length */

	flags = FLAG_CLEAN_SESSION;
	keepalive = 120;

	data[2] = 0x00;
	data[3] = 0x04;
	data[4] = 'M';
	data[5] = 'Q';
	data[6] = 'T';
	data[7] = 'T';
	data[8] = MQTT_VERSION_3_1_1;	/* Protocol Level */
	data[9] = flags;		/* Connect Flags */
	data[10] = keepalive >> 8;	/* Keep Alive MSB */
	data[11] = keepalive & 0xff;	/* Keep Alive LSB */

	/* Payload */
	data[12] = 0;			/* Client ID len MSB */
	data[13] = 5;			/* Client ID len LSB */
	memcpy(&data[14], "md009", 5); /* Client ID ... */
#if 0
	data[19] = 0;
	data[20] = 3;
	memcpy(&data[21], "a/b", 3);	/* Will topic */
	data[24] = 0;
	data[25] = 3;
	memcpy(&data[26], "aaa", 3);	/* Will message */

	printf("Sending CONNECT message: ");
	int i;
	for (i = 0; i < 19; i++)
		printf("%x ", data[i]);
	printf("\n");
#endif

	req.state = MSG_STATE_CONNECT;
	mdx_sem_init(&req.complete, 0);

	mdx_mutex_lock(&c->msg_mtx);
	/* TODO: ensure here that there is no connect messages in the queue */
	list_append(&c->msg_list, &req.node);
	mdx_mutex_unlock(&c->msg_mtx);

	err = mqtt_send(c, data, 19);
	if (err) {
		mdx_mutex_lock(&c->msg_mtx);
		list_remove(&req.node);
		mdx_mutex_unlock(&c->msg_mtx);

		mqtt_event(c, MQTT_EVENT_DISCONNECTED);
		return (MQTT_ERR_CONN);
	}

	/*
	 * Connect message sent.
	 * Now wait for the connection status change.
	 */
	timeout = 10;
	do {
		mqtt_poll(c);
		err = mdx_sem_timedwait(&req.complete, 1000000);
		if (err)
			break;
	} while (timeout--);

	printf("%s: timeout %d err %d\n", __func__, timeout, err);
	/*
	 * Connection could still be established right here.
	 * We will have to check req.status after removing
	 * req from the queue.
	 */

	mdx_mutex_lock(&c->msg_mtx);
	list_remove(&req.node);
	mdx_mutex_unlock(&c->msg_mtx);

	if (req.state == MSG_STATE_CONNACK) {
		switch (req.status) {
		case ERR_ACCEPTED:
			printf("%s: connected\n", __func__);
			mqtt_event(c, MQTT_EVENT_CONNECTED);
			retval = MQTT_ERR_OK;
			break;
		case ERR_REFUSED_PROTOCOL_VERSION:
		case ERR_REFUSED_ID_REJECTED:
		case ERR_REFUSED_SERVER_UNAVAILABLE:
		case ERR_REFUSED_AUTHENTICATION:
		case ERR_REFUSED_AUTHORIZATION:
			retval = MQTT_ERR_UNKNOWN;
			break;
		default:
			/* Invalid error code */
			break;
		};
	} else {
		printf("%s: Timeout\n", __func__);
		retval = MQTT_ERR_TIMEOUT;
	}

	return (retval);
}

int
mqtt_subscribe(struct mqtt_client *c, struct mqtt_request *s)
{
	uint8_t data[128];
	int timeout;
	int retval;
	int pos;
	int err;

	if (s->qos != 0 && s->qos != 1)
		return (MQTT_ERR_REQ);

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

	err = mqtt_send(c, data, pos);
	if (err) {
		mdx_mutex_lock(&c->msg_mtx);
		list_remove(&s->node);
		mdx_mutex_unlock(&c->msg_mtx);
		mqtt_event(c, MQTT_EVENT_DISCONNECTED);
		return (MQTT_ERR_CONN);
	}

	timeout = 10;
	do {
		mqtt_poll(c);
		err = mdx_sem_timedwait(&s->complete, 1000000);
		if (err)
			break;
	} while (timeout--);

	mdx_mutex_lock(&c->msg_mtx);
	list_remove(&s->node);
	mdx_mutex_unlock(&c->msg_mtx);

	if (s->state == MSG_STATE_SUBACK) {
		switch (s->error) {
		case 0x00:
		case 0x01:
		case 0x02:
			printf("Subscribed successfully\n");
			retval = MQTT_ERR_OK;
			break;
		case 0x80:
		default:
			retval = MQTT_ERR_UNKNOWN;
		};
	} else {
		/* No reply received. */
		retval = MQTT_ERR_TIMEOUT;
	}

	return (retval);
}

int
mqtt_publish(struct mqtt_client *c, struct mqtt_request *m)
{
	uint8_t data[128];
	int timeout;
	int retval;
	int err;
	int pos;

	if (m->topic == NULL || m->topic_len == 0 ||
	    m->data == NULL || m->data_len == 0)
		return (MQTT_ERR_REQ);

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
		return (MQTT_ERR_UNKNOWN);
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

	err = mqtt_send(c, data, pos);
	if (err) {
		/* Could not send packet */
		if (m->qos == 1 || m->qos == 2) {
			mdx_mutex_lock(&c->msg_mtx);
			list_remove(&m->node);
			mdx_mutex_unlock(&c->msg_mtx);
		}
		mqtt_event(c, MQTT_EVENT_DISCONNECTED);
		return (MQTT_ERR_CONN);
	}

	/* Packet sent */
	if (m->qos == 0)
		return (0);

	timeout = 10;
	do {
		mqtt_poll(c);
		err = mdx_sem_timedwait(&m->complete, 1000000);
		if (err)
			break;
	} while (timeout--);

	mdx_mutex_lock(&c->msg_mtx);
	list_remove(&m->node);
	mdx_mutex_unlock(&c->msg_mtx);

	if (m->state == MSG_STATE_PUBACK && m->qos == 1) {
		/* puback received */
		retval = 0;
	} else if (m->state == MSG_STATE_PUBCOMP && m->qos == 2) {
		/* pubcomp received */
		retval = 0;
	} else {
		/* Timeout. */
		retval = -1;
	}

	return (retval);
}

int
mqtt_ping(struct mqtt_client *c)
{
	uint8_t data[2];
	int err;

	data[0] = CONTROL_PINGREQ;
	data[1] = 0;	/* Remaining Length */

	err = mqtt_send(c, data, 2);
	if (err)
		return (MQTT_ERR_CONN);

	/* TODO: wait for PINGRESP */

	return (0);
}

/*
 * This function requires a non-blocking socket to operate.
 */
void
mqtt_poll(struct mqtt_client *c)
{
	uint8_t data[128];
	int err;

	err = mqtt_recv(&c->net, data, 1);
	if (err == 0) {
		printf("close 1\n");
		mqtt_event(c, MQTT_EVENT_DISCONNECTED);
	} else if (err == 1) {
		err = handle_recv(c, data, 128);
		if (err == MQTT_ERR_CONN) {
			printf("close 2\n");
			mqtt_event(c, MQTT_EVENT_DISCONNECTED);
		}
	}
}

int
mqtt_init(struct mqtt_client *c)
{

	mdx_mutex_init(&c->mtx);
	mdx_sem_init(&c->sem_send, 1);

	mdx_mutex_init(&c->msg_mtx);
	list_init(&c->msg_list);

	c->next_id = 1;
	c->connected = 0;

	return (0);
}

int
mqtt_deinit(struct mqtt_client *c)
{

	/* TODO: terminate the recv thread here */

	return (0);
}
