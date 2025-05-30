// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
 */
#include <linux/cpuidle.h>
#include <linux/topology.h>
#include <linux/slab.h>
#include <linux/of.h>

#include <mtk_cpuidle_sysfs.h>

#include "mtk_cpuidle_status.h"

#define MTK_CPUIDLE_DRIVE_STATE_GET	(0)
#define MTK_CPUIDLE_DRIVE_STATE_SET	(1)
#define ALL_CPU_ID			(100)

#define LPM_CPUIDLE_STATE_OP(op, _priv) ({\
	op.fs_read = lpm_cpuidle_state_read;\
	op.fs_write = lpm_cpuidle_state_write;\
	op.priv = _priv; })

#define LPM_CPUIDLE_STATE_NODE_INIT(_n, _name, _type) ({\
	_n.name = _name;\
	_n.type = _type;\
	LPM_CPUIDLE_STATE_OP(_n.op, &_n); })

enum LPM_CPUIDLE_STATE_NODE_TYPE {
	LPM_CPUIDLE_STATE_NODE_ENABLED,
	LPM_CPUIDLE_STATE_NODE_MAX
};

struct LPM_CPUIDLE_STATE_NODE {
	const char *name;
	enum idle_param type;
	struct mtk_lp_sysfs_handle handle;
	struct mtk_lp_sysfs_op op;
};

struct mtk_lp_sysfs_handle lpm_entry_cpuidle_state;

struct LPM_CPUIDLE_STATE_NODE state_enabled;
struct LPM_CPUIDLE_STATE_NODE state_latency;
struct LPM_CPUIDLE_STATE_NODE state_residency;

struct MTK_CPUIDLE_DRV_INFO {
	int cpu;
	unsigned int type;
	int param;
	unsigned int state_idx;
	unsigned int val;
	char *p;
	size_t *sz;
};

struct lpm_cpu_topology {
	unsigned int id;
	unsigned int cpu_mask;
};
struct lpm_cpu_topology **topology;

static const char *node_string[NF_IDLE_PARAM] = {
	[IDLE_PARAM_EN]		= "Enabled",
	[IDLE_PARAM_LAT]	= "Exit latency",
	[IDLE_PARAM_RES]	= "Target residency",
};

static const char *node_name[NF_IDLE_PARAM] = {
	[IDLE_PARAM_EN]		= "enabled",
	[IDLE_PARAM_LAT]	= "latency",
	[IDLE_PARAM_RES]	= "residency",
};

struct lpm_idle_state_info {
	char name[CPUIDLE_NAME_LEN];
	unsigned int param;
};

static struct lpm_idle_state_info state_info[CPUIDLE_STATE_MAX];
static unsigned int nr_states;

static long lpm_per_cpuidle_drv_param(void *pData)
{
	struct cpuidle_driver *drv = cpuidle_get_driver();
	struct MTK_CPUIDLE_DRV_INFO *info =
				(struct MTK_CPUIDLE_DRV_INFO *)pData;
	int i = 0;
	size_t sz = *(info->sz);
	char *p = info->p;

	if (!drv)
		return -ENODEV;
	if (info->type == MTK_CPUIDLE_DRIVE_STATE_GET) {
		if (info->cpu == 0) {
			mtk_dbg_cpuidle_log("%-12s:", "state_index");
			for (i = 0; i < nr_states; i++)
				if (i == 0)
					mtk_dbg_cpuidle_log("  %-8d", i);
				else
					mtk_dbg_cpuidle_log("%-15d", i);
			mtk_dbg_cpuidle_log("\n");

			mtk_dbg_cpuidle_log("%-12s:", "state_name");
			for (i = 0; i < nr_states; i++) {
				if (i == 0)
					mtk_dbg_cpuidle_log("  %-8s", state_info[i].name);
				else
					mtk_dbg_cpuidle_log("%-15s", state_info[i].name);
			}
			mtk_dbg_cpuidle_log("\n");
		}

		mtk_dbg_cpuidle_log("%11s%d:", "cpu", info->cpu);
		if (cpu_is_offline(info->cpu))
			mtk_dbg_cpuidle_log("%18s ", "Offline");
		else {
			unsigned int j = 1;

			mtk_dbg_cpuidle_log("  %-8ld",
				mtk_cpuidle_get_param(drv, 0, info->param));
			for (i = 1; i < nr_states; i++) {
				if (!strncmp((drv->states[j]).name, state_info[i].name,
					strlen(state_info[i].name))) {
					mtk_dbg_cpuidle_log("%-15ld",
						mtk_cpuidle_get_param(drv, j, info->param));
					j += 1;
				} else {
					mtk_dbg_cpuidle_log("%-15s", "X");
				}
			}
		}
		mtk_dbg_cpuidle_log("\n");
	} else if (info->type == MTK_CPUIDLE_DRIVE_STATE_SET) {
		for (i = 0; i < drv->state_count; i++) {
			if (!strncmp((drv->states[i]).name,
				state_info[info->state_idx].name,
				strlen(state_info[info->state_idx].name))) {
				mtk_cpuidle_set_param(drv, i, info->param, info->val);
				break;
			}
		}
	}

	*(info->sz) = sz;
	info->p = p;

	return 0;
}

