// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2021 MediaTek Inc.
 */

#include <linux/module.h>
#include <linux/percpu.h>
#include <linux/sched.h>
#include <linux/cpumask.h>
#include <linux/arch_topology.h>
#include <linux/printk.h>
#include <linux/sched/clock.h>
#include <trace/hooks/sched.h>

#include <linux/sched/cputime.h>
#include <sched/sched.h>
#include "sched_avg.h"
#include "common.h"

#define TAG "sched_avg"

enum over_thres_type {
	NO_OVER_THRES = 0,
	OVER_DN_THRES,
	OVER_UP_THRES
};

struct over_thres_stats {
	int nr_over_dn_thres;
	int nr_over_up_thres;
	int dn_thres;
	int up_thres;
	int nr_running_diff;
	atomic_long_t max_task_util;
	u64 nr_over_dn_thres_prod_sum;
	u64 nr_over_up_thres_prod_sum;
	u64 dn_last_update_time;
	u64 up_last_update_time;
};

struct cluster_over_thres_stats {
	u64 last_get_over_thres_time;
	u64 max_capacity;
};

static DEFINE_PER_CPU(struct over_thres_stats, cpu_over_thres_state);
static DEFINE_PER_CPU(u64, nr);
static DEFINE_PER_CPU(u64, nr_max);
static DEFINE_PER_CPU(u64, last_time);
static DEFINE_PER_CPU(spinlock_t, nr_lock) = __SPIN_LOCK_UNLOCKED(nr_lock);
static DEFINE_PER_CPU(spinlock_t, nr_over_thres_lock) = __SPIN_LOCK_UNLOCKED(nr_over_thres_lock);
/*
 * static DEFINE_PER_CPU(u64, nr_prod_sum);
 * static DEFINE_PER_CPU(unsigned long, iowait_prod_sum);
 */
static int init_thres;
static int global_task_util;

/* pelt.h */
#define OVER_THRES_SIZE			2
#define MAX_CLUSTER_NR			3
#define MAX_UTIL_TRACKER_PERIODIC_MS	8

static int init_thres_table(void);
static unsigned int over_thres[OVER_THRES_SIZE] = {30, 50};
static struct cluster_over_thres_stats cluster_over_thres_table[MAX_CLUSTER_NR];

void sched_max_util_task(int *util)
{
	int cpu;
	struct over_thres_stats *cpu_over_thres;
	int max_util = 0;
	ktime_t now;
	static ktime_t max_util_tracker_last_update;

	if (!util)
		return;

	now = ktime_get();
	if (ktime_before(now, ktime_add_ms(
		max_util_tracker_last_update, MAX_UTIL_TRACKER_PERIODIC_MS))) {
		*util = global_task_util;
		return;
	}

	/* update last update time for tracker */
	max_util_tracker_last_update = now;

	global_task_util = 0;

	for_each_possible_cpu(cpu) {
		cpu_over_thres = &per_cpu(cpu_over_thres_state, cpu);
		max_util = atomic_long_read(&cpu_over_thres->max_task_util);
		if (cpu_online(cpu) && max_util > global_task_util)
			global_task_util = max_util;
		atomic_long_set(&cpu_over_thres->max_task_util, 0);
	}
	*util = global_task_util;
}
EXPORT_SYMBOL(sched_max_util_task);

unsigned long capacity_of(int cpu)
{
	return cpu_rq(cpu)->cpu_capacity;
}

unsigned int get_cpu_util_pct(unsigned int cpu, bool orig)
{
	struct cfs_rq *cfs_rq;
	unsigned long capacity;
	unsigned long util;
	unsigned int util_pct;

	cfs_rq = &cpu_rq(cpu)->cfs;
	util = READ_ONCE(cfs_rq->avg.util_avg);

	if (sched_feat(UTIL_EST) && is_util_est_enable())
		util = max_t(unsigned long, util,
			READ_ONCE(cfs_rq->avg.util_est.enqueued));

	capacity = (orig == true) ? capacity_orig_of(cpu) : capacity_of(cpu);
	util = min_t(unsigned long, util, capacity);
	util_pct = (unsigned int)div64_ul((util * 100), capacity);
	return util_pct;
}
EXPORT_SYMBOL(get_cpu_util_pct);

