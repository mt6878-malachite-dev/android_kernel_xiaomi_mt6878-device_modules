# SPDX-License-Identifier: GPL-2.0

# MTK Platform Support

# include path
subdir-ccflags-y += -I$(DEVICE_MODULES_PATH)/sound/soc/mediatek/common/
subdir-ccflags-y += -I$(DEVICE_MODULES_PATH)/sound/soc/mediatek/ultrasound/ultra_common

ifneq ($(CONFIG_MTK_TINYSYS_SCP_SUPPORT),)
subdir-ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/scp/include
endif

# common code
snd-soc-mtk-scp-ultra-objs := \
	mtk-scp-ultra-pcm.o \
	mtk-scp-ultra-mem-control.o \
	mtk-scp-ultra-platform-driver.o \
	mtk-scp-ultra-common.o \
	ultra_ipi.o

obj-$(CONFIG_MTK_ULTRASND_PROXIMITY) += snd-soc-mtk-scp-ultra.o
