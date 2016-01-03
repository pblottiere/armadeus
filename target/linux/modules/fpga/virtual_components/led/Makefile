#
# Makefile for the Armadeus FPGA led driver
#

obj-$(CONFIG_ARMADEUS_FPGA_VIRTUAL_COMPONENTS_LED) := led_ocore.o
led_ocore-objs := g_led.o

ARMADEUS_BASE_DIR=../../../../../..
-include $(ARMADEUS_BASE_DIR)/Makefile.in

KDIR := $(ARMADEUS_LINUX_DIR)
PWD  := $(shell pwd)

test:
	$(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)gcc -Wall testled.c -o testled
	chmod 755 testled


