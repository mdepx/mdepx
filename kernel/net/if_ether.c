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

#include <sys/cdefs.h>
#include <sys/errno.h>
#include <sys/param.h>
#include <sys/mbuf.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet6/ip6_var.h>

static const u_char etherbroadcastaddr[ETHER_ADDR_LEN] =
    { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

static void
ether_input(struct ifnet *ifp, struct mbuf *m)
{
	struct ether_header *eh;
	int type;

	eh = mtod(m, struct ether_header *);
	type = ntohs(eh->ether_type);

	switch(type) {
	case ETHERTYPE_ARP:
		arp_input(ifp, m);
		m_free(m);
		break;
	case ETHERTYPE_IP:
		ip_input(ifp, m);
		break;
	case ETHERTYPE_IPV6:
		ip6_input(ifp, m);
		break;
	default:
		m_free(m);
		break;
	}
}

static int
ether_resolve_addr(struct ifnet *ifp, struct mbuf *m,
    const struct sockaddr *dst, struct route *ro, u_char *phdr)
{
	int error;

	error = -1;

	switch (dst->sa_family) {
	case AF_INET:
		error = arpresolve(ifp, 0, m, dst, phdr);
	default:
		break;
	};

	return (error);
}

static int
ether_output(struct ifnet *ifp, struct mbuf *m,
    const struct sockaddr *dst, struct route *ro)
{
	struct ether_header *eh;
	struct mbuf *m0;
	void *phdr;
	char linkhdr[ETHER_HDR_LEN];
	int error;
	int avail;

	phdr = NULL;
	if (ro != NULL)
		phdr = ro->ro_prepend;
	if (phdr == NULL) {
		phdr = linkhdr;
		error = ether_resolve_addr(ifp, m, dst, ro, phdr);
		if (error != 0)
			return (error == EWOULDBLOCK ? 0 : error);
	}

	avail = m->m_data - m->m_data0;
	if (avail >= ETHER_HDR_LEN) {
		m_adj(m, -ETHER_HDR_LEN);
		m0 = m;
		m0->m_next = NULL;
	} else {
		m0 = m_alloc(ETHER_HDR_LEN);
		m0->m_next = m;
	}

	eh = (struct ether_header *)m0->m_data;
	memcpy(eh, phdr, ETHER_HDR_LEN);

	return ((ifp->if_transmit)(ifp, m0));
}

int
ether_ifattach(struct ifnet *ifp, uint8_t *hwaddr)
{

	ifp->if_addrlen = ETHER_ADDR_LEN;
	ifp->if_hdrlen = ETHER_HDR_LEN;
	ifp->if_mtu = ETHERMTU;
	ifp->if_output = ether_output;
	ifp->if_input = ether_input;
	ifp->if_requestencap = ether_requestencap;
	ifp->if_hw_addr = malloc(ifp->if_addrlen);
	bcopy(hwaddr, ifp->if_hw_addr, ifp->if_addrlen);
	ifp->if_broadcastaddr = etherbroadcastaddr;

	return (MDX_OK);
}
