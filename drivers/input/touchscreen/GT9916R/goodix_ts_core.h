/* SPDX-License-Identifier: GPL-2.0
 *
 * Goodix TouchScreen driver.
 *
 * Copyright (c) 2012-2020, FocalTech Systems, Ltd., all rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef _GOODIX_TS_CORE_H_
#define _GOODIX_TS_CORE_H_
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/firmware.h>
#include <linux/slab.h>
#include <asm/unaligned.h>
#include <linux/vmalloc.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/completion.h>
#include <linux/of_irq.h>
#include <linux/pm_runtime.h>
#ifdef CONFIG_OF
#include <linux/of_gpio.h>
#include <linux/regulator/consumer.h>
#endif
#ifdef CONFIG_FB
#include <linux/notifier.h>
#include <linux/fb.h>
#endif
#include "../xiaomi_touch/xiaomi_touch_common.h"

//#define GOODIX_SPI_NAME                 "GOODIX-TS"
#define GOODIX_CORE_DRIVER_NAME			"goodix_ts"
#define GOODIX_PEN_DRIVER_NAME			"goodix_ts,pen"
#define GOODIX_DRIVER_VERSION			"v1.2.2"
#define GOODIX_MAX_TOUCH				10
#define GOODIX_PEN_MAX_PRESSURE			4096
#define GOODIX_MAX_PEN_KEY				2
#define GOODIX_PEN_MAX_TILT				90
#define GOODIX_CFG_MAX_SIZE				4096
#define GOODIX_MAX_STR_LABLE_LEN		32
#define GOODIX_MAX_FRAMEDATA_LEN		2500

#define GOODIX_NORMAL_RESET_DELAY_MS	100
#define GOODIX_HOLD_CPU_RESET_DELAY_MS  5

#define GOODIX_RETRY_3					3
#define GOODIX_RETRY_5					5
#define GOODIX_RETRY_10					10

#define TS_DEFAULT_FIRMWARE				"goodix_firmware.bin"
#define TS_DEFAULT_CFG_BIN				"goodix_cfg_group.bin"
#define GOODIX_LOCKDOWN_SIZE		8
#define TS_LOCKDOWN_REG				0x10030

#define FLASH_WRITE_MAX_LEN  4096
#define FLASH_READ_MAX_LEN  4096

#define GOODIX_XIAOMI_TOUCHFEATURE
#define GOODIX_DEBUGFS_ENABLE
/* #define CONFIG_TOUCH_BOOST */
#define GRIP_PARAMETER_NUM 8
/*#define GRIP_MODE_DEBUG*/
#define SUPER_RESOLUTION

#define GTP_RESULT_INVALID				0
#define GTP_RESULT_FAIL					1
#define GTP_RESULT_PASS					2
#define CONFIG_TOUCHSCREEN_GOODIX_BRL_SPI

#define PANEL_ORIENTATION_DEGREE_0	0	/* normal portrait orientation */
#define PANEL_ORIENTATION_DEGREE_90	1	/* anticlockwise 90 degrees */
#define PANEL_ORIENTATION_DEGREE_180	2	/* anticlockwise 180 degrees */
#define PANEL_ORIENTATION_DEGREE_270	3	/* anticlockwise 270 degrees */

/* gamemode edge suppression params */

/* deadzone suppression */
#define GAME_DEAD_SUPPRESSION	7
/* deadzone suppression end */

/* edgezone suppression */
/* for PANEL_ORIENTATION_DEGREE_0 & PANEL_ORIENTATION_DEGREE_180 */
#define GAME_EDGE_SUPPRESSION_LONGSIDE_0_180	60
#define GAME_EDGE_SUPPRESSION_SHORTSIDE_0_180	20
/* for PANEL_ORIENTATION_DEGREE_90 & PANEL_ORIENTATION_DEGREE_270 */
#define GAME_EDGE_SUPPRESSION_LONGSIDE_90_270	60
#define GAME_EDGE_SUPPRESSION_SHORTSIDE_90_270	70
/* edgezone suppression end */

/* conrnerzone suppression */
/* conrnerzone suppression case1 */
/*横屏游戏*/
#define GAME_CORNER_SUPPRESSION_NONE	0
#define GAME_CORNER_SUPPRESSION_SMALL	150
#define GAME_CORNER_SUPPRESSION_MEDIUM	280
#define GAME_CORNER_SUPPRESSION_LARGE	400
/*竖屏游戏*/
#define VERTICAL_GAME_CORNER_SUPPRESSION_Y  400
#define VERTICAL_GAME_CORNER_SUPPRESSION_X  250
/* conrner suppression case2 */
#define GAME_CORNER_SUPPRESSION_HOR 	0
#define GAME_CORNER_SUPPRESSION_VER 	0
/* conrnerzone suppression end */

