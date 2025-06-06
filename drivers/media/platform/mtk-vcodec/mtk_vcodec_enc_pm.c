// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2016 MediaTek Inc.
 * Author: Tiffany Lin <tiffany.lin@mediatek.com>
 */

#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/pm_runtime.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <soc/mediatek/smi.h>

#include "mtk_vcodec_enc_pm.h"
#include "mtk_vcodec_enc_pm_plat.h"
#include "mtk_vcodec_util.h"
#include "mtk_vcu.h"
//#include "smi_public.h"

#include <mtk_iommu.h>

#ifdef CONFIG_MTK_PSEUDO_M4U
#include <mach/mt_iommu.h>
#include "mach/pseudo_m4u.h"
#include "smi_port.h"
#endif

#if IS_ENABLED(CONFIG_MTK_IOMMU_MISC_DBG)
#include "iommu_debug.h"
#endif

/*
 * SLB activate callback, cb calls when high priority user to release SLB,
 * venc request SLB depand on whether slb is required.
 * return value
 *    0: venc is not required slb
 *    1: venc is required slb
 */
int mtk_slb_user_activate_request(struct slbc_data *d)
{
	int ret = 1;

	atomic_set(&mtk_venc_slb_cb.request_slbc, 1);
	mtk_v4l2_debug(0, "slb_cb %d/%d cnt %d/%d",
		atomic_read(&mtk_venc_slb_cb.release_slbc),
		atomic_read(&mtk_venc_slb_cb.request_slbc),
		atomic_read(&mtk_venc_slb_cb.perf_used_cnt),
		atomic_read(&mtk_venc_slb_cb.later_cnt));

	return ret;
}

/*
 * SLB deactivate callback, cb calls when high priority user to request SLB,
 * venc would to release SLB depand on whether normal recording or high
 * performance recording.
 * return value
 *    0: venc unable to release SLB when high performance recording
 *    1: venc able to release SLB when normal recording
 */
int mtk_slb_user_deactivate_request(struct slbc_data *d)
{
	int ret;

	if (atomic_read(&mtk_venc_slb_cb.perf_used_cnt) > 0) {
		mtk_v4l2_debug(0, "venc unable to release SLB");
		ret = 0;
	} else {
		mtk_v4l2_debug(0, "venc able to release SLB");
		atomic_set(&mtk_venc_slb_cb.release_slbc, 1);
		ret = 1;
	}

	mtk_v4l2_debug(0, "slb_cb %d/%d cnt %d/%d",
		atomic_read(&mtk_venc_slb_cb.release_slbc),
		atomic_read(&mtk_venc_slb_cb.request_slbc),
		atomic_read(&mtk_venc_slb_cb.perf_used_cnt),
		atomic_read(&mtk_venc_slb_cb.later_cnt));

	return ret;
}

void mtk_venc_init_ctx_pm(struct mtk_vcodec_ctx *ctx)
{
	struct slbc_ops slbc_ops;

	ctx->sram_data.uid = UID_MM_VENC;
	ctx->sram_data.type = TP_BUFFER;
	ctx->sram_data.size = 0;
	ctx->sram_data.flag = FG_POWER;
	slbc_ops.data = &ctx->sram_data;
	slbc_ops.activate = mtk_slb_user_activate_request;
	slbc_ops.deactivate = mtk_slb_user_deactivate_request;

	if (slbc_register_activate_ops(&slbc_ops) != 0) {
		pr_info("register cb slbc_register_activate_ops fail\n");
	}
	if (slbc_request(&ctx->sram_data) >= 0) {
		ctx->use_slbc = 1;
		ctx->slbc_addr = (unsigned int)(unsigned long)ctx->sram_data.paddr;
	} else {
		pr_info("slbc_request fail\n");
		ctx->use_slbc = 0;
	}
	if (ctx->slbc_addr % 256 != 0 || ctx->slbc_addr == 0) {
		pr_info("slbc_addr error 0x%x\n", ctx->slbc_addr);
		ctx->use_slbc = 0;
	}

	if (ctx->use_slbc == 1 && ctx->sram_data.ref == 1) {
		atomic_set(&mtk_venc_slb_cb.release_slbc, 0);
		atomic_set(&mtk_venc_slb_cb.request_slbc, 0);
		atomic_set(&mtk_venc_slb_cb.perf_used_cnt, 0);
		atomic_set(&mtk_venc_slb_cb.later_cnt, 0);
	}

	pr_info("slbc_request %d, 0x%x, 0x%lx, ref %d\n",
	ctx->use_slbc, ctx->slbc_addr, (unsigned long)ctx->sram_data.paddr, ctx->sram_data.ref);

	mtk_v4l2_debug(0, "slb_cb %d/%d cnt %d/%d",
		atomic_read(&mtk_venc_slb_cb.release_slbc),
		atomic_read(&mtk_venc_slb_cb.request_slbc),
		atomic_read(&mtk_venc_slb_cb.perf_used_cnt),
		atomic_read(&mtk_venc_slb_cb.later_cnt));
}

