/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2023 MediaTek Inc.
 * Author: Barry Wang <barry.wang@mediatek.com>
 */

#ifndef _DTS_IOMMU_PORT_MT6878_H_
#define _DTS_IOMMU_PORT_MT6878_H_

#include <dt-bindings/memory/mtk-memory-port.h>

/* table id must be the same as mtk_iommu.h */
#define MM_TAB					(0)
#define APU_TAB					(1)

/* iova region definition */
#define NORMAL_DOM				(0)
#define LK_RESV_DOM				(1)
#define CCU0_DOM				(2)
#define CCU1_DOM				(3)
#define VDO_UP_DOM				(4)
#define VDEC_DOM				(5)

#define APU_DATA_DOM				(0)
#define APU_SEC_DOM				(1)
#define APU_CODE_DOM				(2)

/* Larb0 */
#define M4U_L0_P0_DISP_POSTMASK0		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 0, 0)
#define M4U_L0_P1_DISP_OVL0_2L_HDR		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 0, 1)
#define M4U_L0_P2_DISP_OVL0_2L_RDMA0		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 0, 2)
#define M4U_L0_P3_DISP_OVL2_2L_HDR		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 0, 3)
#define M4U_L0_P4_DISP_OVL2_2L_RDMA0		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 0, 4)
#define M4U_L0_P5_DISP_UFBC_WDMA0		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 0, 5)
#define M4U_L0_P6_DISP_FAKE0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 0, 6)

/* Larb1 */
#define M4U_L1_P0_DISP_OVL1_2L_HDR		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 1, 0)
#define M4U_L1_P1_DISP_OVL1_2L_RDMA0		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 1, 1)
#define M4U_L1_P2_DISP_OVL3_2L_HDR		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 1, 2)
#define M4U_L1_P3_DISP_OVL3_2L_RDMA0		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 1, 3)
#define M4U_L1_P4_DISP_WDMA1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 1, 4)
#define M4U_L1_P5_DISP_FAKE1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 1, 5)

/* Larb2 */
#define M4U_L2_P0_MDP_RDMA0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 2, 0)
#define M4U_L2_P1_MDP_RDMA1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 2, 1)
#define M4U_L2_P2_MDP_WROT0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 2, 2)
#define M4U_L2_P3_MDP_WDMA0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 2, 3)
#define M4U_L2_P4_DISP_FAKE0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 2, 4)

/* Larb3 --dummy */

/* Larb4 */
#define M4U_L4_P0_HW_VDEC_MC_EXT		MTK_M4U_PORT_ID(MM_TAB, VDEC_DOM, 4, 0)
#define M4U_L4_P1_HW_VDEC_UFO_EXT		MTK_M4U_PORT_ID(MM_TAB, VDEC_DOM, 4, 1)
#define M4U_L4_P2_HW_VDEC_PP_EXT		MTK_M4U_PORT_ID(MM_TAB, VDEC_DOM, 4, 2)
#define M4U_L4_P3_HW_VDEC_PRED_RD_EXT		MTK_M4U_PORT_ID(MM_TAB, VDEC_DOM, 4, 3)
#define M4U_L4_P4_HW_VDEC_PRED_WR_EXT		MTK_M4U_PORT_ID(MM_TAB, VDEC_DOM, 4, 4)
#define M4U_L4_P5_HW_VDEC_PPWRAP_EXT		MTK_M4U_PORT_ID(MM_TAB, VDEC_DOM, 4, 5)
#define M4U_L4_P6_HW_VDEC_TILE_EXT		MTK_M4U_PORT_ID(MM_TAB, VDEC_DOM, 4, 6)
#define M4U_L4_P7_HW_VDEC_VLD_EXT		MTK_M4U_PORT_ID(MM_TAB, VDEC_DOM, 4, 7)
#define M4U_L4_P8_HW_VDEC_VLD2_EXT		MTK_M4U_PORT_ID(MM_TAB, VDEC_DOM, 4, 8)
#define M4U_L4_P9_HW_VDEC_AVC_MV_EXT		MTK_M4U_PORT_ID(MM_TAB, VDEC_DOM, 4, 9)
#define M4U_L4_P10_HW_VDEC_RG_CTRL_DMA_EXT	MTK_M4U_PORT_ID(MM_TAB, VDEC_DOM, 4, 10)
#define M4U_L4_P11_HW_VDEC_UFO_ENC_EXT		MTK_M4U_PORT_ID(MM_TAB, VDEC_DOM, 4, 11)

