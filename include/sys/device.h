/*-
 * Copyright (c) 2019 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _SYS_DEVICE_H_
#define _SYS_DEVICE_H_

#include <sys/list.h>

struct mdx_device {
	void			*sc;		/* Software context. */
	void			*ops;		/* Methods. */
	struct entry		node;		/* Entry in the devs list. */
	int			nodeoffset;	/* libfdt node offset. */
	struct mdx_sysinit	*si;
	int			unit;
};

typedef struct mdx_device *mdx_device_t;

struct mdx_device_ops {
	int (*probe)(mdx_device_t dev);
	int (*attach)(mdx_device_t dev);
};

struct mdx_driver {
	const char		*name;
	struct mdx_device_ops	*ops;
	size_t			size;
};

typedef struct mdx_driver mdx_driver_t;

struct mdx_sysinit {
	mdx_driver_t		*dri;
};

#define DRIVER_MODULE(name, driver)		\
	struct mdx_sysinit name##_sysinit	\
	__attribute__((__section__(".sysinit"),	\
	__used__, __aligned__(4))) = {		\
		.dri = &driver,			\
	};

#define	mdx_device_get_softc(dev)	((dev)->sc)

void * mdx_device_alloc_softc(mdx_device_t dev, size_t size);
int mdx_device_probe_and_attach(mdx_device_t dev);
mdx_device_t mdx_device_lookup_by_name(const char *name, int unit);
mdx_device_t mdx_device_lookup_by_offset(int offset);

#endif /* !_SYS_DEVICE_H_ */
