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

#
# Makefile for misc devices that really don't fit anywhere else.
#

#ifdef CONFIG_MTK_M4U
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/m4u/$(MTK_PLATFORM)/
#else
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/iommu/
#endif
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/smi/
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/mmp/
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/cmdq/mailbox/
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/staging/android/ion \
						 -I$(DEVICE_MODULES_PATH)/drivers/staging/android/ion/mtk

ifeq ($(strip $(CONFIG_MTK_CAM_GENIEZONE_SUPPORT)),y)
ifneq (,$(filter $(CONFIG_MTK_CAMERA_ISP_PLATFORM), "mt6885" "mt6873" "mt6853"))
ccflags-y += -DCMDQ_MTEE
endif
endif

