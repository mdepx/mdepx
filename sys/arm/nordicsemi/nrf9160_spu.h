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

#ifndef _ARM_NORDICSEMI_NRF9160_SPU_H_
#define _ARM_NORDICSEMI_NRF9160_SPU_H_

#define	SPU_EVENTS_RAMACCERR	0x100	/* A security violation has been detected for the RAM memory space */
#define	SPU_EVENTS_FLASHACCERR	0x104	/* A security violation has been detected for the flash memory space */
#define	SPU_EVENTS_PERIPHACCERR	0x108	/* A security violation has been detected on one or several peripherals */
#define	SPU_PUBLISH_RAMACCERR	0x180	/* Publish configuration for event RAMACCERR */
#define	SPU_PUBLISH_FLASHACCERR	0x184	/* Publish configuration for event FLASHACCERR */
#define	SPU_PUBLISH_PERIPHACCERR 0x188	/* Publish configuration for event PERIPHACCERR */
#define	SPU_INTEN		0x300	/* Enable or disable interrupt */
#define	SPU_INTENSET		0x304	/* Enable interrupt */
#define	SPU_INTENCLR		0x308	/* Disable interrupt */
#define	SPU_CAP			0x400	/* Show implemented features for the current device */
#define	SPU_EXTDOMAIN_PERM(n)	(0x440 + (n) * 0x4)	/* Access for bus access generated from the external domain n. List capabilities of the external domain n */
#define	SPU_DPPI_PERM(n)	(0x480 + (n) * 0x8)	/* Select between secure and non-secure attribute for the DPPI channels. */
#define	SPU_DPPI_LOCK(n)	(0x484 + (n) * 0x8)	/* Prevent further modification of the corresponding PERM register */
#define	SPU_GPIOPORT_PERM(n)	(0x4C0 + (n) * 0x8)	/* Select between secure and non-secure attribute for pins 0 to 31 of port n. Retained. */
#define	SPU_GPIOPORT_LOCK(n)	(0x4C4 + (n) * 0x8)	/* Prevent further modification of the corresponding PERM register */
#define	SPU_FLASHNSC_REGION(n)	(0x500 + (n) * 0x8)	/* Define which flash region can contain the non-secure callable (NSC) region n */
#define	SPU_FLASHNSC_SIZE(n)	(0x504 + (n) * 0x8)	/* Define the size of the non-secure callable (NSC) region n */
#define	SPU_RAMNSC_REGION(n)	(0x540 + (n) * 0x8)	/* Define which RAM region can contain the non-secure callable (NSC) region n */
#define	SPU_RAMNSC_SIZE(n)	(0x544 + (n) * 0x8)	/* Define the size of the non-secure callable (NSC) region n */
#define	SPU_FLASHREGION_PERM(n)	(0x600 + (n) * 0x8)	/* Access permissions for flash region n */
#define	SPU_RAMREGION_PERM(n)	(0x700 + (n) * 0x8)	/* Access permissions for RAM region n */
#define	SPU_PERIPHID_PERM(n)	(0x800 + (n) * 0x8)	/* List capabilities and access permissions for the peripheral with ID n */

struct spu_softc {
	size_t base;
};

#endif /* !_ARM_NORDICSEMI_NRF9160_SPU_H_ */
