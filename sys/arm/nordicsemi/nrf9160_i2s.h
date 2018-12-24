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

#ifndef _ARM_NORDICSEMI_NRF9160_I2S_H_
#define _ARM_NORDICSEMI_NRF9160_I2S_H_

#define	I2S_TASKS_START		0x000	/* Starts continuous I2S transfer. Also starts MCK generator when this is enabled. */
#define	I2S_TASKS_STOP		0x004	/* Stops I2S transfer. Also stops MCK generator. Triggering this task will cause the STOPPED event to be generated. */
#define	I2S_SUBSCRIBE_START	0x080	/* Subscribe configuration for task START */
#define	I2S_SUBSCRIBE_STOP	0x084	/* Subscribe configuration for task STOP */
#define	I2S_EVENTS_RXPTRUPD	0x104	/* The RXD.PTR register has been copied to internal double-buffers. When the I2S module is started and RX is enabled, this event will be generated for every RXTXD.MAXCNT words that are received on the SDIN pin. */
#define	I2S_EVENTS_STOPPED	0x108	/* I2S transfer stopped. */
#define	I2S_EVENTS_TXPTRUPD	0x114	/* The TDX.PTR register has been copied to internal double-buffers. When the I2S module is started and TX is enabled, this event will be generated for every RXTXD.MAXCNT words that are sent on the SDOUT pin. */
#define	I2S_PUBLISH_RXPTRUPD	0x184	/* Publish configuration for event RXPTRUPD */
#define	I2S_PUBLISH_STOPPED	0x188	/* Publish configuration for event STOPPED */
#define	I2S_PUBLISH_TXPTRUPD	0x194	/* Publish configuration for event TXPTRUPD */
#define	I2S_INTEN		0x300	/* Enable or disable interrupt */
#define	I2S_INTENSET		0x304	/* Enable interrupt */
#define	I2S_INTENCLR		0x308	/* Disable interrupt */
#define	I2S_ENABLE		0x500	/* Enable I2S module. */
#define	I2S_CONFIG_MODE		0x504	/* I2S mode. */
#define	I2S_CONFIG_RXEN		0x508	/* Reception (RX) enable. */
#define	I2S_CONFIG_TXEN		0x50C	/* Transmission (TX) enable. */
#define	I2S_CONFIG_MCKEN	0x510	/* Master clock generator enable. */
#define	I2S_CONFIG_MCKFREQ	0x514	/* Master clock generator frequency. */
#define	I2S_CONFIG_RATIO	0x518	/* MCK / LRCK ratio. */
#define	I2S_CONFIG_SWIDTH	0x51C	/* Sample width. */
#define	I2S_CONFIG_ALIGN	0x520	/* Alignment of sample within a frame. */
#define	I2S_CONFIG_FORMAT	0x524	/* Frame format. */
#define	I2S_CONFIG_CHANNELS	0x528	/* Enable channels. */
#define	I2S_RXD_PTR		0x538	/* Receive buffer RAM start address. */
#define	I2S_TXD_PTR		0x540	/* Transmit buffer RAM start address. */
#define	I2S_RXTXD_MAXCNT	0x550	/* Size of RXD and TXD buffers. */
#define	I2S_PSEL_MCK		0x560	/* Pin select for MCK signal. */
#define	I2S_PSEL_SCK		0x564	/* Pin select for SCK signal. */
#define	I2S_PSEL_LRCK		0x568	/* Pin select for LRCK signal. */
#define	I2S_PSEL_SDIN		0x56C	/* Pin select for SDIN signal. */
#define	I2S_PSEL_SDOUT		0x570	/* Pin select for SDOUT signal. */

struct i2s_softc {
	size_t base;
};

#endif /* !_ARM_NORDICSEMI_NRF9160_I2S_H_ */
