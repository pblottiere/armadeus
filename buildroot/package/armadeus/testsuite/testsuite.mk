################################################################################
#
# Armadeus Test Suite
#
################################################################################

TESTSUITE_VERSION = 1.11
TESTSUITE_SITE = $(TOPDIR)/../target/test/
TESTSUITE_SITE_METHOD = local

ARMADEUS_TESTSUITE_WISHBONE_BUTTON_SRCDIR = $(TOPDIR)/../target/linux/modules/fpga/virtual_components/button/
ARMADEUS_TESTSUITE_TARGET_DIR = $(TARGET_DIR)/usr/bin/testsuite/

define TESTSUITE_COPY_FPGA_SYMLINKS
	rm -f $(@D)/data/fpga/*
	cp -Lr $(TESTSUITE_SITE)/data/fpga/* $(@D)/data/fpga/
endef

TESTSUITE_POST_RSYNC_HOOKS += TESTSUITE_COPY_FPGA_SYMLINKS

define TESTSUITE_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" -C $(ARMADEUS_TESTSUITE_WISHBONE_BUTTON_SRCDIR) test
endef

define TESTSUITE_INSTALL_TARGET_CMDS
	mkdir -p $(ARMADEUS_TESTSUITE_TARGET_DIR)
	$(@D)/install.sh $(ARMADEUS_TESTSUITE_TARGET_DIR) $(BR2_BOARD_NAME)
	$(INSTALL) -D $(ARMADEUS_TESTSUITE_WISHBONE_BUTTON_SRCDIR)/testbutton $(ARMADEUS_TESTSUITE_TARGET_DIR)
endef

$(eval $(generic-package))
