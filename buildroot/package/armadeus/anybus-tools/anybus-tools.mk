#############################################################
#
# anybus-tools
#
#############################################################
ANYBUS_TOOLS_VERSION = 0.1
ANYBUS_TOOLS_BIN_NAME = anybus-tools
ANYBUS_TOOLS_SITE=$(TOPDIR)/../target/packages/anybus-tools/
ifeq ($(generic-package), )
ANYBUS_TOOLS_SITE_METHOD=copy
else
ANYBUS_TOOLS_SITE_METHOD=local
endif
LINUX_DIR=$(BUILD_DIR)/linux-$(KERNEL_VERSION)/

define ANYBUS_TOOLS_BUILD_CMDS
	ln -sf "$(LINUX_DIR)/include/net/anybus_interface.h" "$(STAGING_DIR)/usr/include/net/anybus_interface.h"
	$(MAKE) CC="$(TARGET_CC)" -C $(@D) all
endef

define ANYBUS_TOOLS_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 $(ANYBUS_TOOLS_DIR)/$(ANYBUS_TOOLS_BIN_NAME) $(TARGET_DIR)/usr/bin/$(ANYBUS_TOOLS_BIN_NAME)
endef

ifeq ($(generic-package), )
$(eval $(call GENTARGETS))
else
$(eval $(generic-package))
endif

