#############################################################
#
# straton
#
#############################################################
STRATON_VERSION = 0.1
STRATON_DEPENDENCIES = straton_ios
STRATON_BIN_NAME = straton

# STRATON DEV
ifeq ($(BR2_PACKAGE_STRATON_DEV),y)
STRATON_SITE=$(BR2_PACKAGE_STRATON_DEV_DIR)
ifeq ($(generic-package), )
STRATON_SITE_METHOD=copy
else
STRATON_SITE_METHOD=local
endif

define STRATON_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" STAGING_DIR="$(STAGING_DIR)" \
		BOARD_NAME=$(BOARD_NAME) -C $(@D) all
endef

define STRATON_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 $(STRATON_DIR)/bin/t5hmi $(TARGET_DIR)/usr/bin/$(STRATON_BIN_NAME)
endef
endif # STRATON_DEV

# STRATON BIN
ifeq ($(BR2_PACKAGE_STRATON_BIN),y)
STRATON_SITE=http://ask_for_binary
STRATON_SITE_METHOD=wget
#STRATON_SOURCE=straton-$(STRATON_VERSION).tar.gz

define STRATON_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 $(STRATON_DIR)/straton $(TARGET_DIR)/usr/bin/$(STRATON_BIN_NAME)
endef

endif # STRATON_BIN

# STRATON DEMO
ifeq ($(BR2_PACKAGE_STRATON_DEMO),y)
STRATON_SITE=ftp://ftp2.armadeus.com
STRATON_SOURCE=straton_demo-$(STRATON_VERSION).tar.gz

define STRATON_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 $(STRATON_DIR)/straton $(TARGET_DIR)/usr/bin/$(STRATON_BIN_NAME)
endef

endif # STRATON_BIN

ifeq ($(generic-package), )
$(eval $(call GENTARGETS))
else
$(eval $(generic-package))
endif

