/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __XIAOMI__TOUCH_H
#define __XIAOMI__TOUCH_H
#include <linux/device.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/wait.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/debugfs.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/mempolicy.h>
#include <linux/dma-mapping.h>
#include <linux/export.h>
#include <linux/rtc.h>
#include <linux/seq_file.h>
#include <linux/time.h>
#include <linux/time64.h>
#include <drm/drm_panel.h>
#include "xiaomi_touch_type_common.h"

/*CUR,DEFAULT,MIN,MAX*/
#define VALUE_TYPE_SIZE			6
#define VALUE_GRIP_SIZE			9
#define MAX_BUF_SIZE			256
#define BTN_INFO			0x152
#define MAX_TOUCH_ID			10
#define RAW_BUF_NUM			4
#define THP_CMD_BASE			1000
#define TP_VERSION_SIZE			64
#define PARAM_BUF_NUM			10
#define Touch_GAMETURBOTOOL_BASE 10000

#define COMMON_DATA_BUF_SIZE	10

#define TOUCH_FOD_SUPPORT 1


#define mi_ts_info(fmt, arg...)    pr_info("[MI_TP-INF][%s:%d] "fmt"\n", __func__, __LINE__, ##arg)
#define mi_ts_err(fmt, arg...)     pr_err("[MI_TP-ERR][%s:%d] "fmt"\n", __func__, __LINE__, ##arg)
#define XIAOMI_TOUCH_UTC_PRINT(tag) \
	do { \
		struct timespec64 ts; \
		struct tm tm; \
		ktime_get_real_ts64(&ts); \
		time64_to_tm(ts.tv_sec, 0, &tm); \
		mi_ts_err("%s [xiaomi_touch_utc] [%d-%02d-%02d %02d:%02d:%02d.%06lu]\n", \
			tag, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, \
			tm.tm_hour, tm.tm_min, tm.tm_sec, ts.tv_nsec/1000); \
	} while(0)

// enum suspend_state {
// 	XIAOMI_TOUCH_RESUME = 0,
// 	XIAOMI_TOUCH_SUSPEND,
// 	XIAOMI_TOUCH_LP1,
// 	XIAOMI_TOUCH_LP2,
// };

// enum MODE_CMD {
// 	SET_CUR_VALUE = 0,
// 	GET_CUR_VALUE,
// 	GET_DEF_VALUE,
// 	GET_MIN_VALUE,
// 	GET_MAX_VALUE,
// 	GET_MODE_VALUE,
// 	RESET_MODE,
// 	SET_LONG_VALUE,
// };

enum touch_doze_analysis {
	POWER_RESET = 0,
	RELOAD_FW,
	ENABLE_IRQ,
	DISABLE_IRQ,
	REGISTER_IRQ,
	IRQ_PIN_LEVEL,
	ENTER_SUSPEND,
	ENTER_RESUME,
};

