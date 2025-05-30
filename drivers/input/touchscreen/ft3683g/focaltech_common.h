/*
 *
 * FocalTech fts TouchScreen driver.
 *
 * Copyright (c) 2012-2020, Focaltech Ltd. All rights reserved.
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
/*****************************************************************************
*
* File Name: focaltech_common.h
*
* Author: Focaltech Driver Team
*
* Created: 2016-08-16
*
* Abstract:
*
* Reference:
*
*****************************************************************************/

#ifndef __LINUX_FOCALTECH_COMMON_H__
#define __LINUX_FOCALTECH_COMMON_H__

#include "focaltech_config.h"

/*****************************************************************************
* Macro definitions using #define
*****************************************************************************/
#define FTS_DRIVER_VERSION                  "Focaltech V3.4 20211214"

#define BYTE_OFF_0(x)           (u8)((x) & 0xFF)
#define BYTE_OFF_8(x)           (u8)(((x) >> 8) & 0xFF)
#define BYTE_OFF_16(x)          (u8)(((x) >> 16) & 0xFF)
#define BYTE_OFF_24(x)          (u8)(((x) >> 24) & 0xFF)
#define FLAGBIT(x)              (0x00000001 << (x))
#define FLAGBITS(x, y)          ((0xFFFFFFFF >> (32 - (y) - 1)) & (0xFFFFFFFF << (x)))

#define FLAG_ICSERIALS_LEN      8
#define FLAG_HID_BIT            10
#define FLAG_IDC_BIT            11

#define IC_SERIALS              (FTS_CHIP_TYPE & FLAGBITS(0, FLAG_ICSERIALS_LEN-1))
#define IC_TO_SERIALS(x)        ((x) & FLAGBITS(0, FLAG_ICSERIALS_LEN-1))
#define FTS_CHIP_IDC            ((FTS_CHIP_TYPE & FLAGBIT(FLAG_IDC_BIT)) == FLAGBIT(FLAG_IDC_BIT))
#define FTS_HID_SUPPORTTED      ((FTS_CHIP_TYPE & FLAGBIT(FLAG_HID_BIT)) == FLAGBIT(FLAG_HID_BIT))

#define FTS_MAX_CHIP_IDS        8

#define FTS_CHIP_TYPE_MAPPING {\
{0x90, 0x56, 0x72, 0x00, 0x00, 0x00, 0x00, 0x36, 0xB3},\
}

#define FTS_CHIP_ID_MAPPING   { \
{0x10, {0x821A, 0x8006} }, \
{0x1C, {0x8726, 0x872A, 0x872B} }, \
{0x21, {0x820A, 0x820B} }, \
}

#define FILE_NAME_LENGTH                    128
#define ENABLE                              1
#define DISABLE                             0
#define VALID                               1
#define INVALID                             0
#define FTS_CMD_START1                      0x55
#define FTS_CMD_START2                      0xAA
#define FTS_CMD_START_DELAY                 12
#define FTS_CMD_READ_ID                     0x90
#define FTS_CMD_READ_ID_LEN                 4
#define FTS_CMD_READ_ID_LEN_INCELL          1
#define FTS_CMD_READ_INFO                   0x5E

/*register address*/
#define FTS_REG_INT_CNT                     0x8F
#define FTS_REG_FLOW_WORK_CNT               0x91
#define FTS_REG_WORKMODE                    0x00
#define FTS_REG_WORKMODE_FACTORY_VALUE      0x40
#define FTS_REG_WORKMODE_WORK_VALUE         0x00
#define FTS_REG_ESDCHECK_DISABLE            0x8D
#define FTS_REG_CHIP_ID                     0xA3
#define FTS_REG_CHIP_ID2                    0x9F
#define FTS_REG_POWER_MODE                  0xA5
#define FTS_REG_POWER_MODE_SLEEP            0x03
#define FTS_REG_FW_VER                      0xA6
#define FTS_REG_VENDOR_ID                   0xA8
#define FTS_REG_LCD_BUSY_NUM                0xAB
#define FTS_REG_FACE_DEC_MODE_EN            0xB0
#define FTS_REG_FACTORY_MODE_DETACH_FLAG    0xB4
#define FTS_REG_FACE_DEC_MODE_STATUS        0x01
#define FTS_REG_IDE_PARA_VER_ID             0xB5
#define FTS_REG_IDE_PARA_STATUS             0xB6
#define FTS_REG_GLOVE_MODE_EN               0xC0
#define FTS_REG_COVER_MODE_EN               0xC1
#define FTS_REG_CHARGER_MODE_EN             0x8B
#define FTS_REG_GESTURE_EN                  0xD0
#define FTS_REG_GESTURE_OUTPUT_ADDRESS      0xD3
#define FTS_REG_MODULE_ID                   0xE3
#define FTS_REG_LIC_VER                     0xE4
#define FTS_REG_ESD_SATURATE                0xED

