################################################################################
#
# max5821ctrl
#
################################################################################

MAX5821CTRL_VERSION = 1.0
MAX5821CTRL_SITE = $(TOPDIR)/../target/packages/max5821ctrl
MAX5821CTRL_SITE_METHOD = local
MAX5821CTRL_LICENSE = GPLv2+
MAX5821CTRL_LICENSE_FILES = setDAC.c 

define MAX5821CTRL_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" -C $(@D)
endef

define MAX5821CTRL_INSTALL_TARGET_CMDS
	$(INSTALL) -m 755 $(@D)/setDAC $(TARGET_DIR)/usr/bin/
endef

$(eval $(generic-package))
