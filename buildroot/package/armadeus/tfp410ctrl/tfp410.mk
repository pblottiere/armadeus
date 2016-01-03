################################################################################
#
# tfp410ctrl
#
################################################################################

TFP410CTRL_VERSION = 1.0
TFP410CTRL_SITE = $(TOPDIR)/../target/packages/tfp410ctrl
TFP410CTRL_SITE_METHOD = local
TFP410CTRL_LICENCE = GPLv2+
TFP410CTRL_LICENCE_FILES = tfp410ctrl.c parse-edid.c

define TFP410CTRL_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" -C $(@D)
endef

define TFP410CTRL_INSTALL_TARGET_CMDS
	$(INSTALL) -m 755 $(@D)/tfp410ctrl $(TARGET_DIR)/usr/sbin/
endef

$(eval $(generic-package))
