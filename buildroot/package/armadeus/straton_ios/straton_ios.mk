#############################################################
#
# straton_ios library
#
#############################################################

STRATON_IOS_VERSION = 0.1
STRATON_IOS_DEPENDENCIES = as_devices
STRATON_IOS_BINARY:=libstraton_ios.so
ifeq ($(generic-package), )
STRATON_IOS_SITE_METHOD=copy
else
STRATON_IOS_SITE_METHOD=local
endif
STRATON_IOS_INSTALL_STAGING=YES
STRATON_IOS_SITE=$(TOPDIR)/../target/packages/straton_ios

define STRATON_IOS_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" STAGING_DIR="$(STAGING_DIR)" \
		BOARD_NAME=$(BR2_BOARD_NAME) -C $(@D)
endef

define STRATON_IOS_INSTALL_STAGING_CMDS
	$(INSTALL) -m 0755 $(STRATON_IOS_DIR)/$(STRATON_IOS_BINARY)* $(STAGING_DIR)/usr/lib/
	mkdir -p $(STAGING_DIR)/usr/include/straton_ios/
	$(INSTALL) -m 644 $(STRATON_IOS_DIR)/$(BR2_BOARD_NAME)_ios.h $(STAGING_DIR)/usr/include/straton_ios/straton_ios.h
endef

define STRATON_IOS_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 $(STRATON_IOS_DIR)/$(STRATON_IOS_BINARY)* $(TARGET_DIR)/usr/lib/
endef

ifeq ($(generic-package), )
$(eval $(call GENTARGETS))
else
$(eval $(generic-package))
endif

