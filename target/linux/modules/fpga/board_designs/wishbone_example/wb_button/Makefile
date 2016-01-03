#
# Makefile for the Armadeus Wb_button drivers
#

ifneq ($(KERNELRELEASE)$(KERNELVERSION),)

obj-$(CONFIG_ARMADEUS_FPGA_BOARD_DESIGNS_WISHBONE_EXAMPLE_BUTTON_DRIVER) += wb_example_buttons.o
wb_example_buttons-objs := board_buttons.o
else

ARMADEUS_BASE_DIR=../../../../../../../
include $(ARMADEUS_BASE_DIR)/Makefile.in

KDIR := $(ARMADEUS_LINUX_DIR)
PWD  := $(shell pwd)

test:
	$(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)gcc testbutton.c -o testbutton

endif

