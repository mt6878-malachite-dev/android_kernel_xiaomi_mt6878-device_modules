CLKBUF_TOP = $(DEVICE_MODULES_PATH)/drivers/misc/mediatek/clkbuf/src

ccflags-y += -I$(srctree)$(src)/src/ \
     -I$(CLKBUF_TOP)/

obj-$(CONFIG_MTK_SRCLKEN_RC) += srclken-spm.o
srclken-spm-$(CONFIG_MTK_SRCLKEN_RC) += src/mtk-srclken-rc-spm.o

obj-$(CONFIG_MTK_SRCLKEN_RC) += srclken.o

srclken-$(CONFIG_MTK_SRCLKEN_RC) += src/mtk-srclken-rc.o \
		src/mtk-srclken-rc-hw.o