/**
 * sched_update_nr_prod
 * @cpu: The core id of the nr running driver.
 * @nr: Updated nr running value for cpu.
 * @inc: Whether we are increasing or decreasing the count
 * @return: N/A
 *
 * Update average with latest nr_running value for CPU
 */
void sched_update_nr_prod(int cpu, unsigned long nr_running, int inc)
{
	s64 diff;
	u64 curr_time;
	unsigned long flags;
	struct over_thres_stats *cpu_over_thres;

	spin_lock_irqsave(&per_cpu(nr_lock, cpu), flags);
	curr_time = sched_clock();
	diff = (s64) (curr_time - per_cpu(last_time, cpu));
	/* skip this problematic clock violation */
	if (diff < 0) {
		spin_unlock_irqrestore(&per_cpu(nr_lock, cpu), flags);
		return;
	}
	per_cpu(last_time, cpu) = curr_time;
	/* To prevent to add count, because already do outside */
	per_cpu(nr, cpu) = nr_running;

	if (per_cpu(nr, cpu) > per_cpu(nr_max, cpu))
		per_cpu(nr_max, cpu) = per_cpu(nr, cpu);
	spin_unlock_irqrestore(&per_cpu(nr_lock, cpu), flags);
	spin_lock_irqsave(&per_cpu(nr_over_thres_lock, cpu), flags);
	cpu_over_thres = &per_cpu(cpu_over_thres_state, cpu);
	cpu_over_thres->nr_running_diff = nr_running - cpu_over_thres->nr_over_dn_thres;
	spin_unlock_irqrestore(&per_cpu(nr_over_thres_lock, cpu), flags);
}

void sched_update_nr_running_cb(void *data, struct rq *rq, int count)
{
	sched_update_nr_prod(cpu_of(rq), rq->nr_running, count);
}

static inline unsigned long task_util(struct task_struct *p)
{
	return READ_ONCE(p->se.avg.util_avg);
}

static inline unsigned long _task_util_est(struct task_struct *p)
{
	struct util_est ue = READ_ONCE(p->se.avg.util_est);

	return max(ue.ewma, (ue.enqueued & ~UTIL_AVG_UNCHANGED));
}

static inline unsigned long task_util_est(struct task_struct *p)
{
	return max(task_util(p), _task_util_est(p));
}

int arch_get_nr_clusters(void)
{
	int __arch_nr_clusters = -1;
	int max_id = 0;
	unsigned int cpu;

	/* assume socket id is monotonic increasing without gap. */
	for_each_possible_cpu(cpu) {
		struct cpu_topology *cpu_topo = &cpu_topology[cpu];

		if (cpu_topo->cluster_id > max_id)
			max_id = cpu_topo->cluster_id;
	}
	__arch_nr_clusters = max_id + 1;
	return __arch_nr_clusters;
}
EXPORT_SYMBOL(arch_get_nr_clusters);

int arch_get_cluster_id(unsigned int cpu)
{
	struct cpu_topology *cpu_topo = &cpu_topology[cpu];

	return cpu_topo->cluster_id < 0 ? 0 : cpu_topo->cluster_id;
}
EXPORT_SYMBOL(arch_get_cluster_id);

void arch_get_cluster_cpus(struct cpumask *cpus, int cluster_id)
{
	unsigned int cpu;

	cpumask_clear(cpus);
	for_each_possible_cpu(cpu) {
		struct cpu_topology *cpu_topo = &cpu_topology[cpu];

		if (cpu_topo->cluster_id == cluster_id)
			cpumask_set_cpu(cpu, cpus);
	}
}
EXPORT_SYMBOL(arch_get_cluster_cpus);

unsigned int get_over_threshold(int index)
{
	if (index >= 0 && index <= OVER_THRES_SIZE)
		return over_thres[index];
	return 100;
}
EXPORT_SYMBOL(get_over_threshold);

unsigned int get_max_capacity(unsigned int cid)
{
	if (cid >= arch_get_nr_clusters())
		return 1024;

	return cluster_over_thres_table[cid].max_capacity;
}
EXPORT_SYMBOL(get_max_capacity);

