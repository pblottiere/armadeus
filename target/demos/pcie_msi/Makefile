# Part executed when called from kernel build system:
ifneq ($(KERNELRELEASE),)

obj-m += pcie_msi.o

else

PWD = $(shell pwd)

ARMADEUS_BASE_DIR=$(PWD)/../../..
-include $(ARMADEUS_BASE_DIR)/Makefile.in

CC = $(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)gcc
LD = $(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)ld
OBJDUMP = $(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)objdump
KDIR = $(ARMADEUS_LINUX_DIR)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) ARCH=arm CROSS_COMPILE=$(ARMADEUS_TOOLCHAIN_PREFIX) modules CC=$(CC) LD=$(LD) OBJDUMP=$(OBJDUMP)

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	rm -f modules.order
	rm -rf .tmp_versions/
	rm -f Module.markers

endif
