#
# Makefile for the Armadeus FPGA OpenCore IRQ manager IP
#

ifneq ($(KERNELRELEASE)$(KERNELVERSION),)

# Part executed when called from kernel build system:

obj-$(CONFIG_ARMADEUS_FPGA_BOARD_DESIGNS_WISHBONE_EXAMPLE_IRQ_MNGR_DRIVER) += wb_example_irq_mngr.o
wb_example_irq_mngr-objs := board_irq_mng.o

else

ARMADEUS_BASE_DIR=../../../../../..
include $(ARMADEUS_BASE_DIR)/Makefile.in

KDIR := $(ARMADEUS_LINUX_DIR)
PWD  := $(shell pwd)

# Armadeus custom drivers common targets (default, clean, etc...)
include ../../../Makefile.in

endif