static void over_thresh_chg_notify(void);
int set_over_threshold(unsigned int index, unsigned int val)
{
	if (index >= OVER_THRES_SIZE || val > 100)
		return -EINVAL;

	over_thres[index] = (int)val;
	over_thresh_chg_notify();
	return 0;
}
EXPORT_SYMBOL(set_over_threshold);

enum over_thres_type is_task_over_thres(struct task_struct *p)
{
	struct over_thres_stats *cpu_over_thres;
	unsigned long util;
	int cpu;

	if (!p)
		return NO_OVER_THRES;

	cpu = cpu_of(task_rq(p));
	util = task_util(p);
	cpu_over_thres = &per_cpu(cpu_over_thres_state, cpu);

	/* track task with max utilization */
	if (util > atomic_long_read(&cpu_over_thres->max_task_util))
		atomic_long_set(&cpu_over_thres->max_task_util, util);

	/* check if task is over threshold */
	if (util >= cpu_over_thres->up_thres)
		return OVER_UP_THRES;
	else if (util >= cpu_over_thres->dn_thres)
		return OVER_DN_THRES;
	else
		return NO_OVER_THRES;
}

static DEFINE_SPINLOCK(deferrable_lock);
static void sched_avg_deferred_func(struct work_struct *work)
{
	u64 start_time = 0;

	start_time = sched_clock();
	over_thresh_chg_notify();
	printk_deferred_once("core_clt reset\n");
}
static DECLARE_WORK(sched_avg_deferred_work, sched_avg_deferred_func);

#define DEFERRED_WORK_DEBOUNCE_TIME	(5 * NSEC_PER_SEC)
static void reset_over_thre_deferred(u64 curr_time)
{
	static s64 deferred_work_last_update;
	s64 diff;

	spin_lock(&deferrable_lock);
	diff = (s64) (curr_time - deferred_work_last_update);
	if (diff < DEFERRED_WORK_DEBOUNCE_TIME) {
		spin_unlock(&deferrable_lock);
		return;
	}
	deferred_work_last_update = curr_time;
	spin_unlock(&deferrable_lock);

	schedule_work(&sched_avg_deferred_work);
}

int get_max_nr_running(int cpu)
{
	int max_nr = 0;

	spin_lock(&per_cpu(nr_lock, cpu));
	max_nr = per_cpu(nr_max, cpu);
	/* reset max_nr value */
	per_cpu(nr_max, cpu) = per_cpu(nr, cpu);
	spin_unlock(&per_cpu(nr_lock, cpu));
	return max_nr;
}
EXPORT_SYMBOL(get_max_nr_running);

