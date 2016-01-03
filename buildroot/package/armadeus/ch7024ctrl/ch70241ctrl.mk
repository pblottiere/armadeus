################################################################################
#
# ch7024ctrl
#
################################################################################

CH7024CTRL_VERSION = 1.01
CH7024CTRL_SITE = $(TOPDIR)/../target/packages/ch7024ctrl
CH7024CTRL_SITE_METHOD = local
CH7024CTRL_LICENSE = GPLv2+
CH7024CTRL_LICENSE_FILES = ch7024.c
CH7024CTRL_CONF_FILES = ch*x*.conf

define CH7024CTRL_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" -C $(@D)
endef

define CH7024CTRL_INSTALL_TARGET_CMDS
	$(INSTALL) -m 755 $(@D)/ch7024ctrl $(TARGET_DIR)/usr/sbin/
	$(INSTALL) -m 644 $(@D)/$(CH7024CTRL_CONF_FILES) $(TARGET_DIR)/usr/sbin/
endef

$(eval $(generic-package))
