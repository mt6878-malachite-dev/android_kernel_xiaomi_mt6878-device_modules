#
# Copyright (C) 2015 MediaTek Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#

#FPGA project no need build-in
ifndef CONFIG_MTK_FPGA

# ccci_util is build when ECCCI driver or CCCI driver enabled


ifeq ($(CONFIG_MTK_ECCCI_DRIVER), y)
CCCI_OBJ=y
else ifeq ($(CONFIG_MTK_ECCCI_DRIVER), m)
CCCI_OBJ=m
else
CCCI_OBJ=n
endif

ifneq ($(CCCI_OBJ), n)

ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/ccci_util
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/include/

ifeq ($(CONFIG_MTK_SECURITY_SW_SUPPORT), y)
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/masp/asfv2/asf_export_inc
ccflags-y += -DENABLE_MD_IMG_SECURITY_FEATURE
endif

ifeq ($(CONFIG_MTK_SEC_MODEM_NVRAM_ANTI_CLONE), y)
ccflags-y += -DMTK_SEC_MODEM_NVRAM_ANTI_CLONE
endif

obj-$(CCCI_OBJ) += ccci_util_lib.o
ccci_util_lib-$(CCCI_OBJ) := ccci_util_lib_fo.o \
			ccci_util_lib_load_img.o \
			ccci_util_lib_sys.o \
			ccci_private_log.o \
			ccci_util_lib_main.o \
			ccci_util_ld_md_errno.o \
			ccci_util_broadcast.o \
			ccci_util_pin_broadcast.o \
			ccci_util_sib.o \
			ccci_util_lib_reserved_mem.o \
			ccci_util_md_rat.o \
			ccci_util_boot_args.o \
			ccci_util_img_parsing.o \
			ccci_util_md_mem.o

endif
endif