int sched_get_nr_over_thres_avg(int cluster_id,
				int *dn_avg,
				int *up_avg,
				int *sum_nr_over_dn_thres,
				int *sum_nr_over_up_thres)
{
	u64 curr_time = sched_clock();
	s64 diff;
	u64 dn_tmp_avg = 0, up_tmp_avg = 0;
	u32 cpumask = 0;
	bool clk_faulty = 0;
	unsigned long flags;
	int cpu = 0;
	int cluster_nr;
	struct cpumask cls_cpus;
	bool reset = false;

	/* Need to make sure initialization done. */
	if (!init_thres) {
		*dn_avg = *up_avg = 0;
		*sum_nr_over_dn_thres = *sum_nr_over_up_thres = 0;
		return -EPROTO;
	}

	/* cluster_id need reasonale. */
	cluster_nr = arch_get_nr_clusters();
	if (cluster_id < 0 || cluster_id >= cluster_nr) {
		printk_deferred_once("%s: invalid cluster id %d\n", __func__, cluster_id);
		return -EINVAL;
	}

	/* Time diff can't be zero/negative. */
	diff = (s64)(curr_time -
			cluster_over_thres_table[cluster_id].last_get_over_thres_time);
	if (diff <= 0) {
		*dn_avg = *up_avg = 0;
		*sum_nr_over_dn_thres = *sum_nr_over_up_thres = 0;
		return -EPROTO;
	}

	arch_get_cluster_cpus(&cls_cpus, cluster_id);
	cluster_over_thres_table[cluster_id].last_get_over_thres_time = curr_time;

	/* visit all cpus of this cluster */
	for_each_cpu(cpu, &cls_cpus) {
		struct over_thres_stats *cpu_over_thres;

		spin_lock_irqsave(&per_cpu(nr_over_thres_lock, cpu), flags);

		cpu_over_thres = &per_cpu(cpu_over_thres_state, cpu);

		if ((s64) (curr_time - cpu_over_thres->dn_last_update_time < 0)) {
			clk_faulty = 1;
			cpumask |= 1 << cpu;
			spin_unlock_irqrestore(&per_cpu(nr_over_thres_lock, cpu), flags);
			break;
		}

		if (cpu_over_thres->nr_over_dn_thres < 0 ||
		    cpu_over_thres->nr_over_up_thres < 0 ||
			cpu_over_thres->nr_running_diff < 0) {
			reset = true;
			printk_deferred_once("%s: nr_over_dn_thres: %d nr_over_up_thres:%d nr_running_diff:%d\n",
					TAG, cpu_over_thres->nr_over_dn_thres,
					cpu_over_thres->nr_over_up_thres,
					cpu_over_thres->nr_running_diff);

			spin_unlock_irqrestore(&per_cpu(nr_over_thres_lock, cpu), flags);
			break;
		}
		/* get sum of nr_over_thres */
		*sum_nr_over_dn_thres += cpu_over_thres->nr_over_dn_thres;
		*sum_nr_over_up_thres += cpu_over_thres->nr_over_up_thres;

		/* get prod sum */
		dn_tmp_avg += cpu_over_thres->nr_over_dn_thres_prod_sum;
		dn_tmp_avg += cpu_over_thres->nr_over_dn_thres *
			(curr_time - cpu_over_thres->dn_last_update_time);

		up_tmp_avg += cpu_over_thres->nr_over_up_thres_prod_sum;
		up_tmp_avg += cpu_over_thres->nr_over_up_thres *
			(curr_time - cpu_over_thres->up_last_update_time);

		/* update last update time */
		cpu_over_thres->dn_last_update_time = curr_time;
		cpu_over_thres->up_last_update_time = curr_time;

		/* clear prod_sum */
		cpu_over_thres->nr_over_dn_thres_prod_sum = 0;
		cpu_over_thres->nr_over_up_thres_prod_sum = 0;

		spin_unlock_irqrestore(&per_cpu(nr_over_thres_lock, cpu), flags);
	}

	if (reset)
		reset_over_thre_deferred(curr_time);

	if (clk_faulty) {
		*dn_avg = *up_avg = 0;
		*sum_nr_over_dn_thres = *sum_nr_over_up_thres = 0;
		return -EPROTO;
	}

	*dn_avg = (unsigned int)div64_u64(dn_tmp_avg * 100, (u64) diff);
	*up_avg = (unsigned int)div64_u64(up_tmp_avg * 100, (u64) diff);

	return 0;
}
EXPORT_SYMBOL(sched_get_nr_over_thres_avg);

