#source files
LPM_MODULE_PLAT_PLATFORM_OBJS += lpm_plat.o
LPM_MODULE_PLAT_PLATFORM_OBJS += lpm_plat_apmcu.o

LPM_MODULES_FOLDER=$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/lpm/modules/


ccflags-y += -I$(LPM_MODULES_FOLDER)/include/


MTK_LPM_MODULE_PLATFORM_PLAT_NAME=v1
MTK_LPM_SUSPEND_VERSION = suspend_v1


ccflags-y += -I$(LPM_MODULES_FOLDER)/platform/$(MTK_LPM_MODULE_PLATFORM_PLAT_NAME)/
ccflags-y += -I$(LPM_MODULES_FOLDER)/platform/$(MTK_LPM_SUSPEND_VERSION)/
ccflags-y += -I$(LPM_MODULES_FOLDER)/include/

include $(LPM_MODULES_FOLDER)/platform/$(MTK_LPM_SUSPEND_VERSION)/Makefile


LPM_MODULE_PLAT_PLATFORM = $(LPM_MODULE_PLAT_PLATFORM_OBJS:%.o=%.o)
LPM_MODULE_PLAT_PLATFORM += $(LPM_MODULE_PLAT_PLATFORM_SUSPEND_OBJS:%.o=../$(MTK_LPM_SUSPEND_VERSION)/%.o)


#Generic build
BUILD_MTK_LPM_PLAT_MODUDLE_NAME=mtk-lpm-plat-${MTK_LPM_MODULE_PLATFORM_PLAT_NAME}
obj-$(CONFIG_MTK_LOW_POWER_MODULE) += ${BUILD_MTK_LPM_PLAT_MODUDLE_NAME}.o

${BUILD_MTK_LPM_PLAT_MODUDLE_NAME}-objs += $(LPM_MODULE_PLAT_PLATFORM)