int mtk_vcodec_init_enc_pm(struct mtk_vcodec_dev *mtkdev)
{
	int ret = 0;
#ifndef FPGA_PWRCLK_API_DISABLE
	struct device_node *node;
	struct platform_device *pdev;
	struct device *dev;
	struct mtk_vcodec_pm *pm;
	int larb_index;
	unsigned int clk_id = 0;
	const char *clk_name;
	struct mtk_venc_clks_data *clks_data;

	pdev = mtkdev->plat_dev;
	pm = &mtkdev->pm;
	memset(pm, 0, sizeof(struct mtk_vcodec_pm));
	pm->mtkdev = mtkdev;
	pm->dev = &pdev->dev;
	clks_data = &pm->venc_clks_data;
	dev = &pdev->dev;

	node = of_parse_phandle(dev->of_node, "mediatek,larbs", 0);
	if (!node)
		mtk_v4l2_err("no mediatek,larb found");
	for (larb_index = 0; larb_index < MTK_VENC_MAX_LARB_COUNT; larb_index++) {
		node = of_parse_phandle(pdev->dev.of_node, "mediatek,larbs", larb_index);
		if (!node)
			break;

		pdev = of_find_device_by_node(node);
		if (WARN_ON(!pdev)) {
			of_node_put(node);
			return -1;
		}
		pm->larbvencs[larb_index] = &pdev->dev;
		mtk_v4l2_debug(8, "larbvencs[%d] = %p", larb_index, pm->larbvencs[larb_index]);
		pdev = mtkdev->plat_dev;

		if (!device_link_add(&pdev->dev, pm->larbvencs[larb_index],
					DL_FLAG_PM_RUNTIME | DL_FLAG_STATELESS)) {
			mtk_v4l2_err("%s larb(%d) device link fail\n", __func__, larb_index);
			return -1;
		}
	}

	memset(clks_data, 0x00, sizeof(struct mtk_venc_clks_data));
	while (!of_property_read_string_index(
			pdev->dev.of_node, "clock-names", clk_id, &clk_name)) {
		mtk_v4l2_debug(8, "init clock, id: %d, name: %s", clk_id, clk_name);
		pm->venc_clks[clk_id] = devm_clk_get(&pdev->dev, clk_name);
		if (IS_ERR(pm->venc_clks[clk_id])) {
			mtk_v4l2_err(
				"[VCODEC][ERROR] Unable to devm_clk_get id: %d, name: %s\n",
				clk_id, clk_name);
			return PTR_ERR(pm->venc_clks[clk_id]);
		}
		clks_data->core_clks[clks_data->core_clks_len].clk_id = clk_id;
		clks_data->core_clks[clks_data->core_clks_len].clk_name = clk_name;
		clks_data->core_clks_len++;
		clk_id++;
	}

	pm_runtime_enable(&pdev->dev);
#endif
	return ret;
}

void mtk_vcodec_release_enc_pm(struct mtk_vcodec_dev *mtkdev)
{
#if ENC_EMI_BW
	/* do nothing */
#endif
	pm_runtime_disable(mtkdev->pm.dev);
}