static void over_thresh_chg_notify(void)
{
	int cpu;
	unsigned long flags;
	struct task_struct *p;
	enum over_thres_type over_type = NO_OVER_THRES;
	int nr_over_dn_thres, nr_over_up_thres;
	struct over_thres_stats *cpu_over_thres;
	int cid;
	unsigned int over_threshold = 100;
	int cluster_nr = arch_get_nr_clusters();

	for_each_possible_cpu(cpu) {
		u64 curr_time = sched_clock();

		nr_over_dn_thres = 0;
		nr_over_up_thres = 0;

		cid = arch_get_cluster_id(cpu);
		cpu_over_thres = &per_cpu(cpu_over_thres_state, cpu);

		if (cid < 0 || cid >= cluster_nr) {
			printk_deferred_once("%s: cid=%d is out of nr=%d\n",
			__func__, cid, cluster_nr);
			continue;
		}

		raw_spin_lock_irqsave(&cpu_rq(cpu)->__lock, flags);
		spin_lock(&per_cpu(nr_over_thres_lock, cpu));

		/* update threshold */
		if (cid == 0)
			cpu_over_thres->dn_thres = INT_MAX;
		else {
			over_threshold = get_over_threshold(cid-1);
			cpu_over_thres->dn_thres =
				(int)(cluster_over_thres_table[cid-1].max_capacity*
					over_threshold)/100;
		}

		if (cid == cluster_nr-1)
			cpu_over_thres->up_thres = INT_MAX;
		else {
			over_threshold = get_over_threshold(cid);
			cpu_over_thres->up_thres =
				(int)(cluster_over_thres_table[cid].max_capacity*
					over_threshold)/100;
		}

		/* pick next cpu if not online */
		if (!cpu_online(cpu)) {
			spin_unlock(&per_cpu(nr_over_thres_lock, cpu));
			raw_spin_unlock_irqrestore(&cpu_rq(cpu)->__lock, flags);
			continue;
		}

		/*
		 * re-calculate over_thres count when updated threshold
		 */
		list_for_each_entry(p, &cpu_rq(cpu)->cfs_tasks, se.group_node) {
			struct cc_task_struct *cc_ts =
				&((struct mtk_task *) p->android_vendor_data1)->cc_task;

			over_type = is_task_over_thres(p);
			WRITE_ONCE(cc_ts->over_type, (u64)over_type);

			if (over_type) {
				if (over_type == OVER_UP_THRES) {
					nr_over_dn_thres++;
					nr_over_up_thres++;
				} else {
					nr_over_dn_thres++;
				}
			}
		}

		/*
		 * Threshold for over_thres is changed.
		 * Need to reset stats
		 */
		cpu_over_thres->nr_over_up_thres = nr_over_up_thres;
		cpu_over_thres->nr_over_up_thres_prod_sum = 0;
		cpu_over_thres->up_last_update_time = curr_time;

		cpu_over_thres->nr_over_dn_thres = nr_over_dn_thres;
		cpu_over_thres->nr_over_dn_thres_prod_sum = 0;
		cpu_over_thres->dn_last_update_time = curr_time;

		cpu_over_thres->nr_running_diff = 0;
		spin_unlock(&per_cpu(nr_over_thres_lock, cpu));
		raw_spin_unlock_irqrestore(&cpu_rq(cpu)->__lock, flags);
	}
}

static void update_nr_over_thres_locked(struct over_thres_stats *stats,
					enum over_thres_type over_type,
					struct task_struct *p,
					unsigned long util,
					int inc);


static enum over_thres_type get_over_type(struct over_thres_stats *stats,
					  unsigned long util);

void sched_update_nr_over_thres_prod(struct task_struct *p, int cpu, int inc)
{
	unsigned long flags;
	enum over_thres_type over_type = NO_OVER_THRES;
	struct over_thres_stats *cpu_over_thres = NULL;
	unsigned long util;
	struct cc_task_struct *cc_ts = NULL;

	/* TODO: should be error handle ? */
	if (!init_thres) {
		printk_deferred_once("assertion failed at %s:%d\n",
				__FILE__,
				__LINE__);
		return;
	}

	util = task_util(p);
	spin_lock_irqsave(&per_cpu(nr_over_thres_lock, cpu), flags);
	cpu_over_thres = &per_cpu(cpu_over_thres_state, cpu);

	/* check if task is over threshold */
	over_type = get_over_type(cpu_over_thres, util);
	update_nr_over_thres_locked(cpu_over_thres, over_type, p, util, inc);
	cc_ts = &((struct mtk_task *) p->android_vendor_data1)->cc_task;
	WRITE_ONCE(cc_ts->over_type, (u64)over_type);
	spin_unlock_irqrestore(&per_cpu(nr_over_thres_lock, cpu), flags);
}

#if IS_ENABLED(CONFIG_SMP)
/*
 * Optional action to be done while updating the load average
 */
#define UPDATE_TG	0x1
#define SKIP_AGE_LOAD	0x2
#else
#define UPDATE_TG	0x0
#define SKIP_AGE_LOAD	0x0
#endif

