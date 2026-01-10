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

#ifndef _ARM_STM_STM32N6_DCMIPP_H_
#define _ARM_STM_STM32N6_DCMIPP_H_

#define	DCMIPP_IPGR1		0x000
#define	DCMIPP_IPGR2		0x004
#define	DCMIPP_IPGR3		0x008
#define	DCMIPP_IPGR8		0x01C
#define	DCMIPP_IPC1R1		0x020
#define	DCMIPP_IPC1R3		0x028
#define	DCMIPP_IPC2R1		0x030
#define	DCMIPP_IPC2R2		0x034
#define	DCMIPP_IPC2R3		0x038
#define	DCMIPP_IPC3R1		0x040
#define	DCMIPP_IPC3R2		0x044
#define	DCMIPP_IPC3R3		0x048
#define	DCMIPP_IPC4R1		0x050
#define	DCMIPP_IPC4R2		0x054
#define	DCMIPP_IPC4R3		0x058
#define	DCMIPP_IPC5R1		0x060
#define	DCMIPP_IPC5R2		0x064
#define	DCMIPP_IPC5R3		0x068
#define	DCMIPP_PRHWCFGR		0x100
#define	DCMIPP_PRCR		0x104
#define	DCMIPP_PRESCR		0x108
#define	DCMIPP_PRESUR		0x10C
#define	DCMIPP_PRIER		0x1F4
#define	DCMIPP_PRSR		0x1F8
#define	DCMIPP_PRFCR		0x1FC
#define	DCMIPP_CMCR		0x204
#define	 CMCR_INSEL_CSI2	(1 << 0) /* input selection */
#define	 CMCR_PSFC_S		1 /* Pipe selection for the frame counter */
#define	 CMCR_PSFC_PIPE0	(0 << CMCR_PSFC_S)
#define	 CMCR_PSFC_PIPE1	(1 << CMCR_PSFC_S)
#define	 CMCR_PSFC_PIPE2	(2 << CMCR_PSFC_S)
#define	DCMIPP_CMFRCR		0x208
#define	DCMIPP_CMIER		0x3F0
#define	 CMIER_P2FRAMEIE	(1 << 25)
#define	 CMIER_P2LINEIE		(1 << 24) /* Multi-line capture complete P2 */
#define	DCMIPP_CMSR1		0x3F4
#define	DCMIPP_CMSR2		0x3F8
#define	DCMIPP_CMFCR		0x3FC