static void cpuidle_state_read_param(char **ToUserBuf, size_t *sz, int param)
{
	int cpu;
	struct MTK_CPUIDLE_DRV_INFO drv_info = {
		.type = MTK_CPUIDLE_DRIVE_STATE_GET,
		.param = param,
		.p = *ToUserBuf,
		.sz = sz,
	};

	for_each_possible_cpu(cpu) {
		drv_info.cpu = cpu;
		work_on_cpu(cpu, lpm_per_cpuidle_drv_param,
				&drv_info);
	}

	*ToUserBuf = drv_info.p;
}

static int idle_proc_state_param_setting(char *cmd, size_t *sz, int param)
{
	char *args;
	unsigned int cpu_mask, state_idx = 0, val = 0;
	int cpu = 0, i = 0;
	struct MTK_CPUIDLE_DRV_INFO drv_info;
	struct cmd_param {
		unsigned int id;
		unsigned int mask;
	};

	args = strsep(&cmd, " ");
	if (!args || kstrtoint(args, 10, &cpu) != 0)
		return -EINVAL;

	args = strsep(&cmd, " ");
	if (!args || kstrtouint(args, 10, &state_idx) != 0)
		return -EINVAL;

	args = strsep(&cmd, " ");
	if (!args || kstrtouint(args, 10, &val) != 0)
		return -EINVAL;

	if (!state_idx || state_idx >= nr_states)
		return -EINVAL;

	cpu_mask = 0;

	if (cpu >= 0 && cpu < nr_cpu_ids) {
		cpu_mask = (1 << cpu);
	} else {
		while (topology[i]->id != ALL_CPU_ID) {
			if (cpu == topology[i]->id) {
				cpu_mask = topology[i]->cpu_mask;
				break;
			}
			i++;
		}
		if (cpu == topology[i]->id)
			cpu_mask = topology[i]->cpu_mask;
	}

	for_each_online_cpu(cpu) {
		if ((cpu_mask & (1 << cpu)) == 0)
			continue;

		drv_info.cpu = cpu;
		drv_info.type = MTK_CPUIDLE_DRIVE_STATE_SET;
		drv_info.param = param;
		drv_info.state_idx = state_idx;
		drv_info.val = val;
		drv_info.p = cmd;
		drv_info.sz = sz;

		work_on_cpu(cpu, lpm_per_cpuidle_drv_param,
				&drv_info);
	}

	return 0;
}

static void idle_proc_state_uasge_print(char **ToUserBuf, size_t *size,
						enum idle_param type)
{
	char *p = *ToUserBuf;
	size_t sz = *size;
	u32 num_present_cpus = 0;
	int i = 0;

	num_present_cpus = num_present_cpus();

	if (type > IDLE_PARAM_RES)
		type = IDLE_PARAM_RES;

	mtk_dbg_cpuidle_log("\n======== Command Usage ========\n");
	mtk_dbg_cpuidle_log("%s > /proc/mtk_lpm/cpuidle/state/%s\n",
			"echo [cpu_id] [state_index] [val(dec)]",
			node_name[type]);
	mtk_dbg_cpuidle_log("\t cpu_id: 0~%u -> cpu number\n",
				num_present_cpus);

	while (topology[i]->id != ALL_CPU_ID) {
		mtk_dbg_cpuidle_log("\t         %3d -> all cluster%d CPU\n",
					topology[i]->id, i);
		i++;
	}
	mtk_dbg_cpuidle_log("\t         %3d -> all CPU\n",
					topology[i]->id);

	mtk_dbg_cpuidle_log("\t state_index msut > 0 (index 0 can't be modified)\n");
	mtk_dbg_cpuidle_log("\n");

	*ToUserBuf = p;
}

static ssize_t lpm_cpuidle_state_read(char *ToUserBuf,
					    size_t sz, void *priv)
{
	char *p = ToUserBuf;
	struct LPM_CPUIDLE_STATE_NODE *node =
			(struct LPM_CPUIDLE_STATE_NODE *)priv;

	if (!p || !node)
		return -EINVAL;

		mtk_dbg_cpuidle_log("==== CPU idle state: %s ====\n",
					node_string[node->type]);
		cpuidle_state_read_param(&p, &sz, node->type);
		idle_proc_state_uasge_print(&p, &sz, node->type);

	return p - ToUserBuf;
}

