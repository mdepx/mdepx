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

#ifndef _ARM_NORDICSEMI_NRF9160_PDM_H_
#define _ARM_NORDICSEMI_NRF9160_PDM_H_

#define	PDM_TASKS_START		0x000	/* Starts continuous PDM transfer */
#define	PDM_TASKS_STOP		0x004	/* Stops PDM transfer */
#define	PDM_SUBSCRIBE_START	0x080	/* Subscribe configuration for task START */
#define	PDM_SUBSCRIBE_STOP	0x084	/* Subscribe configuration for task STOP */
#define	PDM_EVENTS_STARTED	0x100	/* PDM transfer has started */
#define	PDM_EVENTS_STOPPED	0x104	/* PDM transfer has finished */
#define	PDM_EVENTS_END		0x108	/* The PDM has written the last sample specified by SAMPLE.MAXCNT (or the last sample after a STOP task has been received) to Data RAM */
#define	PDM_PUBLISH_STARTED	0x180	/* Publish configuration for event STARTED */
#define	PDM_PUBLISH_STOPPED	0x184	/* Publish configuration for event STOPPED */
#define	PDM_PUBLISH_END		0x188	/* Publish configuration for event END */
#define	PDM_INTEN		0x300	/* Enable or disable interrupt */
#define	PDM_INTENSET		0x304	/* Enable interrupt */
#define	PDM_INTENCLR		0x308	/* Disable interrupt */
#define	PDM_ENABLE		0x500	/* PDM module enable register */
#define	PDM_PDMCLKCTRL		0x504	/* PDM clock generator control */
#define	PDM_MODE		0x508	/* Defines the routing of the connected PDM microphones' signals */
#define	PDM_GAINL		0x518	/* Left output gain adjustment */
#define	PDM_GAINR		0x51C	/* Right output gain adjustment */
#define	PDM_RATIO		0x520	/* Selects the ratio between PDM_CLK and output sample rate. Change PDMCLKCTRL accordingly. */
#define	PDM_PSEL_CLK		0x540	/* Pin number configuration for PDM CLK signal */
#define	PDM_PSEL_DIN		0x544	/* Pin number configuration for PDM DIN signal */
#define	PDM_SAMPLE_PTR		0x560	/* RAM address pointer to write samples to with EasyDMA */
#define	PDM_SAMPLE_MAXCNT	0x564	/* Number of samples to allocate memory for in EasyDMA mode */

struct nrf_pdm_softc {
	size_t base;
};

#endif /* !_ARM_NORDICSEMI_NRF9160_PDM_H_ */
