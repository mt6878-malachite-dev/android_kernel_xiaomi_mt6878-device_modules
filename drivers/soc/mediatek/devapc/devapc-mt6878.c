// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2022 MediaTek Inc.
 */

#include <linux/bug.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/io.h>

#include "devapc-mt6878.h"

static const struct mtk_device_num mtk6878_devices_num[] = {
	{SLAVE_TYPE_INFRA, VIO_SLAVE_NUM_INFRA, IRQ_TYPE_INFRA},
	{SLAVE_TYPE_INFRA1, VIO_SLAVE_NUM_INFRA1, IRQ_TYPE_INFRA},
	{SLAVE_TYPE_PERI_PAR, VIO_SLAVE_NUM_PERI_PAR, IRQ_TYPE_INFRA},
	{SLAVE_TYPE_VLP, VIO_SLAVE_NUM_VLP, IRQ_TYPE_VLP},
#if ENABLE_DEVAPC_ADSP
	{SLAVE_TYPE_ADSP, VIO_SLAVE_NUM_ADSP, IRQ_TYPE_ADSP},
#else
	{-1, 0, -1},
#endif
	{SLAVE_TYPE_MMINFRA, VIO_SLAVE_NUM_MMINFRA, IRQ_TYPE_MMINFRA},
	{SLAVE_TYPE_MMUP, VIO_SLAVE_NUM_MMUP, IRQ_TYPE_MMUP},
	{SLAVE_TYPE_GPU, VIO_SLAVE_NUM_GPU, IRQ_TYPE_GPU},
};

