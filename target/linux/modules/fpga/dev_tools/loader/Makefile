#
# Makefile for the Armadeus FPGA loader driver
#

ifneq ($(KERNELRELEASE)$(KERNELVERSION),)

obj-$(CONFIG_ARMADEUS_FPGA_LOADER_DRIVER)	+= fpgaloader.o
ifneq ($(CONFIG_SOC_IMX6Q),)
fpgaloader-objs := altera_cvp.o
else
ifneq ($(CONFIG_SOC_IMX6SOLO),)
fpgaloader-objs := altera_cvp.o
else
fpgaloader-objs := xilinx-fpga-loader.o fpga-loader.o
endif
endif
ifneq ($(CONFIG_DTC),)
ifneq ($(CONFIG_MACH_MX27),)
obj-y += apf27-fpga-loader.o
endif
else
ifneq ($(CONFIG_MACH_APF51),)
obj-y += apf51-fpga-loader.o
endif
ifneq ($(CONFIG_MACH_APF27),)
obj-y += apf27-fpga-loader.o
endif
ifneq ($(CONFIG_MACH_APF9328),)
obj-y += apf9328-fpga-loader.o
endif
endif # 

else

ARMADEUS_BASE_DIR=../../../../../..
include $(ARMADEUS_BASE_DIR)/Makefile.in

KDIR    := $(ARMADEUS_LINUX_DIR)
PWD	:= $(shell pwd)

# Armadeus custom drivers common targets (default, clean, etc...)
include ../../../Makefile.in

endif

