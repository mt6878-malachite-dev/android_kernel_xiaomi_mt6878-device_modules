# SPDX-License-Identifier: GPL-2.0

ccflags-y += -I$(srctree)/drivers/usb/typec
ccflags-y += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/typec/tcpc/inc
CFLAGS_pi3dpx1205a.o += -I$(DEVICE_MODULES_PATH)/drivers/misc/mediatek/typec/mux

obj-$(CONFIG_MTK_USB_TYPEC_MUX) += mux_switch.o
obj-$(CONFIG_TYPEC_MUX_FUSB304)	+= fusb304.o
obj-$(CONFIG_TYPEC_MUX_PTN36241G) += ptn36241g.o
obj-$(CONFIG_TYPEC_MUX_PS5169) += ps5169.o
obj-$(CONFIG_TYPEC_MUX_PS5170) += ps5170.o
obj-$(CONFIG_TYPEC_MUX_PI3DPX1205A) += pi3dpx1205a.o
obj-$(CONFIG_TYPEC_MUX_DP_SELECTOR) += usb_dp_selector.o
