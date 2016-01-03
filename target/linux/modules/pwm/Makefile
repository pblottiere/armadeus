#
# Makefile for the Armadeus PWM driver
#

# Part executed when called from kernel build system:
ifneq ($(KERNELRELEASE)$(KERNELVERSION),)

obj-$(CONFIG_ARMADEUS_PWM_DRIVER)	+= imx-pwm.o
imx-pwm-objs        := pwm.o
obj-$(CONFIG_ARMADEUS_MXS_PWM_DRIVER)	+= mxs-pwm.o

# Part executed when called from standard make in this directory:
# (preferably use Makefile in parent directory)
else

ARMADEUS_BASE_DIR=../../../..
include $(ARMADEUS_BASE_DIR)/Makefile.in

KDIR    := $(ARMADEUS_LINUX_DIR)
PWD	:= $(shell pwd)

# Armadeus custom drivers common targets (default, clean, etc...)
include ../Makefile.in

endif