static void update_nr_over_thres_locked(struct over_thres_stats *stats,
					enum over_thres_type over_type,
					struct task_struct *p,
					unsigned long util,
					int inc)
{
	s64 diff;
	u64 curr_time;

	if (over_type == NO_OVER_THRES)
		return;

	/* track task with max utilization */
	if (util > atomic_long_read(&stats->max_task_util))
		atomic_long_set(&stats->max_task_util, util);

	curr_time = sched_clock();
	if (over_type == OVER_UP_THRES) {
		/* OVER_UP_THRES */
		diff = (s64) (curr_time - stats->dn_last_update_time);
		/* update over_thres for degrading threshold */
		if (diff >= 0) {
			stats->dn_last_update_time = curr_time;
			stats->nr_over_dn_thres_prod_sum +=
				stats->nr_over_dn_thres*diff;
			stats->nr_over_dn_thres += inc;
		}

		diff = (s64) (curr_time - stats->up_last_update_time);
		/* update over_thres for upgrading threshold */
		if (diff >= 0) {
			stats->up_last_update_time = curr_time;
			stats->nr_over_up_thres_prod_sum +=
				stats->nr_over_up_thres*diff;
			stats->nr_over_up_thres += inc;
		}
	} else {/* OVER_DN_THRES */
		diff = (s64) (curr_time - stats->dn_last_update_time);
		if (diff >= 0) {
			stats->dn_last_update_time = curr_time;
			stats->nr_over_dn_thres_prod_sum +=
				stats->nr_over_dn_thres*diff;
			stats->nr_over_dn_thres += inc;
		}
	}
}

static enum over_thres_type get_over_type(struct over_thres_stats *stats,
					  unsigned long util)
{
	/* check if task is over threshold */
	if (util >= stats->up_thres)
		return OVER_UP_THRES;
	else if (util >= stats->dn_thres)
		return OVER_DN_THRES;
	else
		return NO_OVER_THRES;
}

void pelt_se_tp(void *data, struct sched_entity *se)
{
	struct task_struct *p;
	struct over_thres_stats *stats = NULL;
	enum over_thres_type old_type = NO_OVER_THRES;
	enum over_thres_type new_type = NO_OVER_THRES;
	unsigned long flags;
	unsigned long util;
	int cpu;
	struct cc_task_struct *cc_ts = NULL;

	if (!init_thres) {
		printk_deferred_once("assertion failed at %s:%d\n",
				__FILE__,
				__LINE__);
		return;
	}

	if (entity_is_task(se) && se->on_rq) {
		p = task_of(se);
		cpu = cpu_of(task_rq(p));

		spin_lock_irqsave(&per_cpu(nr_over_thres_lock, cpu), flags);
		cc_ts = &((struct mtk_task *) p->android_vendor_data1)->cc_task;
		old_type = READ_ONCE(cc_ts->over_type);

		util = task_util(p);
		stats = &per_cpu(cpu_over_thres_state, cpu);
		new_type = get_over_type(stats, util);

		if (old_type == NO_OVER_THRES && new_type != NO_OVER_THRES)
			update_nr_over_thres_locked(stats, new_type, p, util, 1);
		else if (old_type != NO_OVER_THRES && new_type == NO_OVER_THRES)
			update_nr_over_thres_locked(stats, old_type, p, util, -1);
		else {
			update_nr_over_thres_locked(stats, new_type, p, util, 0);
			/* Fixup up_thres */
			if (old_type == OVER_UP_THRES &&
					new_type == OVER_DN_THRES)
				--stats->nr_over_up_thres;
			if (old_type == OVER_DN_THRES &&
					new_type == OVER_UP_THRES)
				++stats->nr_over_up_thres;
		}
		WRITE_ONCE(cc_ts->over_type, (u64)new_type);
		spin_unlock_irqrestore(&per_cpu(nr_over_thres_lock, cpu), flags);
	}
}


#if IS_ENABLED(CONFIG_FAIR_GROUP_SCHED)
/* Walk up scheduling entities hierarchy */
#define for_each_sched_entity(se) \
	for (; se; se = se->parent)
#else
#define for_each_sched_entity(se) \
	for (; se; se = NULL)
#endif

#if IS_ENABLED(CONFIG_CFS_BANDWIDTH)
static inline bool cfs_bandwidth_used(void)
{
	return true;
}

static inline int cfs_rq_throttled(struct cfs_rq *cfs_rq)
{
	return cfs_bandwidth_used() && cfs_rq->throttled;
}
#endif

void inc_nr_over_thres_running(void *data, struct rq *rq, struct task_struct *p, int flags)
{
#if IS_ENABLED(CONFIG_CFS_BANDWIDTH)
	struct cfs_rq *cfs_rq;
	struct sched_entity *se = &p->se;

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_of(se);

		if (cfs_rq_throttled(cfs_rq))
			return;
	}