/*panel info*/
#define PANEL_MAX_X 1219
#define PANEL_MAX_Y 2711
/*panel info end*/

#define GAME_ARRAY_LEN    4
#define GAME_ARRAY_SIZE   3

enum CORD_PROB_STA {
	CORE_MODULE_UNPROBED = 0,
	CORE_MODULE_PROB_SUCCESS = 1,
	CORE_MODULE_PROB_FAILED = -1,
	CORE_MODULE_REMOVED = -2,
};

enum GOODIX_ERR_CODE {
	GOODIX_EBUS      = (1<<0),
	GOODIX_ECHECKSUM = (1<<1),
	GOODIX_EVERSION  = (1<<2),
	GOODIX_ETIMEOUT  = (1<<3),
	GOODIX_EMEMCMP   = (1<<4),

	GOODIX_EOTHER    = (1<<7)
};

enum IC_TYPE_ID {
	IC_TYPE_NONE,
	IC_TYPE_NORMANDY,
	IC_TYPE_NANJING,
	IC_TYPE_YELLOWSTONE,
	IC_TYPE_BERLIN_A,
	IC_TYPE_BERLIN_B,
	IC_TYPE_BERLIN_D
};

enum GOODIX_IC_CONFIG_TYPE {
	CONFIG_TYPE_TEST = 0,
	CONFIG_TYPE_NORMAL = 1,
	CONFIG_TYPE_HIGHSENSE = 2,
	CONFIG_TYPE_CHARGER = 3,
	CONFIG_TYPE_CHARGER_HS = 4,
	CONFIG_TYPE_HOLSTER = 5,
	CONFIG_TYPE_HOSTER_CH = 6,
	CONFIG_TYPE_OTHER = 7,
	/* keep this at the last */
	GOODIX_MAX_CONFIG_GROUP = 8,
};

enum CHECKSUM_MODE {
	CHECKSUM_MODE_U8_LE,
	CHECKSUM_MODE_U16_LE,
};

#define MAX_SCAN_FREQ_NUM            10
#define MAX_SCAN_RATE_NUM            5
#define MAX_FREQ_NUM_STYLUS          8
#define MAX_STYLUS_SCAN_FREQ_NUM     6
#pragma pack(1)

typedef struct __attribute__((packed)){
	uint32_t checksum;
	uint32_t address;
	uint32_t length;
}flash_head_info_t;

struct frame_head {
	uint8_t sync;
	uint16_t frame_index;
	uint16_t cur_frame_len;
	uint16_t next_frame_len;
	uint32_t data_en; /* 0- 7 for pack_en; 8 - 31 for type en */
	uint8_t touch_pack_index;
	uint8_t stylus_pack_index;
	uint8_t res;
	uint16_t checksum;
};

struct goodix_fw_version {
	u8 rom_pid[6];               /* rom PID */
	u8 rom_vid[3];               /* Mask VID */
	u8 rom_vid_reserved;
	u8 patch_pid[8];              /* Patch PID */
	u8 patch_vid[4];              /* Patch VID */
	u8 patch_vid_reserved;
	u8 sensor_id;
	u8 reserved[2];
	u16 checksum;
};

struct goodix_ic_info_version {
	u8 info_customer_id;
	u8 info_version_id;
	u8 ic_die_id;
	u8 ic_version_id;
	u32 config_id;
	u8 config_version;
	u8 frame_data_customer_id;
	u8 frame_data_version_id;
	u8 touch_data_customer_id;
	u8 touch_data_version_id;
	u8 reserved[3];
};

struct goodix_ic_info_feature { /* feature info*/
	u16 freqhop_feature;
	u16 calibration_feature;
	u16 gesture_feature;
	u16 side_touch_feature;
	u16 stylus_feature;
};

struct goodix_ic_info_param { /* param */
	u8 drv_num;
	u8 sen_num;
	u8 button_num;
	u8 force_num;
	u8 active_scan_rate_num;
	u16 active_scan_rate[MAX_SCAN_RATE_NUM];
	u8 mutual_freq_num;
	u16 mutual_freq[MAX_SCAN_FREQ_NUM];
	u8 self_tx_freq_num;
	u16 self_tx_freq[MAX_SCAN_FREQ_NUM];
	u8 self_rx_freq_num;
	u16 self_rx_freq[MAX_SCAN_FREQ_NUM];
	u8 stylus_freq_num;
	u16 stylus_freq[MAX_FREQ_NUM_STYLUS];
};

