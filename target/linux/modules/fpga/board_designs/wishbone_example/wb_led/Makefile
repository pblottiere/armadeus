#
# Makefile for the Armadeus FPGA led driver
#

ifneq ($(KERNELRELEASE)$(KERNELVERSION),)

obj-$(CONFIG_ARMADEUS_FPGA_BOARD_DESIGNS_WISHBONE_EXAMPLE_LED_DRIVER) += wb_example_led.o
wb_example_led-objs := board_leds.o

else

ARMADEUS_BASE_DIR=../../../../../../../
include $(ARMADEUS_BASE_DIR)/Makefile.in

KDIR := $(ARMADEUS_LINUX_DIR)
PWD  := $(shell pwd)

test:
	$(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)gcc -Wall testled.c -o testled
	chmod 755 testled

endif

