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

#ifndef	_NETINET_IN_H_
#define	_NETINET_IN_H_

#include <sys/socket.h>
#include <net/route.h>
#include <net/if_types.h>

#define	IPPROTO_IP	0
#define	IPPROTO_ICMP	1
#define	IPPROTO_TCP	6
#define	IPPROTO_UDP	17

struct in_addr {
	in_addr_t s_addr;
};

/* Socket address, internet style. */
struct sockaddr_in {
	uint8_t		sin_len;
	sa_family_t	sin_family;
	in_port_t	sin_port;
	struct in_addr	sin_addr;
	char		sin_zero[8];
};

struct in_ifaddr {
	struct ifaddr ia_ifa;
	u_long ia_subnet;
	u_long ia_subnetmask;
	struct sockaddr_in ia_sockmask;
	struct sockaddr_in ia_addr;
};

struct lltable;

/*
 * IPv4 per-interface state.
 */
struct in_ifinfo {
	struct lltable		*ii_llt;	/* ARP state */
};

#define	LLTABLE(ifp)	\
	((struct in_ifinfo *)(ifp)->if_afdata[AF_INET])->ii_llt
#define	IA_SIN(ia)	(&(((struct in_ifaddr *)(ia))->ia_addr))
#define	IA_MASKSIN(ia)	(&(((struct in_ifaddr *)(ia))->ia_sockmask))

void ip_input(struct ifnet *ifp, struct mbuf *m);
void ip_output(struct ifnet *ifp, struct mbuf *m, struct route *ro);

int in_aifaddr(struct ifnet *ifp, struct in_addr in, u_long mask);
int in_ifhasaddr(struct ifnet *ifp, struct in_addr in);

#endif /* !_NETINET_IN_H_ */
