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
#include <sys/mbuf.h>
#include <net/if.h>
#include <net/if_llatbl.h>
#include <netinet/in.h>

STAILQ_HEAD(, ifnet) g_ifnet;

void
if_init(void)
{

	STAILQ_INIT(&g_ifnet);
}

struct ifnet *
if_alloc(u_char type)
{
	struct lltable *llt;
	struct ifnet *ifp;

	ifp = malloc(sizeof(struct ifnet));
	ifp->if_type = type;
	STAILQ_INIT(&ifp->if_addrhead);

	/* Initialize linked list table */
	ifp->if_afdata[AF_INET] = malloc(sizeof(struct in_ifinfo));
	llt = LLTABLE(ifp) = malloc(sizeof(struct lltable));
	LIST_INIT(&llt->lle_head);

	return (ifp);
}

int
if_attach(struct ifnet *ifp, uint8_t *hwaddr)
{
	int ret;

	ret = -1;

	switch (ifp->if_type) {
	case IFT_ETHER:
		ret = ether_ifattach(ifp, hwaddr);
	default:
		break;
	}

	if (ret == 0)
		STAILQ_INSERT_TAIL(&g_ifnet, ifp, if_link);

	return (ret);
}

void
if_input(struct ifnet *ifp, struct mbuf *m)
{

	ifp->if_input(ifp, m);
}