/* Larb5 --dummy */

/* Larb6 --dummy */

/* Larb7 */
#define M4U_L7_P0_VENC_RCPU			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 7, 0)
#define M4U_L7_P1_VENC_REC			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 7, 1)
#define M4U_L7_P2_VENC_BSDMA			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 7, 2)
#define M4U_L7_P3_VENC_SV_COMV			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 7, 3)
#define M4U_L7_P4_VENC_RD_COMV			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 7, 4)
#define M4U_L7_P5_JPGENC_Y_RDMA			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 7, 5)
#define M4U_L7_P6_JPGENC_C_RDMA			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 7, 6)
#define M4U_L7_P7_JPGENC_Q_RDMA			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 7, 7)
#define M4U_L7_P8_VENC_SUB_W_LUMA		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 7, 8)
#define M4U_L7_P9_JPGENC_BSDMA			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 7, 9)
#define M4U_L7_P10_VENC_CUR_LUMA		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 7, 10)
#define M4U_L7_P11_VENC_CUR_CHROMA		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 7, 11)
#define M4U_L7_P12_VENC_REF_LUMA		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 7, 12)
#define M4U_L7_P13_VENC_REF_CHROMA		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 7, 13)
#define M4U_L7_P14_VENC_SUB_R_LUMA		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 7, 14)

/* Larb8 --dummy */

/* Larb9 */
#define M4U_L9_P0_IMGI_T1_B			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 9, 0)
#define M4U_L9_P1_IMGI_T1_N_B			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 9, 1)
#define M4U_L9_P2_IMGCI_T1_B			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 9, 2)
#define M4U_L9_P3_IMGCI_T1_N_B			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 9, 3)
#define M4U_L9_P4_SMTI_T1_B			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 9, 4)
#define M4U_L9_P5_YUVO_T1_B			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 9, 5)
#define M4U_L9_P6_YUVO_T1_N_B			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 9, 6)
#define M4U_L9_P7_YUVCO_T1_B			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 9, 7)
#define M4U_L9_P8_YUVO_T2_B			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 9, 8)
#define M4U_L9_P9_YUVO_T3_B			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 9, 9)
#define M4U_L9_P10_TNCSTO_T1_B			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 9, 10)
#define M4U_L9_P11_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 9, 11)
#define M4U_L9_P12_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 9, 12)
#define M4U_L9_P13_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 9, 13)
#define M4U_L9_P14_SMTO_T1_B			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 9, 14)
#define M4U_L9_P15_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 9, 15)
#define M4U_L9_P16_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 9, 16)
#define M4U_L9_P17_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 9, 17)

/* Larb10 --dummy */

/* Larb11 */
#define M4U_L11_P0_WPE_RDMA_0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 11, 0)
#define M4U_L11_P1_WPE_RDMA_1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 11, 1)
#define M4U_L11_P2_WPE_RDMA_2			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 11, 2)
#define M4U_L11_P3_PIMGI_P1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 11, 3)
#define M4U_L11_P4_PIMGBI_P1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 11, 4)
#define M4U_L11_P5_WPE_WDMA_0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 11, 5)
#define M4U_L11_P6_WROT_P1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 11, 6)
#define M4U_L11_P7_TCCSO_P1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 11, 7)
#define M4U_L11_P8_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 11, 8)
#define M4U_L11_P9_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 11, 9)
#define M4U_L11_P10_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 11, 10)
#define M4U_L11_P11_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 11, 11)

/* Larb12 */
#define M4U_L12_P0_FDVT_RDA_0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 12, 0)
#define M4U_L12_P1_FDVT_WRA_0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 12, 1)
#define M4U_L12_P2_ME_RDMA_0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 12, 2)
#define M4U_L12_P3_ME_WDMA_0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 12, 3)
#define M4U_L12_P4_MEMMG_RDMA_0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 12, 4)
#define M4U_L12_P5_MEMMG_WDMA_0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 12, 5)
#define M4U_L12_P6_ME_2ND_RDMA_0		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 12, 6)
#define M4U_L12_P7_ME_2ND_WDMA_0		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 12, 7)
#define M4U_L12_P8_MEMMG_2ND_RDMA_0		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 12, 8)
#define M4U_L12_P9_MEMMG_2ND_WDMA_0		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 12, 9)
#define M4U_L12_P10_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 12, 10)
#define M4U_L12_P11_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 12, 11)

