/* SPDX-License-Identifier: GPL-2.0 */
/*
 * mt6989-afe-clk.h  --  Mediatek 6989 afe clock ctrl definition
 *
 * Copyright (c) 2023 MediaTek Inc.
 *  Author: Tina Tsai <tina.tsai@mediatek.com>
 */

#ifndef _MT6989_AFE_CLOCK_CTRL_H_
#define _MT6989_AFE_CLOCK_CTRL_H_

#define AP_PLL_CON3 0x000c
#define APLL1_CON0 0x0328
#define APLL1_CON1 0x032c
#define APLL1_CON2 0x0330
#define APLL1_CON4 0x0338
#define APLL1_TUNER_CON0 0x0040

#define APLL2_CON0 0x033c
#define APLL2_CON1 0x0340
#define APLL2_CON2 0x0344
#define APLL2_CON4 0x034c
#define APLL2_TUNER_CON0 0x0044

#define CLK_CFG_6 0x0070
#define CLK_CFG_7 0x0080
#define CLK_CFG_10 0x00b0
#define CLK_CFG_11 0x00c0
#define CLK_CFG_12 0x00d0
#define CLK_CFG_13 0x00e0
#define CLK_CFG_UPDATE 0x004
#define CLK_CFG_UPDATE1 0x008

#define CLK_AUDDIV_0 0x0320
#define CLK_AUDDIV_1 0x0324
#define CLK_AUDDIV_2 0x0328
#define CLK_AUDDIV_3 0x0334
#define CLK_AUDDIV_4 0x0338
#define CLK_AUDDIV_5 0x033c

#define CKSYS_AUD_TOP_CFG 0x032c
#define CKSYS_AUD_TOP_MON 0x0330

