#
# Copyright (C) 2016 MediaTek Inc.
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

$(info $(MTK_PLATFORM))
$(info $(platform))
$(info $(DEVICE_MODULES_PATH))

#ion
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/staging/android/ion \
             -I$(DEVICE_MODULES_PATH)/drivers/staging/android/ion/mtk \
             -I$(DEVICE_MODULES_PATH)/drivers/iommu/
#for sensorType in ccu_set_sensor_info
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/imgsensor/inc
#
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/m4u/mt6873/
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/mach/mt6873/include/mach
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/irq/mt6873/
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/include/
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/include/mt-plat/mt6873/include
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/mmp/
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/include/mt-plat
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/ccu/inc/
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/ccu/src/isp6s/ccu_ext_interface
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/smi/
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/i2c/busses/
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/mmdvfs/
ccflags-y += -I$(DEVICE_MODULES_PATH)/include/dt-bindings/memory/

#obj-$(CONFIG_MTK_VPU) += ccu_hw.o
obj-$(CONFIG_MTK_CCU) += ccu.o


ccu-objs += ccu_hw.o
ccu-objs += ccu_reg.o
ccu-objs += ccu_kd_mailbox.o
ccu-objs += ccu_imgsensor.o
ccu-objs += ccu_qos.o
ccu-objs += ccu_drv.o
ccu-objs += ccu_ipc.o
ccu-objs += ccu_mva.o
# obj-$(CONFIG_MTK_CCU) += ccu_hw.o ccu_reg.o ccu_kd_mailbox.o ccu_i2c_hw.o ccu_sw_ver.o ccu_imgsensor.o ccu_mva.o ccu_qos.o ccu_drv.o ccu_n3d_a.o ccu_i2c.o

