/*-
 * Copyright (c) 2018 Ruslan Bukin <br@bsdpad.com>
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

#ifndef	_NET_IF_H_
#define	_NET_IF_H_

#include <sys/socket.h>
#include <sys/queue.h>
#include <net/route.h>
#include <net/if_types.h>

typedef struct ifnet * if_t;
typedef void (*if_start_fn_t)(if_t);
typedef int (*if_transmit_fn_t)(if_t, struct mbuf *);

struct ifaddr {
	struct sockaddr *ifa_addr;
	struct sockaddr *ifa_netmask;
	STAILQ_ENTRY(ifaddr) ifa_link;
};

STAILQ_HEAD(ifaddrhead, ifaddr);

typedef enum {
	IFENCAP_LL = 1
} ife_type;

struct if_encap_req {
	u_char		*buf;
	size_t		bufsize;
	ife_type	rtype;
	uint32_t	flags;
	int		family;
	char		*lladdr;
	char		*hdata;
	int		lladdr_off;
};

#define	IFENCAP_FLAG_BROADCAST	0x02
#define LLE_MAX_LINKHDR	24

struct ifnet {
	STAILQ_ENTRY(ifnet)	if_link;
	uint8_t			if_type;
	uint8_t			if_addrlen;
	uint8_t			if_hdrlen;
	uint8_t			if_link_state;
	uint32_t		if_mtu;
	int	(*if_output)(struct ifnet *, struct mbuf *,
		    const struct sockaddr *, struct route *);
	void	(*if_input)(struct ifnet *, struct mbuf *);
	int	(*if_requestencap)(struct ifnet *,
		    struct if_encap_req *);
	if_start_fn_t		if_start;
	if_transmit_fn_t	if_transmit;
	struct ifaddrhead	if_addrhead;
	void			*if_hw_addr;
	const uint8_t		*if_broadcastaddr;
	void			*if_softc;
	void			*if_afdata[AF_MAX];
};

#define	IF_LLADDR(ifp) (ifp)->if_hw_addr

void if_init(void);
struct ifnet * if_alloc(u_char type);
int if_attach(struct ifnet *ifp, uint8_t *hwaddr);
void if_input(struct ifnet *ifp, struct mbuf *m);
int ether_ifattach(struct ifnet *ifp, uint8_t *hwaddr);
int ether_requestencap(struct ifnet *ifp, struct if_encap_req *req);

#endif /* !_NET_IF_H_ */
