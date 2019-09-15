/*-
 * Copyright (c) 2017 Ruslan Bukin <br@bsdpad.com>
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

/*
 * SiFive AON Memory Map.
 * SiFive E300 Platform Reference Manual, Version 1.0.1
 */

#ifndef	_SYS_RISCV_SIFIVE_E300G_AON_H_
#define	_SYS_RISCV_SIFIVE_E300G_AON_H_

/* Watchdog Timer Registers */
#define AON_WDOGCFG	0x000
#define	AON_WDOGCOUNT	0x008
#define	AON_WDOGS	0x010
#define	AON_WDOGFEED	0x018
#define	AON_WDOGKEY	0x01C
#define	AON_WDOGCMP	0x020

/* Real-Time Clock Registers */
#define	AON_RTCCFG	0x040
#define	AON_RTCLO	0x048
#define	AON_RTCHI	0x04C
#define	AON_RTCS	0x050
#define	AON_RTCCMP	0x060

/* AON Clock Configuration Registers */
#define	AON_LFROSCCFG	0x070
#define	 LFROSCCFG_EN	(1 << 30)

/* Backup Registers */
#define	AON_BACKUP(x)	(0x080 + (x) * 0x4)

/* Power Management Unit */
#define	AON_PMUWAKEUP		0x100
#define	AON_PMUSLEEPI(x)	(0x120 + (x) * 0x4)
#define	AON_PMUIE		0x140
#define	AON_PMUCAUSE		0x144
#define	AON_PMUSLEEP		0x148
#define	AON_PMUKEY		0x14C
#define	 PMUKEY_VAL		0x51F15E

struct aon_softc {
	uint32_t base;
};

void e300g_aon_init(struct aon_softc *sc, uint32_t base);
void e300g_aon_sleep(struct aon_softc *sc);

#endif /* !_SYS_RISCV_SIFIVE_E300G_AON_H_ */
