# SPDX-License-Identifier: GPL-2.0
#
# Copyright (c) 2021 MediaTek Inc.
#

ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/iommu \
             -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/iommu/ \
             -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/include \
             -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/mach/$(MTK_PLATFORM)/include \
             -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/mdp/$(MTK_PLATFORM) \
             -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/mmp/ \
             -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/smi/ \
             -I$(DEVICE_MODULES_PATH)/drivers/iommu/arm/arm-smmu-v3 \
             -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/vcp/rv/ \
             -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/vcp/include/

ccflags-$(CONFIG_DEBUG_FS) += -DCONFIG_CMDQ_MMPROFILE_SUPPORT

ifeq ($(CONFIG_MICROTRUST_TEE_SUPPORT),m)
include $(DEVICE_MODULES_PATH)/drivers/tee/teei/Makefile.include
endif

ifeq ($(CONFIG_TRUSTONIC_TEE_SUPPORT),m)
include $(DEVICE_MODULES_PATH)/drivers/tee/gud/Makefile.include
endif

ifeq ($(CONFIG_MTK_GZ_TZ_SYSTEM),m)
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/geniezone/public
ccflags-y += -DCMDQ_SECURE_MTEE_SUPPORT

ccflags-y += -DCMDQ_SECURE_SUPPORT
obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += cmdq-sec-drv.o
cmdq-sec-drv-y := cmdq_sec_mtee.o cmdq-sec-mailbox.o cmdq-sec-helper.o

ccflags-y += -DCMDQ_SECURE_CPR_SUPPORT
endif

ccflags-y += -DCMDQ_DCACHE_INVAL

obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += mtk-cmdq-drv-ext.o
mtk-cmdq-drv-ext-y := mtk-cmdq-mailbox-ext.o mtk-cmdq-helper-ext.o cmdq-util.o

obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += cmdq-test.o

ifneq ($(wildcard $(srctree)/$(src)/cmdq-platform-mt6873.c),)
obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += cmdq-platform-mt6873.o
endif
ifneq ($(wildcard $(srctree)/$(src)/cmdq-platform-mt6853.c),)
obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += cmdq-platform-mt6853.o
endif
ifneq ($(wildcard $(srctree)/$(src)/cmdq-platform-mt6885.c),)
obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += cmdq-platform-mt6885.o
endif
ifneq ($(wildcard $(srctree)/$(src)/cmdq-platform-mt6893.c),)
obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += cmdq-platform-mt6893.o
endif
ifneq ($(wildcard $(srctree)/$(src)/cmdq-platform-mt6983.c),)
obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += cmdq-platform-mt6983.o
endif
ifneq ($(wildcard $(srctree)/$(src)/cmdq-platform-mt6879.c),)
obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += cmdq-platform-mt6879.o
endif
ifneq ($(wildcard $(srctree)/$(src)/cmdq-platform-mt6895.c),)
obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += cmdq-platform-mt6895.o
endif
ifneq ($(wildcard $(srctree)/$(src)/cmdq-platform-mt6855.c),)
obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += cmdq-platform-mt6855.o
endif
ifneq ($(wildcard $(srctree)/$(src)/cmdq-platform-mt6985.c),)
obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += cmdq-platform-mt6985.o
endif
ifneq ($(wildcard $(srctree)/$(src)/cmdq-platform-mt6886.c),)
obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += cmdq-platform-mt6886.o
endif
ifneq ($(wildcard $(srctree)/$(src)/cmdq-platform-mt6897.c),)
obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += cmdq-platform-mt6897.o
endif
ifneq ($(wildcard $(srctree)/$(src)/cmdq-platform-mt6989.c),)
obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += cmdq-platform-mt6989.o
endif
ifneq ($(wildcard $(srctree)/$(src)/cmdq-platform-mt6878.c),)
obj-$(CONFIG_MTK_CMDQ_MBOX_EXT) += cmdq-platform-mt6878.o
endif
