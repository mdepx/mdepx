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

#include <sys/cdefs.h>
#include <sys/systm.h>

#include "lpc43xx_usart.h"

#define	RD4(_sc, _reg)		\
	*(volatile uint32_t *)((_sc)->base + _reg)
#define	WR4(_sc, _reg, _val)	\
	*(volatile uint32_t *)((_sc)->base + _reg) = _val

void
lpc43xx_usart_putc(struct usart_softc *sc, char ch)
{

	while ((RD4(sc, USART_LSR) & LSR_TEMT) == 0)
		;

	WR4(sc, USART_THR, (uint8_t)(ch));
}

/*
 * The USART baud rate can be calculated as:
 * baud / pclk = 16 * (256 * dlm + dll) * (1 + DivAddVal / MulVal)
 */

void
lpc43xx_usart_set_baudrate(struct usart_softc *sc,
    struct usart_baud_entry *baud)
{
	uint32_t reg;

	/* Line control enable */
	reg = RD4(sc, USART_LCR);
	reg |= LCR_DLAB;
	WR4(sc, USART_LCR, reg);

	WR4(sc, USART_DLM, baud->dlm);
	WR4(sc, USART_DLL, baud->dll);

	/* Line control disable */
	reg = RD4(sc, USART_LCR);
	reg &= ~LCR_DLAB;
	reg |= LCR_WLS_8;
	WR4(sc, USART_LCR, reg);

	/* Fractional */
	reg = (baud->divaddval << FDR_DIVADDVAL_S);
	reg |= (baud->mulval << FDR_MULVAL_S);
	WR4(sc, USART_FDR, reg);
}

void
lpc43xx_usart_init(struct usart_softc *sc, uint32_t base,
    struct usart_baud_entry *baud)
{

	sc->base = base;

	lpc43xx_usart_set_baudrate(sc, baud);

	/* FIFO Control */
	WR4(sc, USART_FCR, FCR_FIFOEN);
}