struct goodix_ic_info_misc { /* other data */
	u32 cmd_addr;
	u16 cmd_max_len;
	u32 cmd_reply_addr;
	u16 cmd_reply_len;
	u32 fw_state_addr;
	u16 fw_state_len;
	u32 fw_buffer_addr;
	u16 fw_buffer_max_len;
	u32 frame_data_addr;
	u16 frame_data_head_len;
	u16 fw_attr_len;
	u16 fw_log_len;
	u8 pack_max_num;
	u8 pack_compress_version;
	u16 stylus_struct_len;
	u16 mutual_struct_len;
	u16 self_struct_len;
	u16 noise_struct_len;
	u32 touch_data_addr;
	u16 touch_data_head_len;
	u16 point_struct_len;
	u16 panel_x;
	u16 panel_y;
	u32 mutual_rawdata_addr;
	u32 mutual_diffdata_addr;
	u32 mutual_refdata_addr;
	u32 self_rawdata_addr;
	u32 self_diffdata_addr;
	u32 self_refdata_addr;
	u32 iq_rawdata_addr;
	u32 iq_refdata_addr;
	u32 im_rawdata_addr;
	u16 im_readata_len;
	u32 noise_rawdata_addr;
	u16 noise_rawdata_len;
	u32 stylus_rawdata_addr;
	u16 stylus_rawdata_len;
	u32 noise_data_addr;
	u32 esd_addr;
	u32 auto_scan_cmd_addr;
	u32 auto_scan_info_addr;
};

struct goodix_ic_info_other {
	u16 normalize_k_version;
	u32 irrigation_data_addr;
	u32 algo_debug_data_addr;
	u16 algo_debug_data_len;
	u32 update_sync_data_addr;
	u16 screen_max_x;
	u16 screen_max_y;
};

struct goodix_ic_info {
	u16 length;
	struct goodix_ic_info_version version;
	struct goodix_ic_info_feature feature;
	struct goodix_ic_info_param parm;
	struct goodix_ic_info_misc misc;
	struct goodix_ic_info_other other;
};
#pragma pack()

/*
 * struct ts_rawdata_info
 *
 */
#define TS_RAWDATA_BUFF_MAX             7000
#define TS_RAWDATA_RESULT_MAX           100
struct ts_rawdata_info {
	int used_size; //fill in rawdata size
	s16 buff[TS_RAWDATA_BUFF_MAX];
	char result[TS_RAWDATA_RESULT_MAX];
};

#define FRAME_DATA_MAX_LEN	2000
struct ts_framedata {
	unsigned char buff[FRAME_DATA_MAX_LEN];
	int used_size;
};

/*
 * struct goodix_module - external modules container
 * @head: external modules list
 * @initilized: whether this struct is initilized
 * @mutex: mutex lock
 * @wq: workqueue to do register work
 * @core_data: core_data pointer
 */
struct goodix_module {
	struct list_head head;
	bool initilized;
	struct mutex mutex;
	struct workqueue_struct *wq;
	struct goodix_ts_core *core_data;
};

/*
 * struct goodix_ts_board_data -  board data
 * @avdd_name: name of analoy regulator
 * @iovdd_name: name of analoy regulator
 * @reset_gpio: reset gpio number
 * @irq_gpio: interrupt gpio number
 * @irq_flag: irq trigger type
 * @swap_axis: whether swaw x y axis
 * @panel_max_x/y/w/p: resolution and size
 * @pannel_key_map: key map
 * @fw_name: name of the firmware image
 */
struct goodix_ts_board_data {
	char avdd_name[GOODIX_MAX_STR_LABLE_LEN];
	char iovdd_name[GOODIX_MAX_STR_LABLE_LEN];
	int reset_gpio;
	int irq_gpio;
	int avdd_gpio;
	int iovdd_gpio;
	unsigned int  irq_flags;

	unsigned int swap_axis;
	unsigned int panel_max_x;
	unsigned int panel_max_y;
	unsigned int panel_max_w; /*major and minor*/
	unsigned int panel_max_p; /*pressure*/

	bool pen_enable;
	char fw_name[GOODIX_MAX_STR_LABLE_LEN];
	char cfg_bin_name[GOODIX_MAX_STR_LABLE_LEN];
	u32 touch_expert_array[GAME_ARRAY_LEN * GAME_ARRAY_SIZE];
};