void mtk_venc_deinit_ctx_pm(struct mtk_vcodec_ctx *ctx)
{
	if (ctx->use_slbc == 1) {
		pr_debug("slbc_release, %p\n", &ctx->sram_data);
		slbc_release(&ctx->sram_data);

		pr_info("slbc_release ref %d\n", ctx->sram_data.ref);
		if (ctx->sram_data.ref == 0) {
			atomic_set(&mtk_venc_slb_cb.release_slbc, 0);
			atomic_set(&mtk_venc_slb_cb.request_slbc, 0);
			atomic_set(&mtk_venc_slb_cb.perf_used_cnt, 0);
			atomic_set(&mtk_venc_slb_cb.later_cnt, 0);
		}
		if (ctx->enc_params.slbc_encode_performance)
			atomic_dec(&mtk_venc_slb_cb.perf_used_cnt);
	} else {
		if (ctx->later_cnt_once)
			atomic_dec(&mtk_venc_slb_cb.later_cnt);
	}

	mtk_v4l2_debug(0, "slb_cb %d/%d perf %d cnt %d/%d/%d slb_cpu_used_perf %d",
		atomic_read(&mtk_venc_slb_cb.release_slbc),
		atomic_read(&mtk_venc_slb_cb.request_slbc),
		ctx->enc_params.slbc_encode_performance,
		atomic_read(&mtk_venc_slb_cb.perf_used_cnt),
		atomic_read(&mtk_venc_slb_cb.later_cnt),
		ctx->later_cnt_once,
		ctx->enc_params.slbc_cpu_used_performance);

}

void mtk_vcodec_enc_pw_on(struct mtk_vcodec_pm *pm)
{
	int larb_index;
	int ret;

	for (larb_index = 0; larb_index < MTK_VENC_MAX_LARB_COUNT; larb_index++) {
		if (pm->larbvencs[larb_index]) {
			ret = pm_runtime_resume_and_get(pm->larbvencs[larb_index]);
			if (ret)
				mtk_v4l2_err("Failed to get venc larb. index: %d",
					larb_index);
		}
	}
}

void mtk_vcodec_enc_pw_off(struct mtk_vcodec_pm *pm)
{
	int larb_index;

	for (larb_index = 0; larb_index < MTK_VENC_MAX_LARB_COUNT; larb_index++) {
		if (pm->larbvencs[larb_index])
			pm_runtime_put_sync(pm->larbvencs[larb_index]);
	}
}