// enum MODE_TYPE {
// 	Touch_Game_Mode			= 0,
// 	Touch_Active_MODE		= 1,
// 	Touch_UP_THRESHOLD		= 2,
// 	Touch_Tolerance			= 3,
// 	Touch_Aim_Sensitivity		= 4,
// 	Touch_Tap_Stability		= 5,
// 	Touch_Expert_Mode		= 6,
// 	Touch_Edge_Filter		= 7,
// 	Touch_Panel_Orientation		= 8,
// 	Touch_Report_Rate		= 9,
// 	Touch_Fod_Enable		= 10,
// 	Touch_Aod_Enable		= 11,
// 	Touch_Resist_RF			= 12,
// 	Touch_Idle_Time			= 13,
// 	Touch_Doubletap_Mode		= 14,
// 	Touch_Grip_Mode			= 15,
// 	Touch_FodIcon_Enable		= 16,
// 	Touch_Nonui_Mode		= 17,
// 	Touch_Debug_Level		= 18,
// 	Touch_Power_Status		= 19,
// 	Touch_Super_Report              = 29,
// 	Touch_Idle_Scan_Rate            = 30,
// 	Touch_Idle_THD                  = 31,
// 	Touch_Resolution                = 32,
// 	Touch_Boost_EN                  = 33,
// 	Touch_Mode_NUM                  = 34,
// 	THP_LOCK_SCAN_MODE		= THP_CMD_BASE + 0,
// 	THP_FOD_DOWNUP_CTL		= THP_CMD_BASE + 1,
// 	THP_SELF_CAP_SCAN		= THP_CMD_BASE + 2,
// 	THP_REPORT_POINT_SWITCH		= THP_CMD_BASE + 3,
// 	THP_HAL_INIT_READY		= THP_CMD_BASE + 4,
// 	THP_HAL_VSYNC_MODE   		= THP_CMD_BASE + 9,
// 	THP_HAL_CHARGING_STATUS		= THP_CMD_BASE + 10,
// 	THP_HAL_REPORT_RATE       	= THP_CMD_BASE + 11,
// 	THP_HAL_DISPLAY_FPS       	= THP_CMD_BASE + 12,
// 	THP_KNOCK_FRAME_COUNT    	= THP_CMD_BASE + 13,
// 	THP_HAL_TOUCH_SENSOR       	= THP_CMD_BASE + 15,
// 	THP_NORMALIZE_FREQ_SCAN  	= THP_CMD_BASE + 68,
// 	THP_NORMALIZE_K_REQUEST  	= THP_CMD_BASE + 69,
// 	THP_NORMALIZE_B_REQUEST  	= THP_CMD_BASE + 70,
// 	THP_IDLE_BASALINE_UPDATE	= THP_CMD_BASE + 71,
// 	Touch_THP_Dump              = THP_CMD_BASE + 76,
// 	Touch_GAMETURBOTOOL_FOLLOWUP	= Touch_GAMETURBOTOOL_BASE + 1,
// 	Touch_GAMETURBOTOOL_RESPONSE	= Touch_GAMETURBOTOOL_BASE + 2,
// 	Touch_GAMETURBOTOOL_SHAKING		= Touch_GAMETURBOTOOL_BASE + 3,
// 	Touch_GAMETURBOTOOL_NUM,
// };

struct xiaomi_touch_interface {
	int thp_cmd_buf[MAX_BUF_SIZE];
	char thp_cmd_data_buf[MAX_BUF_SIZE];
	int thp_cmd_ready_buf[MAX_BUF_SIZE];
	int thp_cmd_size;
	int thp_cmd_ready_size;
	int thp_cmd_data_size;
	int touch_mode[Touch_Mode_NUM][VALUE_TYPE_SIZE];
	wait_queue_head_t wait_queue;
	wait_queue_head_t wait_queue_ready;
	int touch_event_status;
	int touch_event_ready_status;
	int (*setModeValue)(int Mode, int value);
	int (*setModeLongValue)(int Mode, int value_len, int *value);
	int (*getModeValue)(int Mode, int value_type);
	int (*getModeAll)(int Mode, int *modevalue);
	int (*resetMode)(int Mode);
	int (*prox_sensor_read)(void);
	int (*prox_sensor_write)(int on);
	int (*palm_sensor_read)(void);
	int (*palm_sensor_write)(int on);
	int (*get_touch_rx_num)(void);
	int (*get_touch_tx_num)(void);
	int (*get_touch_freq_num)(void);
	int (*get_touch_x_resolution)(void);
	int (*get_touch_y_resolution)(void);
	int (*set_up_interrupt_mode)(int en);
	int (*enable_touch_raw)(int en);
	int (*enable_clicktouch_raw)(int count);
	int (*enable_touch_delta)(bool en);
	u8 (*panel_vendor_read)(void);
	u8 (*panel_color_read)(void);
	u8 (*panel_display_read)(void);
	char (*touch_vendor_read)(void);
	int (*get_touch_super_resolution_factor)(void);
	int (*set_touch_reg_status)(void);
	int (*set_touch_fw_update)(void);
	int (*set_irq_disable)(void);
	int (*set_irq_enable)(void);
	int (*set_irq_re_register)(void);
	int (*get_irq_status)(void);
	int (*touch_doze_analysis)(int value);
	u8 * (*get_touch_ic_buffer)(void);
	int long_mode_len;
	int long_mode_value[MAX_BUF_SIZE];