#define	DCMIPP_PxFSCR(x)	(0x404 + 0x400 * (x))
#define	 PxFSCR_PIPEN		(1 << 31)
#define	 PxFSCR_FDTFEN		(1 << 30) /* Force Datatype format enable */
#define	 PxFSCR_FDTF_S		24 /* Force Datatype format */
#define	 PxFSCR_VC_S		19 /* Virtual channel ID of the CSI flow */
#define	 PxFSCR_PIPEDIFF	(1 << 18) /* Differentiates Pipe2 from Pipe1 */
#define	 PxFSCR_DTMODE_S	16 /* Flow selection mode */
#define	 PxFSCR_DTIDB_S		8
#define	 PxFSCR_DTIDA_S		0
#define	DCMIPP_PxSRCR(x)	(0x420 + 0x400 * (x))
#define	DCMIPP_PxBPRCR(x)	(0x424 + 0x400 * (x))
#define	DCMIPP_PxBPRSR(x)	(0x428 + 0x400 * (x))
#define	DCMIPP_PxDECR(x)	(0x430 + 0x400 * (x))
#define	DCMIPP_PxBLCCR(x)	(0x440 + 0x400 * (x))
#define	 BLCCR_ENABLE		(1 << 0)
#define	 BLCCR_BLCB_S		8
#define	 BLCCR_BLCG_S		16
#define	 BLCCR_BLCR_S		24
#define	DCMIPP_PxEXCR1(x)	(0x444 + 0x400 * (x))
#define	 EXCR1_SHFR_S		28 /* Red */
#define	 EXCR1_MULTR_S		20 /* Red */
#define	 EXCR1_ENABLE		(1 << 0)
#define	DCMIPP_PxEXCR2(x)	(0x448 + 0x400 * (x))
#define	 EXCR2_SHFG_S		28
#define	 EXCR2_MULTG_S		20
#define	 EXCR2_SHFB_S		12
#define	 EXCR2_MULTB_S		4
#define	DCMIPP_PxST1CR(x)	(0x450 + 0x400 * (x))
#define	DCMIPP_PxST2CR(x)	(0x454 + 0x400 * (x))
#define	DCMIPP_PxST3CR(x)	(0x458 + 0x400 * (x))
#define	DCMIPP_PxSTSTR(x)	(0x45c + 0x400 * (x))
#define	DCMIPP_PxSTSZR(x)	(0x460 + 0x400 * (x))
#define	DCMIPP_PxST1SR(x)	(0x464 + 0x400 * (x))
#define	DCMIPP_PxST2SR(x)	(0x468 + 0x400 * (x))
#define	DCMIPP_PxST3SR(x)	(0x46c + 0x400 * (x))
#define	DCMIPP_PxDMCR(x)	(0x470 + 0x400 * (x))
#define	 DMCR_EN		(1 << 0)
#define	 DMCR_TYPE_S		1
#define	 DMCR_TYPE_RGGB		(0 << DMCR_TYPE_S)
#define	 DMCR_TYPE_GRBG		(1 << DMCR_TYPE_S)
#define	 DMCR_TYPE_GBRG		(2 << DMCR_TYPE_S)
#define	 DMCR_TYPE_BGGR		(3 << DMCR_TYPE_S)
#define	 DMCR_PEAK_S		16
#define	 DMCR_LINEV_S		20
#define	 DMCR_LINEH_S		24
#define	 DMCR_EDGE_S		28
#define	DCMIPP_PxCCCR(x)	(0x480 + 0x400 * (x))
#define	 CCCR_ENABLE		(1 << 0)
#define	 CCCR_TYPE		(1 << 1)	/* R, G and B */
#define	 CCCR_CLAMP		(1 << 2)
#define	DCMIPP_PxCCRR1(x)	(0x484 + 0x400 * (x))
#define	 CCRR1_RG_S		16
#define	 CCRR1_RR_S		0
#define	DCMIPP_PxCCRR2(x)	(0x488 + 0x400 * (x))
#define	 CCRR2_RA_S		16
#define	 CCRR2_RB_S		0
#define	DCMIPP_PxCCGR1(x)	(0x48c + 0x400 * (x))
#define	 CCGR1_GG_S		16
#define	 CCGR1_GR_S		0
#define	DCMIPP_PxCCGR2(x)	(0x490 + 0x400 * (x))
#define	 CCGR2_GA_S		16
#define	 CCGR2_GB_S		0
#define	DCMIPP_PxCCBR1(x)	(0x494 + 0x400 * (x))
#define	 CCBR1_BG_S		16
#define	 CCBR1_BR_S		0
#define	DCMIPP_PxCCBR2(x)	(0x498 + 0x400 * (x))
#define	 CCBR2_BA_S		16
#define	 CCBR2_BB_S		0
#define	DCMIPP_PxCTCR1(x)	(0x4a0 + 0x400 * (x))
#define	DCMIPP_PxCTCR2(x)	(0x4a4 + 0x400 * (x))
#define	DCMIPP_PxCTCR3(x)	(0x4a8 + 0x400 * (x))
#define	DCMIPP_PxFCTCR(x)	(0x500 + 0x400 * (x))
#define	 PxFCTCR_CPTREQ		(1 << 3) /* Capture requested */
#define	 PxFCTCR_CPTMODE_SS	(1 << 2) /* Snapshot mode */
#define	DCMIPP_PxCRSTR(x)	(0x504 + 0x400 * (x))
#define	DCMIPP_PxCRSZR(x)	(0x508 + 0x400 * (x))
#define	DCMIPP_PxDCCR(x)	(0x50c + 0x400 * (x))
#define	DCMIPP_PxDSCR(x)	(0x510 + 0x400 * (x))
#define	 PxDSCR_ENABLE		(1 << 31)
#define	 PxDSCR_VDIV_S		16
#define	 PxDSCR_HDIV_S		0
#define	DCMIPP_PxDSRTIOR(x)	(0x514 + 0x400 * (x))
#define	 PxDSRTIOR_VRATIO_S	16
#define	 PxDSRTIOR_HRATIO_S	0
#define	DCMIPP_PxDSSZR(x)	(0x518 + 0x400 * (x))
#define	 PxDSSZR_VSIZE_S	16
#define	 PxDSSZR_HSIZE_S	0
#define	DCMIPP_PxCMRICR(x)	(0x520 + 0x400 * (x))
#define	DCMIPP_PxRIxCR1(x)	(0x524 + 0x400 * (x))
#define	DCMIPP_PxRIxCR2(x)	(0x528 + 0x400 * (x))
#define	DCMIPP_PxGMCR(x)	(0x570 + 0x400 * (x))
#define	 PxGMCR_EN		(1 << 0) /* Gamma is enabled */
#define	DCMIPP_PxYUVCR(x)	(0x580 + 0x400 * (x))
#define	DCMIPP_PxYUVRR1(x)	(0x584 + 0x400 * (x))
#define	DCMIPP_PxYUVRR2(x)	(0x588 + 0x400 * (x))
#define	DCMIPP_PxYUVGR1(x)	(0x58c + 0x400 * (x))
#define	DCMIPP_PxYUVGR2(x)	(0x590 + 0x400 * (x))
#define	DCMIPP_PxYUVBR1(x)	(0x594 + 0x400 * (x))
#define	DCMIPP_PxYUVBR2(x)	(0x598 + 0x400 * (x))
#define	DCMIPP_PxPPCR(x)	(0x5c0 + 0x400 * (x))
#define	 PxPPCR_FORMAT_S		0
#define	 PxPPCR_FORMAT_RGB888_YUV444_1	(0x0 << PxPPCR_FORMAT_S)
#define	 PxPPCR_FORMAT_RGB565		(0x1 << PxPPCR_FORMAT_S)
#define	 PxPPCR_FORMAT_ARGB8888		(0x2 << PxPPCR_FORMAT_S) /* A=0xff */
#define	 PxPPCR_FORMAT_RGBA8888		(0x3 << PxPPCR_FORMAT_S) /* A=0xff */
#define	 PxPPCR_FORMAT_MONOY8G8_1	(0x4 << PxPPCR_FORMAT_S)
#define	 PxPPCR_FORMAT_YUV444_1_32_AYUV	(0x5 << PxPPCR_FORMAT_S)
#define	 PxPPCR_FORMAT_YUV422_1_16_YUYV	(0x6 << PxPPCR_FORMAT_S)
#define	 PxPPCR_FORMAT_YUV422_2_16_NONE	(0x7 << PxPPCR_FORMAT_S)
#define	 PxPPCR_FORMAT_YUV420_2_12_NV21	(0x8 << PxPPCR_FORMAT_S)
#define	 PxPPCR_FORMAT_YUV420_3_12_YV12	(0x9 << PxPPCR_FORMAT_S)
#define	 PxPPCR_FORMAT_YUV422_1_16_UYVY	(0xa << PxPPCR_FORMAT_S)
#define	 PxPPCR_SWAPRB			(1 << 4)
#define	 PxPPCR_DBM			(1 << 16)
#define	DCMIPP_PxPPM0AR1(x)	(0x5c4 + 0x400 * (x))
#define	DCMIPP_PxPPM0AR2(x)	(0x5c8 + 0x400 * (x))
#define	DCMIPP_PxPPM0PR(x)	(0x5cc + 0x400 * (x))
#define	 PxPPM0PR_PITCH_S	0 /* Number of bytes between two lines. */
#define	DCMIPP_PxSTM0A(x)	(0x5d0 + 0x400 * (x))
#define	DCMIPP_PxPPM1AR1(x)	(0x5d4 + 0x400 * (x))
#define	DCMIPP_PxPPM1AR2(x)	(0x5d8 + 0x400 * (x))
#define	DCMIPP_PxPPM1PR(x)	(0x5dc + 0x400 * (x))
#define	DCMIPP_PxSTM1AR(x)	(0x5e0 + 0x400 * (x))
#define	DCMIPP_PxPPM2AR1(x)	(0x5e4 + 0x400 * (x))
#define	DCMIPP_PxPPM2AR2(x)	(0x5e8 + 0x400 * (x))
#define	DCMIPP_PxSTM2AR(x)	(0x5f0 + 0x400 * (x))
#define	DCMIPP_PxIER(x)		(0x5f4 + 0x400 * (x))
#define	 PxIER_FRAMEIE		(1 << 1)
#define	 PxIER_LINEIE		(1 << 0)
#define	DCMIPP_PxSR(x)		(0x5f8 + 0x400 * (x))
#define	DCMIPP_PxFCR(x)		(0x5fc + 0x400 * (x))
#define	 PxFCR_CFRAMEF		(1 << 1)
#define	DCMIPP_PxCFSCR(x)	(0x604 + 0x400 * (x))
#define	DCMIPP_PxCBPRCR(x)	(0x624 + 0x400 * (x))
#define	DCMIPP_PxCBLCCR(x)	(0x640 + 0x400 * (x))
#define	DCMIPP_PxCEXCR1(x)	(0x644 + 0x400 * (x))
#define	DCMIPP_PxCEXCR2(x)	(0x648 + 0x400 * (x))
#define	DCMIPP_PxCST1CR(x)	(0x650 + 0x400 * (x))
#define	DCMIPP_PxCST2CR(x)	(0x654 + 0x400 * (x))
#define	DCMIPP_PxCST3CR(x)	(0x658 + 0x400 * (x))
#define	DCMIPP_PxCSTSTR(x)	(0x65c + 0x400 * (x))
#define	DCMIPP_PxCSTSZR(x)	(0x660 + 0x400 * (x))
#define	DCMIPP_PxCCCCR(x)	(0x680 + 0x400 * (x))
#define	DCMIPP_PxCCCRR1(x)	(0x684 + 0x400 * (x))
#define	DCMIPP_PxCCCRR2(x)	(0x688 + 0x400 * (x))
#define	DCMIPP_PxCCCGR1(x)	(0x68c + 0x400 * (x))
#define	DCMIPP_PxCCCGR2(x)	(0x690 + 0x400 * (x))
#define	DCMIPP_PxCCCBR1(x)	(0x694 + 0x400 * (x))
#define	DCMIPP_PxCCCBR2(x)	(0x698 + 0x400 * (x))
#define	DCMIPP_PxCCTCR1(x)	(0x6a0 + 0x400 * (x))
#define	DCMIPP_PxCCTCR2(x)	(0x6a4 + 0x400 * (x))
#define	DCMIPP_PxCCTCR3(x)	(0x6a8 + 0x400 * (x))
#define	DCMIPP_PxCFCTCR(x)	(0x700 + 0x400 * (x))
#define	DCMIPP_PxCCRSTR(x)	(0x704 + 0x400 * (x))
#define	DCMIPP_PxCCRSZR(x)	(0x708 + 0x400 * (x))
#define	DCMIPP_PxCDCCR(x)	(0x70c + 0x400 * (x))
#define	DCMIPP_PxCDSCR(x)	(0x710 + 0x400 * (x))
#define	DCMIPP_PxCDSRTIOR(x)	(0x714 + 0x400 * (x))
#define	DCMIPP_PxCDSSZR(x)	(0x718 + 0x400 * (x))
#define	DCMIPP_PxCCMRICR(x)	(0x720 + 0x400 * (x))
#define	DCMIPP_PxCRIxCR1(x)	(0x724 + 0x400 * (x))
#define	DCMIPP_PxCRI1CR2(x)	(0x728 + 0x400 * (x))
#define	DCMIPP_PxCPPCR(x)	(0x7c0 + 0x400 * (x))
#define	DCMIPP_PxCPPM0AR1(x)	(0x7c4 + 0x400 * (x))
#define	DCMIPP_PxCPPM0AR2(x)	(0x7c8 + 0x400 * (x))
#define	DCMIPP_PxCPPM0PR(x)	(0x7cc + 0x400 * (x))
#define	DCMIPP_PxCPPM1AR1(x)	(0x7d4 + 0x400 * (x))
#define	DCMIPP_PxCPPM1AR2(x)	(0x7d8 + 0x400 * (x))
#define	DCMIPP_PxCPPM1PR(x)	(0x7dc + 0x400 * (x))
#define	DCMIPP_PxCPPM2AR1(x)	(0x7e4 + 0x400 * (x))
#define	DCMIPP_PxCPPM2AR2(x)	(0x7e8 + 0x400 * (x))