/* CLK_AUDDIV_0 */
#define APLL12_DIV_I2SIN0_PDN_SFT               0
#define APLL12_DIV_I2SIN0_PDN_MASK              0x1
#define APLL12_DIV_I2SIN0_PDN_MASK_SFT          (0x1 << 0)
#define APLL12_DIV_I2SIN1_PDN_SFT               1
#define APLL12_DIV_I2SIN1_PDN_MASK              0x1
#define APLL12_DIV_I2SIN1_PDN_MASK_SFT          (0x1 << 1)
#define APLL12_DIV_I2SIN2_PDN_SFT               2
#define APLL12_DIV_I2SIN2_PDN_MASK              0x1
#define APLL12_DIV_I2SIN2_PDN_MASK_SFT          (0x1 << 2)
#define APLL12_DIV_I2SIN3_PDN_SFT               3
#define APLL12_DIV_I2SIN3_PDN_MASK              0x1
#define APLL12_DIV_I2SIN3_PDN_MASK_SFT          (0x1 << 3)
#define APLL12_DIV_I2SIN4_PDN_SFT               4
#define APLL12_DIV_I2SIN4_PDN_MASK              0x1
#define APLL12_DIV_I2SIN4_PDN_MASK_SFT          (0x1 << 4)
#define APLL12_DIV_I2SIN6_PDN_SFT               5
#define APLL12_DIV_I2SIN6_PDN_MASK              0x1
#define APLL12_DIV_I2SIN6_PDN_MASK_SFT          (0x1 << 5)
#define APLL12_DIV_I2SOUT0_PDN_SFT              6
#define APLL12_DIV_I2SOUT0_PDN_MASK             0x1
#define APLL12_DIV_I2SOUT0_PDN_MASK_SFT         (0x1 << 6)
#define APLL12_DIV_I2SOUT1_PDN_SFT              7
#define APLL12_DIV_I2SOUT1_PDN_MASK             0x1
#define APLL12_DIV_I2SOUT1_PDN_MASK_SFT         (0x1 << 7)
#define APLL12_DIV_I2SOUT2_PDN_SFT              8
#define APLL12_DIV_I2SOUT2_PDN_MASK             0x1
#define APLL12_DIV_I2SOUT2_PDN_MASK_SFT         (0x1 << 8)
#define APLL12_DIV_I2SOUT3_PDN_SFT              9
#define APLL12_DIV_I2SOUT3_PDN_MASK             0x1
#define APLL12_DIV_I2SOUT3_PDN_MASK_SFT         (0x1 << 9)
#define APLL12_DIV_I2SOUT4_PDN_SFT              10
#define APLL12_DIV_I2SOUT4_PDN_MASK             0x1
#define APLL12_DIV_I2SOUT4_PDN_MASK_SFT         (0x1 << 10)
#define APLL12_DIV_I2SOUT6_PDN_SFT              11
#define APLL12_DIV_I2SOUT6_PDN_MASK             0x1
#define APLL12_DIV_I2SOUT6_PDN_MASK_SFT         (0x1 << 11)
#define APLL12_DIV_FMI2S_PDN_SFT                12
#define APLL12_DIV_FMI2S_PDN_MASK               0x1
#define APLL12_DIV_FMI2S_PDN_MASK_SFT           (0x1 << 12)
#define APLL12_DIV_TDMOUT_M_PDN_SFT             13
#define APLL12_DIV_TDMOUT_M_PDN_MASK            0x1
#define APLL12_DIV_TDMOUT_M_PDN_MASK_SFT        (0x1 << 13)
#define APLL12_DIV_TDMOUT_B_PDN_SFT             14
#define APLL12_DIV_TDMOUT_B_PDN_MASK            0x1
#define APLL12_DIV_TDMOUT_B_PDN_MASK_SFT        (0x1 << 14)
#define APLL_I2SIN0_MCK_SEL_SFT                 16
#define APLL_I2SIN0_MCK_SEL_MASK                0x1
#define APLL_I2SIN0_MCK_SEL_MASK_SFT            (0x1 << 16)
#define APLL_I2SIN1_MCK_SEL_SFT                 17
#define APLL_I2SIN1_MCK_SEL_MASK                0x1
#define APLL_I2SIN1_MCK_SEL_MASK_SFT            (0x1 << 17)
#define APLL_I2SIN2_MCK_SEL_SFT                 18
#define APLL_I2SIN2_MCK_SEL_MASK                0x1
#define APLL_I2SIN2_MCK_SEL_MASK_SFT            (0x1 << 18)
#define APLL_I2SIN3_MCK_SEL_SFT                 19
#define APLL_I2SIN3_MCK_SEL_MASK                0x1
#define APLL_I2SIN3_MCK_SEL_MASK_SFT            (0x1 << 19)
#define APLL_I2SIN4_MCK_SEL_SFT                 20
#define APLL_I2SIN4_MCK_SEL_MASK                0x1
#define APLL_I2SIN4_MCK_SEL_MASK_SFT            (0x1 << 20)
#define APLL_I2SIN6_MCK_SEL_SFT                 21
#define APLL_I2SIN6_MCK_SEL_MASK                0x1
#define APLL_I2SIN6_MCK_SEL_MASK_SFT            (0x1 << 21)
#define APLL_I2SOUT0_MCK_SEL_SFT                22
#define APLL_I2SOUT0_MCK_SEL_MASK               0x1
#define APLL_I2SOUT0_MCK_SEL_MASK_SFT           (0x1 << 22)
#define APLL_I2SOUT1_MCK_SEL_SFT                23
#define APLL_I2SOUT1_MCK_SEL_MASK               0x1
#define APLL_I2SOUT1_MCK_SEL_MASK_SFT           (0x1 << 23)
#define APLL_I2SOUT2_MCK_SEL_SFT                24
#define APLL_I2SOUT2_MCK_SEL_MASK               0x1
#define APLL_I2SOUT2_MCK_SEL_MASK_SFT           (0x1 << 24)
#define APLL_I2SOUT3_MCK_SEL_SFT                25
#define APLL_I2SOUT3_MCK_SEL_MASK               0x1
#define APLL_I2SOUT3_MCK_SEL_MASK_SFT           (0x1 << 25)
#define APLL_I2SOUT4_MCK_SEL_SFT                26
#define APLL_I2SOUT4_MCK_SEL_MASK               0x1
#define APLL_I2SOUT4_MCK_SEL_MASK_SFT           (0x1 << 26)
#define APLL_I2SOUT6_MCK_SEL_SFT                27
#define APLL_I2SOUT6_MCK_SEL_MASK               0x1
#define APLL_I2SOUT6_MCK_SEL_MASK_SFT           (0x1 << 27)
#define APLL_FMI2S_MCK_SEL_SFT                  28
#define APLL_FMI2S_MCK_SEL_MASK                 0x1
#define APLL_FMI2S_MCK_SEL_MASK_SFT             (0x1 << 28)
#define APLL_TDMOUT_MCK_SEL_SFT                 29
#define APLL_TDMOUT_MCK_SEL_MASK                0x1
#define APLL_TDMOUT_MCK_SEL_MASK_SFT            (0x1 << 29)