static ssize_t lpm_cpuidle_state_write(char *FromUserBuf,
					  size_t sz, void *priv)
{
	struct LPM_CPUIDLE_STATE_NODE *node =
			(struct LPM_CPUIDLE_STATE_NODE *)priv;
	char cmd[128];
	int parm;

	if (!FromUserBuf || !node)
		return -EINVAL;

	if (sscanf(FromUserBuf, "%127s %x", cmd, &parm) == 2) {
		idle_proc_state_param_setting(FromUserBuf, &sz, node->type);
		return sz;
	}

	return -EINVAL;
}

static int lpm_cpuidle_state_info_init(void)
{
	struct device_node *node = NULL;
	int i = 1;

	if (strscpy(state_info[0].name, "WFI", sizeof(state_info[0].name)) < 0)
		return -EINVAL;

	state_info[0].param = 0;
	do {
		node = of_find_compatible_node(node, NULL, "arm,idle-state");
		if (node) {
			unsigned int param = 0;
			int j = 0;

			of_property_read_u32(node, "arm,psci-suspend-param", &param);
			for (j = 0; j < i; j++)
				if (param == state_info[j].param)
					break;
			if (j == i) {
				if (strscpy(state_info[i].name, node->name,
						sizeof(state_info[i].name)) < 0)
					return -EINVAL;
				state_info[i].param = param;
				i += 1;
			} else {
				if (strcmp(state_info[j].name, node->name))
					state_info[j].name[strlen(node->name)-2] = '\0';
			}
		}
	} while (node);
	nr_states = i;

	return 0;
}

static int lpm_topology_init(void)
{
	unsigned int cluster_id_map = 0, cluster_num = 0;
	int cluster_id = 0, cpu = 0;

	for_each_present_cpu(cpu) {
		if (cpu_topology[cpu].cluster_id < 0)
			return -EINVAL;
		cluster_id = cpu_topology[cpu].cluster_id;
		cluster_id_map |= (1 << cluster_id);
	}

	cluster_num = hweight32(cluster_id_map);

	topology = kzalloc(sizeof(struct lpm_cpu_topology *) *
				(cluster_num + 1),
				GFP_KERNEL);
	if (!topology)
		return -ENOMEM;

	for (cluster_id = 0; cluster_id <= cluster_num; cluster_id++) {
		topology[cluster_id] = kzalloc(sizeof(struct lpm_cpu_topology),
						GFP_KERNEL);
		if (!topology[cluster_id]) {
			kfree(topology);
			return -ENOMEM;
		}
	}

	for_each_present_cpu(cpu) {
		cluster_id = cpu_topology[cpu].cluster_id;
		topology[cluster_id]->id = (cluster_id + 1) * 10;
		topology[cluster_id]->cpu_mask |= (1 << cpu);
		topology[cluster_num]->id |= ALL_CPU_ID;
		topology[cluster_num]->cpu_mask |= (1 << cpu);
	}

	return 0;
}

void lpm_cpuidle_state_init(void)
{
	int ret = 0;

	ret = lpm_topology_init();
	if (ret) {
		pr_info("[name:mtk_lpm][P] - lpm_topology_init error (%s:%d)\n",
		__func__, __LINE__);
		return;
	}

	ret = lpm_cpuidle_state_info_init();
	if (ret) {
		pr_info("[name:mtk_lpm][P] - lpm_cpuidle_state_info_init error (%s:%d)\n",
		__func__, __LINE__);
		return;
	}

	mtk_cpuidle_sysfs_sub_entry_add("state", MTK_CPUIDLE_SYS_FS_MODE,
				NULL, &lpm_entry_cpuidle_state);

	LPM_CPUIDLE_STATE_NODE_INIT(state_enabled, node_name[IDLE_PARAM_EN],
				    IDLE_PARAM_EN);
	mtk_cpuidle_sysfs_sub_entry_node_add(state_enabled.name,
					MTK_CPUIDLE_SYS_FS_MODE,
					&state_enabled.op,
					&lpm_entry_cpuidle_state,
					&state_enabled.handle);

	LPM_CPUIDLE_STATE_NODE_INIT(state_latency, node_name[IDLE_PARAM_LAT],
				    IDLE_PARAM_LAT);
	mtk_cpuidle_sysfs_sub_entry_node_add(state_latency.name,
					MTK_CPUIDLE_SYS_FS_MODE,
					&state_latency.op,
					&lpm_entry_cpuidle_state,
					&state_latency.handle);

	LPM_CPUIDLE_STATE_NODE_INIT(state_residency, node_name[IDLE_PARAM_RES],
				    IDLE_PARAM_RES);
	mtk_cpuidle_sysfs_sub_entry_node_add(state_residency.name,
					MTK_CPUIDLE_SYS_FS_MODE,
					&state_residency.op,
					&lpm_entry_cpuidle_state,
					&state_residency.handle);
}