void mtk_vcodec_enc_clock_on(struct mtk_vcodec_ctx *ctx, int core_id)
{
	struct mtk_vcodec_pm *pm = &ctx->dev->pm;
	int ret;
	int i, larb_port_num, larb_id;
#ifdef CONFIG_MTK_PSEUDO_M4U
	struct M4U_PORT_STRUCT port;
#endif
	int j;
	struct mtk_venc_clks_data *clks_data;
	struct mtk_vcodec_dev *dev = NULL;
	unsigned long flags;
	unsigned int clk_id = 0;
	unsigned int smi_start_time, smi_end_time;
	unsigned int ccf_start_time, ccf_end_time;
	unsigned int slbc_start_time, slbc_end_time;

	dev = ctx->dev;

#ifndef FPGA_PWRCLK_API_DISABLE
	time_check_start(MTK_FMT_ENC, core_id);

	clks_data = &pm->venc_clks_data;
	smi_start_time = jiffies_to_msecs(jiffies);
	mtk_vcodec_enc_pw_on(&ctx->dev->pm);
	smi_end_time = jiffies_to_msecs(jiffies);
	ccf_start_time = jiffies_to_msecs(jiffies);
	if (core_id == MTK_VENC_CORE_0 ||
		core_id == MTK_VENC_CORE_1 ||
		core_id == MTK_VENC_CORE_2) {
			// enable core clocks
		for (j = 0; j < clks_data->core_clks_len; j++) {
			clk_id = clks_data->core_clks[j].clk_id;
			ret = clk_prepare_enable(pm->venc_clks[clk_id]);
			if (ret) {
				mtk_v4l2_err("clk_prepare_enable id: %d, name: %s fail %d",
					clk_id, clks_data->core_clks[j].clk_name, ret);
			}
		}
	} else {
		mtk_v4l2_err("invalid core_id %d", core_id);
		time_check_end(MTK_FMT_ENC, core_id, 5);
		return;
	}
	ccf_end_time  = jiffies_to_msecs(jiffies);
	time_check_end(MTK_FMT_ENC, core_id, 5);



#endif

	slbc_start_time = jiffies_to_msecs(jiffies);
	spin_lock_irqsave(&dev->enc_power_lock[core_id], flags);
	dev->enc_is_power_on[core_id] = true;
	spin_unlock_irqrestore(&dev->enc_power_lock[core_id], flags);

	if (ctx->sysram_enable == 1) {
		time_check_start(MTK_FMT_ENC, core_id);
		ret = slbc_power_on(&ctx->sram_data);
		time_check_end(MTK_FMT_ENC, core_id, 50);
	}

	time_check_start(MTK_FMT_ENC, core_id);
	if (core_id == MTK_VENC_CORE_0) {
		larb_port_num = dev->venc_ports[0].total_port_num;
		larb_id = 7;
	} else if (core_id == MTK_VENC_CORE_1) {
		larb_port_num = dev->venc_ports[1].total_port_num;
		larb_id = 8;
	} else {
		larb_port_num = dev->venc_ports[2].total_port_num;
		larb_id = 37;
	}

	//enable slbc port configs
	if (pm->larbvencs[core_id] && ctx->sysram_enable == 1) {
		for (i = 0; i < larb_port_num; i++) {
			if (dev->venc_ports[core_id].ram_type[i] == 1) {
				ret = smi_sysram_enable(pm->larbvencs[core_id],
					MTK_M4U_ID(larb_id, i), true, "LARB_VENC");

				if (ret) {
					mtk_v4l2_err("%#x smi_sysram_enable err: %#x\n",
						i, ret);
					mtk_smi_dbg_hang_detect("VENC SLBC");
				}

			} else if (dev->venc_ports[core_id].ram_type[i] > 1) {
				ret =  mtk_smi_sysram_set(pm->larbvencs[core_id],
					MTK_M4U_ID(larb_id, i),
					(dev->venc_ports[core_id].ram_type[i] & 0xf) << 16,
					"LARB_VENC");
				if (ret) {
					mtk_v4l2_err("%#x mtk_smi_sysram_set err: %#x\n",
						i, ret);
					mtk_smi_dbg_hang_detect("VENC SLBC");
				}
			}
		}
	}
	slbc_end_time = jiffies_to_msecs(jiffies);
	time_check_end(MTK_FMT_ENC, core_id, 50);
	if ((slbc_end_time - smi_start_time) > 2) {
		pr_info("%s %d smi time %u  ccf time %u slbc time %u\n",
		__func__, __LINE__,
		(smi_end_time - smi_start_time),
		(ccf_end_time - ccf_start_time),
		(slbc_end_time - slbc_start_time));
	}
#ifdef CONFIG_MTK_PSEUDO_M4U
	time_check_start(MTK_FMT_ENC, core_id);
	if (core_id == MTK_VENC_CORE_0) {
		larb_port_num = SMI_LARB7_PORT_NUM;
		larb_id = 7;
	}

	//enable 34bits port configs
	for (i = 0; i < larb_port_num; i++) {
		port.ePortID = MTK_M4U_ID(larb_id, i);
		port.Direction = 0;
		port.Distance = 1;
		port.domain = 0;
		port.Security = 0;
		port.Virtuality = 1;
		m4u_config_port(&port);
	}
	time_check_end(MTK_FMT_ENC, core_id, 50);
#endif
}

