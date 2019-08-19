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
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>

STAILQ_HEAD(, ifnet) g_ifnet;

int
in_ifhasaddr(struct ifnet *ifp, struct in_addr in)  
{
	struct ifaddr *ifa;
	struct in_ifaddr *ia;

	STAILQ_FOREACH(ifa, &ifp->if_addrhead, ifa_link) {
		ia = (struct in_ifaddr *)ifa;
		printf("%x %x\n", ia->ia_addr.sin_addr.s_addr, in.s_addr);
		if (ia->ia_addr.sin_addr.s_addr == in.s_addr)
			return (1);
	}

	return (0);
}

/* Add interface address */
int
in_aifaddr(struct ifnet *ifp, struct in_addr in, u_long mask)
{
	struct ifaddr *ifa;
	struct in_ifaddr *ia;

	if (in_ifhasaddr(ifp, in))
		return (-1);

	ifa = malloc(sizeof(struct in_ifaddr));
	ifa->ifa_addr = malloc(sizeof(struct sockaddr));
	ifa->ifa_addr->sa_family = AF_INET;
	ia = (struct in_ifaddr *)ifa;
	ia->ia_addr.sin_addr.s_addr = in.s_addr;
	ia->ia_addr.sin_family = AF_INET;
	ia->ia_subnetmask = mask;
	ia->ia_sockmask.sin_addr.s_addr = mask;
	STAILQ_INSERT_TAIL(&ifp->if_addrhead, ifa, ifa_link);

	return (0);
}
