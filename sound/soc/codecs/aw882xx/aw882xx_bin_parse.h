/* SPDX-License-Identifier: GPL-2.0
 * aw882xx_bin_parse.h
 *
 * Copyright (c) 2020 AWINIC Technology CO., LTD
 *
 * Author: Nick Li <liweilei@awinic.com.cn>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 */

#ifndef __AW882XX_BIN_PARSE_H__
#define __AW882XX_BIN_PARSE_H__

#include "aw882xx_device.h"

#define NULL    ((void *)0)
#define GET_32_DATA(w, x, y, z) ((unsigned int)((((uint32_t)w) << 24) | (((uint32_t)x) << 16) | (((uint32_t)y) << 8) | ((uint32_t)z)))
#define BIN_NUM_MAX   100
#define HEADER_LEN    60
/*********************************************************
 *
 * header information
 *
 ********************************************************/
enum bin_header_version_enum {
	HEADER_VERSION_1_0_0 = 0x01000000,
};

enum data_type_enum {
	DATA_TYPE_REGISTER = 0x00000000,
	DATA_TYPE_DSP_REG = 0x00000010,
	DATA_TYPE_DSP_CFG = 0x00000011,
	DATA_TYPE_SOC_REG = 0x00000020,
	DATA_TYPE_SOC_APP = 0x00000021,
	DATA_TYPE_MULTI_BINS = 0x00002000,
};

enum data_version_enum {
	DATA_VERSION_V1 = 0X00000001,	/*default little edian */
	DATA_VERSION_MAX,
};

struct bin_header_info {
	unsigned int header_len; /* Frame header length */
	unsigned int check_sum; /* Frame header information-Checksum */
	unsigned int header_ver; /* Frame header information-Frame header version */
	unsigned int bin_data_type; /* Frame header information-Data type */
	unsigned int bin_data_ver; /* Frame header information-Data version */
	unsigned int bin_data_len; /* Frame header information-Data length */
	unsigned int ui_ver; /* Frame header information-ui version */
	unsigned char chip_type[8]; /* Frame header information-chip type */
	unsigned int reg_byte_len; /* Frame header information-reg byte len */
	unsigned int data_byte_len; /* Frame header information-data byte len */
	unsigned int device_addr; /* Frame header information-device addr */
	unsigned int valid_data_len; /* Length of valid data obtained after parsing */
	unsigned int valid_data_addr; /* The offset address of the valid data obtained after parsing relative to info */

	unsigned int reg_num; /* The number of registers obtained after parsing */
	unsigned int reg_data_byte_len; /* The byte length of the register obtained after parsing */
	unsigned int download_addr; /* The starting address or download address obtained after parsing */
	unsigned int app_version; /* The software version number obtained after parsing */
};

/************************************************************
 *
 * function define
 *
 ************************************************************/
struct bin_container {
	unsigned int len; /* The size of the bin file obtained from the firmware */
	unsigned char data[]; /* Store the bin file obtained from the firmware */
};

struct aw_bin {
	unsigned char *p_addr; /* Offset pointer (backward offset pointer to obtain frame header information and important information) */
	unsigned int all_bin_parse_num; /* The number of all bin files */
	unsigned int multi_bin_parse_num; /* The number of single bin files */
	unsigned int single_bin_parse_num; /* The number of multiple bin files */
	struct bin_header_info header_info[BIN_NUM_MAX]; /* Frame header information and other important data obtained after parsing */
	struct bin_container info; /* Obtained bin file data that needs to be parsed */
};


/*******************awinic audio parse acf***********************/

int aw882xx_dev_parse_check_acf(struct aw_container *aw_cfg);
int aw882xx_dev_parse_acf(struct aw_device *aw_dev, struct aw_container *aw_cfg);


int aw882xx_dev_get_profile_count(struct aw_device *aw_dev);
int aw88xx_dev_get_profile_name(struct aw_device *aw_dev, char *name, int index);
int aw882xx_dev_check_profile_index(struct aw_device *aw_dev, int index);
int aw882xx_dev_get_profile_index(struct aw_device *aw_dev);
int aw882xx_dev_set_profile_index(struct aw_device *aw_dev, int index);
char *aw882xx_dev_get_prof_name(struct aw_device *aw_dev, int index);
struct aw_sec_data_desc *aw882xx_dev_get_prof_data(struct aw_device *aw_dev, int index, int data_type);


#endif
