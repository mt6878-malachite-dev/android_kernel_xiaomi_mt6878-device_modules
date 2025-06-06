#
# Copyright (C) 2021 MediaTek Inc.
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

$(info $(DEVICE_MODULES_PATH))
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/smi/
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/mtk-interconnect/
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/iommu
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/iommu
ccflags-$(CONFIG_MTK_IOMMU_V2) += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include/

ifneq ($(wildcard $(DEVICE_MODULES_PATH)/include/dt-bindings/memory/mt6983-larb-port.h),)
subdir-ccflags-y += -DPDA_TF_DUMP_71_1
else ifneq ($(wildcard $(DEVICE_MODULES_PATH)/include/dt-bindings/memory/mt6879-larb-port.h),)
subdir-ccflags-y += -DPDA_TF_DUMP_71_2
else ifneq ($(wildcard $(DEVICE_MODULES_PATH)/include/dt-bindings/memory/mt6895-larb-port.h),)
subdir-ccflags-y += -DPDA_TF_DUMP_71_3
else ifneq ($(wildcard $(DEVICE_MODULES_PATH)/include/dt-bindings/memory/mt6855-larb-port.h),)
subdir-ccflags-y += -DPDA_TF_DUMP_6s_1
endif

# driver module
obj-y := camera_pda_dummy.o
ifneq ($(wildcard $(srctree)/$(src)/isp_71/camera_pda.c),)
obj-$(CONFIG_MTK_CAMERA_ISP_PDA_SUPPORT) += isp_71/camera_pda.o
endif
ifneq ($(wildcard $(srctree)/$(src)/isp_71/camera_pda_mt6879.c),)
obj-$(CONFIG_MTK_CAMERA_ISP_PDA_SUPPORT) += pda_drv_mt6879.o
endif
ifneq ($(wildcard $(srctree)/$(src)/isp_71/camera_pda_mt6895.c),)
obj-$(CONFIG_MTK_CAMERA_ISP_PDA_SUPPORT) += pda_drv_mt6895.o
endif
ifneq ($(wildcard $(srctree)/$(src)/isp_6s/pda_mt6855.c),)
obj-$(CONFIG_MTK_CAMERA_ISP_PDA_SUPPORT) += pda_drv_mt6855.o
endif
ifneq ($(wildcard $(srctree)/$(src)/isp_7s/pda_mt6985.c),)
obj-$(CONFIG_MTK_CAMERA_ISP_PDA_SUPPORT) += pda_drv_mt6985.o
endif
ifneq ($(wildcard $(srctree)/$(src)/isp_7s/pda_mt6886.c),)
obj-$(CONFIG_MTK_CAMERA_ISP_PDA_SUPPORT) += pda_drv_mt6886.o
endif
ifneq ($(wildcard $(srctree)/$(src)/isp_7sp/pda_mt6897.c),)
obj-$(CONFIG_MTK_CAMERA_ISP_PDA_SUPPORT) += pda_drv_mt6897.o
endif
ifneq ($(wildcard $(srctree)/$(src)/isp_7sp/pda_mt6989.c),)
obj-$(CONFIG_MTK_CAMERA_ISP_PDA_SUPPORT) += pda_drv_mt6989.o
endif
ifneq ($(wildcard $(srctree)/$(src)/isp_7sp/pda_mt6878.c),)
obj-$(CONFIG_MTK_CAMERA_ISP_PDA_SUPPORT) += pda_drv_mt6878.o
endif

pda_drv_mt6879-objs := isp_71/camera_pda_mt6879.o
pda_drv_mt6895-objs := isp_71/camera_pda_mt6895.o
pda_drv_mt6855-objs := isp_6s/camera_pda.o isp_6s/pda_mt6855.o
pda_drv_mt6985-objs := isp_7s/camera_pda.o isp_7s/pda_mt6985.o
pda_drv_mt6886-objs := isp_7s/camera_pda.o isp_7s/pda_mt6886.o
pda_drv_mt6897-objs := isp_7sp/camera_pda.o isp_7sp/pda_mt6897.o
pda_drv_mt6989-objs := isp_7sp/camera_pda.o isp_7sp/pda_mt6989.o
pda_drv_mt6878-objs := isp_7sp/camera_pda.o isp_7sp/pda_mt6878.o
