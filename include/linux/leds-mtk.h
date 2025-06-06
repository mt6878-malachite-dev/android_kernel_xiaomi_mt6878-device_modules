/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2021 MediaTek Inc.
 *
 */

#include <linux/leds.h>

#define MAX_BL_PT_NUM	6

enum mtk_leds_events {
	LED_BRIGHTNESS_CHANGED	= 0,
	LED_STATUS_SHUTDOWN	= 1,
	LED_TYPE_CHANGED	= 2,
};
enum mtk_leds_types {
	LED_TYPE_FILE = 0,
	LED_TYPE_ATOMIC = 1,
	LED_TYPE_FACTORY = 2,
	LED_TYPE_MAX = 3,
};

enum led_mode {
	MT_LED_MODE_CUST_LCM = 4,
	MT_LED_MODE_CUST_BLS_PWM,
	MT_LED_MODE_CUST_BLS_I2C
};

struct led_conf_info {
	int max_hw_brightness;
	int min_hw_brightness;
	int limit_hw_brightness;
	int min_brightness;
	unsigned int aal_enable;
	struct led_classdev cdev;
	int flags;
	enum led_mode mode;
	int connector_id;
	int logic_max_brightness;
#define LED_MT_BRIGHTNESS_HW_CHANGED	BIT(1)
#define LED_MT_BRIGHTNESS_CHANGED	BIT(2)

#ifdef CONFIG_LEDS_MT_BRIGHTNESS_HW_CHANGED
	int brightness_hw_changed;
	struct kernfs_node	*brightness_hw_changed_kn;
#endif
	enum mtk_leds_types led_type;
#if IS_ENABLED(CONFIG_MTK_BATTERY_PERCENT_THROTTLING)
	int reg_battery_percent_pt;
	int bl_bat_pt_per[MAX_BL_PT_NUM];
#endif
};

#ifdef CONFIG_LEDS_MT_BRIGHTNESS_HW_CHANGED
void mt_leds_notify_brightness_hw_changed(
	int connector_id, enum led_brightness brightness);
#else
static inline void mt_leds_notify_brightness_hw_changed(
	int connector_id, enum led_brightness brightness) { }
#endif

int mtk_leds_register_notifier(struct notifier_block *nb);
int mtk_leds_unregister_notifier(struct notifier_block *nb);
int mtk_leds_brightness_set(int connector_id, int level, unsigned int params, unsigned int params_flag);
int setMaxBrightness(int connector_id, int percent, bool enable);
