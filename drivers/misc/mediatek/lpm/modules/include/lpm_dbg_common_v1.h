/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019 MediaTek Inc.
 */

#ifndef __LPM_DBG_COMMON_V1_H__
#define __LPM_DBG_COMMON_V1_H__

/* For sysfs get the last wake status */
enum mtk_spm_wake_status_enum {
	WAKE_STA_R12,
	WAKE_STA_R12_EXT,
	WAKE_STA_RAW_STA,
	WAKE_STA_RAW_EXT_STA,
	WAKE_STA_WAKE_MISC,
	WAKE_STA_TIMER_OUT,
	WAKE_STA_R13,
	WAKE_STA_IDLE_STA,
	WAKE_STA_REQ_STA,
	WAKE_STA_DEBUG_FLAG,
	WAKE_STA_DEBUG_FLAG1,
	WAKE_STA_ISR,

	WAKE_STA_MAX_COUNT,
};

enum {
	WR_NONE = 0,
	WR_UART_BUSY = 1,
	WR_ABORT = 2,
	WR_PCM_TIMER = 3,
	WR_WAKE_SRC = 4,
	WR_DVFSRC = 5,
	WR_PMSR = 6,
	WR_TWAM = 7,
	WR_SPM_ACK_CHK = 8,
	WR_UNKNOWN = 9,
};


/* enum for smc resource request arg */
enum MT_SPM_RES_TYPE {
	MT_SPM_RES_XO_FPM,
	MT_SPM_RES_CK_26M,
	MT_SPM_RES_INFRA,
	MT_SPM_RES_SYSPLL,
	MT_SPM_RES_DRAM_S0,
	MT_SPM_RES_DRAM_S1,
	MT_SPM_RES_VCORE,
	MT_SPM_RES_EMI,
	MT_SPM_RES_PMIC,
	MT_SPM_RES_MAX,
};

enum dbg_ctrl_enum {
	DBG_CTRL_COUNT,
	DBG_CTRL_DURATION,
	DBG_CTRL_MAX,
};

enum MT_SPM_STAT_TYPE {
	SPM_SLP_COUNT,
	SPM_SLP_DURATION,
};

enum MT_SPM_STAT_SCENARIO_TYPE {
	SPM_IDLE_STAT,
	SPM_SUSPEND_STAT,
};

enum MT_SPM_STAT_STATE {
	SPM_STAT_MCUSYS,
	SPM_STAT_F26M,
	SPM_STAT_VCORE,
	NUM_SPM_STAT,
};

struct lpm_stat_record {
	u64 count;
	u64 duration;
};

/* Determine for operand bit */
#define MTK_DUMP_LP_GOLDEN	(1 << 0L)
#define MTK_DUMP_GPIO		(1 << 1L)

#define PCM_32K_TICKS_PER_SEC		(32768)
#define PCM_TICK_TO_SEC(TICK)	(TICK / PCM_32K_TICKS_PER_SEC)

struct lpm_log_helper {
	short cur;
	short prev;
	struct lpm_spm_wake_status *wakesrc;
};

struct lpm_dbg_plat_ops {
	int (*lpm_show_message)(int type, const char *prefix, void *data);
	void (*lpm_save_sleep_info)(void);
	void (*lpm_get_spm_wakesrc_irq)(void);
	int (*lpm_get_wakeup_status)(void);
};


struct spm_condition {
	bool init;
	char **cg_str;
	int cg_cnt;
	char **pll_str;
	int pll_cnt;
	unsigned int cg_shift;
	unsigned int pll_shift;
	int shift_config;
};

#if IS_ENABLED(CONFIG_MTK_ECCCI_DRIVER)
struct md_sleep_status {
	u64 guard_sleep_cnt1;
	u64 sleep_utc;
	u64 sleep_wall_clk;
	u64 sleep_cnt;
	u64 sleep_cnt_reserve;
	u64 sleep_time;
	u64 sleep_time_reserve;
	u64 md_sleep_time; // uS
	u64 gsm_sleep_time; // uS
	u64 wcdma_sleep_time; //uS
	u64 lte_sleep_time; // uS
	u64 nr_sleep_time; // uS
	u64 reserved[51]; //0x60~0x1F0
	u64 guard_sleep_cnt2;
};

void get_md_sleep_time(struct md_sleep_status *md_data);
int is_md_sleep_info_valid(struct md_sleep_status *md_data);
void log_md_sleep_info(void);
extern u32 *md_share_mem;
extern struct md_sleep_status before_md_sleep_status;
extern struct md_sleep_status after_md_sleep_status;
extern struct md_sleep_status cur_md_sleep_status;
#endif

struct lpm_dbg_lp_info {
	struct lpm_stat_record record[NUM_SPM_STAT];
};

extern u64 spm_26M_off_count;
extern u64 spm_26M_off_duration;
extern u64 spm_vcore_off_count;
extern u64 spm_vcore_off_duration;
extern u64 ap_pd_count;
extern u64 ap_slp_duration;
extern struct spm_condition spm_cond;
extern int kernel_suspend_only;

int lpm_dbg_plat_ops_register(struct lpm_dbg_plat_ops *lpm_dbg_plat_ops);

int spm_cond_init(void);
void spm_cond_deinit(void);

int lpm_dbg_common_fs_init(void);
void lpm_dbg_common_fs_exit(void);

int spm_common_dbg_dump(void);
int lpm_dbg_pm_init(void);
void lpm_dbg_pm_exit(void);

#endif /* __MTK_DBG_COMMON_H__ */
