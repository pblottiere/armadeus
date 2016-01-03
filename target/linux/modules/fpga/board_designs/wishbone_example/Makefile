#
# Main Makefile for the Armadeus FPGA related drivers
#
# Add your subdirectories to obj- target
#

ifneq ($(KERNELRELEASE)$(KERNELVERSION),)

# Part executed when called from kernel build system:
obj-$(CONFIG_ARMADEUS_FPGA_BOARD_DESIGNS_WISHBONE_EXAMPLE) +=  wb_button/ wb_irq_mngr/ wb_led/

else

# Armadeus custom drivers common targets (default, clean, etc...)
ARMADEUS_BASE_DIR=../../../../../../
include $(ARMADEUS_BASE_DIR)/Makefile.in

install:
	echo "Installing Armadeus drivers..."
	$(MAKE) -C $(LINUX_DIR) SUBDIRS=$(PWD) INSTALL_MOD_PATH=$(TARGET_DIR) modules_install

test:
	$(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)gcc push-led.c -o push-led

endif