#ifndef FPGA_PWRCLK_API_DISABLE
static void mtk_venc_hw_break(struct mtk_vcodec_dev *dev)
{
	bool timeout_fg, need_break = false;
	struct timespec64 tv_start;
	struct timespec64 tv_end;
	s32 usec, timeout = 100000;
	unsigned int reg_val, i;
	unsigned long flags;
	void __iomem *reg_base = NULL;
	enum mtk_venc_hw_break_mode break_mode;

	mtk_v4l2_debug(3, "%s +", __func__);

	for (i = 0; i < MTK_VENC_HW_NUM; i++) {
		reg_base = dev->enc_reg_base[i];
		spin_lock_irqsave(&dev->enc_power_lock[i], flags);
		if (reg_base != NULL && dev->enc_is_power_on[i] == 1) {
			reg_val = readl(reg_base + 0x1398);
			if ((reg_val & 0x1f) != 0) {
				mtk_v4l2_err("Core %d hw still active 0x%x\n", i, reg_val);
				need_break = true;
			}
			reg_val = readl(reg_base + 0x1448);
			mtk_v4l2_debug(3, "Core %d hw break mode %d\n", i, (reg_val & 0x7f));
			if ((reg_val & 0x7f) == 127)
				break_mode = MTK_VENC_HW_BREAK_SMI_LOCK_MODE;
			else
				break_mode = MTK_VENC_HW_BREAK_PAUSE_MODE;
		}
		spin_unlock_irqrestore(&dev->enc_power_lock[i], flags);
	}


	if (need_break) {
		//pause first, then poll check
		for (i = 0; i < MTK_VENC_HW_NUM; i++) {
			reg_base = dev->enc_reg_base[i];

			spin_lock_irqsave(&dev->enc_power_lock[i], flags);
			if (reg_base == NULL || dev->enc_is_power_on[i] == 0) {
				spin_unlock_irqrestore(&dev->enc_power_lock[i], flags);
				continue;
			}
			spin_unlock_irqrestore(&dev->enc_power_lock[i], flags);
			if (break_mode == MTK_VENC_HW_BREAK_SMI_LOCK_MODE) {
				writel(1, (reg_base + 0x13B8));
			} else {
				if (i == MTK_VENC_CORE_0)
					writel(0xf0000000, (reg_base + 0x5040));

				writel(1, (reg_base + 0x1228));
			}
		}

		for (i = 0; i < MTK_VENC_HW_NUM; i++) {
			timeout_fg = false;
			reg_base = dev->enc_reg_base[i];

			spin_lock_irqsave(&dev->enc_power_lock[i], flags);
			if (reg_base == NULL || dev->enc_is_power_on[i] == 0) {
				spin_unlock_irqrestore(&dev->enc_power_lock[i], flags);
				continue;
			}
			spin_unlock_irqrestore(&dev->enc_power_lock[i], flags);

			if (break_mode == MTK_VENC_HW_BREAK_SMI_LOCK_MODE) {
				mtk_venc_do_gettimeofday(&tv_start);
				while ((readl(reg_base + 0x13B8) & 0x80000000) != 0) {
					mtk_v4l2_err("wait core %d stop value 0x%x\n"
						, i, readl(reg_base + 0x13B8));
					mtk_venc_do_gettimeofday(&tv_end);
					usec = (tv_end.tv_sec - tv_start.tv_sec) * 1000000
							+ (tv_end.tv_nsec - tv_start.tv_nsec);
					if (usec > timeout) {
						timeout_fg = true;
						break;
					}
				}
			} else {
				mtk_venc_do_gettimeofday(&tv_start);
				while ((readl(reg_base + 0x1228) & 0x7FFFFDFC) != 0) {
					mtk_v4l2_err("wait core %d stop value 0x%x\n"
						, i, readl(reg_base + 0x1228));
					mtk_venc_do_gettimeofday(&tv_end);
					usec = (tv_end.tv_sec - tv_start.tv_sec) * 1000000
							+ (tv_end.tv_nsec - tv_start.tv_nsec);
					if (usec > timeout) {
						timeout_fg = true;
						break;
					}
				}
			}

			if (!timeout_fg)
				continue;

			mtk_v4l2_err("venc core %d hw break timeout =>\n", i);
			mtk_v4l2_err("0x28: 0x%x, 0x2C: 0x%x, 0x64: 0x%x, 0x6C: 0x%x",
				readl(reg_base + 0x28), readl(reg_base + 0x2C),
				readl(reg_base + 0x64), readl(reg_base + 0x6C));
			mtk_v4l2_err("0x00: 0x%x, 0x14: 0x%x, 0x24: 0x%x",
				readl(reg_base), readl(reg_base + 0x14), readl(reg_base + 0x24));
			mtk_v4l2_err("0x70: 0x%x, 0x74: 0x%x, 0x78: 0x%x, 0x7C: 0x%x, 0x80: 0x%x, 0x84: 0x%x",
				readl(reg_base + 0x70), readl(reg_base + 0x74),
				readl(reg_base + 0x78), readl(reg_base + 0x7C),
				readl(reg_base + 0x80), readl(reg_base + 0x84));
			mtk_v4l2_err("0x88: 0x%x, 0x8C: 0x%x, 0x90: 0x%x, 0x94: 0x%x",
				readl(reg_base + 0x88), readl(reg_base + 0x8C),
				readl(reg_base + 0x90), readl(reg_base + 0x94));
			mtk_v4l2_err("0x108: 0x%x",
				readl(reg_base + 0x108));
			mtk_v4l2_err("0x200: 0x%x 0x204: 0x%x, 0x208: 0x%x, 0x20C: 0x%x,",
				readl(reg_base + 0x200), readl(reg_base + 0x204),
				readl(reg_base + 0x208), readl(reg_base + 0x20C));
			mtk_v4l2_err("0x210: 0x%x, 0x214: 0x%x, 0x218: 0x%x, 0x21C: 0x%x, 0x220: 0x%x",
				readl(reg_base + 0x210), readl(reg_base + 0x214),
				readl(reg_base + 0x218), readl(reg_base + 0x21C),
				readl(reg_base + 0x220));
			mtk_v4l2_err("0xE0: 0x%x, 0x1B0: 0x%x, 0x1B4: 0x%x, 0x1170: 0x%x",
				readl(reg_base + 0xE0), readl(reg_base + 0x1B0),
				readl(reg_base + 0x1B4), readl(reg_base + 0x1170));
			mtk_v4l2_err("0x11B8: 0x%x 0x11BC: 0x%x, 0x1280: 0x%x, 0x1284: 0x%x, 0x1288: 0x%x",
				readl(reg_base + 0x11B8), readl(reg_base + 0x11BC),
				readl(reg_base + 0x1280), readl(reg_base + 0x1284),
				readl(reg_base + 0x1288));
			mtk_v4l2_err("0x132C: 0x%x 0x1330: 0x%x, 0x1334: 0x%x, 0x1338: 0x%x,",
				readl(reg_base + 0x132C), readl(reg_base + 0x1330),
				readl(reg_base + 0x1334), readl(reg_base + 0x1338));
			mtk_v4l2_err("0x133C: 0x%x, 0x1340: 0x%x, 0x1344: 0x%x, 0x1348: 0x%x",
				readl(reg_base + 0x133C), readl(reg_base + 0x1340),
				readl(reg_base + 0x1344), readl(reg_base + 0x1348));
			mtk_v4l2_err("0x13B8: 0x%x 0x13BC: 0x%x, 0x13C0: 0x%x, 0x13C4: 0x%x, 0x13C8: 0x%x",
				readl(reg_base + 0x13B8), readl(reg_base + 0x13BC),
				readl(reg_base + 0x13C0), readl(reg_base + 0x13C4),
				readl(reg_base + 0x13C8));
			mtk_v4l2_err("0x13CC: 0x%x 0x13D0: 0x%x, 0x13D4: 0x%x, 0x13D8: 0x%x, 0x13DC: 0x%x",
				readl(reg_base + 0x13CC), readl(reg_base + 0x13D0),
				readl(reg_base + 0x13D4), readl(reg_base + 0x13D8),
				readl(reg_base + 0x13DC));
			mtk_v4l2_err("0x13E0: 0x%x 0x13E4: 0x%x, 0x13E8: 0x%x, 0x13EC: 0x%x, 0x13F0: 0x%x",
				readl(reg_base + 0x13E0), readl(reg_base + 0x13E4),
				readl(reg_base + 0x13E8), readl(reg_base + 0x13EC),
				readl(reg_base + 0x13F0));
			mtk_v4l2_err("0x13F4: 0x%x 0x13F8: 0x%x, 0x13FC: 0x%x, 0x1400: 0x%x",
				readl(reg_base + 0x13F4), readl(reg_base + 0x13F8),
				readl(reg_base + 0x13FC), readl(reg_base + 0x1400));
			mtk_v4l2_err("0x1420: 0x%x, 0x1424: 0x%x", readl(reg_base + 0x1420),
				readl(reg_base + 0x1424));
			mtk_v4l2_err("0x13c: 0x%x, 0x484: 0x%x, 0x568: 0x%x",
				readl(reg_base + 0x13c), readl(reg_base + 0x484),
				readl(reg_base + 0x568));
		}

	}
	mtk_v4l2_debug(3, "%s -", __func__);
}
#endif

