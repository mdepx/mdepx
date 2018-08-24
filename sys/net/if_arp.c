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
#include <sys/param.h>
#include <sys/mbuf.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <net/ethernet.h>

#define	ARP_TABLE_SIZE	1024

struct arp_table_entry {
	char	hwaddr[ETHER_ADDR_LEN];
	uint8_t	ipaddr[4];
};

struct arp_table_entry arp_table[ARP_TABLE_SIZE];

static void
arp_request(struct ifnet *ifp, struct mbuf *m)
{
	struct arphdr *ah;

	ah = (struct arphdr *)(m->m_data);

	caddr_t spa, tpa;
	spa = ar_spa(ah);
	tpa = ar_tpa(ah);
	printf("%s: %d.%d.%d.%d -> %d.%d.%d.%d\n", __func__,
	    spa[0], spa[1], spa[2], spa[3],
	    tpa[0], tpa[1], tpa[2], tpa[3]);
}

void    
arp_input(struct ifnet *ifp, struct mbuf *m)
{
	struct arphdr *ah;
	short op;

	ah = (struct arphdr *)(m->m_data);
	op = ntohs(ah->ar_op);

	if ((ah->ar_hrd != htons(ARPHRD_ETHER)) ||
	    (ah->ar_pro != htons(ETHERTYPE_IP)) ||
	    (ah->ar_hln != ETHER_ADDR_LEN) ||
	    (ah->ar_pln != 4))
		return;

	switch (op) {
	case ARPOP_REQUEST:
		arp_request(ifp, m);
		break;
	case ARPOP_REPLY:
		printf("arp reply\n");
		break;
	}
}