enum goodix_fw_update_mode {
	UPDATE_MODE_DEFAULT = 0,
	UPDATE_MODE_FORCE = (1<<0), /* force update mode */
	UPDATE_MODE_BLOCK = (1<<1), /* update in block mode */
	UPDATE_MODE_FLASH_CFG = (1<<2), /* reflash config */
	UPDATE_MODE_SRC_SYSFS = (1<<4), /* firmware file from sysfs */
	UPDATE_MODE_SRC_HEAD = (1<<5), /* firmware file from head file */
	UPDATE_MODE_SRC_REQUEST = (1<<6), /* request firmware */
	UPDATE_MODE_SRC_ARGS = (1<<7), /* firmware data from function args */
};

#define GOODIX_GRIP_PARAMETERS_SIZE 32
#define GOODIX_TOUCH_MODE_PARAMETERS_SIZE 5
#define GOODIX_CORNERFILTER_AREA_STEP_SIZE 4
#define GOODIX_DISPLAY_RESOLUTION_SIZE 2

struct goodix_xiaomi_board_data {
	unsigned int game_mode[GOODIX_TOUCH_MODE_PARAMETERS_SIZE];
	unsigned int active_mode[GOODIX_TOUCH_MODE_PARAMETERS_SIZE];
	unsigned int up_threshold[GOODIX_TOUCH_MODE_PARAMETERS_SIZE];
	unsigned int tolerance[GOODIX_TOUCH_MODE_PARAMETERS_SIZE];
	unsigned int edge_filter[GOODIX_TOUCH_MODE_PARAMETERS_SIZE];
	unsigned int panel_orien[GOODIX_TOUCH_MODE_PARAMETERS_SIZE];
	unsigned int report_rate[GOODIX_TOUCH_MODE_PARAMETERS_SIZE];
	unsigned int cornerzone_filter_hor1[GOODIX_GRIP_PARAMETERS_SIZE];
	unsigned int cornerzone_filter_hor2[GOODIX_GRIP_PARAMETERS_SIZE];
	unsigned int cornerzone_filter_ver[GOODIX_GRIP_PARAMETERS_SIZE];
	unsigned int deadzone_filter_hor[GOODIX_GRIP_PARAMETERS_SIZE];
	unsigned int deadzone_filter_ver[GOODIX_GRIP_PARAMETERS_SIZE];
	unsigned int edgezone_filter_hor[GOODIX_GRIP_PARAMETERS_SIZE];
	unsigned int edgezone_filter_ver[GOODIX_GRIP_PARAMETERS_SIZE];
	u8 deadzone_filter[4 * GRIP_PARAMETER_NUM];
	u8 edgezone_filter[4 * GRIP_PARAMETER_NUM];
	u8 cornerzone_filter[4 * GRIP_PARAMETER_NUM];
	u8 cornerzonegame_filter[4 * GRIP_PARAMETER_NUM];
	u32 check_sum;
};

