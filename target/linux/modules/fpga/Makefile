#
# Main Makefile for the Armadeus FPGA related drivers
#
# Add your subdirectories to obj- target
#

ifneq ($(KERNELRELEASE)$(KERNELVERSION),)
# Part executed when called from kernel build system:

obj-$(CONFIG_ARMADEUS_FPGA_DRIVERS) += dev_tools/ board_designs/ virtual_components/
obj-$(CONFIG_POD_DRIVERS) += POD/
obj-$(CONFIG_ARMADEUS_I2C_OCORE_IRQMNG) += others/i2ctest/
obj-$(CONFIG_ARMADEUS_PS2KBD_DRIVER) += others/ps2keyboard/

else
# Part executed when called from standard make in this directory:

ARMADEUS_BASE_DIR=../../../..
include $(ARMADEUS_BASE_DIR)/Makefile.in

ifeq ($(LINUX_DIR),)
LINUX_DIR:=$(ARMADEUS_LINUX_DIR)
endif
KDIR	:= $(LINUX_DIR)
PWD	:= $(shell pwd)

# Armadeus custom drivers common targets (default, clean, etc...)
include ../Makefile.in

endif