void mtk_vcodec_enc_clock_off(struct mtk_vcodec_ctx *ctx, int core_id)
{
#ifndef FPGA_PWRCLK_API_DISABLE
	struct mtk_vcodec_pm *pm = &ctx->dev->pm;
	int i;
	struct mtk_venc_clks_data *clks_data;
	struct mtk_vcodec_dev *dev = NULL;
	unsigned long flags;
	unsigned int clk_id = 0;

	dev = ctx->dev;

	if (core_id == MTK_VENC_CORE_0)
		mtk_venc_hw_break(dev);

	if (ctx->sysram_enable == 1)
		slbc_power_off(&ctx->sram_data);


	/* avoid translation fault callback dump reg not done */
	spin_lock_irqsave(&dev->enc_power_lock[core_id], flags);
	dev->enc_is_power_on[core_id] = false;
	spin_unlock_irqrestore(&dev->enc_power_lock[core_id], flags);

	clks_data = &pm->venc_clks_data;

	if (core_id == MTK_VENC_CORE_0 ||
		core_id == MTK_VENC_CORE_1 ||
		core_id == MTK_VENC_CORE_2) {
		if (clks_data->core_clks_len > 0) {
			for (i = clks_data->core_clks_len - 1; i >= 0; i--) {
				clk_id = clks_data->core_clks[i].clk_id;
				clk_disable_unprepare(pm->venc_clks[clk_id]);
			}
		}
	} else
		mtk_v4l2_err("invalid core_id %d", core_id);

	mtk_vcodec_enc_pw_off(pm);
#endif
}