	bool is_enable_touchraw;
	int palm_sensor_onoff;
	int thp_downthreshold;
	int thp_upthreshold;
	int thp_movethreshold;
	int thp_noisefilter;
	int thp_islandthreshold;
	int thp_smooth;
	int thp_dump_raw;
	int thp_test_mode;
	int thp_test_result;
	int thp_preset_point;
	int thp_disconnect_detect;
	int thp_disconnect_result;
	char thp_disconnect_type[MAX_BUF_SIZE];
	char tp_hal_version[TP_VERSION_SIZE];
	bool is_enable_touchdelta;
	bool active_status;
	bool finger_status;
	int irq_no;
	int touch_sensor_ctrl_value;
	struct list_head private_data_list;
	spinlock_t private_data_lock;
	struct mutex common_data_buf_lock;
	atomic_t common_data_buf_index;
	common_data_t common_data_buf[COMMON_DATA_BUF_SIZE];
};

struct xiaomi_touch {
	struct miscdevice misc_dev;
	struct device *dev;
	struct class *class;
	struct attribute_group attrs;
	struct mutex mutex;
	struct mutex palm_mutex;
	struct mutex prox_mutex;
	struct mutex gesture_single_tap_mutex;
	struct mutex fod_press_status_mutex;
	struct mutex abnormal_event_mutex;
	wait_queue_head_t wait_queue;
};

#define LAST_TOUCH_EVENTS_MAX		512

enum touch_state {
	EVENT_INIT,
	EVENT_DOWN,
	EVENT_UP,
};

struct touch_event {
	u32 slot;
	enum touch_state state;
	struct timespec64 touch_time;
};

struct last_touch_event {
	int head;
	struct touch_event touch_event_buf[LAST_TOUCH_EVENTS_MAX];
};

struct touch_cmd_info {
	unsigned int param_buf[MAX_BUF_SIZE];
	int thp_cmd_size;
};

struct abnormal_event {
	u16 type;
	u16 code;
	u16 value;
};

#define SENSITIVE_EVENT_BUF_SIZE	(10)
struct abnormal_event_buf {
	int top;
	int bottom;
	bool full_flag;
	struct abnormal_event abnormal_event[SENSITIVE_EVENT_BUF_SIZE];
};

struct xiaomi_touch_pdata {
	struct device_node *of_node;
	struct xiaomi_touch *device;
	struct xiaomi_touch_interface *touch_data[2];
	int suspend_state;
	dma_addr_t phy_base;
	int raw_head;
	int raw_tail;
	int raw_len;
	unsigned int *raw_buf[RAW_BUF_NUM];
	unsigned int *raw_data;
	spinlock_t raw_lock;
	int palm_value;
	bool palm_changed;
	int prox_value;
	bool prox_changed;
	const char *name;
	int fod_press_status_value;
	struct proc_dir_entry *last_touch_events_proc;
	struct proc_dir_entry *tp_hal_version_proc;
	struct last_touch_event *last_touch_events;
	int param_head;
	int param_tail;
	struct touch_cmd_info *touch_cmd_data[PARAM_BUF_NUM];
	spinlock_t param_lock;
	int param_flag;
	struct abnormal_event_buf abnormal_event_buf;
	struct list_head node;
	wait_queue_head_t poll_wait_queue_head;
	atomic_t common_data_index;
};

struct xiaomi_touch *xiaomi_touch_dev_get(int minor);

extern struct class *get_xiaomi_touch_class(void);

extern struct device *get_xiaomi_touch_dev(void);

extern int update_palm_sensor_value(int value);

extern int update_prox_sensor_value(int value);

extern int xiaomitouch_register_modedata(int touchId, struct xiaomi_touch_interface *data);

extern int copy_touch_rawdata(char *raw_base,  int len);

extern int update_touch_rawdata(void);
extern void get_current_timestamp(char* timebuf, int len);

extern int update_clicktouch_raw(void);

extern void last_touch_events_collect(int slot, int state);

int xiaomi_touch_set_suspend_state(int state);

struct drm_panel *xiaomi_touch_check_panel(void);

void add_common_data_to_buf(s8 touch_id, enum common_data_cmd cmd, enum common_data_mode mode, int length, int *data);

extern int notify_gesture_single_tap(void);
#ifdef	TOUCH_FOD_SUPPORT
extern int update_fod_press_status(int value);
#endif

extern void thp_send_cmd_to_hal(int cmd, int value);

extern void update_active_status(bool status);

extern void update_touch_irq_no(int irq_no);

extern int knock_node_init(void);
extern void knock_node_release(void);
extern void register_frame_count_change_listener(void *listener);
extern void update_knock_data(u8 *buf, int size, int frame_id);
extern void knock_data_notify(void);


#endif