/* CLK_AUDDIV_1 */
#define APLL12_DIV_I2SIN0_INV_SFT               0
#define APLL12_DIV_I2SIN0_INV_MASK              0x1
#define APLL12_DIV_I2SIN0_INV_MASK_SFT          (0x1 << 0)
#define APLL12_DIV_I2SIN1_INV_SFT               1
#define APLL12_DIV_I2SIN1_INV_MASK              0x1
#define APLL12_DIV_I2SIN1_INV_MASK_SFT          (0x1 << 1)
#define APLL12_DIV_I2SIN2_INV_SFT               2
#define APLL12_DIV_I2SIN2_INV_MASK              0x1
#define APLL12_DIV_I2SIN2_INV_MASK_SFT          (0x1 << 2)
#define APLL12_DIV_I2SIN3_INV_SFT               3
#define APLL12_DIV_I2SIN3_INV_MASK              0x1
#define APLL12_DIV_I2SIN3_INV_MASK_SFT          (0x1 << 3)
#define APLL12_DIV_I2SIN4_INV_SFT               4
#define APLL12_DIV_I2SIN4_INV_MASK              0x1
#define APLL12_DIV_I2SIN4_INV_MASK_SFT          (0x1 << 4)
#define APLL12_DIV_I2SIN6_INV_SFT               5
#define APLL12_DIV_I2SIN6_INV_MASK              0x1
#define APLL12_DIV_I2SIN6_INV_MASK_SFT          (0x1 << 5)
#define APLL12_DIV_I2SOUT0_INV_SFT              6
#define APLL12_DIV_I2SOUT0_INV_MASK             0x1
#define APLL12_DIV_I2SOUT0_INV_MASK_SFT         (0x1 << 6)
#define APLL12_DIV_I2SOUT1_INV_SFT              7
#define APLL12_DIV_I2SOUT1_INV_MASK             0x1
#define APLL12_DIV_I2SOUT1_INV_MASK_SFT         (0x1 << 7)
#define APLL12_DIV_I2SOUT2_INV_SFT              8
#define APLL12_DIV_I2SOUT2_INV_MASK             0x1
#define APLL12_DIV_I2SOUT2_INV_MASK_SFT         (0x1 << 8)
#define APLL12_DIV_I2SOUT3_INV_SFT              9
#define APLL12_DIV_I2SOUT3_INV_MASK             0x1
#define APLL12_DIV_I2SOUT3_INV_MASK_SFT         (0x1 << 9)
#define APLL12_DIV_I2SOUT4_INV_SFT              10
#define APLL12_DIV_I2SOUT4_INV_MASK             0x1
#define APLL12_DIV_I2SOUT4_INV_MASK_SFT         (0x1 << 10)
#define APLL12_DIV_I2SOUT6_INV_SFT              11
#define APLL12_DIV_I2SOUT6_INV_MASK             0x1
#define APLL12_DIV_I2SOUT6_INV_MASK_SFT         (0x1 << 11)
#define APLL12_DIV_FMI2S_INV_SFT                12
#define APLL12_DIV_FMI2S_INV_MASK               0x1
#define APLL12_DIV_FMI2S_INV_MASK_SFT           (0x1 << 12)
#define APLL12_DIV_TDMOUT_M_INV_SFT             13
#define APLL12_DIV_TDMOUT_M_INV_MASK            0x1
#define APLL12_DIV_TDMOUT_M_INV_MASK_SFT        (0x1 << 13)
#define APLL12_DIV_TDMOUT_B_INV_SFT             14
#define APLL12_DIV_TDMOUT_B_INV_MASK            0x1
#define APLL12_DIV_TDMOUT_B_INV_MASK_SFT        (0x1 << 14)

