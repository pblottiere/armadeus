#############################################################
#
# imxregs debug tool
#
#############################################################

IMXREGS_VERSION = 2015.05
IMXREGS_SITE = $(TOPDIR)/../target/linux/debug/imxregs/
IMXREGS_SITE_METHOD = local
IMXREGS_LICENSE = GPLv2+
IMXREGS_LICENSE_FILES = COPYING

CPU_NAME = $(call qstrip,$(BR2_CPU_NAME))
IMXREGS_BINARY = $(CPU_NAME)regs
IMXREGS_TARGET_BINARY = $(TARGET_DIR)/usr/bin/imxregs

define IMXREGS_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" -C $(@D)
endef

define IMXREGS_INSTALL_TARGET_CMDS
	install -D $(@D)/$(IMXREGS_BINARY) $(IMXREGS_TARGET_BINARY)
endef

$(eval $(generic-package))
