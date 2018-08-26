/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 1982, 1986, 1988, 1993
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
 *	@(#)if_ether.c	8.1 (Berkeley) 6/10/93
 */

#include <sys/cdefs.h>
#include <sys/errno.h>
#include <sys/param.h>
#include <sys/mbuf.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <net/if_llatbl.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>

#define	IF_ADDR_RLOCK(ifp)
#define	IF_ADDR_RUNLOCK(ifp)

/*
 * Stores link-layer header for @ifp in format suitable for if_output()
 * into buffer @buf. Resulting header length is stored in @bufsize.
 *
 * Returns 0 on success.
 */
int
arp_fillheader(struct ifnet *ifp, struct arphdr *ah, int bcast, u_char *buf,
    size_t *bufsize)
{
	struct if_encap_req ereq;
	int error;

	bzero(buf, *bufsize);
	bzero(&ereq, sizeof(ereq));
	ereq.buf = buf;
	ereq.bufsize = *bufsize;
	ereq.rtype = IFENCAP_LL;
	ereq.family = AF_ARP;
	ereq.lladdr = ar_tha(ah);
	ereq.hdata = (u_char *)ah;
	if (bcast)
		ereq.flags = IFENCAP_FLAG_BROADCAST;
	error = ifp->if_requestencap(ifp, &ereq);
	if (error == 0)
		*bufsize = ereq.bufsize;

	return (error);
}

/*
 * Broadcast an ARP request. Caller specifies:
 *	- arp header source ip address
 *	- arp header target ip address
 *	- arp header source ethernet address
 */
void
arprequest(struct ifnet *ifp, const struct in_addr *sip,
    const struct in_addr *tip, u_char *enaddr)
{
	struct mbuf *m;
	struct arphdr *ah;
	struct sockaddr sa;
#ifdef __FreeBSD__
	u_char *carpaddr = NULL;
#endif
	uint8_t linkhdr[LLE_MAX_LINKHDR];
	size_t linkhdrsize;
	struct route ro;
	int error;

	if (sip == NULL) {
		/*
		 * The caller did not supply a source address, try to find
		 * a compatible one among those assigned to this interface.
		 */
		struct ifaddr *ifa;

		IF_ADDR_RLOCK(ifp);
		STAILQ_FOREACH(ifa, &ifp->if_addrhead, ifa_link) {
			if (ifa->ifa_addr->sa_family != AF_INET)
				continue;

#ifdef __FreeBSD__
			if (ifa->ifa_carp) {
				if ((*carp_iamatch_p)(ifa, &carpaddr) == 0)
					continue;
				sip = &IA_SIN(ifa)->sin_addr;
			} else {
				carpaddr = NULL;
				sip = &IA_SIN(ifa)->sin_addr;
			}
#else
			sip = &IA_SIN(ifa)->sin_addr;
#endif

			if (0 == ((sip->s_addr ^ tip->s_addr) &
			    IA_MASKSIN(ifa)->sin_addr.s_addr))
				break;  /* found it. */
		}
		IF_ADDR_RUNLOCK(ifp);
		if (sip == NULL) {
			printf("%s: cannot find matching address\n", __func__);
			return;
		}
	}
	if (enaddr == NULL)
		enaddr = (u_char *)IF_LLADDR(ifp);

	uint32_t len;
	len = sizeof(*ah) + 2 * sizeof(struct in_addr) +
	    2 * ifp->if_addrlen;
	if ((m = m_alloc(len)) == NULL)
		return;
	m->m_len = sizeof(*ah) + 2 * sizeof(struct in_addr) +
		2 * ifp->if_addrlen;
#ifdef __FreeBSD__
	m->m_pkthdr.len = m->m_len;
	M_ALIGN(m, m->m_len);
#endif
	ah = mtod(m, struct arphdr *);
	bzero((caddr_t)ah, m->m_len);
#ifdef MAC
	mac_netinet_arp_send(ifp, m);
#endif
	ah->ar_pro = htons(ETHERTYPE_IP);
	ah->ar_hln = ifp->if_addrlen;		/* hardware address length */
	ah->ar_pln = sizeof(struct in_addr);	/* protocol address length */
	ah->ar_op = htons(ARPOP_REQUEST);
	bcopy(enaddr, ar_sha(ah), ah->ar_hln);
	bcopy(sip, ar_spa(ah), ah->ar_pln);
	bcopy(tip, ar_tpa(ah), ah->ar_pln);
	sa.sa_family = AF_ARP;
	sa.sa_len = 2;

	/* Calculate link header for sending frame */
	bzero(&ro, sizeof(ro));
	linkhdrsize = sizeof(linkhdr);
	error = arp_fillheader(ifp, ah, 1, linkhdr, &linkhdrsize);
	if (error != 0 && error != EAFNOSUPPORT) {
#ifdef __FreeBSD__
		ARP_LOG(LOG_ERR, "Failed to calculate ARP header on %s: %d\n",
		    if_name(ifp), error);
#endif
		return;
	}

	ro.ro_prepend = linkhdr;
	ro.ro_plen = linkhdrsize;
	ro.ro_flags = 0;

	m->m_flags |= M_BCAST;
#ifdef __FreeBSD__
	m_clrprotoflags(m);	/* Avoid confusing lower layers. */
#endif
	(*ifp->if_output)(ifp, m, &sa, &ro);
#ifdef __FreeBSD__
	ARPSTAT_INC(txrequests);
#endif
}
