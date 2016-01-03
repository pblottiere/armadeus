#
# Makefile for the Armadeus FPGA button driver
#

ifneq ($(KERNELRELEASE)$(KERNELVERSION),)
# Part executed when called from kernel build system:

obj-$(CONFIG_ARMADEUS_FPGA_VIRTUAL_COMPONENTS_BUTTON)	+= button_ocore.o
button_ocore-objs := gbutton.o

else
# Part executed when called from standard make in this directory:
# (preferably use Makefile in parent directory)

ARMADEUS_BASE_DIR=../../../../../..
-include $(ARMADEUS_BASE_DIR)/Makefile.in

CC = $(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)gcc
KDIR := $(ARMADEUS_LINUX_DIR)
PWD  := $(shell pwd)

test:
	$(CC) -Wall testbutton.c -o testbutton
	chmod 755 testbutton

endif