#endif
	sched_update_nr_over_thres_prod(p, cpu_of(rq), 1);
}

void dec_nr_over_thres_running(void *data, struct rq *rq, struct task_struct *p, int flags)
{
#if IS_ENABLED(CONFIG_CFS_BANDWIDTH)
	struct cfs_rq *cfs_rq;
	struct sched_entity *se = &p->se;

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_of(se);

		if (cfs_rq_throttled(cfs_rq))
			return;
	}
#endif
	sched_update_nr_over_thres_prod(p, cpu_of(rq), -1);
}

/*
 * if all core ids are set correctly, then return true.
 * That is meaning that there is no repeat core id in same
 * cluster.
 */
static inline bool is_all_cpu_parsed(void)
{
	int cpu = 0, core_id = 0, cluster_id = 0;
	bool all_parsed = true;
	struct cpumask cluster_mask[MAX_CLUSTER_NR];
	struct cpumask *tmp_cpumask = NULL;

	for (cluster_id = 0; cluster_id < MAX_CLUSTER_NR; cluster_id++)
		cpumask_clear(&cluster_mask[cluster_id]);

	for_each_possible_cpu(cpu) {
		core_id = cpu_topology[cpu].core_id;
		cluster_id = cpu_topology[cpu].cluster_id;
		if (core_id < 0 || cluster_id < 0) {
			all_parsed = false;
			break;
		}
		tmp_cpumask = &cluster_mask[cluster_id];
		if (cpumask_test_cpu(core_id, tmp_cpumask)) {
			all_parsed = false;
			break;
		}
		cpumask_set_cpu(core_id, tmp_cpumask);
	}
	return all_parsed;
}

static int init_thres_table(void)
{
	int i, cid, cpu;
	int cluster_nr;
	struct cpumask cls_cpus;
	struct over_thres_stats *cpu_over_thres;
	unsigned int over_threshold = 100;

	if (!is_all_cpu_parsed())
		return -ENFILE;

	if (init_thres)
		return 0;

	printk_deferred_once("%s start.\n", __func__);

	global_task_util = 0;

	/* allocation for clustser information */
	cluster_nr = arch_get_nr_clusters();
	if (cluster_nr <= 0)
		return -EINVAL;

	for (i = 0; i < cluster_nr; i++) {
		arch_get_cluster_cpus(&cls_cpus, i);
		cpu = cpumask_first(&cls_cpus);
		cluster_over_thres_table[i].max_capacity =
			capacity_orig_of(cpu);
	}

	for_each_possible_cpu(cpu) {
		cpu_over_thres = &per_cpu(cpu_over_thres_state, cpu);
		cid = arch_get_cluster_id(cpu);
		cpu_over_thres->nr_over_dn_thres = 0;
		cpu_over_thres->nr_over_up_thres = 0;
		atomic_long_set(&cpu_over_thres->max_task_util, 0);

		if (cid < 0 || cid >= cluster_nr) {
			pr_info("%s: cid=%d is out of nr=%d\n", __func__, cid, cluster_nr);
			continue;
		}

		/*
		 * Initialize threshold for up/down
		 * over-utilization tracking
		 */
		if (cid == 0)
			cpu_over_thres->dn_thres = INT_MAX;
		else {
			over_threshold = get_over_threshold(cid-1);
			cpu_over_thres->dn_thres =
				(int)(cluster_over_thres_table[cid-1].max_capacity*
						over_threshold)/100;
		}

		if (cid == cluster_nr-1)
			cpu_over_thres->up_thres = INT_MAX;
		else {
			over_threshold = get_over_threshold(cid);
			cpu_over_thres->up_thres =
				(int)(cluster_over_thres_table[cid].max_capacity*
						over_threshold)/100;
		}

		pr_info("%s: cpu=%d dn_thres=%d up_thres=%d max_capaicy=%lu\n",
				__func__,
				cpu,
				cpu_over_thres->dn_thres,
				cpu_over_thres->up_thres,
				(unsigned long)cluster_over_thres_table[cid].max_capacity);
	}

	init_thres = 1;
	return 0;
}