/* Larb13 */
#define M4U_L13_P0_CAMSV_B_CQI_E1		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 13, 0)
#define M4U_L13_P1_CAMSV_B0_WDMA		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 13, 1)
#define M4U_L13_P2_CAMSV_B1_WDMA		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 13, 2)
#define M4U_L13_P3_FAKE_ENG			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 13, 3)
#define M4U_L13_P4_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 13, 4)

/* Larb14 */
#define M4U_L14_P0_CAMSV_A_CQI_E1		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 14, 0)
#define M4U_L14_P1_CAMSV_A0_WDMA		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 14, 1)
#define M4U_L14_P2_CAMSV_A1_WDMA		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 14, 2)
#define M4U_L14_P3_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 14, 3)

/* Larb15 */
#define M4U_L15_P0_VIPI_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 0)
#define M4U_L15_P1_VIPI_D1_N			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 1)
#define M4U_L15_P2_BOKPYI_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 2)
#define M4U_L15_P3_IMG3O_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 3)
#define M4U_L15_P4_IMG3CO_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 4)
#define M4U_L15_P5_IMG2O_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 5)
#define M4U_L15_P6_CNRO_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 6)
#define M4U_L15_P7_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 7)
#define M4U_L15_P8_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 8)
#define M4U_L15_P9_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 9)
#define M4U_L15_P10_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 10)
#define M4U_L15_P11_TNCSTI_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 11)
#define M4U_L15_P12_TNCSTI_D4			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 12)
#define M4U_L15_P13_EECSI_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 13)
#define M4U_L15_P14_SNRCSI_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 14)
#define M4U_L15_P15_CSMCSI_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 15)
#define M4U_L15_P16_SMTI_D4			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 16)
#define M4U_L15_P17_SMTI_D6			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 17)
#define M4U_L15_P18_BOKMI_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 18)
#define M4U_L15_P19_BOKMO_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 19)
#define M4U_L15_P20_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 20)
#define M4U_L15_P21_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 21)
#define M4U_L15_P22_TNCO_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 22)
#define M4U_L15_P23_TNCSO_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 23)
#define M4U_L15_P24_TNCSTO_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 24)
#define M4U_L15_P25_SMTO_D4			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 25)
#define M4U_L15_P26_SMTO_D6			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 26)
#define M4U_L15_P27_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 27)
#define M4U_L15_P28_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 15, 28)

/* Larb16 */
#define M4U_L16_P0_CQI_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 16, 0)
#define M4U_L16_P1_RAWI_R2_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 16, 1)
#define M4U_L16_P2_RAWI_R3_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 16, 2)
#define M4U_L16_P3_RAWI_R4_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 16, 3)
#define M4U_L16_P4_RAWI_R5_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 16, 4)
#define M4U_L16_P5_IMGO_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 16, 5)
#define M4U_L16_P6_IMGO_R2_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 16, 6)
#define M4U_L16_P7_FPRI_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 16, 7)
#define M4U_L16_P8_BPCI_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 16, 8)
#define M4U_L16_P9_BPCI_R4_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 16, 9)
#define M4U_L16_P10_LSCI_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 16, 10)
#define M4U_L16_P11_UFEO_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 16, 11)
#define M4U_L16_P12_LTMSO_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 16, 12)
#define M4U_L16_P13_RESERVED                    MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 16, 13)
#define M4U_L16_P14_AFO_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 16, 14)
#define M4U_L16_P15_AAO_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 16, 15)
#define M4U_L16_P16_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 16, 16)

/* Larb17 */
#define M4U_L17_P0_YUVO_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 17, 0)
#define M4U_L17_P1_YUVO_R3_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 17, 1)
#define M4U_L17_P2_YUVO_R2_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 17, 2)
#define M4U_L17_P3_YUVO_R5_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 17, 3)
#define M4U_L17_P4_RGBWI_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 17, 4)
#define M4U_L17_P5_TCYSO_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 17, 5)
#define M4U_L17_P6_DRZHNO_R3_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 17, 6)
#define M4U_L17_P7_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 17, 7)

/* Larb18 --dummy */

/* Larb19 --dummy */

/* Larb20 --dummy */

/* Larb21 --dummy */

/* Larb22 */
#define M4U_L22_P0_WPE_RDMA_0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 22, 0)
#define M4U_L22_P1_WPE_RDMA_1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 22, 1)
#define M4U_L22_P2_WPE_RDMA_2			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 22, 2)
#define M4U_L22_P3_PIMGI_P1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 22, 3)
#define M4U_L22_P4_PIMGBI_P1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 22, 4)
#define M4U_L22_P5_WPE_WDMA_0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 22, 5)
#define M4U_L22_P6_WROT_P1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 22, 6)
#define M4U_L22_P7_TCCSO_P1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 22, 7)
#define M4U_L22_P8_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 22, 8)
#define M4U_L22_P9_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 22, 9)
#define M4U_L22_P10_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 22, 10)
#define M4U_L22_P11_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 22, 11)