struct stm32n6_dcmipp_downsize_config {
	uint32_t hratio;
	uint32_t vratio;
	uint32_t hsize;
	uint32_t vsize;
	uint32_t hdivfactor;
	uint32_t vdivfactor;
};

struct stm32n6_dcmipp_pipe_config {
	uint32_t pipe_id;
	uint32_t base_addr;
	uint32_t pitch;
	uint32_t vc;		/* CSI Virtual Channel */
	uint8_t flow_type;
#define	DCMIPP_FLOW_TYPE_CONTINUOUS	(1)
#define	DCMIPP_FLOW_TYPE_SNAPSHOT	(2)
	uint8_t gamma_en;
	uint8_t format;
#define	DCMIPP_FORMAT_RGB565		0
#define	DCMIPP_FORMAT_RGB888		1
	uint8_t decimation;
	uint8_t debayer;
	uint8_t black_level_calibration;
	uint8_t color_conv;
	uint8_t exposure;
	uint8_t dtida;
};

struct stm32n6_dcmipp_softc {
	uint32_t base;
};

void stm32n6_dcmipp_init(struct stm32n6_dcmipp_softc *sc, uint32_t base);
void stm32n6_dcmipp_status(struct stm32n6_dcmipp_softc *sc);
void stm32n6_dcmipp_setup(struct stm32n6_dcmipp_softc *sc,
    struct stm32n6_dcmipp_pipe_config *conf);
void stm32n6_dcmipp_setup_downsize(struct stm32n6_dcmipp_softc *sc, int pipe,
    struct stm32n6_dcmipp_downsize_config *conf);
void stm32n6_dcmipp_pipe2(struct stm32n6_dcmipp_softc *sc);
void stm32n6_dcmipp_pipe2_frame_request(struct stm32n6_dcmipp_softc *sc, int p);
void stm32n6_dcmipp_intr(void *arg, int irq);

#endif /* !_ARM_STM_STM32N6_DCMIPP_H_ */
