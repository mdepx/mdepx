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
#include <sys/endian.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

static void
echo_reply(struct ifnet *ifp, struct mbuf *m,
    int hlen, int icmplen)
{
	struct icmphdr *hdr;
	struct in_addr t;
	struct ip *ip;

	ip = mtod(m, struct ip *);
	t = ip->ip_dst;
	ip->ip_dst = ip->ip_src;
	ip->ip_src = t;

	m->m_len -= hlen;
	m->m_data += hlen;

	hdr = mtod(m, struct icmphdr *);
	hdr->icmp_type = ICMP_ECHOREPLY;
	hdr->icmp_cksum = 0;
	hdr->icmp_cksum = in_cksum(m, icmplen);

	m->m_len += hlen;
	m->m_data -= hlen;

	ip_output(ifp, m, NULL);
}

void
icmp_input(struct ifnet *ifp, int hlen, struct mbuf *m)
{
	struct icmphdr *hdr;
	struct ip *ip;
	int iplen;
	int iphlen;
	int icmplen;

	ip = mtod(m, struct ip *);
	m->m_len -= hlen;
	m->m_data += hlen;
	hdr = mtod(m, struct icmphdr *);

	iplen = ntohs(ip->ip_len);
	iphlen = (ip->ip_hl << 2);
	icmplen = (iplen - iphlen);
	if (in_cksum(m, icmplen)) {
		m_free(m);
		return;
	}

	m->m_len += hlen;
	m->m_data -= hlen;

	switch (hdr->icmp_type) {
	case ICMP_ECHO:
		echo_reply(ifp, m, hlen, icmplen);
		break;
	default:
		m_free(m);
		break;
	}
}