#if IS_ENABLED(CONFIG_MTK_IOMMU_MISC_DBG)
static int mtk_venc_translation_fault_callback(
	int port, dma_addr_t mva, void *data)
{
	struct mtk_vcodec_dev *dev = (struct mtk_vcodec_dev *)data;
	int larb_id = port >> 5;
	void __iomem *reg_base = NULL;
	int hw_id = 0;
	unsigned long flags;

	mtk_v4l2_err("larb port %d of m4u port %d", larb_id, port & 0x1F);

	if (larb_id == 7) {
		reg_base = dev->enc_reg_base[VENC_SYS];
		hw_id = MTK_VENC_CORE_0;
	} else if (larb_id == 8) {
		reg_base = dev->enc_reg_base[VENC_C1_SYS];
		hw_id = MTK_VENC_CORE_1;
	} else if ((larb_id == 37) || (larb_id == 24)) {
		reg_base = dev->enc_reg_base[VENC_C2_SYS];
		hw_id = MTK_VENC_CORE_2;
	}

	if (dev->tf_info != NULL) {
		dev->tf_info->hw_id  = (__u32)hw_id;
		dev->tf_info->port   = (__u32)port;
		dev->tf_info->tf_mva = (__u64)mva;
		dev->tf_info->has_tf = 1;
		mtk_v4l2_err("TF set tf_info 0x%lx hw_id %d port %d mva 0x%llx",
			(unsigned long)dev->tf_info, dev->tf_info->hw_id,
			dev->tf_info->port & 0x1F, dev->tf_info->tf_mva);
	}

	spin_lock_irqsave(&dev->enc_power_lock[hw_id], flags);
	if (dev->enc_is_power_on[hw_id] == false) {
		mtk_v4l2_err("hw %d power is off !!", hw_id);
		spin_unlock_irqrestore(&dev->enc_power_lock[hw_id], flags);
		return -1;
	}

	if (reg_base != NULL) {
		mtk_v4l2_err("venc tf callback =>");
		mtk_v4l2_err("0x0: 0x%x, 0x14: 0x%x, 0x24: 0x%x, 0x28: 0x%x, 0x2C: 0x%x, 0x64: 0x%x, 0x6C: 0x%x",
			readl(reg_base), readl(reg_base + 0x14), readl(reg_base + 0x24),
			readl(reg_base + 0x28), readl(reg_base + 0x2C),
			readl(reg_base + 0x64), readl(reg_base + 0x6C));
		mtk_v4l2_err("0x70: 0x%x, 0x74: 0x%x, 0x78: 0x%x, 0x7C: 0x%x, 0x80: 0x%x, 0x84: 0x%x",
			readl(reg_base + 0x70), readl(reg_base + 0x74),
			readl(reg_base + 0x78), readl(reg_base + 0x7C),
			readl(reg_base + 0x80), readl(reg_base + 0x84));
		mtk_v4l2_err("0x88: 0x%x, 0x8C: 0x%x, 0x90: 0x%x, 0x94: 0x%x",
			readl(reg_base + 0x88), readl(reg_base + 0x8C),
			readl(reg_base + 0x90), readl(reg_base + 0x94));
		mtk_v4l2_err("0x108: 0x%x",
			readl(reg_base + 0x108));
		mtk_v4l2_err("0x200: 0x%x 0x204: 0x%x, 0x208: 0x%x, 0x20C: 0x%x, 0x210: 0x%x, 0x214: 0x%x, 0x218: 0x%x, 0x21C: 0x%x, 0x220: 0x%x",
			readl(reg_base + 0x200), readl(reg_base + 0x204),
			readl(reg_base + 0x208), readl(reg_base + 0x20C),
			readl(reg_base + 0x210), readl(reg_base + 0x214),
			readl(reg_base + 0x218), readl(reg_base + 0x21C),
			readl(reg_base + 0x220));
		mtk_v4l2_err("0xE0: 0x%x, 0x1B0: 0x%x, 0x1B4: 0x%x, 0x1170: 0x%x",
			readl(reg_base + 0xE0), readl(reg_base + 0x1B0),
			readl(reg_base + 0x1B4), readl(reg_base + 0x1170));
		mtk_v4l2_err("0x11B8: 0x%x 0x11BC: 0x%x, 0x1280: 0x%x, 0x1284: 0x%x, 0x1288: 0x%x",
			readl(reg_base + 0x11B8), readl(reg_base + 0x11BC),
			readl(reg_base + 0x1280), readl(reg_base + 0x1284),
			readl(reg_base + 0x1288));
		mtk_v4l2_err("0x132C: 0x%x 0x1330: 0x%x, 0x1334: 0x%x, 0x1338: 0x%x, 0x133C: 0x%x, 0x1340: 0x%x, 0x1344: 0x%x, 0x1348: 0x%x",
			readl(reg_base + 0x132C), readl(reg_base + 0x1330),
			readl(reg_base + 0x1334), readl(reg_base + 0x1338),
			readl(reg_base + 0x133C), readl(reg_base + 0x1340),
			readl(reg_base + 0x1344), readl(reg_base + 0x1348));
		mtk_v4l2_err("0x1420: 0x%x, 0x1424: 0x%x", readl(reg_base + 0x1420),
			readl(reg_base + 0x1424));
		mtk_v4l2_err("0x13c: 0x%x, 0x484: 0x%x, 0x568: 0x%x",
			readl(reg_base + 0x13c), readl(reg_base + 0x484), readl(reg_base + 0x568));

		//soc base address as VENC_SYS
		if (larb_id == 7) {
			mtk_v4l2_err("0x4064: 0x%x, 0x406c : 0x%x, 0x4074: 0x%x, 0x52c0: 0x%x",
				readl(reg_base + 0x4064), readl(reg_base + 0x406c),
				readl(reg_base + 0x4074), readl(reg_base + 0x52c0));
		} else if (larb_id == 8 || larb_id == 37) {
			reg_base = dev->enc_reg_base[VENC_SYS];
			spin_lock_irqsave(&dev->enc_power_lock[MTK_VENC_CORE_0], flags);
			if (dev->enc_is_power_on[MTK_VENC_CORE_0] == false) {
				mtk_v4l2_err("hw %d power is off !!", MTK_VENC_CORE_0);
				spin_unlock_irqrestore(&dev->enc_power_lock[MTK_VENC_CORE_0],
					flags);
				spin_unlock_irqrestore(&dev->enc_power_lock[hw_id], flags);
				return -1;
			}
			if (reg_base != NULL) {
				mtk_v4l2_err("0x4064: 0x%x, 0x406c : 0x%x, 0x4074: 0x%x, 0x52c0: 0x%x",
					readl(reg_base + 0x4064), readl(reg_base + 0x406c),
					readl(reg_base + 0x4074), readl(reg_base + 0x52c0));
			}
			spin_unlock_irqrestore(&dev->enc_power_lock[MTK_VENC_CORE_0], flags);
		}
	}

	spin_unlock_irqrestore(&dev->enc_power_lock[hw_id], flags);

	return 0;
}
#endif

void mtk_venc_translation_fault_callback_setting(
	struct mtk_vcodec_dev *dev)
{
#if IS_ENABLED(CONFIG_MTK_IOMMU_MISC_DBG)
	int core_id, i;

	for (core_id = 0; core_id < MTK_VENC_HW_NUM; core_id++) {
		for (i = 0; i < dev->venc_ports[core_id].total_port_num; i++) {
			mtk_iommu_register_fault_callback(dev->venc_ports[core_id].port_id[i],
				mtk_venc_translation_fault_callback, (void *)dev, false);
		}
	}
#endif
}