#define MAX_CMD_DATA_LEN 10
#define MAX_CMD_BUF_LEN  16
#pragma pack(1)
struct goodix_ts_cmd {
	union {
		struct {
			u8 state;
			u8 ack;
			u8 len;
			u8 cmd;
			u8 data[MAX_CMD_DATA_LEN];
		};
		u8 buf[MAX_CMD_BUF_LEN];
	};
};
struct goodix_Edge_suppression {
	union {
		struct {
			u8 Length[2];
			u8 Top_DeadArea_MinX[2];
			u8 Top_DeadArea_MinY[2];
			u8 Top_DeadArea_MaxX[2];
			u8 Top_DeadArea_MaxY[2];
			u8 Bot_DeadArea_MinX[2];
			u8 Bot_DeadArea_MinY[2];
			u8 Bot_DeadArea_MaxX[2];
			u8 Bot_DeadArea_MaxY[2];
			u8 Left_DeadArea_MinX[2];
			u8 Left_DeadArea_MinY[2];
			u8 Left_DeadArea_MaxX[2];
			u8 Left_DeadArea_MaxY[2];
			u8 Right_DeadArea_MinX[2];
			u8 Right_DeadArea_MinY[2];
			u8 Right_DeadArea_MaxX[2];
			u8 Right_DeadArea_MaxY[2];
			u8 Top_ClickArea_MinX[2];
			u8 Top_ClickArea_MinY[2];
			u8 Top_ClickArea_MaxX[2];
			u8 Top_ClickArea_MaxY[2];
			u8 Bot_ClickArea_MinX[2];
			u8 Bot_ClickArea_MinY[2];
			u8 Bot_ClickArea_MaxX[2];
			u8 Bot_ClickArea_MaxY[2];
			u8 Left_ClickArea_MinX[2];
			u8 Left_ClickArea_MinY[2];
			u8 Left_ClickArea_MaxX[2];
			u8 Left_ClickArea_MaxY[2];
			u8 Right_ClickArea_MinX[2];
			u8 Right_ClickArea_MinY[2];
			u8 Right_ClickArea_MaxX[2];
			u8 Right_ClickArea_MaxY[2];
			u8 Top_CornerArea_MinX[2];
			u8 Top_CornerArea_MinY[2];
			u8 Top_CornerArea_MaxX[2];
			u8 Top_CornerArea_MaxY[2];
			u8 Bot_CornerArea_MinX[2];
			u8 Bot_CornerArea_MinY[2];
			u8 Bot_CornerArea_MaxX[2];
			u8 Bot_CornerArea_MaxY[2];
			u8 Left_CornerArea_MinX[2];
			u8 Left_CornerArea_MinY[2];
			u8 Left_CornerArea_MaxX[2];
			u8 Left_CornerArea_MaxY[2];
			u8 Right_CornerArea_MinX[2];
			u8 Right_CornerArea_MinY[2];
			u8 Right_CornerArea_MaxX[2];
			u8 Right_CornerArea_MaxY[2];
			u8 Top_GameCornerArea_MinX[2];
			u8 Top_GameCornerArea_MinY[2];
			u8 Top_GameCornerArea_MaxX[2];
			u8 Top_GameCornerArea_MaxY[2];
			u8 Bot_GameCornerArea_MinX[2];
			u8 Bot_GameCornerArea_MinY[2];
			u8 Bot_GameCornerArea_MaxX[2];
			u8 Bot_GameCornerArea_MaxY[2];
			u8 Left_GameCornerArea_MinX[2];
			u8 Left_GameCornerArea_MinY[2];
			u8 Left_GameCornerArea_MaxX[2];
			u8 Left_GameCornerArea_MaxY[2];
			u8 Right_GameCornerArea_MinX[2];
			u8 Right_GameCornerArea_MinY[2];
			u8 Right_GameCornerArea_MaxX[2];
			u8 Right_GameCornerArea_MaxY[2];
		};
		u8 buf[130];
	};
};

struct goodix_tempbuff {
	unsigned int goodix_tempbuff;
};

#pragma pack()

/* interrupt event type */
enum ts_event_type {
	EVENT_INVALID = 0,
	EVENT_TOUCH = (1 << 0), /* finger touch event */
	EVENT_PEN = (1 << 1),   /* pen event */
	EVENT_REQUEST = (1 << 2),
	EVENT_GESTURE = (1 << 3),
};

enum ts_request_type {
	REQUEST_TYPE_CONFIG = 1,
	REQUEST_TYPE_RESET = 3,
};

/* notifier event */
enum ts_notify_event {
	NOTIFY_FWUPDATE_START,
	NOTIFY_FWUPDATE_FAILED,
	NOTIFY_FWUPDATE_SUCCESS,
	NOTIFY_SUSPEND,
	NOTIFY_RESUME,
	NOTIFY_ESD_OFF,
	NOTIFY_ESD_ON,
	NOTIFY_CFG_BIN_FAILED,
	NOTIFY_CFG_BIN_SUCCESS,
};

enum touch_point_status {
	TS_NONE,
	TS_RELEASE,
	TS_TOUCH,
};
/* coordinate package */
struct goodix_ts_coords {
	int status; /* NONE, RELEASE, TOUCH */
	unsigned int x, y, w, p;
};

struct goodix_pen_coords {
	int status; /* NONE, RELEASE, TOUCH */
	int tool_type;  /* BTN_TOOL_RUBBER BTN_TOOL_PEN */
	unsigned int x, y, p;
	signed char tilt_x;
	signed char tilt_y;
};

/* touch event data */
struct goodix_touch_data {
	int touch_num;
	struct goodix_ts_coords coords[GOODIX_MAX_TOUCH];
	u8 tmp_data[32];
	unsigned int overlay;
	int fod_id;
	int t_id;
};

struct goodix_ts_key {
	int status;
	int code;
};

struct goodix_pen_data {
	struct goodix_pen_coords coords;
	struct goodix_ts_key keys[GOODIX_MAX_PEN_KEY];
};

/*
 * struct goodix_ts_event - touch event struct
 * @event_type: touch event type, touch data or
 *	request event
 * @event_data: event data
 */
