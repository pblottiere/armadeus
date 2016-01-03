################################################################################
#
# ad9889ctrl
#
################################################################################

AD9889CTRL_VERSION = 1.0
AD9889CTRL_SITE = $(TOPDIR)/../target/packages/ad9889ctrl
AD9889CTRL_SITE_METHOD = local
AD9889CTRL_LICENCE = GPLv2+
AD9889CTRL_LICENCE_FILES = ad9889ctrl.c

define AD9889CTRL_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" -C $(@D)
endef

define AD9889CTRL_INSTALL_TARGET_CMDS
	$(INSTALL) -m 755 $(@D)/ad9889ctrl $(TARGET_DIR)/usr/sbin/
endef

$(eval $(generic-package))
