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

#ifndef _ARM_NORDICSEMI_NRF9160_SAADC_H_
#define _ARM_NORDICSEMI_NRF9160_SAADC_H_

#define	SAADC_TASKS_START		0x000	/* Start the ADC and prepare the result buffer in RAM */
#define	SAADC_TASKS_SAMPLE		0x004	/* Take one ADC sample, if scan is enabled all channels are sampled */
#define	SAADC_TASKS_STOP		0x008	/* Stop the ADC and terminate any on-going conversion */
#define	SAADC_TASKS_CALIBRATEOFFSET	0x00C	/* Starts offset auto-calibration */
#define	SAADC_SUBSCRIBE_START		0x080	/* Subscribe configuration for task START */
#define	SAADC_SUBSCRIBE_SAMPLE		0x084	/* Subscribe configuration for task SAMPLE */
#define	SAADC_SUBSCRIBE_STOP		0x088	/* Subscribe configuration for task STOP */
#define	SAADC_SUBSCRIBE_CALIBRATEOFFSET	0x08C	/* Subscribe configuration for task CALIBRATEOFFSET */
#define	SAADC_EVENTS_STARTED		0x100	/* The ADC has started */
#define	SAADC_EVENTS_END		0x104	/* The ADC has filled up the Result buffer */
#define	SAADC_EVENTS_DONE		0x108	/* A conversion task has been completed. Depending on the mode, multiple conversions might be needed for a result to be transferred to RAM. */
#define	SAADC_EVENTS_RESULTDONE		0x10C	/* A result is ready to get transferred to RAM. */
#define	SAADC_EVENTS_CALIBRATEDONE	0x110	/* Calibration is complete */
#define	SAADC_EVENTS_STOPPED		0x114	/* The ADC has stopped */
#define	SAADC_EVENTS_CH_LIMITH(n)	(0x118 + (n) * 0x8)	/* Last results is equal or above CH[n].LIMIT.HIGH */
#define	SAADC_EVENTS_CH_LIMITL(n)	(0x11C + (n) * 0x8)	/* Last results is equal or below CH[n].LIMIT.LOW */
#define	SAADC_PUBLISH_STARTED		0x180	/* Publish configuration for event STARTED */
#define	SAADC_PUBLISH_END		0x184	/* Publish configuration for event END */
#define	SAADC_PUBLISH_DONE		0x188	/* Publish configuration for event DONE */
#define	SAADC_PUBLISH_RESULTDONE	0x18C	/* Publish configuration for event RESULTDONE */
#define	SAADC_PUBLISH_CALIBRATEDONE	0x190	/* Publish configuration for event CALIBRATEDONE */
#define	SAADC_PUBLISH_STOPPED		0x194	/* Publish configuration for event STOPPED */
#define	SAADC_PUBLISH_CH_LIMITH(n)	(0x198 + (n) * 0x8)	/* Publish configuration for event CH[n].LIMITH */
#define	SAADC_PUBLISH_CH_LIMITL(n)	(0x19C + (n) * 0x8)	/* Publish configuration for event CH[n].LIMITL */
#define	SAADC_INTEN			0x300	/* Enable or disable interrupt */
#define	SAADC_INTENSET			0x304	/* Enable interrupt */
#define	SAADC_INTENCLR			0x308	/* Disable interrupt */
#define	SAADC_STATUS			0x400	/* Status */
#define	SAADC_ENABLE			0x500	/* Enable or disable ADC */
#define	SAADC_CH_PSELP(n)		(0x510 + (n) * 0x10)	/* Input positive pin selection for CH[n] */
#define	SAADC_CH_PSELN(n)		(0x514 + (n) * 0x10)	/* Input negative pin selection for CH[n] */
#define	SAADC_CH_CONFIG(n)		(0x518 + (n) * 0x10)	/* Input configuration for CH[n] */
#define	SAADC_CH_LIMIT(n)		(0x51C + (n) * 0x10)	/* High/low limits for event monitoring a channel */
#define	SAADC_RESOLUTION		0x5F0	/* Resolution configuration */
#define	SAADC_OVERSAMPLE		0x5F4	/* Oversampling configuration. OVERSAMPLE should not be combined with SCAN. The RESOLUTION is applied before averaging, thus for high OVERSAMPLE a higher RESOLUTION should be used. */
#define	SAADC_SAMPLERATE		0x5F8	/* Controls normal or continuous sample rate */
#define	SAADC_RESULT_PTR		0x62C	/* Data pointer */
#define	SAADC_RESULT_MAXCNT		0x630	/* Maximum number of buffer words to transfer */
#define	SAADC_RESULT_AMOUNT		0x634	/* Number of buffer words transferred since last START */

struct nrf_saadc_softc {
	size_t base;
};

#endif /* !_ARM_NORDICSEMI_NRF9160_SAADC_H_ */
