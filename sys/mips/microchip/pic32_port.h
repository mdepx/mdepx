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

#ifndef _MIPS_MICROCHIP_PIC32_PORT_H_
#define	_MIPS_MICROCHIP_PIC32_PORT_H_

#include <sys/types.h>

enum port_state {
	PORT_INPUT,
	PORT_OUTPUT,
};

#define	PORT_A	0
#define	PORT_B	1
#define	PORT_C	2
#define	PORT_D	3
#define	PORT_E	4
#define	PORT_F	5
#define	PORT_G	6
#define	PORT_H	7
#define	PORT_J	8
#define	PORT_K	9

/* Pin configuration */
#define	PORT_ANSEL(n)		(0x00 + 0x100 * (n))
#define	PORT_TRIS(n)		(0x10 + 0x100 * (n))
#define	PORT_PORT(n)		(0x20 + 0x100 * (n))
#define	PORT_LAT(n)		(0x30 + 0x100 * (n))
#define	PORT_ODC(n)		(0x40 + 0x100 * (n))
#define	PORT_CNPU(n)		(0x50 + 0x100 * (n))
#define	PORT_CNPD(n)		(0x60 + 0x100 * (n))
#define	PORT_CNCON(n)		(0x70 + 0x100 * (n))
#define	PORT_CNEN0(n)		(0x80 + 0x100 * (n))
#define	PORT_CNSTAT(n)		(0x90 + 0x100 * (n))
#define	PORT_CNEN1(n)		(0xA0 + 0x100 * (n))
#define	PORT_CNF(n)		(0xB0 + 0x100 * (n))

/* Output pin selection */
#define	RPOR_NC		0	/* None (not connected) */
#define	RPOR_U2TX	1	/* UART2 Transmit */
#define	RPOR_U2RTS	2	/* UART2 Request-to-Send */
#define	RPOR_SDO2	3	/* SPI2 Data Output */
#define	RPOR_SCK2OUT	4	/* SPI2 Clock Output */
#define	RPOR_SS2OUT	5	/* SPI2 Slave Select Output */
#define	RPOR_OCM2	6	/* SCCP2 Output Compare */
#define	RPOR_OCM3	7	/* SCCP3 Output Compare */
#define	RPOR_CLC1OUT	8	/* CLC1 Output */
#define	RPOR_CLC2OUT	9	/* CLC2 Output */

struct pic32_port_softc {
	uint32_t base;
};

void pic32_port_init(struct pic32_port_softc *sc, uint32_t base);
void pic32_port_ansel(struct pic32_port_softc *sc, uint32_t port, uint32_t pin, int digital);
void pic32_port_tris(struct pic32_port_softc *sc, uint32_t port, uint32_t pin, enum port_state st);
void pic32_port_lat(struct pic32_port_softc *sc, uint32_t port, uint32_t pin, uint8_t enable);
int pic32_port_port(struct pic32_port_softc *sc, uint32_t port, uint32_t pin);
void pic32_port_cnpu(struct pic32_port_softc *sc, uint32_t port, uint32_t pin, uint8_t enable);

#endif /* !_MIPS_MICROCHIP_PIC32_PORT_H_ */
