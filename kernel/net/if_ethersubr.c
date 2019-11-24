/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 1982, 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)if_ethersubr.c	8.1 (Berkeley) 6/10/93
 * $FreeBSD$
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

/*
 * Handle link-layer encapsulation requests.
 */
int
ether_requestencap(struct ifnet *ifp, struct if_encap_req *req)
{
	struct ether_header *eh;
	struct arphdr *ah;
	uint16_t etype;
	const u_char *lladdr;

	if (req->rtype != IFENCAP_LL)
		return (EOPNOTSUPP);

	if (req->bufsize < ETHER_HDR_LEN)
		return (ENOMEM);

	eh = (struct ether_header *)req->buf;
	lladdr = req->lladdr;
	req->lladdr_off = 0;

	switch (req->family) {
	case AF_INET:
		etype = htons(ETHERTYPE_IP);
		break;
	case AF_INET6:
		etype = htons(ETHERTYPE_IPV6);
		break;
	case AF_ARP:
		ah = (struct arphdr *)req->hdata;
		ah->ar_hrd = htons(ARPHRD_ETHER);

		switch(ntohs(ah->ar_op)) {
		case ARPOP_REVREQUEST:
		case ARPOP_REVREPLY:
			etype = htons(ETHERTYPE_REVARP);
			break;
		case ARPOP_REQUEST:
		case ARPOP_REPLY:
		default:
			etype = htons(ETHERTYPE_ARP);
			break;
		}

		if (req->flags & IFENCAP_FLAG_BROADCAST)
			lladdr = ifp->if_broadcastaddr;
		break;
	default:
		return (EAFNOSUPPORT);
	}

	memcpy(&eh->ether_type, &etype, sizeof(eh->ether_type));
	memcpy(eh->ether_dhost, lladdr, ETHER_ADDR_LEN);
	memcpy(eh->ether_shost, IF_LLADDR(ifp), ETHER_ADDR_LEN);
	req->bufsize = sizeof(struct ether_header);

	return (MDX_OK);
}