struct goodix_ts_event {
	int retry;
	enum ts_event_type event_type;
	u8 request_code; /* represent the request type */
	u8 gesture_type;
	struct goodix_touch_data touch_data;
	struct goodix_pen_data pen_data;
};

enum goodix_ic_bus_type {
	GOODIX_BUS_TYPE_I2C,
	GOODIX_BUS_TYPE_SPI,
	GOODIX_BUS_TYPE_I3C,
};

struct goodix_bus_interface {
	int bus_type;
	int ic_type;
	struct device *dev;
	int (*read)(struct device *dev, unsigned int addr,
			 unsigned char *data, unsigned int len);
	int (*write)(struct device *dev, unsigned int addr,
			unsigned char *data, unsigned int len);
};

struct goodix_ts_hw_ops {
	int (*power_on)(struct goodix_ts_core *cd, bool on);
	int (*dev_confirm)(struct goodix_ts_core *cd);
	int (*resume)(struct goodix_ts_core *cd);
	int (*suspend)(struct goodix_ts_core *cd);
	int (*gesture)(struct goodix_ts_core *cd, int gesture_type);
	int (*reset)(struct goodix_ts_core *cd, int delay_ms);
	int (*irq_enable)(struct goodix_ts_core *cd, bool enable);
	int (*read)(struct goodix_ts_core *cd, unsigned int addr,
		    unsigned char *data, unsigned int len);
	int (*write)(struct goodix_ts_core *cd, unsigned int addr,
		     unsigned char *data, unsigned int len);
	int (*send_cmd)(struct goodix_ts_core *cd, struct goodix_ts_cmd *cmd);
	int (*send_config)(struct goodix_ts_core *cd, u8 *config, int len);
	int (*read_config)(struct goodix_ts_core *cd, u8 *config_data, int size);
	int (*read_version)(struct goodix_ts_core *cd, struct goodix_fw_version *version);
	int (*get_ic_info)(struct goodix_ts_core *cd, struct goodix_ic_info *ic_info);
	int (*esd_check)(struct goodix_ts_core *cd);
	int (*event_handler)(struct goodix_ts_core *cd, struct goodix_ts_event *ts_event);
	int (*after_event_handler)(struct goodix_ts_core *cd); /* clean sync flag */
	int (*get_capacitance_data)(struct goodix_ts_core *cd, struct ts_rawdata_info *info);
	int (*charger_on)(struct goodix_ts_core *cd, bool on);
	int (*palm_on)(struct goodix_ts_core *cd, bool on);
	int (*game)(struct goodix_ts_core *cd, u8 data0, u8 data1);
	int (*get_frame_data)(struct goodix_ts_core *cd, struct ts_framedata *info);
	int (*switch_report_rate)(struct goodix_ts_core *cd, bool on);
	int (*read_flash)(struct goodix_ts_core *cd, unsigned int addr,
			unsigned char *data, unsigned int len);
	int (*write_flash)(struct goodix_ts_core *cd, unsigned int addr,
			unsigned char *data, unsigned int len);
};

/*
 * struct goodix_ts_esd - esd protector structure
 * @esd_work: esd delayed work
 * @esd_on: 1 - turn on esd protection, 0 - turn
 *  off esd protection
 */
struct goodix_ts_esd {
	bool irq_status;
	atomic_t esd_on;
	struct delayed_work esd_work;
	struct notifier_block esd_notifier;
	struct goodix_ts_core *ts_core;
};

enum goodix_core_init_stage {
	CORE_UNINIT,
	CORE_INIT_FAIL,
	CORE_INIT_STAGE1,
	CORE_INIT_STAGE2
};

struct goodix_ic_config {
	int len;
	u8 data[GOODIX_CFG_MAX_SIZE];
};
enum ts_work_stat {
	TP_NORMAL,
	TP_GESTURE,
	TP_SLEEP,
};
struct goodix_ts_core {
	int init_stage;
	struct platform_device *pdev;
	struct goodix_fw_version fw_version;
	struct goodix_ic_info ic_info;
	struct goodix_bus_interface *bus;
	struct goodix_ts_board_data board_data;
	struct goodix_xiaomi_board_data goodix_xiaomi_board_data;
	struct goodix_ts_hw_ops *hw_ops;
	struct input_dev *input_dev;
	struct input_dev *pen_dev;
	struct class *goodix_tp_class;
	struct device *goodix_touch_dev;
	/* TODO counld we remove this from core data? */
	struct goodix_ts_event ts_event;
	unsigned long touch_id;
	u8 eventsdata;
	u8 mes[32];
	u8 ssg[32];

