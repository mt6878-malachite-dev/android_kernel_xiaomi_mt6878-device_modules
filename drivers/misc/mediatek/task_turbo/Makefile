# Copyright (C) 2019 MediaTek Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See http://www.gnu.org/licenses/gpl-2.0.html for more details.
#
#
ccflags-y += -I$(srctree)
ccflags-y += -I$(srctree)/$(src)              # needed for trace events
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/sched/
obj-$(CONFIG_MTK_TASK_TURBO) += task_turbo.o
