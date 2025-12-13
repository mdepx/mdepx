/*-
 * Copyright (c) 2025 Ruslan Bukin <br@bsdpad.com>
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

#ifndef _ARM_STM_STM32N6_RISAF_H_
#define _ARM_STM_STM32N6_RISAF_H_

#define	RISAF_CR		0x000
#define	RISAF_IASR		0x008
#define	RISAF_IACR		0x00C
#define	RISAF_IAESR		0x020
#define	RISAF_IADDR		0x024
#define	RISAF_REG_CFGR(x)	(0x040 + 0x40 * (x - 1))
#define	 CFGR_BREN		(1 << 0) /* Base region enable */
#define	 CFGR_SEC		(1 << 8) /* Secure region */
#define	 CFGR_PRIVC(x)		(1 << (16 + (x)))
#define	RISAF_REG_STARTR(x)	(0x044 + 0x40 * (x - 1))
#define	RISAF_REG_ENDR(x)	(0x048 + 0x40 * (x - 1))
#define	RISAF_REG_CIDCFGR(x)	(0x04C + 0x40 * (x - 1))
#define	 CIDCFGR_RDEN_C(x)	(1 << (x))
#define	 CIDCFGR_WREN_C(x)	(1 << (16 + (x)))
#define	RISAF_REG_ACFGR(x)	(0x050 + 0x40 * (x - 1))
#define	 ACFGR_SREN		(1 << 0) /* Subregion enable */
#define	 ACFGR_RLOCK		(1 << 1) /* Resource lock */
#define	 ACFGR_SRCID_S		4 /* Subregion CID */
#define	 ACFGR_SRCID_M		(0x7 << ACFGR_SRCID_S)
#define	 ACFGR_SEC		(1 << 8) /* Secure subregion */
#define	 ACFGR_PRIV		(1 << 9) /* Privileged subregion */
#define	 ACFGR_RDEN		(1 << 12) /* Read enable */
#define	 ACFGR_WREN		(1 << 13) /* Write enable */
#define	RISAF_REG_ASTARTR(x)	(0x054 + 0x40 * (x - 1))
#define	RISAF_REG_AENDR(x)	(0x058 + 0x40 * (x - 1))
#define	RISAF_REG_ANESTR(x)	(0x05C + 0x40 * (x - 1))
#define	RISAF_REG_BCFGR(x)	(0x060 + 0x40 * (x - 1))
#define	RISAF_REG_BSTARTR(x)	(0x064 + 0x40 * (x - 1))
#define	RISAF_REG_BENDR(x)	(0x068 + 0x40 * (x - 1))
#define	RISAF_REG_BNESTR(x)	(0x06C + 0x40 * (x - 1))

struct risaf_config {
	/* Base region. */
	size_t base_start;
	size_t base_end;
	uint8_t base_cid_write;
	uint8_t base_cid_read;
	uint8_t base_sec;

	/* A sub region. */
	size_t suba_start;
	size_t suba_end;
	uint8_t suba_rd;
	uint8_t suba_wr;
	uint8_t suba_cid;
	uint8_t suba_sec;
};

struct stm32n6_risaf_softc {
	uint32_t base;
};

void stm32n6_risaf_init(struct stm32n6_risaf_softc *sc, uint32_t base);
void stm32n6_risaf_setup(struct stm32n6_risaf_softc *sc, uint32_t reg_id,
    struct risaf_config *conf);

#endif /* !_ARM_STM_STM32N6_RISAF_H_ */