int get_over_thres_stats(char *buf, int buf_size)
{
	int cpu;
	int len = 0;
	struct over_thres_stats *cpu_over_thres;

	for_each_possible_cpu(cpu) {
		cpu_over_thres = &per_cpu(cpu_over_thres_state, cpu);

		len += snprintf(buf+len, buf_size-len,
			"cpu=%d capacity=%lu dn_thres=%d up_thres=%d\n",
			cpu, cpu_rq(cpu)->cpu_capacity_orig,
			cpu_over_thres->dn_thres,
			cpu_over_thres->up_thres);
	}

	for_each_possible_cpu(cpu) {
		cpu_over_thres = &per_cpu(cpu_over_thres_state, cpu);

		len += snprintf(buf+len, buf_size-len,
			"cpu=%d nr_over_dn_thres=%d nr_over_up_thres=%d\n",
			cpu, cpu_over_thres->nr_over_dn_thres,
			cpu_over_thres->nr_over_up_thres);
	}

	return len;
}

static ssize_t show_over_util(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	unsigned int len = 0, i;
	unsigned int max_len = 4096;

	for (i = 0; i < OVER_THRES_SIZE; i++) {
		len += snprintf(buf+len,
			max_len-len, "over_util threshold[%u]=%d max=100\n",
				i, over_thres[i]);
	}
	len += get_over_thres_stats(buf+len, max_len-len);
	/* show maximum task_utils */
	len += snprintf(buf+len, max_len-len,
			"maximum task_utils = %d\n", global_task_util);
	return len;
}

static struct kobj_attribute over_util_attr =
__ATTR(over_util, 0400, show_over_util, NULL);

static struct attribute *sched_avg_attrs[] = {
	&over_util_attr.attr,
	NULL,
};

static struct attribute_group sched_avg_attr_group = {
	.attrs = sched_avg_attrs,
};

static int init_attribs(void)
{
	int ret = 0;
	struct kobject *kobj = NULL;

	kobj = kobject_create_and_add("sched-avg", &cpu_subsys.dev_root->kobj);

	if (kobj) {
		ret = sysfs_create_group(kobj, &sched_avg_attr_group);
		if (ret)
			kobject_put(kobj);
		else
			kobject_uevent(kobj, KOBJ_ADD);
	} else
		ret = -ENOMEM;

	return ret;
}

int init_sched_avg(void)
{
	int ret, ret_error_line;

	ret = init_thres_table();
	if (ret) {
		ret_error_line = __LINE__;
		goto failed;
	}

	ret = init_attribs();
	if (ret) {
		ret_error_line = __LINE__;
		goto failed;
	}

	ret = register_trace_android_rvh_enqueue_task_fair(
			inc_nr_over_thres_running, NULL);
	if (ret) {
		ret_error_line = __LINE__;
		goto failed;
	}

	ret = register_trace_android_rvh_dequeue_task_fair(
			dec_nr_over_thres_running, NULL);
	if (ret) {
		ret_error_line = __LINE__;
		goto failed_deprobe_enqueue_task_fair;
	}


	ret = register_trace_pelt_se_tp(pelt_se_tp, NULL);
	if (ret) {
		ret_error_line = __LINE__;
		goto failed_deprobe_dequeue_task_fair;
	}

	ret = register_trace_sched_update_nr_running_tp(
			sched_update_nr_running_cb, NULL);
	if (ret) {
		ret_error_line = __LINE__;
		goto failed_deprobe_pelt_se_tp;
	}

	/*
	 * reset over_thress value to prevent mis-patch
	 * condition after KO is ready to install.
	 */
	over_thresh_chg_notify();
	pr_info("%s: init finish! ", TAG);
	return 0;

failed_deprobe_pelt_se_tp:
	unregister_trace_pelt_se_tp(pelt_se_tp, NULL);
failed_deprobe_dequeue_task_fair:
failed_deprobe_enqueue_task_fair:
	/* restrict vendor hook can't unregister */
	tracepoint_synchronize_unregister();
failed:
	pr_info("%s: init failed ret %d line %d\n",
			TAG, ret, ret_error_line);
	return ret;
}

void exit_sched_avg(void)
{
	unregister_trace_pelt_se_tp(pelt_se_tp, NULL);
	unregister_trace_sched_update_nr_running_tp(sched_update_nr_running_cb, NULL);
}