/* Larb23 --dummy */

/* Larb24 --dummy */

/* Larb25 */
#define M4U_L25_P0_MRAW0_CQ1_M1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 25, 0)
#define M4U_L25_P1_MRAW0_IMGBO_M1		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 25, 1)
#define M4U_L25_P2_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 25, 2)
#define M4U_L25_P3_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 25, 3)
#define M4U_L25_P4_PDAI_A_0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 25, 4)
#define M4U_L25_P5_PDAI_A_1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 25, 5)
#define M4U_L25_P6_PDAI_A_2			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 25, 6)
#define M4U_L25_P7_PDAI_A_3			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 25, 7)
#define M4U_L25_P8_PDAI_A_4			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 25, 8)
#define M4U_L25_P9_PDAI_A_0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 25, 9)
#define M4U_L25_P10_MRAW0_IMGBO_M1		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 25, 10)
#define M4U_L25_P11_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 25, 11)
#define M4U_L25_P12_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 25, 12)
#define M4U_L25_P13_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 25, 13)

/* Larb26 --dummy */

/* Larb27 --dummy */

/* Larb28 */
#define M4U_L28_P0_IMGI_T1_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 0)
#define M4U_L28_P1_IMGI_T1_N_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 1)
#define M4U_L28_P2_IMGCI_T1_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 2)
#define M4U_L28_P3_IMGCI_T1_N_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 3)
#define M4U_L28_P4_YUVO_T1_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 4)
#define M4U_L28_P5_YUVO_T1_N_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 5)
#define M4U_L28_P6_YUVO_T2_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 6)
#define M4U_L28_P7_YUVO_T3_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 7)
#define M4U_L28_P8_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 8)
#define M4U_L28_P9_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 9)
#define M4U_L28_P10_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 10)
#define M4U_L28_P11_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 11)
#define M4U_L28_P12_SMTI_T1_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 12)
#define M4U_L28_P13_SMTI_T4_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 13)
#define M4U_L28_P14_TNCSTI_T1_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 14)
#define M4U_L28_P15_TNCSTI_T4_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 15)
#define M4U_L28_P16_LTMSTI_T1_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 16)
#define M4U_L28_P17_YUVCO_T1_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 17)
#define M4U_L28_P18_TNCSTO_T1_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 18)
#define M4U_L28_P19_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 19)
#define M4U_L28_P20_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 20)
#define M4U_L28_P21_TNCSTO_T1_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 21)
#define M4U_L28_P22_SMTO_T1_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 22)
#define M4U_L28_P23_SMTO_T4_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 23)
#define M4U_L28_P24_LTMSO_T1_A			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 24)
#define M4U_L28_P25_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 25)
#define M4U_L28_P26_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 28, 26)

/* Larb29 */
#define M4U_L29_P0_CAMSV_CQI_E2			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 29, 0)
#define M4U_L29_P1_CAMSV_CQI_E3			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 29, 1)
#define M4U_L29_P2_CAMSV_CQI_E4			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 29, 2)
#define M4U_L29_P3_CAMSV_CQI_E5			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 29, 3)
#define M4U_L29_P4_CAMSV_E2_WDMA		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 29, 4)
#define M4U_L29_P5_CAMSV_E3_WDMA		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 29, 5)
#define M4U_L29_P6_CAMSV_E4_WDMA		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 29, 6)
#define M4U_L29_P7_CAMSV_E5_WDMA		MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 29, 7)
#define M4U_L29_P8_CAMSV_R_0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 29, 8)
#define M4U_L29_P9_CAMSV_R_1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 29, 9)

/* Larb30 */
#define M4U_L30_P0_CCUI_0			MTK_M4U_PORT_ID(MM_TAB, CCU0_DOM, 30, 0)
#define M4U_L30_P1_CCUO_0			MTK_M4U_PORT_ID(MM_TAB, CCU0_DOM, 30, 1)
#define M4U_L30_P2_CCUI_1			MTK_M4U_PORT_ID(MM_TAB, CCU1_DOM, 30, 2)
#define M4U_L30_P3_CCUO_1			MTK_M4U_PORT_ID(MM_TAB, CCU1_DOM, 30, 3)