#define FTS_PALM_EN                         0x9a
#define FTS_PALM_DATA                       0x9b
#define FTS_PALM_ON                         0x05
#define FTS_PALM_OFF                        0x00
#define FTS_REG_FOD_OUTPUT_ADDRESS          0xE1
#define BTN_INFO                            0x152
#define FTS_REG_GESTURE_DOUBLETAP_ON        0x01
#define FTS_REG_GESTURE_FOD_ON              0x02
#define FTS_REG_GESTURE_SUPPORT             0xCF
#define FTS_REG_SENSIVITY                   0x80
#define FTS_REG_THDIFF                      0x85
#define FTS_REG_EDGE_FILTER_EN              0x8c
#define FTS_REG_EDGE_FILTER_LEVEL           0x8d
#define FTS_REG_GAMEMODE                    0xC1
#define FTS_REG_ORIENTATION                 0x8C
#define FTS_PANEL_CHANGE_FPS                0x8A
#define FTS_GESTURE_CTRL                    0xD1
#define FTS_GESTURE_DOUBLETAP               0x04
#define FTS_GESTURE_AOD                     0x07

#define FTS_SYSFS_ECHO_ON(buf)      (buf[0] == '1')
#define FTS_SYSFS_ECHO_OFF(buf)     (buf[0] == '0')

#define kfree_safe(pbuf) do  {\
	kfree(pbuf);\
	pbuf = NULL;\
} while (0)

/*****************************************************************************
*  Alternative mode (When something goes wrong, the modules may be able to solve the problem.)
*****************************************************************************/
/*
 * point report check
 * default: disable
 */
#define FTS_POINT_REPORT_CHECK_EN               0

/*****************************************************************************
* Global variable or extern global variabls/functions
*****************************************************************************/
struct ft_chip_t {
    u16 type;
    u8 chip_idh;
    u8 chip_idl;
    u8 rom_idh;
    u8 rom_idl;
    u8 pb_idh;
    u8 pb_idl;
    u8 bl_idh;
    u8 bl_idl;
};

struct ft_chip_id_t {
    u16 type;
    u16 chip_ids[FTS_MAX_CHIP_IDS];
};

struct ts_ic_info {
    bool is_incell;
    bool hid_supported;
    struct ft_chip_t ids;
    struct ft_chip_id_t cid;
};

/*****************************************************************************
* DEBUG function define here
*****************************************************************************/
#if FTS_DEBUG_EN
#define FTS_DEBUG(fmt, args...) pr_info("[FTS_TS]%s:" fmt "\n", __func__, ##args)
#define FTS_FUNC_ENTER() pr_info("[FTS_TS]%s: Enter\n", __func__)
#define FTS_FUNC_EXIT() pr_info("[FTS_TS]%s: Exit(%d)\n", __func__, __LINE__)
#else /* #if FTS_DEBUG_EN*/
#define FTS_DEBUG(fmt, args...)
#define FTS_FUNC_ENTER()
#define FTS_FUNC_EXIT()
#endif

#define FTS_INFO(fmt, args...) pr_info("[FTS_TS/I]%s:" fmt "\n", __func__, ##args)
#define FTS_ERROR(fmt, args...) pr_err("[FTS_TS/E]%s:" fmt "\n", __func__, ##args)
#endif /* __LINUX_FOCALTECH_COMMON_H__ */
