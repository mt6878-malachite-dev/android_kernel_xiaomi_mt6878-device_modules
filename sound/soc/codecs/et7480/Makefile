#
# Makefile for the kernel et7480 type-c switch device drivers.
#
obj-m += et7480.o

MTK_CUSTOM_PATH := $(DEVICE_MODULES_PATH)/drivers/misc/mediatek
subdir-ccflags-y += \
	-I$(MTK_CUSTOM_PATH)/typec
subdir-ccflags-y += \
	-I$(MTK_CUSTOM_PATH)/power_throttling
subdir-ccflags-y += \
    -I$(MTK_CUSTOM_PATH)/pbm

subdir-ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/power/supply/
subdir-ccflags-y += -I$(DEVICE_MODULES_PATH)/sound/soc/codecs/