static const struct INFRAAXI_ID_INFO infra_mi_id_to_master[] = {
	{"ADSPSYS_M1_M",        { 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0 } },
	{"CONN_M",              { 0, 0, 1, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0 } },
	{"CQDMA_M",             { 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0 } },
	{"DEBUG_M",             { 0, 0, 0, 1, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0 } },
	{"GPU_EB_M",            { 0, 0, 0, 1, 0, 0, 0, 1, 0, 2, 2, 2, 2, 2, 2, 2 } },
	{"CPUM_M",              { 0, 0, 0, 1, 0, 0, 1, 1, 0, 2, 0, 0, 0, 0, 0, 0 } },
	{"DXCC_M",              { 0, 0, 0, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 0, 0, 0 } },
	{"MCUPM_M",             { 0, 0, 0, 1, 0, 0, 1, 0, 1, 2, 2, 2, 2, 2, 2, 0 } },
	{"MCUPM_M",             { 0, 0, 1, 1, 1, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0 } },
	{"VLPSYS_M",            { 0, 0, 1, 1, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0 } },
	{"DPMAIF_M",            { 0, 0, 0, 0, 1, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0 } },
	{"HWCCF_M@APB",         { 0, 0, 1, 0, 1, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0 } },
	{"THERM_M@AHB-M",       { 0, 0, 1, 0, 1, 0, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0 } },
	{"CCU_M@AHB-S",         { 0, 0, 1, 0, 1, 0, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0 } },
	{"THERM_M2@AHB-M",      { 0, 0, 1, 0, 1, 0, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0 } },
	{"SSR_M",               { 0, 0, 0, 1, 1, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0 } },
	{"PERI2INFRA1_M",       { 0, 0, 0, 1, 1, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 0 } },
	{"MSDC0_M",             { 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 2, 2, 0, 2, 0 } },
	{"SSUSB_M",             { 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 2, 2, 0, 2, 0 } },
	{"APDMA_INT_M",         { 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 2, 2, 2, 2, 0 } },
	{"AUDIO_M",             { 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 2, 2, 0, 2, 0 } },
	{"SPI3_M",              { 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 2, 2, 0, 2, 0 } },
	{"SPI4_M",              { 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 2, 2, 0, 2, 0 } },
	{"SPI5_M",              { 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 2, 2, 0, 2, 0 } },
	{"SPI6_M",              { 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 2, 2, 0, 2, 0 } },
	{"SPI7_M",              { 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 2, 2, 0, 2, 0 } },
	{"APDMA_EXT_M",         { 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 2, 2, 2, 2, 0 } },
	{"SPI0_M",              { 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 2, 2, 0, 2, 0 } },
	{"SPI1_M",              { 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 2, 2, 0, 2, 0 } },
	{"SPI2_M",              { 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 2, 2, 0, 2, 0 } },
	{"PWM_M",               { 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 2, 2, 0, 2, 0 } },
	{"MSDC1_M",             { 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 2, 2, 0, 2, 0 } },
	{"UFS0_M",              { 0, 0, 0, 1, 1, 0, 1, 1, 1, 2, 2, 0, 0, 0, 2, 0 } },
	{"IPU_M@APB",           { 0, 0, 0, 0, 0, 1, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0 } },
	{"INFRA_BUS_HRE_M@APB", { 0, 0, 0, 0, 0, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0 } },
	{"MCU_AP_M",            { 1, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0 } },
	{"MM2SLB1_M",           { 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0 } },
	{"HFRP2INFRA_M",        { 0, 1, 0, 1, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0 } },
	{"GCE_D_M",             { 0, 1, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
	{"GCE_M_M",             { 0, 1, 1, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
	{"MD_AP_M",             { 1, 1, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
};

static const char * const mminfra_domain[] = {
	"AP",
	"SSPM",
	"CCU",
	"SCP",
	"GCE",
	"GZ",
	"MMuP",
	"others",
};

static const char *infra_mi_trans(uint32_t bus_id)
{
	int master_count = ARRAY_SIZE(infra_mi_id_to_master);
	const char *master = "UNKNOWN_MASTER_FROM_INFRA";
	int i, j;

	for (i = 0; i < master_count; i++) {
		for (j = 0; j < INFRAAXI_MI_BIT_LENGTH; j++) {
			if (infra_mi_id_to_master[i].bit[j] == 2)
				continue;
			if (((bus_id >> j) & 0x1) ==
					infra_mi_id_to_master[i].bit[j])
				continue;
			break;
		}
		if (j == INFRAAXI_MI_BIT_LENGTH) {
			pr_info(PFX "%s %s %s\n",
				"catch it from INFRAAXI_MI",
				"Master is:",
				infra_mi_id_to_master[i].master);
			master = infra_mi_id_to_master[i].master;
		}
	}

	return master;
}

static const char *mt6878_bus_id_to_master(uint32_t bus_id, uint32_t vio_addr,
		int slave_type, int shift_sta_bit, uint32_t domain)
{
	pr_debug(PFX "%s:0x%x, %s:0x%x, %s:0x%x, %s:%d\n",
		"bus_id", bus_id, "vio_addr", vio_addr,
		"slave_type", slave_type,
		"shift_sta_bit", shift_sta_bit);

	if (vio_addr <= SRAM_END_ADDR) {
		pr_info(PFX "vio_addr is from on-chip SRAMROM\n");
		return infra_mi_trans(bus_id);
	} else if (slave_type == SLAVE_TYPE_VLP) {
		/* mi3 */
		if ((vio_addr >= VLP_SCP_START_ADDR) && (vio_addr <= VLP_SCP_END_ADDR)) {
			if ((bus_id & 0x3) == 0x0)
				return "SSPM_M";
			else if ((bus_id & 0x3) == 0x1)
				return "SPM_M/DPMSR_AHB_M";
			else if ((bus_id & 0x3) == 0x2)
				return infra_mi_trans(bus_id >> 2);
			else
				return "UNKNOWN_MASTER_TO_SCP";
		/* mi1 */
		} else if ((vio_addr <= VLP_INFRA_END) ||
			(vio_addr >= VLP_INFRA_1_START && vio_addr <= VLP_INFRA_1_END)) {
			if ((bus_id & 0x3) == 0x0)
				return "SCP_M";
			else if ((bus_id & 0x3) == 0x1)
				return "SSPM_M";
			else if ((bus_id & 0x3) == 0x2)
				return "SPM_M/DPMSR_AHB_M";
			else
				return "UNKNOWN_MASTER_TO_INFRA";
		/* mi2 */
		} else {
			if ((bus_id & 0x3) == 0x0)
				return "SCP_M";
			else if ((bus_id & 0x3) == 0x1)
				return "SSPM_M";
			else if ((bus_id & 0x3) == 0x2)
				return "SPM_M/DPMSR_AHB_M";
			else
				return infra_mi_trans(bus_id >> 2);
		}
	} else if (slave_type == SLAVE_TYPE_MMINFRA) {
		/* ISP slave */
		if (((vio_addr >= IMG_START_ADDR) && (vio_addr <= IMG_END_ADDR)) ||
			((vio_addr >= CAM_START_ADDR) && (vio_addr <= CAM_END_ADDR))) {
			if ((bus_id & 0x1) == 0x0)
				return "GCEM_direct";
			else if ((bus_id & 0xf) == 0x1)
				return infra_mi_trans(bus_id >> 4);
			else if ((bus_id & 0xf) == 0x3)
				return "MMINFRA_HRE";
			else if ((bus_id & 0xf) == 0x5)
				return "GCED";
			else if ((bus_id & 0xf) == 0x7)
				return "GCEM";
			else if ((bus_id & 0xf) == 0x9)
				return "HFRP";
			else
				return mminfra_domain[domain];
		/* VENC/VDEC slave*/
		} else if ((vio_addr >= CODEC_START_ADDR) && (vio_addr <= CODEC_END_ADDR)) {
			if ((bus_id & 0x1) == 0x0)
				return "HFRP_direct";
			else if ((bus_id & 0xf) == 0x1)
				return infra_mi_trans(bus_id >> 4);
			else if ((bus_id & 0xf) == 0x3)
				return "MMINFRA_HRE";
			else if ((bus_id & 0xf) == 0x5)
				return "GCED";
			else if ((bus_id & 0xf) == 0x7)
				return "GCEM";
			else if ((bus_id & 0xf) == 0x9)
				return "HFRP";
			else
				return mminfra_domain[domain];
		/* DISP/MML */
		} else if (((vio_addr >= DISP_START_ADDR) && (vio_addr <= DISP_END_ADDR)) ||
			((vio_addr >= MML_START_ADDR) && (vio_addr <= MML_END_ADDR))) {
			if ((bus_id & 0x1) == 0x0)
				return "GCED_direct";
			else if ((bus_id & 0xf) == 0x1)
				return infra_mi_trans(bus_id >> 4);
			else if ((bus_id & 0xf) == 0x3)
				return "MMINFRA_HRE";
			else if ((bus_id & 0xf) == 0x5)
				return "GCED";
			else if ((bus_id & 0xf) == 0x7)
				return "GCEM";
			else if ((bus_id & 0xf) == 0x9)
				return "HFRP";
			else
				return mminfra_domain[domain];
		/* other mminfra slave*/
		} else {
			if ((bus_id & 0x7) == 0x0)
				return infra_mi_trans(bus_id >> 3);
			else if ((bus_id & 0x7) == 0x1)
				return "MMINFRA_HRE";
			else if ((bus_id & 0x7) == 0x2)
				return "GCED";
			else if ((bus_id & 0x7) == 0x3)
				return "GCEM";
			else if ((bus_id & 0xf) == 0x4)
				return "HFRP";
			else
				return mminfra_domain[domain];
		}
	} else if (slave_type == SLAVE_TYPE_MMUP) {
		return mminfra_domain[domain];
	} else if (slave_type == SLAVE_TYPE_GPU) {
		/* PD_BUS */
		if ((vio_addr >= GPU_PD_START) && (vio_addr <= GPU_PD_END)) {
			if (domain == 0x6) {
				if ((bus_id & 0x3) == 0x0)
					return "GPUEB_RV33_P";
				else if ((bus_id & 0x3) == 0x1)
					return "GPUEB_RV33_D";
				else if ((bus_id & 0x3) == 0x2)
					return "GPUEB_DMA";
				else
					return "UNKNOWN_MASTER_TO_GPU";
			} else {
				return infra_mi_trans(bus_id);
			}
		/* AO_BUS */
		} else {
			if ((bus_id & 0x3f) == 0x2a) {
				if (((bus_id >> 8) & 0x3) == 0x0)
					return "GPUEB_RV33_P";
				else if (((bus_id >> 8) & 0x3) == 0x1)
					return "GPUEB_RV33_D";
				else if (((bus_id >> 8) & 0x3) == 0x2)
					return "GPUEB_DMA";
				else
					return "UNKNOWN_MASTER_TO_GPU";
			} else {
				return infra_mi_trans(bus_id);
			}
		}
	}

	return infra_mi_trans(bus_id);
}

/* violation index corresponds to subsys */
const char *index_to_subsys(int slave_type, uint32_t vio_index,
		uint32_t vio_addr)
{
	int i;

	pr_debug(PFX "%s %s %d, %s %d, %s 0x%x\n",
			__func__,
			"slave_type", slave_type,
			"vio_index", vio_index,
			"vio_addr", vio_addr);

	/* Filter by violation index */
	if (slave_type == SLAVE_TYPE_INFRA) {
		for (i = 0; i < VIO_SLAVE_NUM_INFRA; i++) {
			if (vio_index == mt6878_devices_infra[i].vio_index)
				return mt6878_devices_infra[i].device;
		}
	} else if (slave_type == SLAVE_TYPE_INFRA1) {
		for (i = 0; i < VIO_SLAVE_NUM_INFRA1; i++) {
			if (vio_index == mt6878_devices_infra1[i].vio_index)
				return mt6878_devices_infra1[i].device;
		}
	} else if (slave_type == SLAVE_TYPE_PERI_PAR) {
		for (i = 0; i < VIO_SLAVE_NUM_PERI_PAR; i++) {
			if (vio_index == mt6878_devices_peri_par[i].vio_index)
				return mt6878_devices_peri_par[i].device;
		}
	} else if (slave_type == SLAVE_TYPE_VLP) {
		for (i = 0; i < VIO_SLAVE_NUM_VLP; i++) {
			if (vio_index == mt6878_devices_vlp[i].vio_index)
				return mt6878_devices_vlp[i].device;
		}
#if ENABLE_DEVAPC_ADSP
	} else if (slave_type == SLAVE_TYPE_ADSP) {
		for (i = 0; i < VIO_SLAVE_NUM_ADSP; i++) {
			if (vio_index == mt6878_devices_adsp[i].vio_index)
				return mt6878_devices_adsp[i].device;
		}
#endif
	} else if (slave_type == SLAVE_TYPE_MMINFRA) {
		for (i = 0; i < VIO_SLAVE_NUM_MMINFRA; i++) {
			if (vio_index == mt6878_devices_mminfra[i].vio_index)
				return mt6878_devices_mminfra[i].device;
		}
	} else if (slave_type == SLAVE_TYPE_MMUP) {
		for (i = 0; i < VIO_SLAVE_NUM_MMUP; i++) {
			if (vio_index == mt6878_devices_mmup[i].vio_index)
				return mt6878_devices_mmup[i].device;
		}
	} else if (slave_type == SLAVE_TYPE_GPU) {
		for (i = 0; i < VIO_SLAVE_NUM_GPU; i++) {
			if (vio_index == mt6878_devices_gpu[i].vio_index)
				return mt6878_devices_gpu[i].device;
		}
	}

	return "OUT_OF_BOUND";
}

static void mm2nd_vio_handler(void __iomem *infracfg,
			      struct mtk_devapc_vio_info *vio_info,
			      bool mdp_vio, bool disp2_vio, bool mmsys_vio)
{
	uint32_t vio_sta, vio_dbg, rw;
	uint32_t vio_sta_num;
	uint32_t vio0_offset;
	char mm_str[64] = { 0};
	void __iomem *reg;
	int i;

	if (!infracfg) {
		pr_err(PFX "%s, param check failed, infracfg ptr is NULL\n",
				__func__);
		return;
	}

	if (mdp_vio) {
		vio_sta_num = INFRACFG_MDP_VIO_STA_NUM;
		vio0_offset = INFRACFG_MDP_SEC_VIO0_OFFSET;

		strncpy(mm_str, "INFRACFG_MDP_SEC_VIO", sizeof(mm_str));

	} else if (disp2_vio) {
		vio_sta_num = INFRACFG_DISP2_VIO_STA_NUM;
		vio0_offset = INFRACFG_DISP2_SEC_VIO0_OFFSET;

		strncpy(mm_str, "INFRACFG_DISP2_SEC_VIO", sizeof(mm_str));

	} else if (mmsys_vio) {
		vio_sta_num = INFRACFG_MM_VIO_STA_NUM;
		vio0_offset = INFRACFG_MM_SEC_VIO0_OFFSET;

		strncpy(mm_str, "INFRACFG_MM_SEC_VIO", sizeof(mm_str));

	} else {
		pr_err(PFX "%s: param check failed, %s:%s, %s:%s, %s:%s\n",
				__func__,
				"mdp_vio", mdp_vio ? "true" : "false",
				"disp2_vio", disp2_vio ? "true" : "false",
				"mmsys_vio", mmsys_vio ? "true" : "false");
		return;
	}

	/* Get mm2nd violation status */
	for (i = 0; i < vio_sta_num; i++) {
		reg = infracfg + vio0_offset + i * 4;
		vio_sta = readl(reg);
		if (vio_sta)
			pr_info(PFX "MM 2nd violation: %s%d:0x%x\n",
					mm_str, i, vio_sta);
	}

	/* Get mm2nd violation address */
	reg = infracfg + vio0_offset + i * 4;
	vio_info->vio_addr = readl(reg);

	/* Get mm2nd violation information */
	reg = infracfg + vio0_offset + (i + 1) * 4;
	vio_dbg = readl(reg);

	vio_info->domain_id = (vio_dbg & INFRACFG_MM2ND_VIO_DOMAIN_MASK) >>
		INFRACFG_MM2ND_VIO_DOMAIN_SHIFT;

	vio_info->master_id = (vio_dbg & INFRACFG_MM2ND_VIO_ID_MASK) >>
		INFRACFG_MM2ND_VIO_ID_SHIFT;

	rw = (vio_dbg & INFRACFG_MM2ND_VIO_RW_MASK) >>
		INFRACFG_MM2ND_VIO_RW_SHIFT;
	vio_info->read = (rw == 0);
	vio_info->write = (rw == 1);
}

static uint32_t mt6878_shift_group_get(int slave_type, uint32_t vio_idx)
{
	return 31;
}

void devapc_catch_illegal_range(phys_addr_t phys_addr, size_t size)
{
	phys_addr_t test_pa = 0x17a54c50;

	/*
	 * Catch BROM addr mapped
	 */
	if (phys_addr < SRAM_START_ADDR) {
		pr_err(PFX "%s %s:(%pa), %s:(0x%lx)\n",
				"catch BROM address mapped!",
				"phys_addr", &phys_addr,
				"size", size);
		BUG_ON(1);
	}

	if ((phys_addr <= test_pa) && (phys_addr + size > test_pa)) {
		pr_err(PFX "%s %s:(%pa), %s:(0x%lx), %s:(%pa)\n",
				"catch VENCSYS address mapped!",
				"phys_addr", &phys_addr,
				"size", size, "test_pa", &test_pa);
		BUG_ON(1);
	}
}

static struct mtk_devapc_dbg_status mt6878_devapc_dbg_stat = {
	.enable_ut = PLAT_DBG_UT_DEFAULT,
	.enable_KE = PLAT_DBG_KE_DEFAULT,
	.enable_AEE = PLAT_DBG_AEE_DEFAULT,
	.enable_WARN = PLAT_DBG_WARN_DEFAULT,
	.enable_dapc = PLAT_DBG_DAPC_DEFAULT,
};

static const char * const slave_type_to_str[] = {
	"SLAVE_TYPE_INFRA",
	"SLAVE_TYPE_INFRA1",
	"SLAVE_TYPE_PERI_PAR",
	"SLAVE_TYPE_VLP",
	"SLAVE_TYPE_ADSP",
	"SLAVE_TYPE_MMINFRA",
	"SLAVE_TYPE_MMUP",
	"SLAVE_TYPE_GPU",
	"WRONG_SLAVE_TYPE",
};

static int mtk_vio_mask_sta_num[] = {
	VIO_MASK_STA_NUM_INFRA,
	VIO_MASK_STA_NUM_INFRA1,
	VIO_MASK_STA_NUM_PERI_PAR,
	VIO_MASK_STA_NUM_VLP,
	VIO_MASK_STA_NUM_ADSP,
	VIO_MASK_STA_NUM_MMINFRA,
	VIO_MASK_STA_NUM_MMUP,
	VIO_MASK_STA_NUM_GPU,
};

static struct mtk_devapc_vio_info mt6878_devapc_vio_info = {
	.vio_mask_sta_num = mtk_vio_mask_sta_num,
	.sramrom_vio_idx = SRAMROM_VIO_INDEX,
	.mdp_vio_idx = MDP_VIO_INDEX,
	.disp2_vio_idx = DISP2_VIO_INDEX,
	.mmsys_vio_idx = MMSYS_VIO_INDEX,
	.sramrom_slv_type = SRAMROM_SLAVE_TYPE,
	.mm2nd_slv_type = MM2ND_SLAVE_TYPE,
};

static const struct mtk_infra_vio_dbg_desc mt6878_vio_dbgs = {
	.vio_dbg_mstid = INFRA_VIO_DBG_MSTID,
	.vio_dbg_mstid_start_bit = INFRA_VIO_DBG_MSTID_START_BIT,
	.vio_dbg_dmnid = INFRA_VIO_DBG_DMNID,
	.vio_dbg_dmnid_start_bit = INFRA_VIO_DBG_DMNID_START_BIT,
	.vio_dbg_w_vio = INFRA_VIO_DBG_W_VIO,
	.vio_dbg_w_vio_start_bit = INFRA_VIO_DBG_W_VIO_START_BIT,
	.vio_dbg_r_vio = INFRA_VIO_DBG_R_VIO,
	.vio_dbg_r_vio_start_bit = INFRA_VIO_DBG_R_VIO_START_BIT,
	.vio_addr_high = INFRA_VIO_ADDR_HIGH,
	.vio_addr_high_start_bit = INFRA_VIO_ADDR_HIGH_START_BIT,
};

static const struct mtk_sramrom_sec_vio_desc mt6878_sramrom_sec_vios = {
	.vio_id_mask = SRAMROM_SEC_VIO_ID_MASK,
	.vio_id_shift = SRAMROM_SEC_VIO_ID_SHIFT,
	.vio_domain_mask = SRAMROM_SEC_VIO_DOMAIN_MASK,
	.vio_domain_shift = SRAMROM_SEC_VIO_DOMAIN_SHIFT,
	.vio_rw_mask = SRAMROM_SEC_VIO_RW_MASK,
	.vio_rw_shift = SRAMROM_SEC_VIO_RW_SHIFT,
};

static const uint32_t mt6878_devapc_pds[] = {
	PD_VIO_MASK_OFFSET,
	PD_VIO_STA_OFFSET,
	PD_VIO_DBG0_OFFSET,
	PD_VIO_DBG1_OFFSET,
	PD_VIO_DBG2_OFFSET,
	PD_APC_CON_OFFSET,
	PD_SHIFT_STA_OFFSET,
	PD_SHIFT_SEL_OFFSET,
	PD_SHIFT_CON_OFFSET,
	PD_VIO_DBG3_OFFSET,
};

static struct mtk_devapc_soc mt6878_data = {
	.dbg_stat = &mt6878_devapc_dbg_stat,
	.slave_type_arr = slave_type_to_str,
	.slave_type_num = SLAVE_TYPE_NUM,
	.device_info[SLAVE_TYPE_INFRA] = mt6878_devices_infra,
	.device_info[SLAVE_TYPE_INFRA1] = mt6878_devices_infra1,
	.device_info[SLAVE_TYPE_PERI_PAR] = mt6878_devices_peri_par,
	.device_info[SLAVE_TYPE_VLP] = mt6878_devices_vlp,
#if ENABLE_DEVAPC_ADSP
	.device_info[SLAVE_TYPE_ADSP] = mt6878_devices_adsp,
#endif
	.device_info[SLAVE_TYPE_MMINFRA] = mt6878_devices_mminfra,
	.device_info[SLAVE_TYPE_MMUP] = mt6878_devices_mmup,
	.device_info[SLAVE_TYPE_GPU] = mt6878_devices_gpu,
	.ndevices = mtk6878_devices_num,
	.vio_info = &mt6878_devapc_vio_info,
	.vio_dbgs = &mt6878_vio_dbgs,
	.sramrom_sec_vios = &mt6878_sramrom_sec_vios,
	.devapc_pds = mt6878_devapc_pds,
	.irq_type_num = IRQ_TYPE_NUM,
	.subsys_get = &index_to_subsys,
	.master_get = &mt6878_bus_id_to_master,
	.mm2nd_vio_handler = &mm2nd_vio_handler,
	.shift_group_get = mt6878_shift_group_get,
};

static const struct of_device_id mt6878_devapc_dt_match[] = {
	{ .compatible = "mediatek,mt6878-devapc" },
	{},
};

static const struct dev_pm_ops devapc_dev_pm_ops = {
	.suspend_noirq	= devapc_suspend_noirq,
	.resume_noirq = devapc_resume_noirq,
};

static int mt6878_devapc_probe(struct platform_device *pdev)
{
	return mtk_devapc_probe(pdev, &mt6878_data);
}

static int mt6878_devapc_remove(struct platform_device *dev)
{
	return mtk_devapc_remove(dev);
}

static struct platform_driver mt6878_devapc_driver = {
	.probe = mt6878_devapc_probe,
	.remove = mt6878_devapc_remove,
	.driver = {
		.name = KBUILD_MODNAME,
		.of_match_table = mt6878_devapc_dt_match,
		.pm = &devapc_dev_pm_ops,
	},
};

module_platform_driver(mt6878_devapc_driver);

MODULE_DESCRIPTION("Mediatek MT6878 Device APC Driver");
MODULE_AUTHOR("Louis Yeh <louis-cy.yeh@mediatek.com>");
MODULE_LICENSE("GPL");