/* CLK_AUDDIV_2 */
#define APLL12_CK_DIV_I2SIN0_SFT                0
#define APLL12_CK_DIV_I2SIN0_MASK               0xff
#define APLL12_CK_DIV_I2SIN0_MASK_SFT           (0xff << 0)
#define APLL12_CK_DIV_I2SIN1_SFT                8
#define APLL12_CK_DIV_I2SIN1_MASK               0xff
#define APLL12_CK_DIV_I2SIN1_MASK_SFT           (0xff << 8)
#define APLL12_CK_DIV_I2SIN2_SFT                16
#define APLL12_CK_DIV_I2SIN2_MASK               0xff
#define APLL12_CK_DIV_I2SIN2_MASK_SFT           (0xff << 16)
#define APLL12_CK_DIV_I2SIN3_SFT                24
#define APLL12_CK_DIV_I2SIN3_MASK               0xff
#define APLL12_CK_DIV_I2SIN3_MASK_SFT           (0xff << 24)

/* AUD_TOP_CFG */
#define AUD_TOP_CFG_SFT                         0
#define AUD_TOP_CFG_MASK                        0xffffffff
#define AUD_TOP_CFG_MASK_SFT                    (0xffffffff << 0)

/* AUD_TOP_MON */
#define AUD_TOP_MON_SFT                         0
#define AUD_TOP_MON_MASK                        0xffffffff
#define AUD_TOP_MON_MASK_SFT                    (0xffffffff << 0)

/* CLK_AUDDIV_3 */
#define APLL12_CK_DIV_I2SIN4_SFT                0
#define APLL12_CK_DIV_I2SIN4_MASK               0xff
#define APLL12_CK_DIV_I2SIN4_MASK_SFT           (0xff << 0)
#define APLL12_CK_DIV_I2SIN6_SFT                8
#define APLL12_CK_DIV_I2SIN6_MASK               0xff
#define APLL12_CK_DIV_I2SIN6_MASK_SFT           (0xff << 8)
#define APLL12_CK_DIV_I2SOUT0_SFT               16
#define APLL12_CK_DIV_I2SOUT0_MASK              0xff
#define APLL12_CK_DIV_I2SOUT0_MASK_SFT          (0xff << 16)
#define APLL12_CK_DIV_I2SOUT1_SFT               24
#define APLL12_CK_DIV_I2SOUT1_MASK              0xff
#define APLL12_CK_DIV_I2SOUT1_MASK_SFT          (0xff << 24)

/* CLK_AUDDIV_4 */
#define APLL12_CK_DIV_I2SOUT2_SFT               0
#define APLL12_CK_DIV_I2SOUT2_MASK              0xff
#define APLL12_CK_DIV_I2SOUT2_MASK_SFT          (0xff << 0)
#define APLL12_CK_DIV_I2SOUT3_SFT               8
#define APLL12_CK_DIV_I2SOUT3_MASK              0xff
#define APLL12_CK_DIV_I2SOUT3_MASK_SFT          (0xff << 8)
#define APLL12_CK_DIV_I2SOUT4_SFT               16
#define APLL12_CK_DIV_I2SOUT4_MASK              0xff
#define APLL12_CK_DIV_I2SOUT4_MASK_SFT          (0xff << 16)
#define APLL12_CK_DIV_I2SOUT6_SFT               24
#define APLL12_CK_DIV_I2SOUT6_MASK              0xff
#define APLL12_CK_DIV_I2SOUT6_MASK_SFT          (0xff << 24)

/* CLK_AUDDIV_5 */
#define APLL12_CK_DIV_FMI2S_SFT                 0
#define APLL12_CK_DIV_FMI2S_MASK                0xff
#define APLL12_CK_DIV_FMI2S_MASK_SFT            (0xff << 0)
#define APLL12_CK_DIV_TDMOUT_M_SFT              8
#define APLL12_CK_DIV_TDMOUT_M_MASK             0xff
#define APLL12_CK_DIV_TDMOUT_M_MASK_SFT         (0xff << 8)
#define APLL12_CK_DIV_TDMOUT_B_SFT              16
#define APLL12_CK_DIV_TDMOUT_B_MASK             0xff
#define APLL12_CK_DIV_TDMOUT_B_MASK_SFT         (0xff << 16)