	/* every pointer of this array represent a kind of config */
	struct goodix_ic_config *ic_configs[GOODIX_MAX_CONFIG_GROUP];
	struct regulator *avdd;
	struct regulator *iovdd;
	struct pinctrl *pinctrl;
	struct pinctrl_state *pin_sta_active;
	struct pinctrl_state *pin_sta_suspend;
	struct pinctrl_state *pin_sta_boot;
	struct pinctrl_state *pinctrl_state_spimode;
	struct pinctrl_state *pinctrl_dvdd_enable;
	struct pinctrl_state *pinctrl_dvdd_disable;
	int power_on;
	int irq;
	size_t irq_trig_cnt;

	atomic_t irq_enabled;
	atomic_t suspended;
	/* when this flag is true, driver should not clean the sync flag */
	bool tools_ctrl_sync;
	bool fod_finger;
	bool fod_display_enabled;
	bool in_sleep;
	bool in_suspend;
	struct goodix_Edge_suppression edge_data;

	struct notifier_block ts_notifier;
	struct goodix_ts_esd ts_esd;

#ifdef CONFIG_FB
	struct notifier_block fb_notifier;
#endif
	struct notifier_block charger_notifier;
	struct workqueue_struct *event_wq;
	struct workqueue_struct *gesture_wq;
	struct workqueue_struct *game_wq;
	struct work_struct suspend_work;
	struct work_struct resume_work;
	struct work_struct charger_work;
	struct work_struct gesture_work;
	struct work_struct game_work;
	struct work_struct grip_mode_work;
	struct work_struct power_supply_work;
	struct work_struct self_check_work;
	u8 lockdown_info[GOODIX_LOCKDOWN_SIZE];
	struct proc_dir_entry *tp_lockdown_info_proc;
	struct proc_dir_entry *tp_fw_version_proc;
	struct proc_dir_entry *tp_selftest_proc;
#ifdef GOODIX_DEBUGFS_ENABLE
	struct dentry *debugfs;
#endif
	struct mutex report_mutex;
	struct mutex core_mutex;
	struct mutex edge_data_mutex;
	struct mutex sleep_to_gesture_mutex;
	int work_status;
	int gesture_enabled;
	int gamemode_enabled;
	int double_wakeup;
	int aod_status;
	int fod_status;
	int fod_icon_status;
	int nonui_status;
	int charger_status;
	int gtp_mode;
	int gtp_value;
	int gtp_direction_value;
	int palm_status;
	int result_type;
	int power_status;
	int report_rate;
	bool tp_pm_suspend;
	struct completion pm_resume_completion;
	struct notifier_block notifier;
};

/* external module structures */
enum goodix_ext_priority {
	EXTMOD_PRIO_RESERVED = 0,
	EXTMOD_PRIO_FWUPDATE,
	EXTMOD_PRIO_GESTURE,
	EXTMOD_PRIO_HOTKNOT,
	EXTMOD_PRIO_DBGTOOL,
	EXTMOD_PRIO_DEFAULT,
};

#define EVT_HANDLED				0
#define EVT_CONTINUE			0
#define EVT_CANCEL				1
#define EVT_CANCEL_IRQEVT		1
#define EVT_CANCEL_SUSPEND		1
#define EVT_CANCEL_RESUME		1
#define EVT_CANCEL_RESET		1

struct goodix_ext_module;
/* external module's operations callback */
struct goodix_ext_module_funcs {
	int (*init)(struct goodix_ts_core *core_data,
		    struct goodix_ext_module *module);
	int (*exit)(struct goodix_ts_core *core_data,
		    struct goodix_ext_module *module);
	int (*before_reset)(struct goodix_ts_core *core_data,
			    struct goodix_ext_module *module);
	int (*after_reset)(struct goodix_ts_core *core_data,
			   struct goodix_ext_module *module);
	int (*before_suspend)(struct goodix_ts_core *core_data,
			      struct goodix_ext_module *module);
	int (*after_suspend)(struct goodix_ts_core *core_data,
			     struct goodix_ext_module *module);
	int (*before_resume)(struct goodix_ts_core *core_data,
			     struct goodix_ext_module *module);
	int (*after_resume)(struct goodix_ts_core *core_data,
			    struct goodix_ext_module *module);
	int (*irq_event)(struct goodix_ts_core *core_data,
			 struct goodix_ext_module *module);
};

/*
 * struct goodix_ext_module - external module struct
 * @list: list used to link into modules manager
 * @name: name of external module
 * @priority: module priority vlaue, zero is invalid
 * @funcs: operations callback
 * @priv_data: private data region
 * @kobj: kobject
 * @work: used to queue one work to do registration
 */
