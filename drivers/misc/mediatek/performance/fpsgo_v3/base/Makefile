#
# Copyright (C) 2017 MediaTek Inc.
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
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#

MTK_TOP = $(DEVICE_MODULES_PATH)/drivers/misc/mediatek/
FPSGO_TOP = $(MTK_TOP)/performance/fpsgo_v3/
BASE_TOP = $(FPSGO_TOP)/base/
COMP_TOP = $(FPSGO_TOP)/composer/
FBT_TOP = $(FPSGO_TOP)/fbt/

obj-y += \
	src/fpsgo_base.o \
	src/fpsgo_sysfs.o \

ccflags-y += \
	-I$(srctree)/include/ \
	-I$(MTK_TOP)/include/ \
	-I$(BASE_TOP)/include/ \
	-I$(COMP_TOP)/include/ \
	-I$(FBT_TOP)/include/ \