/* APLL */
#define APLL1_W_NAME "APLL1"
#define APLL2_W_NAME "APLL2"
enum {
	MT6989_APLL1 = 0,
	MT6989_APLL2,
};

enum {
	CLK_HOPPING = 0,
	CLK_F26M,
	CLK_UL0_ADC_CLK,
	CLK_UL0_ADC_HIRES_CLK,
	CLK_UL1_ADC_CLK,
	CLK_UL1_ADC_HIRES_CLK,
	CLK_APLL1,
	CLK_APLL2,
	CLK_APLL1_TUNER,
	CLK_APLL2_TUNER,
	CLK_NLE,

	CLK_MUX_AUDIO,
	CLK_MUX_AUDIOINTBUS,
	CLK_TOP_MAINPLL_D4_D4,
	/* apll related mux */
	CLK_TOP_MUX_AUD_1,
	CLK_TOP_APLL1_CK,
	CLK_TOP_MUX_AUD_2,
	CLK_TOP_APLL2_CK,
	CLK_TOP_MUX_AUD_ENG1,
	CLK_TOP_APLL1_D4,
	CLK_TOP_MUX_AUD_ENG2,
	CLK_TOP_APLL2_D4,
	CLK_TOP_MUX_AUDIO_H,
	CLK_TOP_I2SIN0_M_SEL,
	CLK_TOP_I2SIN1_M_SEL,
	CLK_TOP_FMI2S_M_SEL,
	CLK_TOP_TDMOUT_M_SEL,
	CLK_TOP_APLL12_DIV_I2SIN0,
	CLK_TOP_APLL12_DIV_I2SIN1,
	CLK_TOP_APLL12_DIV_FMI2S,
	CLK_TOP_APLL12_DIV_TDMOUT_M,
	CLK_TOP_APLL12_DIV_TDMOUT_B,
	CLK_CLK26M,
	CLK_PERAO_AUDIO_SLV_CK_PERI,
	CLK_PERAO_AUDIO_MST_CK_PERI,
	CLK_PERAO_INTBUS_CK_PERI,
	CLK_PERAO_ENGEN1_CK,
	CLK_PERAO_ENGEN2_CK,
	CLK_PERAO_TDMOUT_B_CK,
	CLK_PERAO_H_AUD_CK,
	CLK_NUM
};

struct mtk_base_afe;

int mt6989_init_clock(struct mtk_base_afe *afe);
int mt6989_afe_enable_clock(struct mtk_base_afe *afe);
void mt6989_afe_disable_clock(struct mtk_base_afe *afe);
int mt6989_afe_disable_apll(struct mtk_base_afe *afe);
int mt6989_afe_enable_ao_clock(struct mtk_base_afe *afe);


int mt6989_afe_sram_request(struct mtk_base_afe *afe);
void mt6989_afe_sram_release(struct mtk_base_afe *afe);

int mt6989_afe_dram_request(struct device *dev);
int mt6989_afe_dram_release(struct device *dev);

int mt6989_apll1_enable(struct mtk_base_afe *afe);
void mt6989_apll1_disable(struct mtk_base_afe *afe);

int mt6989_apll2_enable(struct mtk_base_afe *afe);
void mt6989_apll2_disable(struct mtk_base_afe *afe);

int mt6989_get_apll_rate(struct mtk_base_afe *afe, int apll);
int mt6989_get_apll_by_rate(struct mtk_base_afe *afe, int rate);
int mt6989_get_apll_by_name(struct mtk_base_afe *afe, const char *name);

extern void aud_intbus_mux_sel(unsigned int aud_idx);

/* these will be replaced by using CCF */
int mt6989_mck_enable(struct mtk_base_afe *afe, int mck_id, int rate);
int mt6989_mck_disable(struct mtk_base_afe *afe, int mck_id);

int mt6989_set_audio_int_bus_parent(struct mtk_base_afe *afe,
				    int clk_id);

#endif
