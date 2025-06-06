# Copyright (c) 2015-2019, MICROTRUST Incorporated
# All Rights Reserved.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.

subdir-ccflags-y  += -I$(srctree)/$(src)/common/include
#ccflags-y  += -I$(src)/common/include
subdir-ccflags-y  += \
	-I$(srctree)/$(src)/tz_dcih \
	-I$(srctree)/$(src)/public \
	-I$(srctree)/$(src)/tz_driver/include

ifneq ($(filter y m, $(CONFIG_MTPROF)),)
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/mtprof/
endif

$(info "=----------------1---------------------------")
subdir-ccflags-$(CONFIG_MICROTRUST_DEBUG) += -DDEBUG
#add teei with ffa module
obj-m += isee-ffa.o
isee-ffa-y :=          tee/tee_core.o \
                       tee/capi_proxy.o \
                       tee/tee_client_api.o \
                       tee/tee_shm.o \
                       tee/tee_shm_pool.o \
                       tee/soter/core.o \
                       tee/soter/call.o \
                       tee/soter/soter_ffa_abi.o \
                       ut_keymaster/ut_keymaster.o \
                       tz_driver/teei_fp.o \
                       tz_driver/teei_client_transfer_data.o \
                       tz_driver/teei_smc_call.o \
                       tz_driver/sysfs.o \
                       tz_driver/teei_cancel_cmd.o \
                       tz_driver/fdrv.o \
                       tz_driver/notify_queue.o \
                       tz_driver/switch_queue.o \
                       tz_driver/teei_keymaster.o \
                       tz_driver/teei_task_link.o \
                       tz_driver/tz_log.o \
                       tz_driver/irq_register.o \
                       tz_driver/teei_client_main.o \
                       tz_driver/backward_driver.o \
                       tz_vfs/fp_vendor.o \
                       tz_vfs/vfsFun.o \
                       tz_dcih/tz_dcih_test.o \
                       tz_dcih/tz_dcih.o

isee-ffa-y +=              teei_fp/fp_func.o

ifneq ($(filter y m, $(CONFIG_MICROTRUST_TEST_DRIVERS)),)
isee-ffa-y +=          tests/main.o \
                       tests/xtest_isee_1000.o \
                       tests/xtest_isee_3000.o \
                       tests/xtest_main.o

subdir-ccflags-y  += -DTEEI_TEST_DRIVERS
endif

ifneq ($(filter y m, $(CONFIG_MICROTRUST_UNITTEST_SUPPORT)),)
isee-ffa-y +=              ut_tester/ut_tester.o
subdir-ccflags-y  += -DTEEI_UNITTEST_SUPPORT
endif

#add teei kernel driver without ffa
obj-m += isee.o
isee-y :=              tee/tee_core.o \
                       tee/capi_proxy.o \
                       tee/tee_client_api.o \
                       tee/tee_shm.o \
                       tee/tee_shm_pool.o \
                       tee/soter/core.o \
                       tee/soter/call.o \
                       tee/soter/teei_ffa_dummy.o \
                       ut_keymaster/ut_keymaster.o \
                       tz_driver/teei_fp.o \
                       tz_driver/teei_client_transfer_data.o \
                       tz_driver/teei_smc_call.o \
                       tz_driver/sysfs.o \
                       tz_driver/teei_cancel_cmd.o \
                       tz_driver/fdrv.o \
                       tz_driver/notify_queue.o \
                       tz_driver/switch_queue.o \
                       tz_driver/teei_keymaster.o \
                       tz_driver/teei_task_link.o \
                       tz_driver/tz_log.o \
                       tz_driver/irq_register.o \
                       tz_driver/teei_client_main.o \
                       tz_driver/backward_driver.o \
                       tz_vfs/fp_vendor.o \
                       tz_vfs/vfsFun.o \
                       tz_dcih/tz_dcih_test.o \
                       tz_dcih/tz_dcih.o

isee-y +=              teei_fp/fp_func.o

ifneq ($(filter y m, $(CONFIG_MICROTRUST_TEST_DRIVERS)),)
isee-y +=              tests/main.o \
                       tests/xtest_isee_1000.o \
                       tests/xtest_isee_3000.o \
                       tests/xtest_main.o

subdir-ccflags-y  += -DTEEI_TEST_DRIVERS
endif

ifneq ($(filter y m, $(CONFIG_MICROTRUST_UNITTEST_SUPPORT)),)
isee-y +=              ut_tester/ut_tester.o
subdir-ccflags-y  += -DTEEI_UNITTEST_SUPPORT
endif

$(info "=----------------2---------------------------")
#obj-$(CONFIG_MICROTRUST_TZ_DRIVER) += tz_driver/
#obj-$(CONFIG_MICROTRUST_VFS_DRIVER) += tz_vfs/
#obj-$(CONFIG_MICROTRUST_FP_DRIVER) += teei_fp/
#obj-$(CONFIG_MICROTRUST_KEYMASTER_DRIVER) += ut_keymaster/
#obj-y += tz_dcih/
#obj-$(CONFIG_MICROTRUST_TEST_DRIVERS) += tests/
#obj-$(CONFIG_MICROTRUST_UNITTEST_SUPPORT) += ut_tester/