/* Larb31 --dummy */

/* Larb32 --dummy */

/* Larb33 */
#define M4U_L33_P0_CQI_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 33, 0)
#define M4U_L33_P1_RAWI_R2_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 33, 1)
#define M4U_L33_P2_RAWI_R3_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 33, 2)
#define M4U_L33_P3_RAWI_R4_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 33, 3)
#define M4U_L33_P4_RAWI_R5_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 33, 4)
#define M4U_L33_P5_IMGO_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 33, 5)
#define M4U_L33_P6_IMGO_R2_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 33, 6)
#define M4U_L33_P7_FPRI_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 33, 7)
#define M4U_L33_P8_BPCI_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 33, 8)
#define M4U_L33_P9_BPCI_R4_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 33, 9)
#define M4U_L33_P10_LSCI_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 33, 10)
#define M4U_L33_P11_UFEO_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 33, 11)
#define M4U_L33_P12_LTMSO_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 33, 12)
#define M4U_L33_P13_RESERVED                    MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 33, 13)
#define M4U_L33_P14_AFO_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 33, 14)
#define M4U_L33_P15_AAO_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 33, 15)
#define M4U_L33_P16_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 33, 16)

/* Larb34 */
#define M4U_L34_P0_YUVO_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 34, 0)
#define M4U_L34_P1_YUVO_R3_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 34, 1)
#define M4U_L34_P2_YUVO_R2_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 34, 2)
#define M4U_L34_P3_YUVO_R5_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 34, 3)
#define M4U_L34_P4_RGBWI_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 34, 4)
#define M4U_L34_P5_TCYSO_R1_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 34, 5)
#define M4U_L34_P6_DRZHNO_R3_U0			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 34, 6)
#define M4U_L34_P7_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 34, 7)

/* Larb35 --dummy */

/* Larb36 --dummy */

/* Larb37 --dummy */

/* Larb38 */
#define M4U_L38_P0_TNRAIMI_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 0)
#define M4U_L38_P1_TNRWI_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 1)
#define M4U_L38_P2_TNRCI_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 2)
#define M4U_L38_P3_SMTI_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 3)
#define M4U_L38_P4_SMTCI_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 4)
#define M4U_L38_P5_IMG4CO_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 5)
#define M4U_L38_P6_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 6)
#define M4U_L38_P7_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 7)
#define M4U_L38_P8_TNRMO_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 8)
#define M4U_L38_P9_SMTO_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 9)
#define M4U_L38_P10_SMTCO_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 10)
#define M4U_L38_P11_TNRSI_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 11)
#define M4U_L38_P12_TNRSO_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 12)
#define M4U_L38_P13_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 13)
#define M4U_L38_P14_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 14)
#define M4U_L38_P15_IMGI_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 15)
#define M4U_L38_P16_IMGCI_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 16)
#define M4U_L38_P17_IMGCI_N_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 17)
#define M4U_L38_P18_DMGI_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 18)
#define M4U_L38_P19_RECI_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 19)
#define M4U_L38_P20_RECI_D3			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 20)
#define M4U_L38_P21_RECBI_D2			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 21)
#define M4U_L38_P22_IMG4O_D1			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 22)
#define M4U_L38_P23_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 23)
#define M4U_L38_P24_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 24)
#define M4U_L38_P25_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 25)
#define M4U_L38_P26_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 26)
#define M4U_L38_P27_RESERVED			MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 38, 27)

/* fake larb39 */
#define M4U_L39_CCU0				MTK_M4U_PORT_ID(MM_TAB, CCU0_DOM, 39, 0)

/* fake larb40 */
#define M4U_L40_CCU1				MTK_M4U_PORT_ID(MM_TAB, CCU1_DOM, 40, 0)

/* fake larb41 */
#define M4U_L41_GCE_DM				MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 41, 0)
#define M4U_L41_GCE_MM				MTK_M4U_PORT_ID(MM_TAB, NORMAL_DOM, 41, 1)
#define M4U_L41_VIDEO_UP			MTK_M4U_PORT_ID(MM_TAB, VDO_UP_DOM, 41, 2)

/* fake larb42 reserved */
#define M4U_L42_APU_DATA			MTK_M4U_PORT_ID(APU_TAB, APU_DATA_DOM, 42, 0)
#define M4U_L42_APU_CODE			MTK_M4U_PORT_ID(APU_TAB, APU_CODE_DOM, 42, 1)

#endif /* _DTS_IOMMU_PORT_MT6878_H_ */
