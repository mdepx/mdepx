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
#include <net/if_llatbl.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>

#define	IF_ETHER_DEBUG
#undef	IF_ETHER_DEBUG

#ifdef	IF_ETHER_DEBUG
#define	dprintf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define	dprintf(fmt, ...)
#endif

#define SIN(s) ((const struct sockaddr_in *)(s))

int
arpresolve(struct ifnet *ifp, int is_gw, struct mbuf *m,
    const struct sockaddr *dst, u_char *phdr)
{
	const struct sockaddr_in *gw;
	struct llentry *lle, *next;
	struct lltable *llt;

	llt = LLTABLE(ifp);

	/* Table lookup */
	LIST_FOREACH_SAFE(lle, &llt->lle_head, lle_next, next) {
		dprintf("%s: lookup lle: r_hdrlen %d\n",
		    __func__, lle->r_hdrlen);
		gw = (const struct sockaddr_in *)dst;
		if (gw->sin_addr.s_addr == lle->addr4.s_addr) {
			/* Found */
			memcpy(phdr, lle->r_linkdata, lle->r_hdrlen);
			return (MDX_OK);
		}
	}

	/* Not found */
	arprequest(ifp, NULL, &SIN(dst)->sin_addr, NULL);

	return (MDX_ERROR);
}

static void
arp_addentry(struct ifnet *ifp, caddr_t sha, caddr_t spa)
{
	uint8_t linkhdr[LLE_MAX_LINKHDR];
	size_t linkhdrsize;
	struct if_encap_req ereq;
	int error;
	struct lltable *llt;
	struct llentry *lle;

	linkhdrsize = sizeof(linkhdr);
	bzero(linkhdr, linkhdrsize);
	bzero(&ereq, sizeof(ereq));
	ereq.buf = linkhdr;
	ereq.bufsize = linkhdrsize;
	ereq.rtype = IFENCAP_LL;
	ereq.family = AF_INET;
	ereq.lladdr = sha;

	error = ifp->if_requestencap(ifp, &ereq);
	if (error == 0)
		linkhdrsize = ereq.bufsize;

	llt = LLTABLE(ifp);
	lle = malloc(sizeof(struct llentry));
	bcopy(spa, &lle->addr4.s_addr, 4);
	lle->r_hdrlen = linkhdrsize;
	memcpy(lle->r_linkdata, linkhdr, linkhdrsize);
	LIST_INSERT_HEAD(&llt->lle_head, lle, lle_next);
}

static void
arp_reply(struct ifnet *ifp, struct mbuf *m)
{
	struct llentry *lle, *next;
	struct lltable *llt;
	struct arphdr *ah;
	struct in_addr in;
	caddr_t spa, tpa;

	ah = (struct arphdr *)(m->m_data);

	spa = ar_spa(ah);
	tpa = ar_tpa(ah);
	printf("%s: %d.%d.%d.%d -> %d.%d.%d.%d\n", __func__,
	    spa[0], spa[1], spa[2], spa[3],
	    tpa[0], tpa[1], tpa[2], tpa[3]);

	bcopy(tpa, &in.s_addr, 4);
	if (!in_ifhasaddr(ifp, in))
		return;

	llt = LLTABLE(ifp);

	bcopy(spa, &in.s_addr, 4);
	LIST_FOREACH_SAFE(lle, &llt->lle_head, lle_next, next) {
		if (in.s_addr == lle->addr4.s_addr)
			return;
	}

	arp_addentry(ifp, ar_sha(ah), ar_spa(ah));
}

static void
arp_reply_to_request(struct ifnet *ifp, struct mbuf *m)
{
	uint8_t linkhdr[LLE_MAX_LINKHDR];
	size_t linkhdrsize;
	struct arphdr *ah;
	struct arphdr *ah2;
	struct in_addr in;
	struct sockaddr sa;
	struct route ro;
	caddr_t spa, tpa;
	struct mbuf *m1;

	ah = (struct arphdr *)(m->m_data);

	spa = ar_spa(ah);
	tpa = ar_tpa(ah);
	printf("%s: %d.%d.%d.%d -> %d.%d.%d.%d\n", __func__,
	    spa[0], spa[1], spa[2], spa[3],
	    tpa[0], tpa[1], tpa[2], tpa[3]);

	bcopy(tpa, &in.s_addr, 4);
	if (!in_ifhasaddr(ifp, in))
		return;

	m1 = m_alloc(28);
	if (m1 == NULL)
		return;
	ah2 = (struct arphdr *)(m1->m_data);
	ah2->ar_op = htons(ARPOP_REPLY);
	ah2->ar_hrd = htons(ARPHRD_ETHER);
	ah2->ar_pro = htons(ETHERTYPE_IP);
	ah2->ar_hln = ETHER_ADDR_LEN;
	ah2->ar_pln = 4;

	bcopy(ifp->if_hw_addr,    (void *)ar_sha(ah2), ah->ar_hln);
	bcopy((void *)ar_sha(ah), (void *)ar_tha(ah2), ah->ar_hln);
	bcopy((void *)ar_tpa(ah), (void *)ar_spa(ah2), ah->ar_pln);
	bcopy((void *)ar_spa(ah), (void *)ar_tpa(ah2), ah->ar_pln);

	sa.sa_family = AF_ARP;
	sa.sa_len = 2;

	bzero(&ro, sizeof(ro));
	bzero(&linkhdr, sizeof(linkhdr));
	linkhdrsize = sizeof(linkhdr);
	arp_fillheader(ifp, ah2, 0, linkhdr, &linkhdrsize);

	ro.ro_prepend = linkhdr;
	ro.ro_plen = linkhdrsize;

	ifp->if_output(ifp, m1, &sa, &ro);
}

void    
arp_input(struct ifnet *ifp, struct mbuf *m)
{
	struct arphdr *ah;
	short op;

	/* Trim ethernet header */
	m_adj(m, ETHER_HDR_LEN);

	ah = (struct arphdr *)(m->m_data);
	op = ntohs(ah->ar_op);

	if ((ah->ar_hrd != htons(ARPHRD_ETHER)) ||
	    (ah->ar_pro != htons(ETHERTYPE_IP)) ||
	    (ah->ar_hln != ETHER_ADDR_LEN) ||
	    (ah->ar_pln != 4))
		return;

	switch (op) {
	case ARPOP_REQUEST:
		arp_reply_to_request(ifp, m);
		break;
	case ARPOP_REPLY:
		arp_reply(ifp, m);
		break;
	}
}
