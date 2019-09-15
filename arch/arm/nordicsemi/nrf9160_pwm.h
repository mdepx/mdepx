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

#ifndef _ARM_NORDICSEMI_NRF9160_PWM_H_
#define _ARM_NORDICSEMI_NRF9160_PWM_H_

#define	PWM_TASKS_STOP			0x004	/* Stops PWM pulse generation on all channels at the end of current PWM period, and stops sequence playback */
#define	PWM_TASKS_SEQSTART(n)		(0x008 + (n) * 0x4)	/* Loads the first PWM value on all enabled channels from sequence n, and starts playing that sequence at the rate defined in SEQ[n]REFRESH and/or DECODER.MODE. Causes PWM generation to start if not running. n == 0,1 */
#define	PWM_TASKS_NEXTSTEP		0x010	/* Steps by one value in the current sequence on all enabled channels if DECODER.MODE=NextStep. Does not cause PWM generation to start if not running. */
#define	PWM_SUBSCRIBE_STOP		0x084	/* Subscribe configuration for task STOP */
#define	PWM_SUBSCRIBE_SEQSTART(n)	(0x088 + (n) * 0x4)	/* Subscribe configuration for task SEQSTART[n] */
#define	PWM_SUBSCRIBE_NEXTSTEP		0x090	/* Subscribe configuration for task NEXTSTEP */
#define	PWM_EVENTS_STOPPED		0x104	/* Response to STOP task, emitted when PWM pulses are no longer generated */
#define	PWM_EVENTS_SEQSTARTED(n)	(0x108 + (n) * 0x4)	/* First PWM period started on sequence n */
#define	PWM_EVENTS_SEQEND(n)		(0x110 + (n) * 0x4)	/* Emitted at end of every sequence n, when last value from RAM has been applied to wave counter */
#define	PWM_EVENTS_PWMPERIODEND		0x118	/* Emitted at the end of each PWM period */
#define	PWM_EVENTS_LOOPSDONE		0x11C	/* Concatenated sequences have been played the amount of times defined in LOOP.CNT */
#define	PWM_PUBLISH_STOPPED		0x184	/* Publish configuration for event STOPPED */
#define	PWM_PUBLISH_SEQSTARTED(n)	(0x188 + (n) * 0x4)	/* Publish configuration for event SEQSTARTED[n] */
#define	PWM_PUBLISH_SEQEND(n)		(0x190 + (n) * 0x4)	/* Publish configuration for event SEQEND[n] */
#define	PWM_PUBLISH_PWMPERIODEND	0x198	/* Publish configuration for event PWMPERIODEND */
#define	PWM_PUBLISH_LOOPSDONE		0x19C	/* Publish configuration for event LOOPSDONE */
#define	PWM_SHORTS			0x200	/* Shortcuts between local events and tasks */
#define	PWM_INTEN			0x300	/* Enable or disable interrupt */
#define	PWM_INTENSET			0x304	/* Enable interrupt */
#define	PWM_INTENCLR			0x308	/* Disable interrupt */
#define	PWM_ENABLE			0x500	/* PWM module enable register */
#define	PWM_MODE			0x504	/* Selects operating mode of the wave counter */
#define	PWM_COUNTERTOP			0x508	/* Value up to which the pulse generator counter counts */
#define	PWM_PRESCALER			0x50C	/* Configuration for PWM_CLK */
#define	PWM_DECODER			0x510	/* Configuration of the decoder */
#define	PWM_LOOP			0x514	/* Number of playbacks of a loop */
#define	PWM_SEQ_PTR(n)			(0x520 + (n) * 0x20)	/* Beginning address in RAM of this sequence */
#define	PWM_SEQ_CNT(n)			(0x524 + (n) * 0x20)	/* Number of values (duty cycles) in this sequence */
#define	PWM_SEQ_REFRESH(n)		(0x528 + (n) * 0x20)	/* Number of additional PWM periods between samples loaded into compare register */
#define	PWM_SEQ_ENDDELAY(n)		(0x52C + (n) * 0x20)	/* Time added after the sequence */
#define	PWM_PSEL_OUT(n)			(0x560 + (n) * 0x4)	/* Output pin select for PWM channel n, n == 0..3 */

struct pwm_softc {
	size_t base;
};

#endif /* !_ARM_NORDICSEMI_NRF9160_PWM_H_ */