struct goodix_ext_module {
	struct list_head list;
	char *name;
	enum goodix_ext_priority priority;
	const struct goodix_ext_module_funcs *funcs;
	void *priv_data;
	struct kobject kobj;
	struct work_struct work;
};

/*
 * struct goodix_ext_attribute - exteranl attribute struct
 * @attr: attribute
 * @show: show interface of external attribute
 * @store: store interface of external attribute
 */
struct goodix_ext_attribute {
	struct attribute attr;
	ssize_t (*show)(struct goodix_ext_module *module, char *var);
	ssize_t (*store)(struct goodix_ext_module *module, const char *var, size_t size);
};

/* external attrs helper macro */
#define __EXTMOD_ATTR(_name, _mode, _show, _store)	{	\
	.attr = {.name = __stringify(_name), .mode = _mode },	\
	.show   = _show,	\
	.store  = _store,	\
}

/* external attrs helper macro, used to define external attrs */
#define DEFINE_EXTMOD_ATTR(_name, _mode, _show, _store)	\
static struct goodix_ext_attribute ext_attr_##_name = \
	__EXTMOD_ATTR(_name, _mode, _show, _store)

/* log macro */
extern bool debug_log_flag;
#define ts_info(fmt, arg...)	pr_info("[GTP-INF][%s:%d] "fmt"\n", __func__, __LINE__, ##arg)
#define	ts_err(fmt, arg...)		pr_err("[GTP-ERR][%s:%d] "fmt"\n", __func__, __LINE__, ##arg)
#define ts_debug(fmt, arg...)	{if (debug_log_flag) pr_info("[GTP-DBG][%s:%d] "fmt"\n", __func__, __LINE__, ##arg); }

/*
 * get board data pointer
 */
static inline struct goodix_ts_board_data *board_data(
		struct goodix_ts_core *core)
{
	if (!core)
		return NULL;
	return &(core->board_data);
}

/**
 * goodix_register_ext_module - interface for external module
 * to register into touch core modules structure
 *
 * @module: pointer to external module to be register
 * return: 0 ok, <0 failed
 */
int goodix_register_ext_module(struct goodix_ext_module *module);
/* register module no wait */
int goodix_register_ext_module_no_wait(struct goodix_ext_module *module);
/**
 * goodix_unregister_ext_module - interface for external module
 * to unregister external modules
 *
 * @module: pointer to external module
 * return: 0 ok, <0 failed
 */
int goodix_unregister_ext_module(struct goodix_ext_module *module);
/* remove all registered ext module
 * return 0 on success, otherwise return < 0
 */
int goodix_ts_blocking_notify(enum ts_notify_event evt, void *v);
struct kobj_type *goodix_get_default_ktype(void);
struct kobject *goodix_get_default_kobj(void);

struct goodix_ts_hw_ops *goodix_get_hw_ops(void);
int goodix_get_config_proc(struct goodix_ts_core *cd);

int goodix_spi_bus_init(void);
void goodix_spi_bus_exit(void);
int goodix_i2c_bus_init(void);
void goodix_i2c_bus_exit(void);

u32 goodix_append_checksum(u8 *data, int len, int mode);
int checksum_cmp(const u8 *data, int size, int mode);
int is_risk_data(const u8 *data, int size);
u32 goodix_get_file_config_id(u8 *ic_config);
void goodix_rotate_abcd2cbad(int tx, int rx, s16 *data);
int goodix_gesture_enable(int enable);

int goodix_fw_update_init(struct goodix_ts_core *core_data);
void goodix_fw_update_uninit(void);
int goodix_do_fw_update(struct goodix_ic_config *ic_config, int mode);

int goodix_get_ic_type(struct device_node *node);
int gesture_module_init(void);
void gesture_module_exit(void);
int inspect_module_init(struct goodix_ts_core *core_data);
void inspect_module_exit(void);
int goodix_tools_init(void);
void goodix_tools_exit(void);
int goodix_get_rawdata(struct device *dev, struct ts_rawdata_info *info);
int goodix_ts_get_lockdown_info(struct goodix_ts_core *cd);

/* extern int mi_disp_set_fod_queue_work(u32 fod_btn, bool from_touch); */
void goodix_set_grip_filter(int *source, int *sum);
void goodix_set_edge_filter_normal(void);
void goodix_set_edge_filter_game(int edge_filter_corner_size_index);
int brl_game(struct goodix_ts_core *cd, u8 data0, u8 data1);
extern int brl_Edge_suppression(struct goodix_ts_core *cd);

#endif
